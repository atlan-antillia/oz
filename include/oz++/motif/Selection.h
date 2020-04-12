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
 *      Selection.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/CommonObject.h>
#include <Xm/Xm.h>

namespace OZ {
  
class Selection :public CommonObject {
private:
  
  Widget    widget;
  Atom*     selType; 
  Atom*    target;
  Atom*     type; 
  XtPointer*   value;
  unsigned long*  length;
  int*     format;
  Boolean    result;
  
public:
  Selection(Widget widget1, Atom* selType1, 
    Atom* target1, Atom* type1, XtPointer* value1,
    unsigned long* length1, int* format1) 
  {
    widget  = widget1;
    selType  = selType1;
    target  = target1;
    type  = type1;
    value  = value1;
    length  = length1;
    format  = format1;
    result  = False;
  } 
  
  Widget          getWidget() { return widget; }
  Atom*           getSelType() { return selType;    }
  Atom*           getTarget()  { return target; }
  Atom*           getType()    { return type; }
  XtPointer*      getValue()   { return value; }
  unsigned long*  getLength()  { return length; }
  int*            getFormat()  { return format; }
  Boolean    getResult() { return result; }

  void  setSelType(Atom selType1) { *selType = selType1; }
  void  setTarget(Atom target1)   { *target = target1; }
  void  setType(Atom type1)       { *type = type1; }
  void  setValue(XtPointer value1) { *value = value1; }
  void  setLength(unsigned long length1) { *length = length1; }
  void  setFormat(int format1) { *format = format1; }
  void  setResult(Boolean result1) { result = result1; }
};

}

