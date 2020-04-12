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
 *      ScrolledListBox.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/ScrolledWindow.h>
#include <oz++/motif/ListBox.h>

namespace OZ {
  
class ScrolledListBox :public ScrolledWindow {
private:
  SmartPtr<ListBox> list;

public:
  ScrolledListBox(View* parent, const char* name, Args& args)
  :ScrolledWindow()
  {
    ScrolledListBox::create(parent, name, args);
  }

  ScrolledListBox()
  :ScrolledWindow()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    ScrolledWindow::create(parent, name, args);
  
    list = new ListBox(this, "list", args);   
  }
  
  ~ScrolledListBox() 
  {
  }
  
  void manage() 
  {
    View::manage();
    if (list) {
      list->manage();  
    }
  }
  
  ListBox* getList() 
  {
    return list;
  }  
	
  void clear()
  {
    if (list) {
      list -> deleteAllItems();		
    }
  }
};

}


