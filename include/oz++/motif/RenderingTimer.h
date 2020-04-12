/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      RenderingTimer.h
 *
 *****************************************************************************/

//2016/03/27 Simple renderingTimer thread classs.
// This thread sends the Expose event to the view periodically
// specified by interal(msec).
 
#pragma once

#include <oz++/Thread.h>
#include <oz++/motif/View.h>

namespace OZ {
const char* XmNrenderingInterval = "renderingInterval";

class RenderingTimer :public Thread {
private:
  bool   looping;
  View*  view;
  int    interval; //msec 
public:
  RenderingTimer(View* view, int msec)
  :looping(true),
   view(view),
   interval(msec)
  {
    if (view == NULL) {
      throw IException("Invalid argument. view is NULL.");
    }
  }
  
public:
  ~RenderingTimer()
  {
    terminate();
    wait(); //2019/04/05
  }

public:
  void terminate()
  {
    view    = NULL;
    looping = false;
  }
  
public:
  void run()
  {
     int microsec = interval*1000;

     while(looping) {
       //Sleep for microsec.
       usleep(microsec);
       if (view) {
         //Send Expose event to the view.
         view -> sendExposeEvent();
         view -> flush();
       }
     }
  }
};

}

