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
 *      MessageBox.h
 *
 *****************************************************************************/

//2017/10/10 Modified to use create method.

#pragma once

#include <oz++/motif/BulletinBoard.h>
#include <Xm/MessageB.h>

namespace OZ {
  
class MessageBox :public BulletinBoard {
public:
  MessageBox(View* parent, const char* name, Args& args)
  :BulletinBoard(parent, name, xmMessageBoxWidgetClass, 
        args) 
  { 
  }
  
  MessageBox()
  :BulletinBoard()
  {
  }

  virtual void create(View* parent, const char* name, Args& args)
  {
    BulletinBoard::create(parent, name, xmMessageBoxWidgetClass, 
        args); 
  }

  Widget  getChild(unsigned char child) 
  {
    return XmMessageBoxGetChild(getWidget(), child); 
  }
  
  void setMessage(const char* message)
  {
    Widget child = getMessageLabel();
    CompoundString cs(message);
    set(XmNmessageString, cs);
  }

  Widget getMessageLabel()
  {
    return getChild(XmDIALOG_MESSAGE_LABEL);
  }

  Widget getOKButton()
  {
    return getChild(XmDIALOG_OK_BUTTON); 
  }

  Widget getCancelButton()
  {
    return getChild(XmDIALOG_CANCEL_BUTTON );
  }

  Widget getHelpButton()
  {
    return getChild(XmDIALOG_HELP_BUTTON );
  }

  void  unmanage(unsigned char child) 
  {
    Widget widget =  XmMessageBoxGetChild(getWidget(), child);
    if(widget) {
      XtUnmanageChild(widget);
    }
  }
};

}

