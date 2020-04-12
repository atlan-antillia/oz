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
 *      Outline.h
 *
 *****************************************************************************/

//2015/08/09 Updated
//2017/10/15 Modifed to use create method.

#pragma once

#include <oz++/motif/Manager.h>
#include <Xm/Outline.h>
#include <Xm/OutlineP.h>
#include <oz++/LinkedListT.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/Label.h>

#include <oz++/motif/PushButtonGadget.h>
#include <oz++/motif/LabelGadget.h>

#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>

//Core -> Composite -> Constraint -> XmManager -> XmHierarchy -> XmOutline

namespace OZ {
  
class Outline :public Manager {
private:
  //The following list is used for a garbage collection of View nodes created on Tree.
  LinkedList nodes;
 
public:
  Outline(View* parent, const char* name, Args& args)
  :Manager(parent, name, xmOutlineWidgetClass, args) 
  {
  }

  Outline()
  :Manager()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Manager::create(parent, name, xmOutlineWidgetClass, args);
  }

  OutlineConstraints topNodeOfDisplay()
  {
    Widget w = getWidget();
    return XmOutline_top_node_of_display(w);
  }

  Dimension maxWidth()
  {
    Widget w = getWidget();
    return XmOutline_max_width(w);
  }

  Dimension maxWidgetWidth()
  {
    Widget w = getWidget();
    return XmOutline_max_widget_width(w);
  }

  XmList childOperationsList()
  {
    Widget w = getWidget();
    return XmOutline_child_op_list(w);
  }

  XPoint ulPoint()
  {
    Widget w = getWidget();
    return XmOutline_ul_point(w);
  }
  XPoint lrPoint()
  {
    Widget w = getWidget();
    return XmOutline_lr_point(w);
  }

  GC drawGC()
  {
    Widget w = getWidget();
    return XmOutline_draw_gc(w);
  }

  Dimension indentSpace()
  {
    Widget w = getWidget();
    return XmOutline_indent_space(w);
  }

  Boolean constrainWidth()
  {
    Widget w = getWidget();
    return XmOutline_constrain_width(w);
  }

  Boolean connectNodes()
  {
    Widget w = getWidget();
    return XmOutline_connect_nodes(w);
  }

  View* addPushButton(View* parent_node, const char* name, Args& args,
        CommonObject* object = NULL, Callback callback=NULL)
  {
    if (args.has(XmNnodeState) == False) {
      args.set(XmNnodeState, XmOpen);  //default
    }
    if (parent_node != NULL) {
      args.set(XmNparentNode, parent_node->getWidget());
    }
    if (args.has(XmNlineStyle) == False) {
      args.set(XmNlineStyle, 99);
    }
    PushButton* pushb = new PushButton(this, name, args);
    pushb -> nondestructive();
    if (object && callback) {
      pushb -> addCallback(XmNactivateCallback, object,
            callback, (XtPointer)name);
    }
    nodes.add(pushb);
    return pushb;
  }

public:
  View* addLabel(View* parent_node, const char* name, Args& args)
  {
    if (args.has(XmNnodeState) == False) {
      args.set(XmNnodeState, XmOpen);  //default
    }
    if (parent_node != NULL) {
      args.set(XmNparentNode, parent_node->getWidget());
    }
    if (args.has(XmNlineStyle) == False) {
      args.set(XmNlineStyle, 99);
    }
    
    Label* label = new Label(this, name, args);
    label -> nondestructive();
    nodes.add(label);
    return label;
  }


public:
  int getParentNodes(View* node, LinkedListT<CompoundString>& list)
  {
    int count = 0;
    if (node == NULL) {
      return count; 
    }
  
    //The following views list can be used to GC for temporary created View objects. 
    LinkedListT<View> views;
   
    View* work =new View(node->getWidget());
    views.add(work);
   
    count++;
    XmString xms;
    work -> get(XmNlabelString, (XtArgVal)&xms);
    list.addFirst(new CompoundString(xms));
   
    while (true) {
      Widget parent = NULL;
      work ->get(XmNparentNode, (XtArgVal)&parent);
      if (parent == NULL) {
        break;
      }
      View* next = new View (parent);
      views.add(next);
      next ->get(XmNlabelString, (XtArgVal)&xms);
      work = next;
      list.addFirst(new CompoundString(xms));
       count++;
    }
    return count;
  }
};

}


