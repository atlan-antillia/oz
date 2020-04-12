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
 *     ApplicationContext.h
 *
 *****************************************************************************/

// Oz++ 1.0

#pragma once

#include <oz++/CommonObject.h>
#include <Xm/Xm.h>
#include <oz++/motif/Action.h>
#include <oz++/motif/ActionProcTable.h>
#include <oz++/LinkedList.h>

namespace OZ {

class ApplicationContext :public CommonObject {
private:
  XtAppContext  context;
  LinkedList  actionList;
  static void commonActionProc(Widget widget, XEvent* event,
                         char** params, int num)
  {
    printf("commonActionProc\n");
    printf("num %d\n", num);
    if (num < 0) {
     return;
    }
    if (params && num>0) {
      for (int i = 0; i<num; i++) {
        printf("Params [%d] %s\n", i, params[i]);
      }
    }
    
    int id = 0;
    if (*params != NULL) {
      id = atoi(*params);
      printf("id %d\n", id); 
      params++;
    }

    Action* action = ActionProcTable::get(widget, id);
    if (action) {
      action -> setCallData(event);
      action -> setClientData(params);
      action -> setOptionData(to_voidptr(num-1));
      //action -> setOptionData((void*)(num-1));
      action -> call(*action);
    }
  }


  static void  commonInputCallback(XtPointer clientData,
        int* fd, XtInputId* id)
  {
    Action* action = (Action*)clientData;
    if (action) {
      action -> setOptionData(fd);
      action -> setCallData(id);
      action -> call(*action);
    }
  }

  static void  commonTimerCallback(XtPointer clientData, XtIntervalId* id)
  {
    Action* action = (Action*)clientData;
    if (action) {
      action -> setCallData(id);
      action -> call(*action);
    }
  }

  static Boolean  commonWorkProc(XtPointer clientData)
  {
    Action* action = (Action*)clientData;
    if (action) {
      action -> call(*action);
      return action->getValue();
    }
    return False;
  }

public:
  ApplicationContext()
  {
    context = XtCreateApplicationContext();
  }

  ~ApplicationContext()
  {
  	clear();
  }
	
  void clear()
  {
    if (context) {
      XtDestroyApplicationContext(context);
  	  context = NULL;
    }
  }

  void  addAction(const char* name)
  {
    static XtActionsRec actionRec;
    actionRec.string = (char*)name;
    actionRec.proc   = (XtActionProc)&ApplicationContext::commonActionProc;
    printf("addAction %s\n", name);
    XtAppAddActions(context, &actionRec, 1);
  }

  void  addActions(XtActionList actions, Cardinal num)
  {
    XtAppAddActions(context, actions, num);
  }

  void  addInput(int source, XtPointer condition,
      CommonObject* callee,
      Callback callback, XtPointer clientData,
      XtInputId* id)
  {
    Action* action = new Action(NULL, callee, callback, clientData);
    *id = XtAppAddInput(context, source, condition,
          commonInputCallback,
          action);
    // 2014.10.16
    actionList.add(action);
  }


  void  addTimeOut(unsigned long interval, CommonObject* callee,
      Callback callback, XtPointer clientData,
      XtIntervalId* id)
  {
    Action* action = new Action(NULL, callee, callback, clientData);
    *id =  XtAppAddTimeOut(context, interval,
          commonTimerCallback,
          action);
    // 2014.10.16
    actionList.add(action);
  }

  void  addWorkProc(CommonObject* callee,
      Callback callback, XtPointer clientData,
      XtWorkProcId* id)
  {
    Action* action = new Action(NULL, callee, callback, clientData);
    *id = XtAppAddWorkProc(context,
                commonWorkProc,
                action);
    // 2014.10.16
    actionList.add(action);
  }

  void  error(char* message)
  {
    XtAppError(context, message);
  }

  unsigned long getSelectionTimeout()
  {
    return XtAppGetSelectionTimeout(context);
  }

  void  mainLoop()
  {
    XtAppMainLoop(context);
  }

  void  nextEvent(XEvent* event)
  {
    XtAppNextEvent(context, event);
  }

  Boolean  peekEvent(XEvent* event)
  {
    return XtAppPeekEvent(context, event);
  }

  void  setFallbackResources(char** resources)
  {
    XtAppSetFallbackResources(context, resources);
  }

  void  warning(char* message)
  {
    XtAppWarning(context, message);
  }

  XtAppContext getContext()
  {
    return context;
  }

  void  removeAction(Action* action)
  {
    actionList.remove(action);
  }
};

}


