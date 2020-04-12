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
 *      MainWindow.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/Manager.h>
#include <Xm/MainW.h>
//2015/01/18 Added the following header files related on Menu classes.
#include <oz++/motif/MenuItem.h>
#include <oz++/motif/MenuBar.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/CascadeButton.h>
#include <oz++/motif/PulldownMenu.h>

namespace OZ {
  
class MainWindow :public Manager {
public:
  MainWindow(View* parent, const char* name, Args& args)
  :Manager(parent, name, xmMainWindowWidgetClass, args) 
  {
  }

  MainWindow()
  :Manager()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Manager::create(parent, name, xmMainWindowWidgetClass, args); 
  }
  
  Widget sep1() 
  {
    return XmMainWindowSep1(getWidget());
  }
  
  Widget sep2() 
  {
    return XmMainWindowSep2(getWidget());
  }
  
  Widget sep3() 
  {
    return XmMainWindowSep3(getWidget());
  }
  
  void  setAreas(Widget menubar, Widget command,
          Widget hscroll,  Widget vscroll,
          Widget work) 
  {
    XmMainWindowSetAreas(getWidget(), 
      menubar, command,
      hscroll, vscroll, 
      work);
  }
};

}
