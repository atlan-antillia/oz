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
 *      Container.h
 *
 *****************************************************************************/

// 2015/01/22
// 2015/08/28 Added HashTable member variable to manage elements(IconGadget 
// objects)  contained in this Container object.

// 2015/09/05 Modified the member variriable table to a ptr to HashTable.
// 2015/09/05 Added removeAllItems method.

#pragma once

#include <oz++/HashTable.h>
#include <oz++/motif/Manager.h>
#include <Xm/Container.h>
#include <Xm/ContainerP.h>

namespace OZ {
  
class Container :public Manager {
private:
  static const int HASH_TABLE_SIZE = 971;

private:
  SmartPtr<HashTable> table;
  
public:
  Container(View* parent, const char* name, Args& args) 
  :Manager()
  {
    Container::create(parent, name, args);
  }

  Container()
  :Manager()
  {
  }

  void create(View* parent, const char* name, Args& args) 
  {
    Manager::create(parent, name, xmContainerWidgetClass, args);
    //2015/09/05
    table = new HashTable(HASH_TABLE_SIZE, false);

    if(!parent->isDialog()) {
      manage();
    }
  }

  ~Container()
  {
    unmanage();
  }

  //<added date="2015/08/30">
  int  getItemChildren( Widget     item,
                        WidgetList  *item_children)
  {
    return XmContainerGetItemChildren(
                        getWidget(),
                        item,
                       item_children);
  }

  void relayout()
  {
    XmContainerRelayout( getWidget() );
  }

  void  reorder( WidgetList cwid_list,
               int         cwid_count)
  {
    XmContainerReorder(getWidget(),
                       cwid_list,
                       cwid_count);
  }

  Boolean cut(Time timestamp)
  {
    return XmContainerCut(getWidget(), timestamp);
  }

  Boolean copy(Time timestamp)
  {
    return XmContainerCopy(getWidget(), timestamp);
  }

  Boolean paste()
  {
    return XmContainerPaste( getWidget() );
  }

  Boolean copyLink(Time  timestamp)
  {
    return XmContainerCopyLink(getWidget(), timestamp);
  }

  Boolean pasteLink()
  {
    return XmContainerPasteLink(getWidget());
  }
  //</added>

  void add(View* view) 
  {
    Widget w = view ->getWidget();
    table->add((Key)w, view, 0);
  }

  View* lookup(Widget w) 
  {
     return (View*)table->lookup((Key)w, 0);
  }

  void removeAllItems()
  {
     table -> enableGC();
     table -> clear();
     delete table;
     table = new HashTable(HASH_TABLE_SIZE, false);
  }

  //2015/09/06
  void clearSelectedItems()
  {
    Widget w = getWidget();
    ((XmContainerWidget)(w))->container.selected_items      = NULL; 
    ((XmContainerWidget)(w))->container.selected_item_count = 0; 
  }     

};
}

