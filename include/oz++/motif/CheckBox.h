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
 *      CheckBox.h
 *
 *****************************************************************************/

//2017/10/10 Updated.

#pragma once

#include <oz++/motif/RowColumn.h>
#include <oz++/motif/ToggleButtonGadget.h>

namespace OZ {
  
class CheckBox :public RowColumn {
private:
  int num;
  ToggleButtonGadget** kids;
  
public:
  CheckBox(View* parent, const char* name, Args& args)
  :RowColumn()
  {
    CheckBox::create(parent, name, args);
  }

  CheckBox()
  :RowColumn()
  {
  }

  ~CheckBox()
  {
    delete [] kids;
  }

  void create(View* parent, const char* name, Args& args)
  {
    RowColumn::create(parent, name,
        args.set(XmNradioBehavior, False)
            .set(XmNpacking, XmPACK_COLUMN)
            .set(XmNisHomogeneous, TRUE)
            .set(XmNentryClass,
          (XtArgVal)xmToggleButtonGadgetClass));
  
    Args ar;
    char string[100];
    XmString* xmsTable = (XmString*) args.get(XmNbuttons);

    num  = (int) args.get(XmNbuttonCount);
    kids = new ToggleButtonGadget*[num];
    for(int i = 0; i<num; i++) {
      sprintf(string, "button_%d", i);
      ar.reset();
      ar.set(XmNlabelString, (XtArgVal)xmsTable[i]);
      kids[i] = new ToggleButtonGadget(this, string, ar);
      kids[i] -> manage();
    }
  }
 
  Action* addCallbackToKids(const char* name, View* view, Callback callback, 
        XtPointer data)
  {
    for(int i = 0; i< num; i++) {
      kids[i] -> addCallback(name, view, callback, to_voidptr(i));
    }
    return NULL; 
  }

  Boolean getState(int nth)
  {
    Boolean state = False;
    if(nth >= 0 && nth < num) {
      kids[nth] -> get(XmNset, (XtArgVal)&state);
    }
    return state;
  }

};

}

