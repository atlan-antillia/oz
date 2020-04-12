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
 *  ImageView.cpp
 *
 *****************************************************************************/

//2019/01/03
#include <opencv2/stitching.hpp>

#include <oz++/StringT.h>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVImageView.h>

namespace OZ {

class MainView :public OpenCVMainView {

private:
  //Inner class start
  class SimpleView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat blurredImage; 
    cv::Mat scaledImage;

  public:

    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    SimpleView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
      try {
        const char* filename = (const char*)args.get(XmNimageFileName);
        int imageLoadingFlag = args.get(XmNimageLoadingFlag);
        loadImage(filename, imageLoadingFlag);

      } catch (OZ::Exception ex) {
        caught(ex);
      }
    } 

    ~SimpleView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR)
    {
      try {
        originalImage = readImage(filename, imageLoadingFlag);
        blurredImage  = originalImage.clone();
        int ksize = 7;
        int sigma = 11;
        blur(ksize, sigma);

        scaleImage(blurredImage, scaledImage, 80);  

        refresh();
      } catch (OZ::Exception& ex) {
        caught(ex);
      }
    }    

    void blur(int ksize, int sigma)
    {
      //void GaussianBlur(const Mat& src, Mat& dst, Size ksize, 
      //       double sigmaX, 
      //       double sigmaY  = 0, 
      //       int borderType = BORDER_DEFAULT)

      cv::GaussianBlur(originalImage, blurredImage, cv::Size(ksize, ksize), 
            (double)sigma, //sigmaX, 
            (double)sigma); //sigmaY
      refresh();
    }
  };
  //Inner class end
	
private:
  SmartPtr<SimpleView>  view;
  StringT<char>         imageFile;
  int                   imageLoadingFlag;
  static const int      CP_WIDTH=180;

  void resize(Dimension w, Dimension h)
  {
    if (view) {
      view->View::resize(0, 0, w-CP_WIDTH, h, 0);
    }
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args) 
  {
    BulletinBoard* bboard = getBulletinBoard();

    imageFile = "../images/flower.png";
    imageLoadingFlag = CV_LOAD_IMAGE_COLOR;

    try {
      Args ar;

      ar.set(XmNimageFileName, imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);

      view = new SimpleView(bboard, "", ar);

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
    args.set(XmNwidth,  700);
    args.set(XmNheight, 500);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

