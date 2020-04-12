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
 *  OpenCVVideoProperties.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/opencv/OpenCVObject.h>

namespace OZ {

class OpenCVVideoProperties {

public:
  static const int PROP_COUNT = 18;
  double values[PROP_COUNT];

public:
  OpenCVVideoProperties(cv::VideoCapture& cap)
  {
    for (int i = 0; i<PROP_COUNT; i++) {
      values[i] = 0.0;
    }
    int i = 0;

    int propIds[PROP_COUNT] = {
      CV_CAP_PROP_POS_MSEC,
      CV_CAP_PROP_POS_FRAMES,
      CV_CAP_PROP_POS_AVI_RATIO,
      CV_CAP_PROP_FRAME_WIDTH,
      CV_CAP_PROP_FRAME_HEIGHT,
      CV_CAP_PROP_FPS,
      CV_CAP_PROP_FOURCC,
      CV_CAP_PROP_FRAME_COUNT,
      CV_CAP_PROP_FORMAT,
      CV_CAP_PROP_MODE,
      CV_CAP_PROP_BRIGHTNESS,
      CV_CAP_PROP_CONTRAST,
      CV_CAP_PROP_SATURATION,
      CV_CAP_PROP_HUE,
      CV_CAP_PROP_GAIN,
      CV_CAP_PROP_EXPOSURE,
      CV_CAP_PROP_CONVERT_RGB,
    };

    try {
      //cv::throw_if_error();
      for (int i = 0; i<PROP_COUNT; i++) {
      values[i] = cap.get(propIds[i]);
      }
    } catch (...) {
    }
  }

  double* get(int& count)
  {
    count = PROP_COUNT;
    return values;    
  }

};

}

