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
 *      Action.h
 *
 *****************************************************************************/


#pragma once


#include <oz++/CommonObject.h>
#include <Xm/Xm.h>
#include <oz++/motif/IView.h>

namespace OZ {
  
typedef void (CommonObject::*Callback)(CommonObject& );

class Action :public CommonObject {
private:
  const char*    name;  // Store a name of XmNsomethingCallback.
  CommonObject*  sender;
  CommonObject*  receiver;
  Callback  callback;
  void*    clientData;
  void*    callData;
  void*    optionData;
  Boolean    value;

public:
  Action(CommonObject* sender1, CommonObject* receiver1, 
    Callback callback1, void* clientData1, const char* name1=NULL) 
    
  {
    sender     = sender1;
    receiver   = receiver1;
    callback   = callback1;
    clientData = clientData1;
    callData   = NULL;
    optionData = NULL;
    value      = False;
    name       = name1;
  }

  void    call(CommonObject& object) 
  {
    if(receiver && callback) 
      (receiver->*callback)(object);
  }
  
  void* getCallData() 
  {
    return callData;
  } 
  
  void* getClientData() 
  {
    return clientData;
  }
  
  int getClientIntData()
  {
    return to_integer(clientData);
  }


  void* getOptionData() 
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
  
  void setCallData(void* callData1) 
  {
    callData = callData1;
  }
  
  void setClientData(void* clientData1) 
  {
    clientData = clientData1;
  }
  
  void setOptionData(void* optionData1) 
  {
    optionData = optionData1;
  }
  
  void setSender(CommonObject* sender1) 
  {
    sender = sender1;
  }
  
  void  setValue(Boolean value1) 
  {
    value = value1;
  }

  const char* getName() 
  {
    return name;
  }
 
  Widget getSenderWidget()
  {
   Widget widget;
   IView* view = (IView*)getSender();
   if (view) {
    widget = view ->getWidget(); 
   }
   return widget;
  }
};

}

