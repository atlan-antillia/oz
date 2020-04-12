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
 *      ScrollBar.h
 *
 *****************************************************************************/

//2017/10/16 Modified to use create method.

#pragma once

#include <oz++/motif/Primitive.h>
#include <Xm/ScrollBar.h>

namespace OZ {
  
class ScrollBar :public Primitive {
public:
  ScrollBar(View* parent, const char* name, Args& args)
  :Primitive(parent, name, xmScrollBarWidgetClass, args) 
  {
  }

  ScrollBar()
  :Primitive()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Primitive::create(parent, name, xmScrollBarWidgetClass, args);
  }

  ScrollBar(Widget w)
  :Primitive(w)
  {
  }

  void getValue(int& val)
  {
    int value = 0;
    int size  = 0;
    int inc   = 0;
    int page_inc = 0;
    getValues(&value, &size, &inc, &page_inc);
    val = value;
  }

  void getSize(int& val)
  {
    int value = 0;
    int size  = 0;
    int inc   = 0;
    int page_inc = 0;
    getValues(&value, &size, &inc, &page_inc);
    val = size;
  }

  void getInc(int& val)
  {
    int value = 0;
    int size  = 0;
    int inc   = 0;
    int page_inc = 0;
    getValues(&value, &size, &inc, &page_inc);
    val = inc;
  }

  void getPageInc(int& val)
  {
    int value = 0;
    int size  = 0;
    int inc   = 0;
    int page_inc = 0;
    getValues(&value, &size, &inc, &page_inc);
    val = page_inc;
  }

  void getValues(int* value, int* size, int* inc, int* page_inc) 
  {
    XmScrollBarGetValues(getWidget(), value, size, 
          inc, page_inc);
  }
  
  void setValues(int value, int size, int inc, int page_inc, 
      Boolean notify) 
  {
    XmScrollBarSetValues(getWidget(), value, size, 
          inc, page_inc, notify);
  }
};

}

