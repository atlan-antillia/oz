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
 *      DropSite.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/View.h>
#include <oz++/motif/DropProcTable.h>
#include <Xm/DragDrop.h>

namespace OZ {
  
class DropSite :public CommonObject {
private:
  View*   receiver;
  static void commonDropProc(Widget widget, XtPointer clientData,
                        XtPointer callData)
  {
    Action* action = DropProcTable::get(widget);
    action -> setCallData(callData);
    action -> setClientData(clientData);
    action -> call(*action);
  }


public:
  DropSite(View* receiver1, Args* args)
  {
    receiver = receiver1;
    args -> set(XmNdropSiteActivity, XmDROP_SITE_INACTIVE);
    XmDropSiteRegister(receiver->getWidget(), 
      args->getArgList(), args->count());
  }

 
  Action* registerDropProc(View* view, Callback callback,
                        XtPointer data)
  {
    Action* action = new Action(receiver, view, callback, data);
    DropProcTable::put(receiver->getWidget(), action);
    Args ar;
    ar.set(XmNdropSiteActivity, XmDROP_SITE_ACTIVE);
    ar.set(XmNdropProc, (XtArgVal)&DropSite::commonDropProc);
    update(&ar);
    return action;
  }
                       
  void    retrieve(Args* args) 
  {
    XmDropSiteRetrieve(receiver -> getWidget(),
                        args->getArgList(), args->count());
  }
  
  void    unregister() 
  {
    XmDropSiteUnregister(receiver -> getWidget());
  }
  
  void    update(Args* args) 
  {
    XmDropSiteUpdate(receiver -> getWidget(),
                        args->getArgList(), args->count());
  }
};

}


