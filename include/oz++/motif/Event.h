/******************************************************************************
 *
 * Copyright (c) 2014-2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      Event.h
 *
 *****************************************************************************/

// 2015/01/18 Updated to add new methods for processTraversal, isKeyPressed, isButtonPressed and so on.
// 2016/03/27 Added getKeySym method.

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/motif/Selection.h>
#include <oz++/motif/IView.h>

namespace OZ {
  
typedef void (CommonObject::*Handler)(CommonObject&);

class Event :public CommonObject {
private:
  CommonObject* sender;
  CommonObject* receiver;
  Handler       handler;
  void*         clientData;
  XEvent*       xevent;  
  void*         optionData;
  Boolean       value;
  
public:
  Event(CommonObject* sender1, CommonObject* receiver1, 
    Handler handler1, void* clientData1)
  {
    sender     = sender1;
    receiver   = receiver1;
    handler    = handler1;
    clientData = clientData1;
    xevent     = NULL;
    optionData = NULL;
    value      = False;
  }

  void    call(CommonObject& object) 
  {
    if(receiver && handler) {
      (receiver->*handler)(object);
    }
  }
  
  XEvent* getXEvent() 
  {
    return xevent;
  } 
  
  //2015/01/18
  Boolean isEnterKey()
  {
    Boolean rc = False;
    if (xevent && xevent->type == KeyPress) {
       KeySym k;
       char buf[10];
       XLookupString((XKeyEvent *)xevent, buf, sizeof(buf), &k, NULL);
       if (k == XK_space || k == XK_Return || k == XK_KP_Enter) {
        rc = TRUE;  
       }
    }
    return rc;
  }

  //2015/03/20
  Boolean isEscapeKeyPressed()
  {
    Boolean rc = False;
    if (xevent && xevent->type == KeyPress) {
       KeySym k;
       char buf[10];
       XLookupString((XKeyEvent *)xevent, buf, sizeof(buf), &k, NULL);
       if (k == XK_Escape) {
        rc = TRUE;
       }
    }
    return rc;
  }

  //2015/03/20
  Boolean isEnterKeyPressed()
  {
    Boolean rc = False;
    if (xevent && xevent->type == KeyPress) {
       KeySym k;
       char buf[10];
       XLookupString((XKeyEvent *)xevent, buf, sizeof(buf), &k, NULL);
       if (k == XK_space || k == XK_Return || k == XK_KP_Enter) {
        rc = TRUE;  
       }
    }
    return rc;
  }

  //2015/03/20
  Boolean isEnterKeyReleased()
  {
    Boolean rc = False;
    if (xevent && xevent->type == KeyRelease) {
       KeySym k;
       char buf[10];
       XLookupString((XKeyEvent *)xevent, buf, sizeof(buf), &k, NULL);
       if (k == XK_space || k == XK_Return || k == XK_KP_Enter) {
        rc = TRUE;  
       }
    }
    return rc;
  }
  
  Boolean isKeyPressed(KeySym key)
  {
    Boolean rc = False;
    if (xevent && xevent->type == KeyPress) {
       KeySym k;
       char buf[10];
       XLookupString((XKeyEvent *)xevent, buf, sizeof(buf), &k, NULL);
       if (k == key) {
        rc = TRUE;  
       }
    }
    return rc;
  }

  Boolean isButtonPressed(int number=1)
  {
    Boolean rc = False;
    if (xevent && xevent->type == ButtonPress && xevent->xbutton.button == number) {
      rc = TRUE;
    }
    return rc;
  }
  
  Boolean isButtonReleased(int number=1)
  {
    Boolean rc = False;
    if (xevent && xevent->type == ButtonRelease && xevent->xbutton.button == number) {
      rc = TRUE;
    }
    return rc;
  }
  
  
  void*  getClientData() 
  {
    return clientData;
  }
  
  //2017/08/20
  int  getClientIntData()
  {
    return to_integer(clientData);
  }

  void*  getOptionData() 
  {
    return optionData;
  } 
  
  CommonObject* getSender() 
  {
    return sender;
  }
  
  Boolean getValue() 
  {
    return value;
  }
  
  void   setXEvent(XEvent* xevent1) 
  {
    xevent = xevent1; 
  }
  
  void  setClientData(void* clientData1) 
  {
    clientData = clientData1;
  }
  
  void  setOptionData(void* optionData1) 
  {
    optionData = optionData1;
  }
  
  void  setSender(CommonObject* sender1) 
  {
    sender = sender1;
  }
  
  void  setValue(Boolean value1) 
  {
    value = value1;
  }

  //2015/01/17 
  void processTraversal()
  {
    IView* view = (IView*)getSender();
    if (view) {
      Widget widget = view ->getWidget();
      XmProcessTraversal(widget, XmTRAVERSE_CURRENT);
    }
  }
  
  int x()
  { 
     if (xevent) {
       return xevent->xbutton.x;
     } else {
       return -1;
     }
  }
  
  int y()
  {
    if (xevent) {
      return xevent->xbutton.y;
    } else {
      return -1;
    }
  }
/*  
  Window window() 
  {
    
    Window w = 0;
    if (xevent) {
      w = xevent->window;
    }
    return w;
  }
*/
  
  Widget getSenderWidget() 
  {
    IView* view = (IView*)getSender();
    Widget widget;
    if (view) {
      widget = view ->getWidget();
    }
    return widget;
  }

  //2016/03/27
  KeySym getKeySym()
  {
    XEvent* xev = getXEvent();
    KeySym k;
    char buf[10];
    XLookupString((XKeyEvent *)xev, buf, sizeof(buf), &k, NULL);
    return k;  
  }
};

}

