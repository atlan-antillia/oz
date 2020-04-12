/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      Menu.h
 *
 *****************************************************************************/

//2015/08/10 Modified to manage kids by LinkedList.

#pragma once

#include <oz++/motif/ShellView.h>
#include <oz++/motif/CascadeButton.h>
#include <oz++/motif/CascadeButtonGadget.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/LabelGadget.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/PushButtonGadget.h>
#include <oz++/motif/Separator.h>
#include <oz++/motif/SeparatorGadget.h>
#include <oz++/motif/MenuItem.h>
#include <Xm/MenuShell.h>
#include <oz++/LinkedList.h>

namespace OZ {
  
enum {  
  CASCADEBUTTON, CASCADEBUTTONGADGET, 
  LABEL,         LABELGADGET, 
  PUSHBUTTON,    PUSHBUTTONGADGET,
  SEPARATOR,     SEPARATORGADGET, 
  TOGGLEBUTTON,  TOGGLEBUTTONGADGET
};


class Menu :public ShellView {
private:
  LinkedList items;

protected:
  Menu() { }

  View* addItem(MenuItem* item,
    View* view, Callback callback)
  {
    Args  args;
    View* kid = NULL;
    int   type = item -> type;
    const char* name  = item -> name;
    const char* label = item -> label;
    const char mnemonic = item -> mnemonic;
    const char* accelerator = item -> accelerator;
    const char* accelText   = item -> accelText;;

    int id = item -> id;
    void* mid = to_voidptr(id); //2017/08/120
 
    CompoundString cs(label);
    if(strlen(label)) {
      args.set(XmNlabelString, (XtArgVal)cs.get());
    }  
    if (mnemonic != ZERO) {
      args.set(XmNmnemonic, (XtArgVal)mnemonic);
    }
    if (accelerator != NULL) {
      args.set(XmNaccelerator, (XtArgVal)accelerator);
    }
    CompoundString acl(accelText);
    if (accelText != NULL) {
      args.set(XmNacceleratorText, (XtArgVal)acl.get());
    }

    switch(type) {
               
    case CASCADEBUTTON:
        kid = new CascadeButton(this, name, args);
        kid -> addCallback(XmNactivateCallback, view,
            callback, (XtPointer)mid);
        break;
    case CASCADEBUTTONGADGET:
        kid = new CascadeButtonGadget(this, name, args);
        kid -> addCallback(XmNactivateCallback, view,
             callback, (XtPointer)mid);
        break;
    case PUSHBUTTON:
        kid = new PushButton(this, name, args);
        kid -> addCallback(XmNactivateCallback, view, 
          callback, (XtPointer)mid);  
        break;
    case PUSHBUTTONGADGET:
        kid = new PushButtonGadget(this, name, args);
        kid -> addCallback(XmNactivateCallback, view, 
          callback, (XtPointer)mid);  
        break;
    case LABEL:
        kid = new Label(this, name, args);
        break;
    case LABELGADGET:
        kid = new LabelGadget(this, name, args);
        break;
    case SEPARATOR:
        kid = new Separator(this, name, args);
        break;
    case SEPARATORGADGET:
        kid = new SeparatorGadget(this, name, args);
        break;
    default: 
        break;
    }
    if(kid) {
      kid -> manage();    
    }
     return kid;
  }

public:
  Menu(View* parent, const char* name, Args& args)
  :ShellView(parent, name, xmMenuShellWidgetClass, args) 
  { 
  }

  void addKid(View* kid)
  {
    items.add(kid);
  }

  virtual  void addItem(MenuItem* item, int num, View* view, Callback callback)
  {
    for(int i = 0; i<num; i++) {
      View* kid = addItem(item,  view, callback);
      addKid(kid);

      if(item->subItem) {
        /* for future extension */
      }
      item++;
    }
  }
};

}

