/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer.
 *
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *  ScrolledImageBlurView.cpp
 *
 *****************************************************************************/

//2019/01/03
#include <opencv2/stitching.hpp>
#include <oz++/StringT.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/FileOpenDialog.h>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVScrolledImageView.h>

namespace OZ {

class MainView :public OpenCVMainView {

private:
  //Inner class start
  class SimpleView: public OpenCVScrolledImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat blurredImage; 

  public:
    void display()
    {
      show(blurredImage);
    }

  public:
    SimpleView(View* parent, const char* name, Args& args)
    :OpenCVScrolledImageView(parent, name, args)
    {
      const char* fileName = NULL;
      int   loadingFlag       = 1;
      if (args.has(XmNimageFileName)) {
        fileName = (const char*)args.get(XmNimageFileName);
      }
      if (args.has(XmNimageLoadingFlag)) {
        loadingFlag = (int)args.get(XmNimageLoadingFlag);
      }
      loadImage(fileName, loadingFlag);
    } 

    ~SimpleView()
    {
    }
    
    void loadImage(const char* fileName, int loadingFlag)
    {
      originalImage = readImage(fileName, loadingFlag);
    }

    void blur(int ksize, int sigma)
    {
      //void GaussianBlur(const Mat& src, Mat& dst, Size ksize, 
      //       double sigmaX, 
      //       double sigmaY  = 0, 
      //       int borderType = BORDER_DEFAULT)

      blurredImage = originalImage.clone();

      cv::GaussianBlur(originalImage, blurredImage, cv::Size(ksize, ksize), 
            (double)sigma, //sigmaX, 
            (double)sigma); //sigmaY

      setMat(blurredImage);
      invalidate();
      refresh();
      flush();
    }
  };
  //Inner class end
	
private:
  StringT<char>         imageFile;
  int                   imageLoadingFlag;
  int                   ksize;
  int                   sigma;

  SmartPtr<Label>       label;
  SmartPtr<SimpleView>  view;
  SmartPtr<FileOpenDialog> fileDialog;

  void resize(Dimension w, Dimension h)
  {
    if (view && label) {
    int LP_HEIGHT =30;
    int CP_WIDTH = 180;
    int ww = w -CP_WIDTH;
    label -> reshape(0, 0, ww, LP_HEIGHT);
    view->reshape(0, LP_HEIGHT, ww, h-LP_HEIGHT*2-4);
    }
  }

  void updateLabel(const char* filename)
  {
    CompoundString cs(filename);
    label->set(XmNlabelString, cs);
  }

  virtual void fileOpen(Action& action)
  {
    fileDialog->popup();
  }

  void ok(Action& action)
  {
    try {  
      imageFile  = fileDialog->getFileName();
      const char* filename = (const char*)imageFile;
      printf("filename: %s\n", filename);
      fileDialog->popdown();
      updateLabel(filename);
    
      view -> loadImage(filename, imageLoadingFlag);
      view -> blur(ksize, sigma);
      view -> unmap();
      view -> map();

      resize(width(), height());
      sendConfigureEvent(); 
      flush();
    } catch (OZ::Exception& ex) {
       caught(ex);
    } 
  }
  
public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args) 
  {
    imageFile = "../images/Figure.png";
    imageLoadingFlag = CV_LOAD_IMAGE_COLOR;

    try {
      BulletinBoard* bboard = getBulletinBoard();
      CompoundString fileNamecs(imageFile);
      Args ar;
      ar.set(XmNlabelString, fileNamecs);
      ar.set(XmNalignment,  XmALIGNMENT_BEGINNING);
      label = new Label(bboard, "", ar);

      ar.reset();
      ar.set(XmNimageFileName, imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      view = new SimpleView(bboard, "", ar);

      ksize = 7;
      sigma = 12;
      view->blur(ksize, sigma);

      fileDialog = new FileOpenDialog(this, "FileOpenDialog", ar);
      fileDialog  -> getOkButton()
                  -> addCallback(XmNactivateCallback, this,
                          (Callback)&MainView::ok, NULL); 
      sendConfigureEvent(); 

    } catch(OZ::Exception& ex) {
      caught(ex);
    }
  }
  ~MainView()
  {
  }
};

}

//
int main(int argc, char** argv) 
{
  try {
    const char*  appclass = argv[0];
    OpenCVApplication applet(appclass, argc, argv);

    Args args;
    args.set(XmNwidth,  600);
    args.set(XmNheight, 400);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

