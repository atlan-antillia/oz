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
 *  OpenCVVideoCaptureView.h
 *
 *****************************************************************************/

// 2017/08/10
// On cv::VideoCapture
// See: http://docs.opencv-3.0.org/3.1.0/d8/dfe/classcv_1_1VideoCapture.html

#pragma once

#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVVideoCapture.h>

namespace OZ {
  
class OpenCVVideoCaptureView :public OpenCVMainView {
private:
  static const int INVALID_DEVICE_INDEX       = -1;

  static const int DEFAULT_VIDEO_DEVICE_INDEX =  0;  


  OpenCVVideoCapture  videoCapture;
  
public:
  //Constructor
  OpenCVVideoCaptureView(OpenCVApplication& applet, 
                         const char* caption,
                         Args& args)
  :OpenCVMainView(applet, caption, args) 
  {    
    int fps         = (int)args.get(XmNvideoFps);
    int deviceIndex = (int)args.get(XmNvideoDeviceIndex);
    int frameWidth  = (int)args.get(XmNvideoFrameWidth);
    int frameHeight = (int)args.get(XmNvideoFrameHeight);

    videoCapture.set(deviceIndex, frameWidth, frameHeight, fps);

    bool autoStart = (bool)args.get(XmNcaptureAutoStart);
    if (autoStart) {
      startCapture();
    }
  }

  ~OpenCVVideoCaptureView()
  {
    closeVideoDevice();
  }

  void setDeviceIndex(int index)
  {
    videoCapture.setDeviceIndex(index);
  }

  void closeVideoDevice()
  {
    try {
      videoCapture.close();
    } catch (...) {
      //
    }
    setTitle(getAppName());   
  }
  
  virtual void startCapture()
  {
    int deviceIndex = videoCapture.getDeviceIndex();
    try {

      videoCapture.open();
 
      char title[PATH_MAX];
      sprintf(title, "Capturing deviceIndex=%d  - %s", 
                        deviceIndex, getAppName());
      setTitle(title);
      
    } catch (Exception& ex) {
      caught(ex);
    }
  }
  
  void openVideoFile(const char* filename, int fps, int width, int height)
  {
    try {
      videoCapture.open(filename);
      videoCapture.setFps(fps),
      videoCapture.setVideoFrameSize(width, height);

      char title[PATH_MAX];
      sprintf(title, "Opened a file=%s  - %s",
                        filename, getAppName());
      setTitle(title);
    } catch (Exception& ex) {
      caught(ex);
    }
  }

  void openVideoFile(const char* filename)
  {
    try {
      videoCapture.open(filename);

      char title[PATH_MAX];
      sprintf(title, "Opened a file=%s  - %s",
                        filename, getAppName());
      setTitle(title);
    } catch (Exception& ex) {
      caught(ex);
    }
  }

  virtual void stopCapture()
  {
    closeVideoDevice();
  }
  
  bool isVideoDeviceOpened()
  {
    return videoCapture.isOpened();
  }

  bool readVideoFrame(cv::Mat& frame)
  {
    return videoCapture.read(frame);
  }

  cv::VideoCapture& getVideoCapture()
  {
    return videoCapture.getVideoCapture();
  }
};

}


