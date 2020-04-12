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
 *  ImageFlipping.cpp
 *
 *****************************************************************************/

//2017/05/15

//2019/01/03
#include <opencv2/stitching.hpp>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVImageView.h>

namespace OZ {

class MainView :public OpenCVMainView {

private:
  ///////////////////////////////////////////////////////////
  //Inner class starts.
  class SimpleView : public OpenCVImageView {
  private:
    cv::Mat image;
    void display()
    {
      show(image);
    }
    
  public:
    SimpleView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
    }
    
    ~SimpleView()
    {
    }

    void setFlippedImage(cv::Mat& mat, int flip)
    {
      try {
        cv::Mat flipped;
    
        cv::flip(mat, flipped, flip);
        image = flipped;        
        refresh();
        
      } catch (OZ::Exception ex) {
        caught(ex);
      }
    }
  };
  //Inner class ends.
  ///////////////////////////////////////////////////////////
	
private:

  static const int VIEW_COUNT = 3;
  SmartPtr<SimpleView>  view[VIEW_COUNT];


  void resize(Dimension w, Dimension h)
  {
    if (view[0]) {
      for (int i = 0; i<VIEW_COUNT; i++) {
        view[i] -> reshape( w*i/VIEW_COUNT, 0,  w/VIEW_COUNT, h);
      }
    }
    flush();
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args) 
  {
    BulletinBoard* bboard = getBulletinBoard();
    const char* imageFile = "../images/cat.jpg";
    const StringT<char> fullpath = getFullpath(imageFile);
    int imageLoadingFlag = CV_LOAD_IMAGE_COLOR;

    try {
      Args ar;
      cv::Mat  mat = imread((const char*)fullpath, CV_LOAD_IMAGE_COLOR);

      int flip = -1;
      for (int i = 0; i<VIEW_COUNT; i++) {
        view[i] = new SimpleView(bboard, "", ar); 
        view[i]->setFlippedImage(mat, flip++);
      }  
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
    args.set(XmNwidth, 900);
    args.set(XmNheight, 300);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

