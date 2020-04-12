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
 *  ImageViewer.cpp
 *
 *****************************************************************************/

//2019/01/03
#include <opencv2/stitching.hpp>
#include <oz++/motif/Label.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/FileOpenDialog.h>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/opencv/OpenCVScaleComboBox.h>

namespace OZ {

class MainView : public OpenCVMainView {

private:
  //////////////////////////////////////////////
  //Inner class starts.

  class SimpleView  :public OpenCVImageView {
  private:
  cv::Mat originalImage;
  cv::Mat scaledImage;

  void display()
  {
    show(scaledImage);
  }

  public:
  SimpleView(View* parent, const char* name, Args& args)
  :OpenCVImageView(parent, name, args)
  {
    const char* filename = "";
    int   loadingFlag    = 1;
    int   scalingRatio   = 100;

    if (args.has(XmNimageFileName)) {
      filename = (const char*)args.get(XmNimageFileName);
    }
    if (args.has(XmNimageLoadingFlag)) {
      loadingFlag = (int)args.get(XmNimageLoadingFlag); 
    }
    if (args.has(XmNimageScalingRatio)) {
      scalingRatio = (int)args.get(XmNimageScalingRatio); 
    }

    loadImage(filename, loadingFlag, scalingRatio);
  }

  void loadImage(const char* filename, int loadingFlag, int scalingRatio)
  {
    originalImage = readImage(filename, loadingFlag);
    scaleImage(originalImage, scaledImage, scalingRatio); 
  }

  void rescale(int scalingRatio)
  {
    scaledImage.release();
    scaleImage(originalImage, scaledImage, scalingRatio); 
  }
 
  };
  //Inner class ends.
  //////////////////////////////////////////////


  SmartPtr<Label>          label;
  SmartPtr<SimpleView>     view;
  SmartPtr<RowColumn>      controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;
  SmartPtr<FileOpenDialog> fileDialog;
  StringT<char>            imageFileName;
  int                      imageLoadingFlag;
  int                      imageScalingRatio;

  void fileOpen(Action& action)
  {
    fileDialog->popup();
  }

  void updateLabel(const char* filename)
  {
     CompoundString cs(filename);
     label->set(XmNlabelString, cs);    
  }

  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      view -> rescale(imageScalingRatio); 
    }
  }  

  void ok(Action& action)
  {
    try {  
      const char* filename = fileDialog->getFileName();
      imageFileName = filename;
      printf("filename: %s\n", filename);
      fileDialog->popdown();
      updateLabel(filename);
      view->invalidate();
      view->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio);

      resize(width(), height());
      flush();
    } catch (OZ::Exception& ex) {
       caught(ex);
    } 
  }

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 200;
    int LB_HEIGHT = 30;
    int ww =  w-CP_WIDTH;
    int hh = h - LB_HEIGHT;
    if (label && view && controlPane ) {
      BulletinBoard* bboard = getBulletinBoard();
      bboard -> unmanage();
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      view         -> reshape(0, LB_HEIGHT, ww, hh);
      controlPane  -> reshape(ww-1, LB_HEIGHT, CP_WIDTH+1, hh);

     // controlPane -> unmap();
     // controlPane -> map();
      bboard-> manage();
    }
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args)
  {
    BulletinBoard* bboard = getBulletinBoard();

    try {
      Args ar;
      imageFileName     = "../images/flower.png";
      imageLoadingFlag  = CV_LOAD_IMAGE_COLOR;
      imageScalingRatio = 60;

      //1 Create a label to display a filename.
      ar.set(XmNalignment,  XmALIGNMENT_BEGINNING);
      label = new Label(bboard, "", ar);
      updateLabel(imageFileName);

      //2 Create a simpleview.
      ar.reset();
      ar.set(XmNimageFileName,    imageFileName);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);
      view = new SimpleView(bboard, "", ar);

      //3 Create a controlPane.
      ar.reset();
      controlPane = new RowColumn(bboard, "", ar);

      //4 Create a scaleComboBox.
      const char* defaultScale = "60%";
      ar.reset();
      CompoundString scaler("Scale");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNdefaultScale, defaultScale);
      scaleComboBox = new OpenCVScaleComboBox(controlPane, "", ar);
      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);

      //5 Create a fileOpendialog
      ar.reset();
      fileDialog = new FileOpenDialog(this, "FileOpenDialog", ar);
      fileDialog  -> getOkButton()
                  -> addCallback(XmNactivateCallback, this,
                          (Callback)&MainView::ok, NULL);  
    } catch (OZ::Exception& ex) {
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
    args.set(XmNwidth,  500);
    args.set(XmNheight, 400);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

