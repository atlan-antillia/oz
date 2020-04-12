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
 *      ToolBar.h
 *
 *****************************************************************************/

//2017/10/16 Modified to use create method.

#pragma once

#include <oz++/motif/RowColumn.h>
#include <oz++/motif/ToggleButtonGadget.h>
#include <oz++/motif/PixmapFile.h>

namespace OZ {
  
//extern const char* XmNpaletteBehavior;
const char* XmNpaletteBehavior = "paletteBehavior";

class ToolBar :public RowColumn {
private:
  int                  num;
  PixmapFile**         normalPixmap;
  PixmapFile**         selectPixmap;
  ToggleButtonGadget** kids;

  static void toolBarCallback(Widget widget, XtPointer client, 
        XtPointer call)
  {
    Arg args[1];
    Boolean set;
    XtSetArg(args[0], XmNset, &set);
    XtGetValues(widget, args, 1);
    if(set == TRUE) {
      Action* action = (Action*)client;
      action->setCallData(call);
      action->call(*action);
    }
  }

public:
  ToolBar(View* parent, const char* name, Args& args)
  :RowColumn()
  {
    ToolBar::create(parent, name, args);
  }

  ToolBar()
  :RowColumn()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    RowColumn::create(parent, name, 
        args.set(XmNpaletteBehavior,  TRUE)
            .set(XmNorientation, XmHORIZONTAL)
            .set(XmNradioBehavior, TRUE)
            .set(XmNpacking, XmPACK_COLUMN)
            .set(XmNisHomogeneous, TRUE)
            .set(XmNentryClass, (XtArgVal)xmToggleButtonGadgetClass));
  
    Args ar;
    char string[256];

    XmString* xmsTable = NULL;
    char**   items = NULL;
    int type = (int) args.get(XmNlabelType);
    if(type == 0) {
      type = XmSTRING;
     }
   
    if(type == XmSTRING) {
      xmsTable = (XmString*)args.get(XmNbuttons);
    }
    if(type == XmPIXMAP) {
      items    = (char**)args.get(XmNbuttons);
    }

    Boolean behavior = (int) args.get(XmNpaletteBehavior);
    int shadow       = (int) args.get(XmNshadowThickness);
    int thickness    = (int) args.get(XmNhighlightThickness);
    if(shadow == 0) shadow = 2;

    num   = (int) args.get(XmNbuttonCount);
    kids  = new ToggleButtonGadget*[num];

    normalPixmap = NULL;
    selectPixmap = NULL;
   if(type == XmPIXMAP) {
    normalPixmap = new PixmapFile*[num];
   }
   if(behavior == TRUE) {
    selectPixmap = new PixmapFile*[num];
   }
   
    Pixel fg, bg;
    get(XmNforeground, (XtArgVal)&fg);
    get(XmNbackground, (XtArgVal)&bg);

    for(int i = 0; i<num; i++) {
      sprintf(string, "button_%d", i);
      ar.reset();
      ar.set(XmNlabelType, type);
      if(type == XmSTRING) {
        ar.set(XmNlabelString, (XtArgVal)xmsTable[i]);
      }
      if(type == XmPIXMAP) {
        normalPixmap[i] = new PixmapFile(getDisplay(), items[i], fg, bg);
        ar.set(XmNlabelPixmap, (XtArgVal)normalPixmap[i]->get());
      }
      if(behavior == TRUE) {
        selectPixmap[i] = new PixmapFile(getDisplay(), items[i], bg, fg);
        ar.set(XmNindicatorOn, False);
        ar.set(XmNfillOnSelect, TRUE);
        ar.set(XmNshadowThickness, shadow);
        ar.set(XmNhighlightThickness, thickness);
        ar.set(XmNselectPixmap, (XtArgVal)selectPixmap[i]->get());
      }
      kids[i] = new ToggleButtonGadget(this, string, ar); 
      kids[i] -> manage();
    }
  }

  ~ToolBar()
  {
    for(int i = 0; i<num; i++) {
      if(normalPixmap) delete normalPixmap[i];
      if(selectPixmap) delete selectPixmap[i];
      delete kids[i];
    }
  }

  Action* addCallbackToKids(const char* name, View* view, Callback callback,
        XtPointer data)
  {
    for(int i = 0; i< num; i++) {
      Action* action = new Action(this, view, callback, to_voidptr(i));
      XtAddCallback(kids[i] ->getWidget(), name,
        &ToolBar::toolBarCallback, action);  
    }
    return NULL; 
  }


  ToggleButtonGadget** getKids() 
  { 
    return kids; 
  }  

  int  getNumOfKids() 
  { 
    return num; 
  }
};

}


