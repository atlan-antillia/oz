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
 *      ValueTransfer.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/View.h>
#include <oz++/motif/Transfer.h>
#include <Xm/DragDrop.h>

namespace OZ {

class ValueTransfer :public CommonObject {
private:
  View*    view;
  XtPointer transferId;
  Atom      target;

private:
  static void commonTransferProc(Widget  widget,
             XtPointer clientData,
             XtPointer callData)
  {
    printf("commonTransferProc\n");
    Action* action = (Action*)clientData;
    action->setCallData(callData); 

    action -> call(*action);
    delete action;
  }

public:
  ValueTransfer(View* view, XtPointer transferId)
  :view(view),
  transferId(transferId)
  {
  }

  ~ValueTransfer()
  {
  }

  void succeeded()
  {
    XmTransferDone(transferId,  XmTRANSFER_DONE_SUCCEED);
  }

  void failed()
  {
    XmTransferDone(transferId, XmTRANSFER_DONE_FAIL);
  }
 
  void  registerTransferProc(Atom target, CommonObject* callee,
        Callback callback, XtPointer data)
  {
    Widget widget = view->getWidget();
    Display* display = XtDisplay(widget);

    Action* transferAction = new Action(this, callee, callback, data);
    Time time = XtLastTimestampProcessed(display);
    XmTransferValue(transferId, target,  
                (XtCallbackProc)commonTransferProc, 
                transferAction, time);
  }

};

}

