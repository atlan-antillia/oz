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
 *      TabbedWindow.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/Manager.h>
#include <Xm/TabStack.h>

namespace OZ {

class TabbedWindow :public Manager {
public:
  TabbedWindow(View* parent, const char* name, Args& args)
  :Manager(parent, name, xmTabStackWidgetClass, args)
  {
  }

  TabbedWindow()
  :Manager()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Manager::create(parent, name, xmTabStackWidgetClass, args);
  }

  Widget getSelectedTab()
  {
    return XmTabStackGetSelectedTab(getWidget());
  }
 
  void selectTab(View* view, Boolean notify)
  {
    if (view) {
      XmTabStackSelectTab(view->getWidget(), notify);
    }
  }
 
  void selectTab(Widget widget, Boolean notify)
  {
    XmTabStackSelectTab(widget, notify);
  }
 
  Widget indexToWidget(int index)
  {
    return XmTabStackIndexToWidget(getWidget(), index);
  }
 
  Widget xyToWidget(int x, int y)
  {
    return XmTabStackXYToWidget(getWidget(), x, y);
  }
};

}

