/******************************************************************************
 *
 * Copyright (c) 2015  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  TimerThread.cpp
 *
 *****************************************************************************/


#include <oz++/TimerThread.h>

namespace OZ {

class ClockThread :public Thread {
private:
  TimerThread* timer;
  int count;

private:
  void timeOut(CommonObject* object)
  {
     CharString* time = (CharString*)object;
     printf("%s\n", (char*)(*time));
  }

public:
  ClockThread(int second, int count)
  :Thread(),
  timer(NULL),
  count(count)
  {
    timer = new TimerThread(second);
    timer -> setTimeout(this, (Timeout)&ClockThread::timeOut);
  }

public:
  ~ClockThread()
  {
    timer -> stop();
    timer -> wait();
  }

public:
  void run() 
  {
    timer -> start();
    int n = 0;
    while (n++ < count) {
      sleep(1);
    }
  }
};
}

//
int main(int argc, char* argv[])
{
  try {
    ClockThread clock(3, 20);
    clock.start();
    clock.wait();
 
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}
