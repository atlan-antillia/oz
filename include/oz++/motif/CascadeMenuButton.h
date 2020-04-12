/******************************************************************************
 *
 * Copyright (c) 2015-2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  CascadeMenuButton.h
 *
 *****************************************************************************/

//2017/05/20 Added the following method.
//  void recreatePulldownMenu(MenuBar* menubar, const char* name)

#pragma once

#include <oz++/motif/MenuBar.h>
#include <oz++/motif/MenuItem.h>
#include <oz++/motif/CascadeButton.h>
#include <oz++/motif/PulldownMenu.h>

namespace OZ {

class CascadeMenuButton :public CascadeButton {

private:
   SmartPtr<PulldownMenu> pulldown;

public:
  CascadeMenuButton(MenuBar* menubar, const char* name, Args& args)
  :CascadeButton()
  {
    CascadeMenuButton::create(menubar, name, args);
  };

  CascadeMenuButton()
  :CascadeButton()
  {
  }

  void create(MenuBar* menubar, const char* name, Args& args)
  {
    CascadeButton::create(menubar, name, args);

    if (args.has(XmNlabelString) == FALSE) {
      CompoundString cs(name);
      set(XmNlabelString, cs);
    }

    //Create an empty pulldown menu.
    Args ar;
    pulldown = new PulldownMenu(menubar, name,  ar);
    set(XmNsubMenuId, pulldown);
  }

  ~CascadeMenuButton()
  {
  }

public:
  //2017/05/29 
  void recreatePulldownMenu(MenuBar* menubar, const char* name)
  {
    if (pulldown) {
      set(XmNsubMenuId, (XtArgVal)NULL);
      pulldown = NULL;
      Args ar;
      pulldown = new PulldownMenu(menubar, name,  ar);
      set(XmNsubMenuId, pulldown);
    }
  }

  void addItems(MenuItem* items, int itemsCount, 
                  View* owner, Callback callback)
  {
    if (items != NULL && itemsCount >0) {
      pulldown -> addItem(items, itemsCount,
               owner, callback);
    } else {
      throw IException("Invalid argument");
    }
  }
};

}

