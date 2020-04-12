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
 *      TextField.h
 *
 *****************************************************************************/

//2015/02/25 Updated
//2017/10/10 Modified to use create method.

#pragma once

#include <oz++/StringT.h>
#include <oz++/motif/Primitive.h>
#include <Xm/TextF.h>

namespace OZ {
  
class TextField :public Primitive {
	
protected:
  //2015/02/25 
  TextField(View* parent, const char* name, WidgetClass widgetClass, Args& args)
  :Primitive(parent, name, widgetClass, args) 
  { 
  }

public:
  TextField(View* parent, const char* name, Args& args)
  :Primitive(parent, name, xmTextFieldWidgetClass, args) 
  { 
  }

  TextField()
  :Primitive()
  {
  }  

  virtual void create(View* parent, const char* name, Args& args)
  {
    Primitive::create(parent, name, xmTextFieldWidgetClass, args); 
  }

  virtual void create(View* parent, const char* name, 
          WidgetClass widgetClass, Args& args)
  {
    Primitive::create(parent, name, widgetClass, args); 
  }

  void clearSelection(Time time) 
  {
    XmTextFieldClearSelection(getWidget(), time);
  }
  
  Boolean copy(Time time) 
  {
    return XmTextFieldCopy(getWidget(), time);
  }
  
  Boolean cut(Time time) 
  {
    return XmTextFieldCut(getWidget(), time);
  }
  
  int  getBaseLine() 
  {
    return XmTextFieldGetBaseline(getWidget());
  }
  
  Boolean  getEditable() 
  {
    return XmTextFieldGetEditable(getWidget());
  }
  
  XmTextPosition  getInsertionPosition() 
  {
    return XmTextFieldGetInsertionPosition(getWidget());
  }
  
  XmTextPosition  getLastPosition() 
  {
    return XmTextFieldGetLastPosition(getWidget());
  }
  
  int  getMaxLength() 
  {
    return XmTextFieldGetMaxLength(getWidget());
  }
  
  char*  getSelection() 
  {
    return XmTextFieldGetSelection(getWidget());
  }
  
  wchar_t* getSelectionWcs() 
  {
    return XmTextFieldGetSelectionWcs(getWidget());
  }
  
  Boolean getSelectionPosition(XmTextPosition* left, 
             XmTextPosition* right) 
  {
    return XmTextFieldGetSelectionPosition(getWidget(),
              left, right);
  }
  
  char*  getString() 
  {
    return XmTextFieldGetString(getWidget());
  }

  void  getString(StringT<char>& string) 
  {
    //2017/05/10
    char* str = XmTextFieldGetString(getWidget());
    string = str;
    XtFree(str);
  }
  
  //2015/09/05
  wchar_t*  getStringWcs() 
  {
    return XmTextFieldGetStringWcs(getWidget());
  }
 
  //2015/09/05
  void  getStringWcs(StringT<wchar_t>& string) 
  {
    wchar_t* str =  XmTextFieldGetStringWcs(getWidget());
    string = str;
    free((void*)str);
  }
  
  int  getSubstring(XmTextPosition start, int numChar, int bufferSize,
        char* buffer) 
  {
    return XmTextFieldGetSubstring(getWidget(), start, numChar,
        bufferSize, buffer);
  }
  
  int  getSubstring(XmTextPosition start, int numChar, int bufferSize,
        wchar_t* buffer) 
  {
    return XmTextFieldGetSubstringWcs(getWidget(), start, numChar,
        bufferSize, buffer);
  }
  
  void  insert(XmTextPosition pos, char* string) 
  {
    XmTextFieldInsert(getWidget(), pos, string);
  }
  
  void  insertWcs(XmTextPosition pos, wchar_t* wcstring) 
  {
    XmTextFieldInsertWcs(getWidget(), pos, wcstring);
  }
  
  Boolean paste() 
  {
    return XmTextFieldPaste(getWidget());
  }
  
  Boolean posToXY(XmTextPosition pos, Position* x, Position* y) 
  {
    return XmTextFieldPosToXY(getWidget(), pos, x, y);
  }
  
  Boolean remove() 
  {
    return XmTextFieldRemove(getWidget());
  }
  
  void  replace(XmTextPosition from, XmTextPosition to, char* string) 
  {
    XmTextFieldReplace(getWidget(), from, to, string);
  }
  
  void  replaceWcs(XmTextPosition from, XmTextPosition to, 
      wchar_t* wcstring) 
  {
    XmTextFieldReplaceWcs(getWidget(), from, to, wcstring);
  }
  
  void  setAddMode(Boolean mode) 
  {
    XmTextFieldSetAddMode(getWidget(), mode);
  }
  
  void  setEditable(Boolean mode) 
  {
    XmTextFieldSetEditable(getWidget(), mode);
  }
  
  void  setHighlight(XmTextPosition left, XmTextPosition right,
        XmHighlightMode mode) 
  {
    XmTextFieldSetHighlight(getWidget(), left, right, mode);
  }
  
  void  setInsertionPosition(XmTextPosition pos) 
  { 
    XmTextFieldSetInsertionPosition(getWidget(), pos); 
  }
  
  void  setMaxLength(int length) 
  {
    XmTextFieldSetMaxLength(getWidget(), length);
  }
  
  void  setSelection(XmTextPosition first, XmTextPosition last,
        Time time) 
  {
    XmTextFieldSetSelection(getWidget(), first, last, time);
  }
  
  void set(const char* name, CompoundString& cs)
  {
    if (name == XmNlabelString) {
      char* string = cs.unparse();
      setString(string);
      XtFree(string);
    }
  }

  void set(const char* name, const char* string)
  {
    if (name == XmNlabelString) {
      setString(string);
    }
  }

  void  setString(char* string) 
  {
    XmTextFieldSetString(getWidget(), string);
  }
  
  void  setString(const char* string) 
  {
    XmTextFieldSetString(getWidget(), (char*)string);
  }
  
  void  setStringWcs(wchar_t* wcstring) 
  {
    XmTextFieldSetStringWcs(getWidget(), wcstring);
  }
  
  void  setStringWcs(const wchar_t* wcstring) 
  {
    XmTextFieldSetStringWcs(getWidget(), (wchar_t*)wcstring);
  }
  
  void  showPosition(XmTextPosition pos) 
  {
    XmTextFieldShowPosition(getWidget(), pos);
  }
  
  XmTextPosition xYToPos(Position x, Position y) 
  {
    return XmTextFieldXYToPos(getWidget(), x, y);
  }
};

}

