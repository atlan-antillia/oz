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
 *      CompoundStringList.h
 *
 *****************************************************************************/

//2015/02/03 Updated
//2015/02/03 Added a new constructor

#pragma once

#include <oz++/Collection.h>
#include <oz++/Exception.h>
#include <Xm/Xm.h>
#include <oz++/motif/CompoundString.h>

namespace OZ {
 
class CompoundStringList :public Collection {
private:
  static const int MAX = 1024;
  int    count;
  int     max;
  XmString* xmsList;
	
private:
  //2015/02/03
  void realloc()
  {
    int cmax = max;
    //1 Define new max size
    max += MAX;
    //2 Allocate XmString array of new size max
    XmString* list = new XmString[max];
    //3 Nullify
    for (int i = 0; i<max; i++) {
      list[i] = NULL;	
    }
    //4 Copy already allocate xmsList to the new list
    for (int n = 0; n<cmax; n++) {
      list[n] = xmsList[n];
    }
    xmsList = list;
  }
  
	
public:
  //2015/02/15
  CompoundStringList()
  :count(0),
  xmsList(NULL),
  max(MAX)
  {
    count = 0;
    xmsList = new XmString[max];
    for(int i = 0; i< max; i++) {
      xmsList[i] = NULL;
    } 
  }


public:
  //2015/02/22 Modified
 void add(const char* string)
  {
    if (string) {
      if (count >= max) {
        realloc();		
      }
      //2015/02/22
      //printf("%d %s\n", count, string);
      xmsList[count++] = XmStringCreateLocalized((char*)string);
    } else {
      throw IException("Invalid argument");	
    }
  }
	
public:
  //2015/02/03
  void add(const char** string, Cardinal num)
  {
    if (string && num > 0) {
      for (int i = 0; i<num; i++) {
        add(string[i]);
      }
    } else {
      throw IException("Invalid argument");	
    }
  }

public:
  //2015/02/03
  void add(const XmString tab, const char** string, Cardinal num)
  {
    if (count >= max) {
      realloc();
    }
    xmsList[count++] = tab;
  }
	
  //2015/02/22	
public:
  //type will take a value XmMULTIBYTE_TEXT, XmWIDECHAR_TEXT or XmCHARSET_TEXT. 
  CompoundStringList(char** strings, Cardinal num, 
  	  XmStringTag tag, XmTextType type, XmStringTag rendition)
  :count(0),
  xmsList(NULL),
  max(0)
  {
    if (strings && num > 0) {
      count = num;
      max   = num;
      xmsList = new XmString[count];
      for(int i = 0; i< count; i++) {
        xmsList[i] = XmStringGenerate((XtPointer)strings[i], tag, type, rendition);
      } 
    } else {
      throw IException("Invalid argument");	
    }
  }
	
public:
  CompoundStringList(const char** strings, Cardinal num)
  :count(0),
  xmsList(NULL),
  max(0)
  {
    if (strings && num > 0) {
      count = num;
      max   = num;
      xmsList = new XmString[count];
      for(int i = 0; i< count; i++) {
        xmsList[i] = XmStringCreateLocalized((char*)strings[i]);
      } 
    } else {
      throw IException("Invalid argument");	
    }
  }
	
public:
  CompoundStringList(char** strings, Cardinal num)
  :count(0),
  xmsList(NULL),
  max(0)
  {
    if (strings && num > 0) {
      count = num;
      max   = num;
      xmsList = new XmString[count];
      for(int i = 0; i< count; i++) {
        xmsList[i] = XmStringCreateLocalized((char*)strings[i]);
      } 
    } else {
      throw IException("Invalid argument");	
    }
  }

  ~CompoundStringList()
  {
    for(int i = 0; i< max; i++) {
      XmStringFree(xmsList[i]);
      xmsList[i] = NULL;
    } 
    delete [] xmsList; //2015/02/03
    xmsList = NULL;
  }

public:
  //2015/02/22
  void add(char* string, XmStringTag tag, XmTextType type, XmStringTag rendition)
  {
    if (string) {
      if (count >= max) {
        realloc();		
      }
  	  //printf("count=%d %s\n", count, string);
      xmsList[count++] = XmStringGenerate((XtPointer)string, tag, type, rendition);	
     } else {
       throw IException("Invalid argument");	
    }
  }

  XmString* get() 
  { 
     return xmsList; 
  }

  int  getLength() 
  { 
    return count; 
  }

  int  size() 
  { 
    return count; 
  }

  XmString  get(Cardinal num) 
  { 
    if (num < count) {
      return xmsList[num];
    } else {
      throw IException("Invalid argument %d", num);	
    }
  }
};

}

