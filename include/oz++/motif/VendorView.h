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
 *      VendorView.h
 *
 *****************************************************************************/

//2017/10/10 Modified to use create method.

#pragma once

#include <oz++/motif/ShellView.h>
#include <Xm/Protocols.h>
#include <Xm/AtomMgr.h>

namespace OZ {
  
class  VendorView :public ShellView {
protected:
  VendorView(Application& applet, const char* name, 
      const WidgetClass widgetClass, Args& args)
  :ShellView(applet, name, widgetClass, args)
  {
    //Constructor
  }

  VendorView(View* parent, const char* name,
        const WidgetClass widgetClass, Args& args)
  :ShellView()
  {
    VendorView::create(parent, name, widgetClass, args);
  }

  VendorView()
  :ShellView()
  {
  }

  virtual void create(View* parent, const char* name, 
        const WidgetClass widgetClass, Args& args)
  {
    ShellView::create(parent, name, widgetClass, args);
  }
  
public:
  void  activateProtocol(Atom property, Atom protocol) 
  {
    XmActivateProtocol(getWidget(), property, protocol);
  }
  
  void  activateWMProtocol(Atom protocol) 
  {
    XmActivateWMProtocol(getWidget(), protocol);
  }
  
  void  addProtocols(Atom property, Atom* protocols, 
      Cardinal num_protocols) 
  {
    XmAddProtocols(getWidget(), property, protocols, 
        num_protocols);
  }
  
  Action* addProtocolCallback(Atom property, Atom protocol, 
        View* view, Callback callback, 
        XtPointer data)
  {
    Action* action = new Action(this, view, callback, data);
    XmAddProtocolCallback(getWidget(),
      property,
      protocol,
      getCommonCallback(),
      action);
    return action;
  }

  Action* addWMProtocolCallback(Atom protocol, 
        View* view, Callback callback, 
        XtPointer data)
  {
    Action* action = new Action(this, view, callback, data);
    XmAddWMProtocolCallback(getWidget(),
      protocol,
      getCommonCallback(),
      action);
    return action;
  }


  void  AddWMProtocols(Atom* protocols, Cardinal num_protocols) 
  {
    XmAddWMProtocols(getWidget(), protocols, num_protocols);
  }
};

}

