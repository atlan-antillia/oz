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
 *      Text.h
 *
 *****************************************************************************/

//2017/10/10 Modified to use create method.

#pragma once

#include <oz++/motif/Primitive.h>
#include <Xm/Text.h>
//
namespace OZ {

class Text :public Primitive {

public:
  Text(View* parent, const char* name, Args& args)
  :Primitive(parent, name, xmTextWidgetClass, args)
  {
  }

  Text()
  :Primitive()
  {
  }

  virtual void create(View* parent, const char* name, Args& args)
  {
    Primitive::create(parent, name, xmTextWidgetClass, args);
  }
 
  void  add(const char* string)
  {
    XmTextPosition last = getLastPosition();
    replace(last, last, string);
  }

  void  clearSelection(Time time)
  {
    XmTextClearSelection(getWidget(), time);
  }

  Boolean copy(Time time)
  {
    return XmTextCopy(getWidget(), time);
  }

  Boolean cut(Time time)
  {
    return XmTextCut(getWidget(), time);
  }

  void  disableRedisplay()
  {
    XmTextDisableRedisplay(getWidget());
  }

  void  enableRedisplay()
  {
    XmTextEnableRedisplay(getWidget());
  }

  Boolean findString(XmTextPosition start, const char* string,
      XmTextDirection dir, XmTextPosition* pos)
  {
    return XmTextFindString(getWidget(),
        start, (char*)string, dir, pos);
  }

  Boolean findStringWcs(XmTextPosition start, wchar_t* wcstring,
                        XmTextDirection dir, XmTextPosition* pos)
  {
    return XmTextFindStringWcs(getWidget(),
                                start, wcstring, dir, pos);
  }

  int  getBaseline()
  {
    return XmTextGetBaseline(getWidget());
  }

  Boolean getEditable()
  {
    return XmTextGetEditable(getWidget());
  }

  XmTextPosition  getInsertionPosition()
  {
    return XmTextGetInsertionPosition(getWidget());
  }

  XmTextPosition  getLastPosition()
  {
    return XmTextGetLastPosition(getWidget());
  }

  int  getMaxLength()
  {
    return XmTextGetMaxLength(getWidget());
  }

  char*  getSelection()
  {
    return XmTextGetSelection(getWidget());
  }

  Boolean getSelectionPosition(XmTextPosition* left,
             XmTextPosition* right)
  {
    return XmTextGetSelectionPosition(getWidget(),
              left, right);
  }

  wchar_t*  getSelectionWcs()
  {
    return XmTextGetSelectionWcs(getWidget());
  }

  XmTextSource getSource()
  {
    return XmTextGetSource(getWidget());
  }

  char*  getString()
  {
    return XmTextGetString(getWidget());
  }

  wchar_t*  getStringWcs()
  {
    return XmTextGetStringWcs(getWidget());
  }

  int  getSubstring(XmTextPosition start, int numChar, int bufferSize,
        char* buffer)
  {
    return XmTextGetSubstring(getWidget(), start, numChar,
        bufferSize, buffer);
  }

  int  getSubstringWcs(XmTextPosition start, int numChar, int bufferSize,
        wchar_t* buffer)
  {
    return XmTextGetSubstringWcs(getWidget(), start, numChar,
        bufferSize, buffer);
  }

  XmTextPosition getTopCharacter()
  {
    return XmTextGetTopCharacter(getWidget());
  }

  void  insert(XmTextPosition pos, char* string)
  {
    XmTextInsert(getWidget(), pos, string);
  }

  void  insertWcs(XmTextPosition pos, wchar_t* wcstring)
  {
    XmTextInsertWcs(getWidget(), pos, wcstring);
  }

  Boolean load(const char* filename)
  {
    Boolean rc = False;
    FILE* fp = fopen(filename, "r");
    if(fp) {
      unsigned int length = 1000000;
      struct stat statbuf;
      if(stat(filename, &statbuf) == 0) {
        length = statbuf.st_size;
        char* string = new char[length+1];
        size_t len = fread(string, sizeof(char), length, fp);
        fclose(fp);
        if (len > 0) {
          string[length] = '\0';
          setString(string);
        }
        delete []string;
        rc = TRUE;
	  }
    }
    return rc;
  }

  Boolean paste()
  {
    return XmTextPaste(getWidget());
  }

  Boolean posToXY(XmTextPosition pos, Position* x, Position* y)
  {
    return XmTextPosToXY(getWidget(), pos, x, y);
  }

  Boolean remove()
  {
    return XmTextRemove(getWidget());
  }

  void  replace(XmTextPosition from, XmTextPosition to, const char* string)
  {
    XmTextReplace(getWidget(), from, to, (char*)string);
  }

  void  replaceWcs(XmTextPosition from, XmTextPosition to,
      wchar_t* wcstring)
  {
    XmTextReplaceWcs(getWidget(), from, to, wcstring);
  }

  Boolean save(const char* filename)
  {
    Boolean rc = False;
    char* string   = getString();
    if (string == NULL) {
      return rc;
    }

    int   length   = strlen(string);

    FILE* fp = fopen(filename, "w");
    if (fp) {
      fwrite(string, sizeof(char), length, fp);
      fclose(fp);
      rc = TRUE;
    }
    return rc;
  }


  void  scroll(int num)
  {
    XmTextScroll(getWidget(), num);
  }

  void  setAddMode(Boolean mode)
  {
    XmTextSetAddMode(getWidget(), mode);
  }

  void  setCursorPosition(XmTextPosition pos)
  {
    XmTextSetCursorPosition(getWidget(), pos);
  }

  void  setEditable(Boolean mode)
  {
    XmTextSetEditable(getWidget(), mode);
  }

  void  setHighlight(XmTextPosition left, XmTextPosition right,
        XmHighlightMode mode)
  {
    XmTextSetHighlight(getWidget(), left, right, mode);
  }

  void  setInsertionPosition(XmTextPosition pos)
  {
    XmTextSetInsertionPosition(getWidget(), pos);
  }

  void  setMaxLength(int length)
  {
    XmTextSetMaxLength(getWidget(), length);
  }

  void  setSelection(XmTextPosition first, XmTextPosition last,
        Time time)
  {
    XmTextSetSelection(getWidget(), first, last, time);
  }

  void  setSource(XmTextSource source, XmTextPosition top_char,
           XmTextPosition cursor_pos)
  {
    XmTextSetSource(getWidget(), source, top_char, cursor_pos);
  }

  void  setString(const char* string)
  {
    XmTextSetString(getWidget(), (char*)string);
  }

  void  setStringWcs(wchar_t* wcstring)
  {
    XmTextSetStringWcs(getWidget(), wcstring);
  }

  void  setTopCharacter(XmTextPosition pos)
  {
    XmTextSetTopCharacter(getWidget(), pos);
  }

  void  showPosition(XmTextPosition pos)
  {
    XmTextShowPosition(getWidget(), pos);
  }
  XmTextPosition xYToPos(Position x, Position y)
  {
    return XmTextXYToPos(getWidget(), x, y);
  }
};

}

