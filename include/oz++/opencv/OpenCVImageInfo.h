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
 *  OpenCVImageInfo.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/Exception.h>
#include <oz++/motif/View.h>
#include <oz++/motif/DC.h>
#include <oz++/opengl/OpenGLImageInfo.h>
#include <oz++/opencv/OpenCVObject.h>

namespace OZ {

class OpenCVImageInfo {
public:
  OpenCVImageInfo()
  {
  }

  void getImageInfo(cv::Mat& mat, OpenGLImageInfo& imageInfo, 
              bool flip = false)
  {
    cv::Mat bgra = mat;
    if ( mat.channels() == 1) {
      // if mat is grayscale, we convert  to bgra 
      cv::cvtColor(mat, bgra, cv::COLOR_GRAY2BGR);
    }

    IplImage image =  cvIplImage(bgra); // 2020/02/10 bgra;

    //Check Iplimage depth field.
    //Is the depth value 8?
    dprintf("fromMatToXImage depth=%d channels=%d\n", 
	image.depth, image.nChannels);

    cv::Mat cmat;
    if (image.depth != IPL_DEPTH_8U) {
      bgra.convertTo(cmat, CV_8U);
      image =  cvIplImage(cmat); //2020/02/10 cmat;
    }

    //assert(image.depth ==IPL_DEPTH_8U);
    if (image.depth != IPL_DEPTH_8U) {
      throw IException("Unsupported image depth: %d", image.depth);
    }
    int depth     = image.depth*image.nChannels;

    int width     = image.width;

    int height    = image.height;
    
    int widthStep = image.widthStep;

    uint32_t* rgb = new uint32_t[width * height];

    const char* p =  image.imageData;

    for(int i = 0; i < height; i++){
      int step = widthStep * i;

      for(int j = 0; j < width; j++){
        int w = step + j * 3;

	//image.imagetData is in BGR order
	 uchar b = *(p + w    );
         uchar g = *(p + w + 1);
         uchar r = *(p + w + 2);
		    		 
	 uint32_t pixel =  (r << 16) + (g << 8) + b;
         if (flip) {
	   rgb[j + (height-i-1) * width ] = pixel;
         } else {
	   rgb[j + i* width ] = pixel;
         }
       } //for
     } //for

     imageInfo.depth    = depth; //image.depth* image.nChannels; 
     imageInfo.channels = image.nChannels;
     imageInfo.width  = width;
     imageInfo.height = height;

     imageInfo.widthStep = widthStep;

     imageInfo.imageSize = width * height * sizeof(uint32_t); //Image byte size
     imageInfo.imageData = rgb;
  }
};

}

