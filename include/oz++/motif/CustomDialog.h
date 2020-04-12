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
 *      CustomDialog.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/PopupView.h>
#include <oz++/motif/PanedWindow.h>

#include <oz++/motif/Form.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/CompoundStringList.h>

//2015/02/10
#define XmNworkAreaHeight  "workAreaHeight"
#define XmNbuttonAreaHeight  "buttonAreaHeight"

namespace OZ {


class CustomDialog :public PopupView {
private:
  PanedWindow*  panedWindow;
  Form*         workArea;
  Form*        buttonArea;
  PushButton**  pushButton;
  int           num;
  int          workAreaHeight;
  int          buttonAreaHeight;
	
  static const int BUTTON_AREA_HEIGHT    = 80;
  static const int DIALOG_MINIMUM_HEIGHT = 200;
  	
public:
  CustomDialog(View* parent, const char* name, Args& args)
  :PopupView()
  {
    CustomDialog::create(parent, name, args);
  }

  CustomDialog()
  :PopupView()
  {
  }

  virtual void create(View* parent, const char* name, Args& args)
  {
    PopupView::create(parent, name, 
     //xmDialogShellWidgetClass,
     transientShellWidgetClass, 
     args);
  
    buttonAreaHeight = BUTTON_AREA_HEIGHT; //default
    if (args.has(XmNbuttonAreaHeight)) {
      buttonAreaHeight = (int)args.has(XmNbuttonAreaHeight);
      if (buttonAreaHeight > BUTTON_AREA_HEIGHT) {
        buttonAreaHeight = BUTTON_AREA_HEIGHT;
      }
    }
    int h = args.get(XmNheight);
    if (h < DIALOG_MINIMUM_HEIGHT) {
      set(XmNheight, DIALOG_MINIMUM_HEIGHT);
       h = DIALOG_MINIMUM_HEIGHT;
    }

    num   = (int) args.get(XmNbuttonCount);
    XmString* xmsTable = (XmString*) args.get(XmNbuttons);

    args.set(XmNsashWidth,  1);
    args.set(XmNsashHeight, 1);
    panedWindow = new PanedWindow(this, "panedWindow", args);
    panedWindow -> manage();
    
    //2015/08/28 
    //args.reset();
    Args ar;
    if (args.has(XmNworkAreaHeight)) {
      ar.set(XmNheight, args.get(XmNworkAreaHeight));
      printf("workAreaHeight %d\n", (int)args.get(XmNworkAreaHeight));
      workAreaHeight = (int)args.get(XmNworkAreaHeight);
    } else {
      ar.set(XmNheight, h - buttonAreaHeight);
      workAreaHeight = h - buttonAreaHeight;
    }
    workArea = new Form(panedWindow, "workArea", ar);
    workArea -> manage();

    ar.reset();
    args.set(XmNheight, buttonAreaHeight);
    buttonArea = new Form(panedWindow, "buttonArea", ar);
    buttonArea -> manage();
    if(num > 0 && xmsTable != NULL) {
      addButton(xmsTable, num);
    }
  }

  void addButton(CompoundStringList& csl)
  {
    XmString* xms = csl.get();
    int count     = csl.getLength();
    if (xms !=NULL  && count > 0) {
      addButton(xms, count);
    }
  }
	
  void addButton(XmString* xms, int count)
  {
    if (xms != NULL && count > 0) {
      Args ar;
      num = count;
      pushButton = new PushButton*[num];
      char name[128];
      //int fract = 30*num + 10*(num+1);
      int fract = 20*num + 10*(num+1);
      buttonArea->set(XmNfractionBase, fract);

      for(int i = 0; i<num; i++) {
        sprintf(name, "button_%d", i);
        ar.reset();
        ar.set(XmNlabelString,      (XtArgVal)xms[i]);
        ar.set(XmNleftAttachment,   XmATTACH_POSITION);
        ar.set(XmNtopAttachment,    XmATTACH_FORM);
        ar.set(XmNrightAttachment,  XmATTACH_POSITION);
        ar.set(XmNbottomAttachment, XmATTACH_FORM);
        ar.set(XmNtopOffset,    5);
        ar.set(XmNbottomOffset, 5);
        //ar.set(XmNleftPosition,  10*(i+1) + 30*i);
        //ar.set(XmNrightPosition, 10*(i+1) + 30*(i+1));
        ar.set(XmNleftPosition,  10*(i+1) + 20*i);
        ar.set(XmNrightPosition, 10*(i+1) + 20*(i+1));

        pushButton[i] = new PushButton(buttonArea, name, ar);
        pushButton[i] -> manage();
      }
  	
      Dimension height = 0;
      pushButton[0] -> get(XmNheight, (XtArgVal)&height);
      buttonArea -> set(XmNpaneMinimum, height*2);  
      buttonArea -> set(XmNpaneMaximum, height*2);
      buttonArea -> set(XmNdefaultButton, 
      pushButton[0] -> getWidget());
    }
  }

  ~CustomDialog()
  {
    for(int i = 0; i<num; i++) {
      delete pushButton[i];
    }
    delete [] pushButton; //2015/02/10
    delete buttonArea;
    delete workArea;
    delete panedWindow;
  }

  PushButton* getNthButton(int n) 
  { 
    return pushButton[n]; 
  }

  Form* getWorkArea() 
  { 
    return workArea; 
  }

  int getWorkAreaHeight() 
  { 
    return workAreaHeight;
  }

  int getButtonAreaHeight() 
  { 
    return buttonAreaHeight;
  }
};
}


