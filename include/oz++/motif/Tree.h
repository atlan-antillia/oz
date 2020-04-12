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
 *      Tree.h
 *
 *****************************************************************************/

//2015/6/01/31 Updated
//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/Manager.h>
#include <Xm/Tree.h>
#include <Xm/TreeP.h>
#include <oz++/LinkedListT.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/Label.h>

#include <oz++/motif/PushButtonGadget.h>
#include <oz++/motif/LabelGadget.h>

#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>

//OpenMotif 2.2 and later. (Contributed Widget)
//Core - Composite - Constraint - XmManager - XmHierarchy - XmTree

namespace OZ {
  
class Tree :public Manager {
private:
  //The following list is used for garbage collection of View nodes created on Tree.
  LinkedList nodes;
 
public:
  Tree(View* parent, const char* name, Args& args)
  :Manager(parent, name, xmTreeWidgetClass, args) 
  {
  }

  Tree()
  :Manager()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Manager::create(parent, name, xmTreeWidgetClass, args); 
  }
 
  int horizontalNodeSpace()
  { 
    Widget w = getWidget();
    return XmTree_h_node_space(w);
  }

  int verticalNodeSpace()
  { 
    Widget w = getWidget();
    return XmTree_v_node_space(w);
  }
 
  int connectStyle()
  { 
    Widget w = getWidget();
    return XmTree_connect_style(w);
  }
 
  int maxWidth()
  { 
    Widget w = getWidget();
    return XmTree_max_width(w);
  }
 
  int maxHeight()
  { 
    Widget w = getWidget();
    return XmTree_max_height(w);
  }
 
  //List of child operations
  XmList childOperationpList()
  { 
    Widget w = getWidget();
    return XmTree_child_op_list(w); 
  }
 
  XPoint ulPoint()
  { 
    Widget w = getWidget();
    return XmTree_ul_point(w);
  }
 
 
  XPoint lrPoint()
  { 
    Widget w = getWidget();
    return XmTree_lr_point(w);
  }
 
  int orientationt()
  { 
    Widget w = getWidget();
    return XmTree_orientation(w);
  }
 
  int compressStyle()
  { 
    Widget w = getWidget();
    return XmTree_compress_style(w);
  }
 
  int verticalDelta()
  { 
    Widget w = getWidget();
    return XmTree_vertical_delta(w);
  }
 
  int horizontalDelta()
  { 
    Widget w = getWidget();
    return XmTree_horizontal_delta(w);
  }

public:
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


