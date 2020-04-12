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
 *      CompoundString.h
 *
 *****************************************************************************/

// 2015/01/22 Modified to use XmStringCreateLocalized in CompounString constructor.

// 2015/08/28 Added a contructor and unparse methods to return char* string to
// the StringT<char>& argument of each method in the following way.
//
//  CompoundString(XmString text, StringT<char>& returnString)
//  void unparse(StringT<char>& returnString)

#pragma once

#include <oz++/Collection.h>
#include <oz++/StringT.h>
#include <oz++/motif/DC.h>

namespace OZ {
  
class CompoundString :public Collection {
private:
  Position  x;
  Position  y;
  unsigned char  alignment;
  unsigned char  direction;
  Dimension  width;
  XRectangle*  clip;
  XmString  string;
  
public:
  CompoundString()
  {
    x   = y = 0;
    alignment = XmALIGNMENT_CENTER;
    direction = XmSTRING_DIRECTION_L_TO_R;
    clip = NULL;
    width = 0;
    string = NULL;
  }

public:
  CompoundString(const char* text)
  {
    x   = y = 0;
    alignment = XmALIGNMENT_CENTER;
    direction = XmSTRING_DIRECTION_L_TO_R;
    clip = NULL;
    width = 0;
    //2015/01/22 Modified
    string = XmStringCreateLocalized((char*)text);
  }

public:
  //2018/04/10
  CompoundString(StringT<char>& text)
  {
    x   = y = 0;
    alignment = XmALIGNMENT_CENTER;
    direction = XmSTRING_DIRECTION_L_TO_R;
    clip = NULL;
    width = 0;
    //2015/01/22 Modified
    string = XmStringCreateLocalized((char*)(const char*)text);
  }

public:
  //2018/04/10
  CompoundString(std::string& text)
  {
    x   = y = 0;
    alignment = XmALIGNMENT_CENTER;
    direction = XmSTRING_DIRECTION_L_TO_R;
    clip = NULL;
    width = 0;
    //2015/01/22 Modified
    string = XmStringCreateLocalized((char*)text.c_str());
  }

public:
  //Constructor
  CompoundString(XmString text)
  {
    x   = y = 0;
    alignment = XmALIGNMENT_CENTER;
    direction = XmSTRING_DIRECTION_L_TO_R;
    clip = NULL;
    width = 0;
    //Make a copy of text argument
    string = XmStringCopy(text);
  }

public:
  //2015/08/28 
  //Constructor
  CompoundString(XmString text, StringT<char>& charString)
  {
    x   = y = 0;
    alignment = XmALIGNMENT_CENTER;
    direction = XmSTRING_DIRECTION_L_TO_R;
    clip = NULL;
    width = 0;
    //Make a copy of text argument
    string = XmStringCopy(text);
    char* str = NULL;
    get(&str);
    charString = str;
    XtFree(str);
  }


public:
  CompoundString(const char* text, char* tag, XmStringDirection dir, 
      Boolean sep)
  {
    x   = y = 0;
    alignment = XmALIGNMENT_CENTER;
    direction = dir; 
    clip = NULL;
    width = 0;
    string = XmStringSegmentCreate((char*)text, tag, direction, sep); 
  }

public:
  //2015/02/07
  CompoundString(XmStringComponentType component_type, //XmSTRING_COMPONENT_TAB
       unsigned int length = 0,
       XtPointer value     = NULL)
  {
    x   = y = 0;
    alignment = XmALIGNMENT_CENTER;
    direction = XmSTRING_DIRECTION_L_TO_R;
    clip = NULL;
    width = 0;
 
    string = XmStringComponentCreate(
       component_type,
       length,
       value);
  }
	

public:
 //2015/02/07
  CompoundString(const char* text, 
  	XmStringTag rendition, 
  	XmTextType   type = XmCHARSET_TEXT, //XmMULTIBYTE_TEXT or XmWIDECHAR_TEXT. If a charset should be created, type has a value of XmCHARSET_TEXT
 	XmStringTag  tag  = XmFONTLIST_DEFAULT_TAG             //XmFONTLIST_DEFAULT_TAG //XmFONTLIST_DEFAULT_TAG
   )
  {
    x   = y = 0;
    alignment = XmALIGNMENT_CENTER;
    direction = XmSTRING_DIRECTION_L_TO_R;
    clip = NULL;
    width = 0;
    string = XmStringGenerate((XtPointer)text, tag, type, rendition);
  	
  }

  ~CompoundString() 
  {
    XmStringFree(string);
  }

  //2015/01/22 Added
  char* unparse()
  {
    return (char *) XmStringUnparse(string, NULL,
                                    XmCHARSET_TEXT,
                                    XmCHARSET_TEXT,
                                    NULL, 0,
                                    XmOUTPUT_ALL);

  }

  //2015/08/28 Added
  void unparse(StringT<char>& charString)
  {
    char* unparsed = (char*) XmStringUnparse(string, NULL,
                                    XmCHARSET_TEXT,
                                    XmCHARSET_TEXT,
                                    NULL, 0,
                                    XmOUTPUT_ALL);
    charString = unparsed;
    XtFree(unparsed);
  }

  void  setAlignment(unsigned char alignment1) 
  {
    alignment = alignment1; 
  }
  
  void  append(const char* string1) 
  {
    XmString xms =XmStringCreateLocalized((char*)string1);
    string = XmStringConcat(string, xms);
  }
  
  XmString  get() 
  {
    return string;
  }

  operator XmString() 
  {
    return string;
  }

  void     get(char** string1)
  {
    XmStringContext    context;
    XmStringCharSet    charset;
    XmStringDirection  direction;
    Boolean            separator;

    XmStringInitContext(&context, string);
    XmStringGetNextSegment(context, string1, &charset,
                          &direction, &separator);
    XmStringFreeContext(context);
  }

  Dimension baseline(XmRenderTable renderTable) 
  {
    return XmStringBaseline(renderTable, string);
  }
  
  Boolean  byteCompare(CompoundString* cs) 
  {
    return XmStringByteCompare(string, cs->get());
  }
  
  void  setClipRectangle(XRectangle* rect) 
  {
    clip = rect;
  }
  
  Boolean  compare(CompoundString* cs) 
  {
    return XmStringCompare(string, cs->get());
  }
  
/*
  CompoundString* concat(CompoundString* cs) 
  {
    string = XmStringConcat(string, cs->get());
    return this; 
  }
*/
	
  CompoundString& concat(CompoundString& cs) 
  {
    string = XmStringConcat(string, cs.get());
    return *this; 
  }

  CompoundString& concatAndFree(CompoundString& cs) 
  {
    string = XmStringConcatAndFree(string, cs.get());
    return *this; 
  }

  void    draw(DC* dc)
  {
    Dimension width1 = width;
    if(width1 == 0) {
      width1 = getWidth(dc->getRenderTable());
    }
    XmStringDraw(dc->getDisplay(), dc->getDrawable(), 
            dc -> getRenderTable(), string, dc->getGC(), x, y,
            width1, alignment, direction, clip);
  }
	
  //2015/01/15
  void    draw(int x1, int y1, DC* dc)
  {
    Dimension width1 = width;
    if(width1 == 0) {
      width1 = getWidth(dc->getRenderTable());
    }
    XmStringDraw(dc->getDisplay(), dc->getDrawable(), 
            dc -> getRenderTable(), string, dc->getGC(), x1, y1,
            width1, alignment, direction, clip);
  }
  
  void    drawImage(DC* dc)
  {
    Dimension width1 = width;
    if(width1 == 0) {
      width1 = getWidth(dc->getRenderTable());
    }
    XmStringDrawImage(dc->getDisplay(), dc->getDrawable(), 
            dc -> getRenderTable(), string, dc->getGC(), x, y,
            width1, alignment, direction, clip);
  }

  void    drawImage(int x1, int y1, DC* dc)
  {
    Dimension width1 = width;
    if(width1 == 0) {
      width1 = getWidth(dc->getRenderTable());
    }
    XmStringDrawImage(dc->getDisplay(), dc->getDrawable(), 
            dc -> getRenderTable(), string, dc->getGC(), x1, y1,
            width1, alignment, direction, clip);
  }

  void  drawUnderline(DC* dc, XmString underline)
  {
    Dimension width1 = width;
    if(width1 == 0) {
      width1 = getWidth(dc->getRenderTable());
    }
    XmStringDrawUnderline(dc->getDisplay(), dc->getDrawable(), 
               dc->getRenderTable(), string, dc->getGC(), x, y,
               width1, alignment, direction, clip, underline);
  }

  void  setDirection(unsigned char dir) 
  {
    direction = dir; 
  }
  
  Boolean  empty() 
  {
    return XmStringEmpty(string);
  }
  
  void  extent(XmRenderTable renderTable, 
      Dimension* width, Dimension* height) 
  {
    XmStringExtent(renderTable, string, width, height);
  }
  
  Boolean getContent(char** text) 
  {
    return XmStringGetLtoR(string, XmFONTLIST_DEFAULT_TAG, text);
  }
  
  unsigned char  getDirection() 
  {
    return direction;
  }
  
  unsigned char  getAlignment() 
  {
    return alignment;
  }

  Boolean  hasSubstring(XmString substring) 
  {
    return XmStringHasSubstring(string, substring);
  }
  
  Dimension height(XmRenderTable renderTable) 
  {
    return XmStringHeight(renderTable, string);
  }
  
  int  length() 
  {
    return XmStringLength(string);
  }
  
  int  lineCount() 
  {
    return XmStringLineCount(string);
  }
  
  void  move(Position x1, Position y1) 
  {
    x = x1; y = y1;
  }
  
  void  set(XmString xms) 
  {
    string = xms;
  }

  //2017/10/10
  void setCopy(XmString xms)
  {
    //Make a copy of text argument
    string = XmStringCopy(xms);
  }

  //2017/10/10
  void set(const char* text)  
  {
    string = XmStringCreateLocalized((char*)text);
  }

  Boolean isEmpty()
  {
    if (string) {
      return TRUE;
    } else {
      return False;
    }
  }

  void  setWidth(Dimension width1) 
  {
    width = width1;
  }

  char*  toCompoundText() 
  {
     return  XmCvtXmStringToCT(string);
  }

  Dimension getWidth(XmRenderTable renderTable) 
  {
    return XmStringWidth(renderTable, string); 
  }
	
  Dimension getHeight(XmRenderTable renderTable) 
  {
    return XmStringHeight(renderTable, string); 
  }

};

}


