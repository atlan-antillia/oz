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
 *  OpenCVVideoReaderThread.h
 *
 *****************************************************************************/

// 2017/12/25
// On cv::VideoCapture
// See: http://docs.opencv.org/3.1.0/d8/dfe/classcv_1_1VideoCapture.html

#pragma once

#include <oz++/StringT.h>
#include <oz++/Thread.h>
#include <oz++/motif/Application.h>
#include <oz++/opencv/OpenCVVideoCapture.h>
#include <oz++/opencv/OpenCVScrolledImageView.h>

namespace OZ {
  
class OpenCVVideoReaderThread :public Thread {
private:
  static const int  DEFAULT_INTERVAL_MSEC = 30;

  OpenCVScrolledImageView*   view;  //Shallow copy

  OpenCVVideoCapture&  videoCapture; //Opened videoCapture in a main thread.

  int               capturingInterval; //milisecond
  
  bool              looping;
  
public:
  //Constructor
  OpenCVVideoReaderThread(
            OpenCVScrolledImageView* renderv, 
            OpenCVVideoCapture&      capture,
            int interval = DEFAULT_INTERVAL_MSEC)
  :Thread(),
  view(renderv),
  videoCapture(capture),
  capturingInterval(interval),
  looping(true)
  {    
    if (interval <5 || interval > 1000) {
      interval = DEFAULT_INTERVAL_MSEC;
    }
    if (renderv == NULL) {
      throw IException("Invalid  renderv parameter");
    }
  }
  
  ~OpenCVVideoReaderThread()
  {
    terminate();
  }

  //Thread main procedure which can be started by Thread::start method.
  virtual void run()
  {
    int microsec = capturingInterval*1000;
    while( looping ) {
      if (view && videoCapture.isOpened() ) {
        //Read a video frame from the videoCapture object, 
        // into the cv::Mat frame.
        cv::Mat frame;
        if (videoCapture.read(frame)) {
          if (view) {
            view->realize();
            view->render(frame);

            //The following line will call XFlush.
            view->flush();
          }
        } else {
          printf("Failed to read video\n");
          //We should break this while loop
          break;
        }
      }
      usleep(microsec);
    }
  }

  void terminate()
  {
    try {
      view = NULL;
      looping = false;
      usleep(100*1000);
      videoCapture.close();
    } catch (Exception& ex) {
    }
  }
};

}

