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
 *      MessageDialog.h
 *
 *****************************************************************************/

//2017/10/10 Modified to use create method.

#pragma once

#include <oz++/motif/DialogView.h>
#include <oz++/motif/MessageBox.h>

namespace OZ {
  
class MessageDialog :public DialogView {
private:
  SmartPtr<MessageBox> messagebox;

public:
  MessageDialog(View* parent, const char* name, Args& args)
  :DialogView()
  {
    MessageDialog::create(parent, name, args);
  }

  MessageDialog()
  :DialogView()
  {
  }

  virtual void create(View* parent, const char* name, Args& args)
  {
    DialogView::create(parent, name, args);
    messagebox = new MessageBox(this, name, args); 
  }

  ~MessageDialog() 
  {
  }
  
  void  manage() 
  {
    messagebox -> manage();
  }

  MessageBox* getMessageBox() 
  {
    return messagebox;
  }


  void show(const char* message)
  {
    getMessageBox() -> setMessage(message);
    manage();
  }

  void setMessage(const char* message)
  {
     getMessageBox() -> setMessage(message);
  }

  Widget getCancelButton()
  {
    return getMessageBox() -> getChild(XmDIALOG_CANCEL_BUTTON );
  }

  Widget getHelpButton()
  {
    return getMessageBox() -> getChild(XmDIALOG_HELP_BUTTON );
  } 

  void unmanageCancelButton()
  {
     getMessageBox()->unmanage(XmDIALOG_CANCEL_BUTTON);
  }
  void unmanageHelpButton()
  {
     getMessageBox()->unmanage(XmDIALOG_HELP_BUTTON);
  }
};

}
