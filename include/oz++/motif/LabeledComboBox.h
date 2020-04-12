/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      LabeledComboBox.h
 *
 *****************************************************************************/

//2017/10/15 Updated to use create method.

#pragma once

#include <oz++/motif/LabeledFrame.h>
#include <oz++/motif/DropdownComboBox.h>

namespace OZ {
  
class LabeledComboBox :public LabeledFrame {
private:
  SmartPtr<DropdownComboBox> comboBox;

public:
  LabeledComboBox(View* parent, const char* name, Args& args)
  :LabeledFrame()
  {
    LabeledComboBox::create(parent, name, args);
  }

  LabeledComboBox()
  :LabeledFrame()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    LabeledFrame::create(parent, name, args); 
   
    Args ar;
    int width = 180;
    if (args.has(XmNwidth)) {
      width = (int)args.get(XmNwidth);
    }
    ar.set(XmNwidth, width);
    if (args.has(XmNitems) && args.has(XmNitemCount) ) { 
      XtArgVal csl = (XtArgVal)args.get(XmNitems);
      ar.set(XmNitems, csl);
      int count = (int)args.get(XmNitemCount);
      ar.set(XmNitemCount, count);
    }
    if (args.has(XmNvisibleItemCount)){
       int visibleCount = (int)args.get(XmNvisibleItemCount);

       int count = (int)args.get(XmNitemCount);

       if (visibleCount >10 && visibleCount > count) {
         visibleCount = count;
       }
       ar.set(XmNvisibleItemCount, visibleCount);
    }
    //2017/05/29
    if (args.has(XmNselectedPosition)){
       int position = (int)args.get(XmNselectedPosition);
       ar.set(XmNselectedPosition, position);
    }
    if (args.has(XmNselectedItem)){
       XtArgVal csl = (int)args.get(XmNselectedItem);
       ar.set(XmNselectedItem, csl);
    }
    
    comboBox = new DropdownComboBox(this, "", ar);
  }
	
  DropdownComboBox* getComboBox()
  {
     return comboBox;
  }
	
  ~LabeledComboBox()
  {
  }

  void addCallback(const char* name, CommonObject* object,
	Callback callback, void* data)
  {
     comboBox->addCallback(name, object, callback, data);
  }

  int getSelectedPosition()
  {
     return (int)comboBox->get(XmNselectedPosition);
  }

  void setSelectedPosition(int index)
  {
     comboBox->set(XmNselectedPosition, index);
  }
};

}

