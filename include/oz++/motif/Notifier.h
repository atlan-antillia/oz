/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      Notifier.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/Label.h>
#include <oz++/motif/Color.h>
#include <oz++/CharString.h>

namespace OZ {

const char* XmNnotifierData    = "notifierData";
const char* XmNenterBackground = "enterBackground";
const char* XmNenterForeground = "enterForeground";
const char* XmNleaveBackground = "leaveBackground";
const char* XmNleaveForeground = "leaveForeground";

 
class Notifier: public Label {
private:
  CharString notifierData;
 
  Pixel leaveBackground;
  Pixel leaveForeground;

  Pixel enterBackground;
  Pixel enterForeground;

  void enterWindow(Event& event)
  {
    set(XmNbackground, enterBackground);
    set(XmNforeground, enterForeground);
  }

  void leaveWindow(Event& event)
  {
    set(XmNbackground, leaveBackground);
    set(XmNforeground, leaveForeground);
  }

public:
  Notifier(View* parent, const char* name, Args& args)
  :Label()
  {
    Notifier::create(parent, name, args);
  }

  Notifier()
  :Label()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Label::create(parent, name, 
                      args.set(XmNbackground, parent->whitePixel())
                          .set(XmNrecomputeSize, False) ) ;
  
    Color color(this);
    if (args.has(XmNenterBackground)) {
      enterBackground = color.allocNamedColor((const char*)args.get(XmNenterBackground));
    } else {
      enterBackground = color.allocNamedColor("navy");
    }
    if (args.has(XmNenterForeground)) {
      enterForeground = color.allocNamedColor((const char*)args.get(XmNenterForeground));
    } else{
      enterForeground = color.allocNamedColor("white");
    }
    if (args.has(XmNnotifierData)) {
      notifierData = (const char*)args.get(XmNnotifierData); 
    }
    leaveBackground = color.whitePixel();
    leaveForeground = color.blackPixel();

    addEventHandler(EnterWindowMask, this,
          (Handler)&Notifier::enterWindow, NULL);

    addEventHandler(LeaveWindowMask, this,
          (Handler)&Notifier::leaveWindow, NULL);
  }

  ~Notifier()
  {
  }
 
  const char* getNotifierData() 
  {
    return (const char*)notifierData;
  } 
};
 
}

