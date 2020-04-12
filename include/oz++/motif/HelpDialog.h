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
 *      HelpDialog.h
 *
 *****************************************************************************/

//2016/12/26 Updated to be able to add a callback to a child button.
#pragma once

#include <oz++/motif/PushButton.h>
#include <oz++/motif/MessageBox.h>
#include <oz++/motif/DialogView.h>

namespace OZ {
  
class HelpDialog :public DialogView {
private:
  SmartPtr<PushButton> ok;
  SmartPtr<MessageBox> messagebox;

public:
  HelpDialog(View* parent, const char* name, Args& args)
  :DialogView()
  {
    HelpDialog::create(parent, name, args);
  }

  HelpDialog()
  :DialogView()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    DialogView::create(parent, name, args);
  
    messagebox = new MessageBox(this, name, args); 
    messagebox -> unmanage(XmDIALOG_CANCEL_BUTTON);
    messagebox -> unmanage(XmDIALOG_HELP_BUTTON);
    Widget okw = messagebox->getChild(XmDIALOG_OK_BUTTON );
    ok  = new PushButton(okw);
  }

  ~HelpDialog() 
  {
  }

/*
The child parameter takes one of the following values:
 XmDIALOG_CANCEL_BUTTON 
 XmDIALOG_DEFAULT_BUTTON 
 XmDIALOG_HELP_BUTTON 
 XmDIALOG_MESSAGE_LABEL 
 XmDIALOG_OK_BUTTON 
 XmDIALOG_SEPARATOR 
 XmDIALOG_SYMBOL_LABEL 
 */
  void addCallback(unsigned char child, const char* name, 
        XtCallbackProc callback, void* data)
  {
    Widget widget = messagebox->getChild(child);
    if (widget) {
     XtAddCallback(widget, name, callback, data);
    }
  }
  void addOKCallback(const char* name, CommonObject* object,
        Callback callback, void* data)
  {
    ok->addCallback(name, object, callback, data);
  }
  
  void  manage() 
  {
    messagebox -> manage();
  }

  MessageBox* getMessageBox() 
  {
    return messagebox;
  }

  void setMessage(const char* message)
  {
    CompoundString cs(message);
    messagebox -> set(XmNmessageString, cs);
  }
};

}
