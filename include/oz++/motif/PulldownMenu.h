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
 *      PulldownMenu.h
 *
 *****************************************************************************/

//2015/08/10 Updated to support cascading pulldown menus.
//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/Menu.h>
#include <oz++/motif/MenuItem.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/LinkedList.h>

namespace OZ {
  
class PulldownMenu :public Menu {
private:
  //To manage subPulldownMenu in LinkedList object.
  //This is used for automatic Garbage Collection.
  LinkedList subPulldownList;

public:
  PulldownMenu(View* parent, const char* name, Args& args) 
  :Menu()
  {
    PulldownMenu::create(parent, name, args);
  }

  PulldownMenu()
  :Menu()
  {
  }

  void create(View* parent, const char* name, Args& args) 
  {
    if (parent == NULL) {
      throw IException("Invalid parent parameter.");
    }
    Widget pulldownm = XmCreatePulldownMenu(parent->getWidget(),
                 (char*)name, args.getArgList(),
                       args.count() );
    if (pulldownm == NULL) {
      throw IException("Failed to XmCreatePulldownMenu.");
    }
    setWidget(pulldownm);
  }
  
  virtual  void addItem(MenuItem* item, int num, View* view, Callback callback)
  {
    for(int i = 0; i<num; i++) {
      View* kid = Menu::addItem(item,  view, callback);
      addKid(kid);

      if(item->subItem) {
        Args ar;
        PulldownMenu* subPulldown = new PulldownMenu(this, "", ar);
        kid -> set(XmNsubMenuId, subPulldown); 

        //For GC
        subPulldownList.add(subPulldown);

        subPulldown -> addItem(item->subItem, item->subItemsCount, view, callback);
      }
      item++;
    }
  }

};

}

