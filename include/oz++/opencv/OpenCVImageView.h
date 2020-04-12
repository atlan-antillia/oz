/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  OpenCVImageView.h
 *
 *****************************************************************************/

#pragma once
#include <oz++/ModuleFileName.h>
#include <oz++/Exception.h>
#include <oz++/motif/Pixelmap.h>
#include <oz++/motif/Canvas.h>
/*#ifdef OPENCV
#include <opencv2/opencv.hpp>            // C++
#include <opencv2/highgui/highgui_c.h>   // C
#include <opencv2/imgproc/imgproc_c.h>   // C
#endif
*/
#include <oz++/opencv/OpenCVObject.h>
#include <oz++/opencv/OpenCVImageConverter.h>
#include <oz++/AuxDef.h>

namespace OZ {

class OpenCVImageView: public Canvas {

private:
  Pixmap  pixmap;
  cv::Mat rgbImage;

public:
  //Please redefine a virtual display method in your own class 
  //derived from this OpenView class.
  virtual void display()
  {
     printf("OpenImageView::display\n");
  }

  virtual void scaleImage(cv::Mat& originalImage, cv::Mat& scaledImage, 
   int scalingRatio)
  {
    try {
      if (scalingRatio < 1) {
       scalingRatio = 10;
      }
      float ratio = (float)scalingRatio/100.0f;

      cv::resize(originalImage, scaledImage, cv::Size(), 
		ratio, ratio);
      //2017/05/18
      invalidate();
      refresh();
      //  
     } catch (cv::Exception& ex) {
         ;
     }
  }

  void resizeToImageSize(cv::Mat& image)
  {
    if (!image.empty()) {
      Dimension w = image.cols;
      Dimension h = image.rows;
      resize(w, h);
    }
  }

  cv::Mat readImage(const char* filename, int loadingFlag)
  {
    ModuleFileName module;
    const StringT<char> fullpath = module.getFullpath(filename);
    cv::Mat image = cv::imread((const char*)fullpath, loadingFlag);
    if (image.empty()) {
      throw IException("Failed to imread: %s", filename);
    }
    return image;
  }

  void invalidate()
  {
    pixmap = None;
    rgbImage.release();
  }

  void show(cv::Mat& mat)
  {
    if (!mat.empty()) {
      if (pixmap == None) {
        dprintf("show d =%d c=%d\n", mat.depth(), mat.channels());

        if ( mat.channels() == 1) {
          // if mat is grayscale, we convert  to rgbImage
          dprintf("Convert grayscale image to rgb image\n");
          rgbImage.release();
          cvtColor(mat, rgbImage, CV_GRAY2RGB);
          pixmap = OpenCVImageConverter::convertToPixmap(this, rgbImage);

        } else if ( mat.channels() ==3) { 
          // mat is rgbImage  
          pixmap = OpenCVImageConverter::convertToPixmap(this, mat);
        } else {
          throw IException("Unsupported cv::Mat image");
        }
      }
      
      if (pixmap != None) {
        DC dc(this);
        Display* display = dc.getDisplay();
        Drawable src  = pixmap;
        Drawable dst  = dc.getDrawable();
        GC       gc   = dc.getGC();
        dc.copyArea(src, 0, 0, width(), height(), 0, 0);

        //flush(); //XFlush
      }
    }
  }

  virtual void expose(Action& action)
  {
    //Call a virtual display method to display OpenGL figures.
    display();
    //flush(); //XFlush
  }

  void reshape(int x, int y, int w, int h)
  {
    View::resize(x, y, w, h, 0);
  }

public:
  OpenCVImageView(View* parent, const char* name, Args& args)
  :Canvas(parent, name,  args.set(XmNtraversalOn, 0))
  ,pixmap(None)
  {
  }

  ~OpenCVImageView()
  {
    rgbImage.release();
  }
	
  virtual void save(const char* filename, const cv::Mat& image)
  {
    if (!image.empty()) {
/*
      std::string name = filename;
      if (!cv::imwrite(name, image)) {
        throw IException("Failed to write image file: %s", filename);
      }
*/
    }
  }

  void rescale(cv::Mat& image, cv::Mat& scaledImage, int scalingRatio)
  {
    if (scalingRatio < 1) {
      scalingRatio = 10;
    }
    float ratio = (float)scalingRatio/100.0f;

    cv::resize(image, scaledImage, cv::Size(),
                ratio, ratio);
  }

  void refresh()
  {
    sendExposeEvent();
  }
};

}

