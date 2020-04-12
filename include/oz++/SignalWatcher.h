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
 *  SignalWatcher.h
 *
 *****************************************************************************/

#include <oz++/SignalDescriptor.h>
#include <oz++/Thread.h>
#include <oz++/SignalSet.h>
#include <oz++/SignalParser.h>

namespace OZ {

class SignalWatcher: public Thread {
private:
  bool looping;  
  SignalDescriptor signalDescriptor;
  SignalParser  parser;
  
public:
  SignalWatcher(int fd, SignalSet& set, int flag=0)
  :Thread(),
  looping(true),
  signalDescriptor(fd, set, flag) 
  {
  }

public:
  
  void setCallback(CommonObject* object, Signaled signaled)
  {
    parser.setCallback(object, signaled);
  }

  virtual void run()
  {
    while (looping) {
     signalDescriptor.read(parser);
    }
  }

  void stop()
  {
    looping = false;
    signalDescriptor.close();
  }
};
}

