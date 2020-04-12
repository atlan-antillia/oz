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
 *      Responder.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/TemplateDialog.h>
#include <oz++/motif/PushButtonGadget.h>

namespace OZ {

class Responder :public TemplateDialog {
private:
  unsigned int       num;
  PushButtonGadget** pushBG;

public:
  Responder(View* parent, const char* name, Args& args)
  :TemplateDialog()
  {
    Responder::create(parent, name, args);
  }

  Responder()
  :TemplateDialog()
  {
  }

  void create(View* parent, char* name, Args& args)
  {
    TemplateDialog::create(parent, name, args);
  
    MessageBox* box = getMessageBox();
    XmString* xms   = (XmString*)args.get(XmNbuttons);
    num             = (int)args.get(XmNbuttonCount);

    pushBG = new PushButtonGadget*[num];
    char    buffer[20];
    for(unsigned int i = 0; i<num; i++) {
      sprintf(buffer, "button_%d", i);
      args.reset();
      if(xms[i]) args.set(XmNlabelString, (XtArgVal)xms[i]);
      pushBG[i] = new PushButtonGadget(box, buffer, args);
      pushBG[i] ->manage();
    }
    box -> set(XmNdefaultButton, (XtArgVal)pushBG[0]->getWidget());
  }

  ~Responder()
  {
    for(unsigned int i = 0; i<num; i++)
      delete pushBG[i];
  }

  PushButtonGadget* getButton(unsigned int n)
  {
    if(n < num) {
      return pushBG[n];
    } else {
      return NULL;
    }
  }
};

}

