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
 *      DragContext.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/View.h>
#include <oz++/motif/Selection.h>
#include <Xm/DragDrop.h>

namespace OZ {

class DragContext :public CommonObject {
private:
  Widget  dragContext;
  View*  caller;
  Args*  args;
  Action* dragAction;

  static  Boolean commonConvertProc(Widget dragcon, Atom* select,
      Atom* target, Atom* type, XtPointer* value,
                  unsigned long* length, int* format)
  {
    printf("commonConvertProc\n");
    Action* action = NULL;
    Selection selection(dragcon, select, target,
      type, value, length, format);
    Arg arg[1];
    XtSetArg(arg[0], XmNclientData, &action);
    XtGetValues(dragcon, arg, 1);
    if(action) {
      action -> call(selection);
    }
    return selection.getResult();
  }

public:
  DragContext(View* caller1, Args* args1)
  {
    caller  = caller1;
    dragContext = NULL;

    args  = new Args();
    args -> copy(args1);
    args -> set(XmNconvertProc,(XtArgVal)commonConvertProc);
    //(XtArgVal)&DragContext::commonConvertProc);
    dragAction = NULL;
  }


  ~DragContext()
  {
    delete dragAction;
    delete  args;
  }

  void  registerConvertProc(CommonObject* callee, Callback callback,
      XtPointer data)
  {
    printf("registerConvertProc\n");
    if(dragAction == NULL) {

      dragAction = new Action(caller, callee, callback, data);
      args -> set(XmNclientData, (XtArgVal)dragAction);
    }
  }

  void  start(Action* action)
  {
    printf("DragContext::start\n");
    dragContext = XmDragStart(caller->getWidget(),
        (XEvent*)action->getCallData(),
        args->getArgList(), args->count());
  }

};

}


