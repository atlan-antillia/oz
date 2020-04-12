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
 *      OpenCVScrolledImageView.h
 *
 *****************************************************************************/

#pragma once
#include <oz++/ModuleFileName.h>
#include <oz++/motif/ScrolledImageView.h>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVImageConverter.h>
#include <opencv2/imgcodecs.hpp> 
namespace OZ {
  
class OpenCVScrolledImageView :public ScrolledImageView {
private:
  cv::Mat   image;
     
  bool      keepScrollBarPosition;

  cv::Mat& getMat()
  {
    return image; 
  }

  void createPixmap()
  {
    try {
      Pixmap pixmap = OpenCVImageConverter::convertToPixmap(this, image);
      setPixmap(pixmap, keepScrollBarPosition);
    } catch (OZ::Exception& ex) {
      caught(ex);
    }
  }

public:
  void show(cv::Mat& mat)
  {
    if (!mat.empty()) {
      image = mat;
      Pixmap pixmap = getPixmap();

      if (pixmap == None) {
       createPixmap();
      }
      ScrolledImageView::display();
    }
  }

  void setMat(cv::Mat& mat) 
  {
    clear();
    image = mat;
  }

public:
  OpenCVScrolledImageView(View* parent, const char* name, Args& args)
    :ScrolledImageView(parent, name, args)
   ,keepScrollBarPosition(false)
  {
  }

  ~OpenCVScrolledImageView() 
  {
  }
  
  void setKeepScrollBarPosition(bool flag)
  {
    keepScrollBarPosition = flag;
  }

  cv::Mat readImage(const char* filename, int loadingFlag)
  {
    ModuleFileName module;
    const StringT<char> fullpath = module.getFullpath(filename);
    std::string fname((const char*)fullpath);
    cv::Mat mat =  cv::imread(fname, loadingFlag);
    if (mat.empty()) {
      throw IException("Failed to imread :%s", filename);
    } else {
      setMat(mat);     
      return mat;
    }
  }

  virtual void setScrollRange(int scalingRatio)
  {
    try {
      if (scalingRatio < 1) {
       scalingRatio = 10;
      }
      float ratio = (float)scalingRatio/100.0f;

      Dimension hsize, vsize;
      getScrollBarSliderSize(hsize, vsize);
      float fhsize = (float)hsize * ratio;
      float fvsize = (float)vsize * ratio;

      setScrollBarMaximum(fhsize, fvsize); //2017/06/10
      setScrollBarSliderSize((int)fhsize, (int)fvsize);
    } catch (Exception& ex) {

    }
  }

  virtual void scaleImageKeepingThumbPosition(cv::Mat& originalImage, 
                   cv::Mat& scaledImage, int scalingRatio)
  {
    try {
      if (scalingRatio < 1) {
       scalingRatio = 10;
      }
      float ratio = (float)scalingRatio/100.0f;

      cv::resize(originalImage, scaledImage, cv::Size(),
                            ratio, ratio);
      //invalidate();
     } catch (cv::Exception& ex) {
         ;
     }
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
      Dimension hsize, vsize;
      getScrollBarSliderSize(hsize, vsize);
      float fhsize = (float)hsize * ratio;
      float fvsize = (float)vsize * ratio;

      setScrollBarMaximum(fhsize, fvsize); //2017/06/10
      setScrollBarSliderSize((int)fhsize, (int)fvsize);

      invalidate();
      refresh();
      //
     } catch (cv::Exception& ex) {
         ;
     }
  }

  void invalidate()
  {
    clear(); 
  }

  void refresh()
  {
    unmanage();
    manage();
    sendExposeEvent();
  }

  virtual void render(cv::Mat& mat)
  {
  }
};

}


