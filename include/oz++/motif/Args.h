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
 *      Args.h
 *
 *****************************************************************************/

// 2015/01/22 Updated to add some set method

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/motif/IView.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>
#include <oz++/motif/PixmapFile.h>
#include <oz++/motif/Pixelmap.h>

#include <Xm/Xm.h>
#include <oz++/motif/FontList.h>

//#include <oz++/motif/RenderTable.h>

namespace OZ {

class Args :public CommonObject {

private:
  static const int ARG_MAX = 100; //2015/01/28 Modified 50;
  
  int  max;
  int  num;
  ArgList args;
  
public:
  Args()
  {  
    num = 0;
    args = new Arg[ARG_MAX];
    max = ARG_MAX;  
 }

  ~Args() 
  { 
    delete [] args; 
  }
 
  Args&   reset() 
  {
    num = 0;
    return *this;
  }
  
  ArgList  getArgList() 
  { 
    return args; 
  }
	
  int   count() 
  { 
    return num; 
  }

  Args&  set(const char* name, unsigned int value)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
    	args[num].value = (XtArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, XtArgVal value)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, int value)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, double value)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, Atom atom)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)atom;
      num++;
      }
    return *this;
  }

  Args&  set(const char* name, const char* string)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)string;
      num++;
    }
    return *this;
  }
  	
  Args&  set(const char* name, const StringT<char>& value)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)(const char*)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, unsigned short value)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, XmString xmstring)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)xmstring;
      num++;
    }
    return *this;
  }

  //2017/05/10
  Args&  set(const char* name, Pixelmap* value)
  {
    if(num >= 0 && num < max && value) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)value -> get();
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, PixmapFile* value)
  {
    if(num >= 0 && num < max && value) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)value -> get();
      num++;
    }
    return *this;
  }


  Args&  set(const char* name, Widget w)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)w;
      num++;
    }
    return *this;
  }

  //2017/05/10
  Args&  setIfNotSet(const char* name, XtArgVal value)
  {
    if (!has(name)) {
      set(name, value); 
    }
    return *this; 
  }

  Args&  set(const char* name, CompoundString& cstring)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)(XmString)cstring;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, CompoundStringList& cstring)
  {
    if(num >= 0 && num < max) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)cstring.get();
      num++;
    }
    return *this;
  }
	
  //2015/01/22 Added 
  Args&  set(const char* name, FontList* fontList)
  {
    if(num >= 0 && num < max && fontList) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)fontList -> get() ;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, IView* view)
  {
    if(num >= 0 && num < max && view) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)view->getWidget();
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, XmRenderTable table)
  {
    if(num >= 0 && num < max && table) {
      args[num].name  = (char*)name;
      args[num].value = (XtArgVal)table;
      num++;
    }
    return *this;
  }

  void  copy(Args* args1)
  {
    Arg* ar = args1->getArgList();
    num = args1->count();

    for(int i= 0; i<num; i++) {
      args[i].name  = ar[i].name;
      args[i].value = ar[i].value;
    }
  }


  XtArgVal get(const char* name)
  {
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0)
        return args[i].value;
    }
    return (XtArgVal)NULL;
  }

  //2015/01/26 Added 
  Boolean get(const char* name, XtArgVal& value)
  {
    Boolean rc = False;
    for(int i = 0; i<num; i++) {
    	if(strcmp(args[i].name, name) == 0) {
          value = args[i].value;
    	  rc = TRUE;
    	  break;
    	}
    }
    return rc;
  }

  Boolean get(const char* name, CompoundString& value)
  {
    Boolean rc = False;
    for(int i = 0; i<num; i++) {
        if(strcmp(args[i].name, name) == 0) {
          XmString xms = (XmString)args[i].value;
          value = xms;
          rc = TRUE;
          break;
        }
    }
    return rc;
  }

  //2015/01/26 Added 
  Boolean has(const char* name)
  {
    Boolean rc = False;
    for(int i = 0; i<num; i++) {
    	if(strcmp(args[i].name, name) == 0) {
          //value = args[i].value;
    	  rc = TRUE;
    	  break;
    	}
    }
    return rc;
  }
	
  Args&  put(const char* name, XtArgVal value)
  {
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        args[i].value = value;
        return *this;
      }
    }
    return set(name, value);  
  }

};

}

