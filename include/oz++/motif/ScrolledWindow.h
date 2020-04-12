/******************************************************************************
 *
 * Copyright (c) 2014-2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      ScrolledWindow.h
 *
 *****************************************************************************/

//2017/10/10 Updated to use create method.

#pragma once

#include <oz++/motif/Manager.h>
#include <Xm/ScrolledW.h>

namespace OZ {

class ScrolledWindow :public Manager {
public:
  ScrolledWindow(View* parent, const char* name, Args& args)
  :Manager(parent, name, xmScrolledWindowWidgetClass, args)
  {
  }

  ScrolledWindow()
  :Manager()
  {
  }

  virtual void create(View* parent, const char* name, Args& args)
  {
    Manager::create(parent, name, xmScrolledWindowWidgetClass, args);
  }  

  void setArea(Widget hscroll, Widget vscroll, Widget work)
  {
    XmScrolledWindowSetAreas(getWidget(), hscroll, vscroll, work);
  }

public:
  virtual void reshape(int x, int y, int w, int h)
  {
    View::reshape(x, y, w, h);

    Widget hsb = (Widget)get(XmNhorizontalScrollBar);
    Widget vsb = (Widget)get(XmNverticalScrollBar);
    Dimension bw = 0;
    get(XmNborderWidth, (XtArgVal)&bw);
    Dimension st = 0;
    get(XmNshadowThickness, (XtArgVal)&st);
    Dimension ht = 0;
    get(XmNhighlightThickness, (XtArgVal)&ht);
    View hsbv(hsb);
    View vsbv(vsb);

    int ww = View::width();
    int hh = View::height();

    //Get real visble region: ww and hh.
    if (vsbv.isManaged()) {
      ww -=  vsbv.View::width();
    } 
    if (hsbv.isManaged()) {
      hh -=  hsbv.View::height();
    }

    Widget clip = (Widget)get(XmNclipWindow);
    if (clip) {
      //printf("ScrolledWindow clip w=%d h=%d ww=%d hh=%d bw=%d st=%d ht=%d\n",
      //         w, h, ww, hh, bw, st, ht);
      XtMoveWidget(clip, bw+st, bw+st);
      XtResizeWidget(clip, ww-2*bw-2*st-ht, hh-2*bw-2*st-ht, 0);
    }
  }
};

}

