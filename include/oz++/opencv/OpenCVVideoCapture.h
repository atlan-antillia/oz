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
 *  OpenCVVideoCapture.h
 *
 *****************************************************************************/

// 2017/08/10
// On cv::VideoCapture
// See: http://docs.opencv.org/trunk/d8/dfe/classcv_1_1VideoCapture.html

#pragma once

#include <oz++/opencv/OpenCVObject.h>
#include <opencv2/video/background_segm.hpp>

namespace OZ {
  
class OpenCVVideoCapture :public OpenCVObject {
private:
  cv::VideoCapture videoCapture;

  static const int  DEFAULT_VIDEO_DEVICE_INDEX = 0; 
  static const int  DEFAULT_FPS = 10;
  static const int  VIDEO_FRAME_MIN_WIDTH  = 320;
  static const int  VIDEO_FRAME_MIN_HEIGHT = 240;

  int    deviceIndex;
  int    fps;
  int    frameWidth;
  int    frameHeight;

public:
  //Constructor

  OpenCVVideoCapture()
  :deviceIndex(DEFAULT_VIDEO_DEVICE_INDEX)
   ,fps(DEFAULT_FPS)
   ,frameWidth(VIDEO_FRAME_MIN_WIDTH)
   ,frameHeight(VIDEO_FRAME_MIN_HEIGHT)
  {    
  }

  ~OpenCVVideoCapture()
  {
    close();
  }

  int getDeviceIndex()
  {
    return deviceIndex;
  }

  void setDeviceIndex(int index)
  {
    deviceIndex = index;
  }

  void set(int devIndex, int framew, int frameh, int f)
  {
    deviceIndex = devIndex;
    if (framew >0)
    frameWidth  = framew;
    if (frameh >0) 
    frameHeight = frameh;
    if (f > 0) 
     fps = f;
  }

  void open()
  {
    printf("1 Try to open %d\n", deviceIndex);
    if (videoCapture.open(deviceIndex)) {
      if (!isOpened()) {
        throw IException("Failed to open deviceIndex=%d", deviceIndex);
      }
     /* 
      printf("1 Try to setFrameSize %d %d\n", frameWidth, frameHeight);
      if (!setVideoFrameSize(frameWidth, frameHeight)) {
        throw IException("Failed to setVideoFrameSize(%d, %d)",
             frameWidth, frameHeight);   
      }
      printf("open %d %d\n", frameWidth, frameHeight);
     
      printf("1 Try to setFps %d\n", fps);
      if (!setFps(fps)) {
        throw IException("Faile to setFps %d", fps);
      }
      */
    }
  }

  void open(int devIndex, int framew, int frameh)
  {
    deviceIndex = devIndex;
    frameWidth  = framew;
    frameHeight = frameh;

    open();
  }
  
  void open(int deviceIndex)
  {
    printf("2 Try to open %d\n", deviceIndex);
    if (videoCapture.open(deviceIndex)) {
      if (!isOpened()) {
        throw IException("Failed to open deviceIndex=%d", deviceIndex);
      }
    }
/*
    printf("2 Try to setFrameSize %d %d\n", frameWidth, frameHeight);
    if (!setVideoFrameSize(frameWidth, frameHeight)) {
        throw IException("Failed to setVideoFrameSize(%d, %d)",
             frameWidth, frameHeight);   
    }

    printf("2 Try to setFps %d\n", fps);
    if (!setFps(fps)) {
      throw IException("Faile to setFps %d", fps);
    }
*/
  }

  //Open a video file
  void open(const char* filename)
  {
    if (videoCapture.open(filename)) {
      if (!isOpened()) {
        throw IException("Failed to open file=%d", filename);
      }
    }
  }

  void close()
  {
    if (isOpened()) {
      videoCapture.release();
    }
  }
  
  bool read(cv::Mat& frame)
  {
    bool rc = false;

    if (isOpened() ) {
      do {
        rc = videoCapture.read(frame);
      } while(frame.empty());

    }
    return rc;
  }

  bool isOpened()
  {
    return videoCapture.isOpened();
  }

  double getProperty(int prop)
  {
    double value = 0.0;
    if (isOpened()) {
       value = videoCapture.get(prop);
    }
    return value;
  }

  bool setProperty(int prop, double value)
  {
    bool rc = false;
    if (isOpened()) {
      rc = videoCapture.set(prop, value);
    }
    return rc;
  }


  cv::VideoCapture& getVideoCapture()
  {
    return videoCapture;
  }
  
  bool getVideoFrameSize(int& w, int& h)
  {
    bool rc = false;
    w = 0;
    h = 0;
    if (isOpened()) {
      w = (int)videoCapture.get(CAP_PROP_FRAME_WIDTH);
      h = (int)videoCapture.get(CAP_PROP_FRAME_HEIGHT);
      rc = true;
    }
    return rc;
  }

  bool setFps(int fps)
  {
    bool rc = false;
    if (isOpened()) {
      if (fps <1) {
        fps = 5;
      }
      if (fps >60) {
        fps = 30;
      }
      videoCapture.set(CAP_PROP_FPS,  (double)fps);
      rc = false;
    }
    return rc;
  }

  bool setVideoFrameSize(int w, int h)
  {
    bool rc = false;
    if (isOpened()) {
      if (w < VIDEO_FRAME_MIN_WIDTH ) {
       w = VIDEO_FRAME_MIN_WIDTH; 
      }
      if (h < VIDEO_FRAME_MIN_HEIGHT ) {
       h = VIDEO_FRAME_MIN_HEIGHT; 
      }
      if (!videoCapture.set(CAP_PROP_FRAME_WIDTH,  (double)w)) {
        return false; 
      }
      if (!videoCapture.set(CAP_PROP_FRAME_HEIGHT, (double)h)) {
        return false;
      }
      return true;
    }
    return rc;
  }
};

}


