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
 *  OpenCVVideoWriterThread.h
 *
 *****************************************************************************/

// 2017/12/28

#pragma once

#include <oz++/Thread.h>
#include <oz++/opencv/OpenCVObject.h>
#include <oz++/opencv/OpenCVVideoWriter.h>
#include <oz++/Pair.h>
#include <oz++/StringT.h>

namespace OZ {
    
class OpenCVVideoWriterThread :public Thread {
private:
  static const int  DEFAULT_INTERVAL_MSEC = 30;

  Application*    application;
  OpenCVVideoCapture& videoCapture;  //Opened videoCapture in a main thread.

  cv::Mat&             videoFrame;   // Read frame in a main thread.

  OpenCVVideoWriter& videoWriter;
  int              writingInterval; //msec
 
public:
  OpenCVVideoWriterThread(Application* applet,
               OpenCVVideoCapture& capture,
               OpenCVVideoWriter& writer,
               int interval)
  :Thread(),
  application(applet),
  videoCapture(capture),
  videoWriter(writer),
  writingInterval(interval)
  {
    if (interval <5 || interval > 1000) {
      interval = DEFAULT_INTERVAL_MSEC;
    }
    if (applet == NULL) {
      throw IException("Invalid  applet parameter");
    }
  }

  ~OpenCVVideoWriterThread()
  {
    terminate();
  }
  
  //Define your own filter method in a subclass derived from this class.
  virtual void filter(cv::Mat& in, cv::Mat& out)
  {
    //Default filtering 
    out = in;  //Shallow copy
  } 

  //Thread main procedure which can be started by Thread::start method.
  virtual void run()
  {
    int microsec = capturingInterval*1000;
    while( looping ) {
      if (application->processPendingEvent()) {
       ;;//Pending XEvents have been processed if there were in a queue.

      } else {
        if (view && videoCapture.isOpened() ) {
          //Read a video frame from the videoCapture object,
          // into the videoFrame allocated in the main thread.
          if (videoCapture.read(videoFrame)) {
            cv::Mat filtered;
            //Call virtual filter method;
            filter(videoFrame, filtered);

            videoWriter.write(filtered); 

          } else {
            printf("Failed to read videoFile\n");
            //Probably, we should break this while loop
            break;
          }
        }
      }
      usleep(microsec);
    }
  }

  void terminate()
  {
    try {
      videoWriter.close();
      looping = false;
    } catch (Exception& ex) {
    }
  }
};

}


