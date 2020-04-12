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
 *      ComboBox.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/Primitive.h>
#include <Xm/ComboBox.h>

namespace OZ {
  
class ComboBox :public Primitive {
public:
  ComboBox(View* parent, const char* name, Args& args)
  :Primitive(parent, name, xmComboBoxWidgetClass, 
		args.set(XmNpositionMode, XmZERO_BASED) ) //2017/05/01 
  {
    // Consturctor
  }

  ComboBox()
  :Primitive()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Primitive::create(parent, name, xmComboBoxWidgetClass, 
		args.set(XmNpositionMode, XmZERO_BASED) );
  }

  int  getItemCount()
  {
    int count = 0;
    get(XmNitemCount, count);
    return count;
  }

  void add(const char* items[], int count)
  {
    Widget widget = getWidget();
    int position = getItemCount();
    for (int i = 0; i<count; i++) {
      CompoundString cs(items[i]);
      XmComboBoxAddItem(widget, cs, position + i, TRUE);
    }
    update();
  }

  void add(int position, const char* string)
  {
    //position is a 1-based integer 
    Widget widget = getWidget();
    CompoundString cs(string);
    XmComboBoxAddItem(widget, cs, position, TRUE);
    //update();
  }
   
  void addFirst(const char* string)
  {
    int position = 0;
    add(position, string);
  }
  
  void addLast(const char* string)
  {
    int position = getItemCount();
    //add(position, string);
    add(position+1, string);
  }

  void add(const char* string)
  {
    addLast(string);
  }

  void removeAll()
  {
    int count = getItemCount();
    Widget widget = getWidget();
    for (int i = 0; i<count; i++) {
      XmComboBoxDeletePos(widget, 0);
    }
    update();
  }

  void clear()
  {
    removeAll();
  }

  void remove(int position)
  {
    int count = getItemCount();
    Widget widget = getWidget();
    if (position >=0 && position < count) {
      XmComboBoxDeletePos(widget, position);
    }
    update();
  }
  void select(const char* item)
  {
    Widget widget = getWidget();
    CompoundString cs(item);
    XmComboBoxSelectItem(widget, cs);
  }
  
  void update()
  {
    Widget widget = getWidget();  
    XmComboBoxUpdate(widget);
  }
};

}

