/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *     DataField.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/TextField.h>
#include <Xm/DataF.h>

/*
  XmNvalidateCallback 
typedef struct _XmDataFieldCallbackStruct {
    Widget   w;       // The XmDataField 
    String   text;    // Proposed string 
    Boolean  accept;  // Accept return value, for validation 
} XmDataFieldCallbackStruct  
 */
 
namespace OZ {
  
class DataField :public TextField {
public:
  DataField(View* parent, const char* name, Args& args)
  :TextField(parent, name, xmDataFieldWidgetClass, args) 
  { 
  }

  DataField()
  :TextField()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    TextField::create(parent, name, xmDataFieldWidgetClass, args); 
  }

/*  void clearSelection(Time time) 
  {
    XmDataFieldClearSelection(getWidget(), time);
  }
*/  
  Boolean copy(Time time) 
  {
    return XmDataFieldCopy(getWidget(), time);
  }
  
  Boolean cut(Time time) 
  {
    return XmDataFieldCut(getWidget(), time);
  }
  
/*  int  getBaseLine() 
  {
    return XmDataFieldGetBaseline(getWidget());
  }
	
  Boolean  getEditable() 
  {
    return XmDataFieldGetEditable(getWidget());
  }
  
  XmTextPosition  getInsertionPosition() 
  {
    return XmDataFieldGetInsertionPosition(getWidget());
  }
  
  XmTextPosition  getLastPosition() 
  {
    return XmDataFieldGetLastPosition(getWidget());
  }
  
  int  getMaxLength() 
  {
    return XmDataFieldGetMaxLength(getWidget());
  }
*/  
  char*  getSelection() 
  {
    return XmDataFieldGetSelection(getWidget());
  }
  
/*  wchar_t* getSelectionWcs() 
  {
    return XmDataFieldGetSelectionWcs(getWidget());
  }
*/  
  Boolean getSelectionPosition(XmTextPosition* left, 
             XmTextPosition* right) 
  {
    return XmDataFieldGetSelectionPosition(getWidget(),
              left, right);
  }
  
  char*  getString() 
  {
    return XmDataFieldGetString(getWidget());
  }
  
  wchar_t*  getStringWcs() 
  {
    return XmDataFieldGetStringWcs(getWidget());
  }
  
/*  int  getSubstring(XmTextPosition start, int numChar, int bufferSize,
        char* buffer) 
  {
    return XmDataFieldGetSubstring(getWidget(), start, numChar,
        bufferSize, buffer);
  }
  
  int  getSubstring(XmTextPosition start, int numChar, int bufferSize,
        wchar_t* buffer) 
  {
    return XmDataFieldGetSubstringWcs(getWidget(), start, numChar,
        bufferSize, buffer);
  }
  
  void  insert(XmTextPosition pos, char* string) 
  {
    XmDataFieldInsert(getWidget(), pos, string);
  }
  
  void  insertWcs(XmTextPosition pos, wchar_t* wcstring) 
  {
    XmDataFieldInsertWcs(getWidget(), pos, wcstring);
  }
*/  
  Boolean paste() 
  {
    return XmDataFieldPaste(getWidget());
  }
  
/*  Boolean posToXY(XmTextPosition pos, Position* x, Position* y) 
  {
    return XmDataFieldPosToXY(getWidget(), pos, x, y);
  }
  
  Boolean remove() 
  {
    return XmDataFieldRemove(getWidget());
  }
  
  void  replace(XmTextPosition from, XmTextPosition to, char* string) 
  {
    XmDataFieldReplace(getWidget(), from, to, string);
  }
  
  void  replaceWcs(XmTextPosition from, XmTextPosition to, 
      wchar_t* wcstring) 
  {
    XmDataFieldReplaceWcs(getWidget(), from, to, wcstring);
  }
*/  
  void  setAddMode(Boolean mode) 
  {
    XmDataFieldSetAddMode(getWidget(), mode);
  }
  
  void  setEditable(Boolean mode) 
  {
    XmDataFieldSetEditable(getWidget(), mode);
  }
  
  void  setHighlight(XmTextPosition left, XmTextPosition right,
        XmHighlightMode mode) 
  {
    XmDataFieldSetHighlight(getWidget(), left, right, mode);
  }
  
  void  setInsertionPosition(XmTextPosition pos) 
  { 
    XmDataFieldSetInsertionPosition(getWidget(), pos); 
  }
  
/*  void  setMaxLength(int length) 
  {
    XmDataFieldSetMaxLength(getWidget(), length);
  }
*/  
  void  setSelection(XmTextPosition first, XmTextPosition last,
        Time time) 
  {
    XmDataFieldSetSelection(getWidget(), first, last, time);
  }
  
  void  setString(char* string) 
  {
    XmDataFieldSetString(getWidget(), string);
  }
  
/*  void  setStringWcs(wchar_t* wcstring) 
  {
    XmDataFieldSetStringWcs(getWidget(), wcstring);
  }
*/  
  void  showPosition(XmTextPosition pos) 
  {
    XmDataFieldShowPosition(getWidget(), pos);
  }
  
  XmTextPosition xYToPos(Position x, Position y) 
  {
    return XmDataFieldXYToPos(getWidget(), x, y);
  }
};

}

