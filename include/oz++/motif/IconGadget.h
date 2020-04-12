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
 *      IconGadget.h
 *
 *****************************************************************************/

//2015/08/28 Added the following methods:
// setParent, getParent, getHierarchy, find.
//2017/10/15 Updated to use create method.

#pragma once

#include <oz++/motif/Gadget.h>
#include <Xm/IconG.h>
#include <oz++/File.h>
#include <oz++/LinkedListT.h>
#include <oz++/LinkedList.h>
#include <oz++/StringBufferT.h>

namespace OZ {

const char* XmNnodeType = "nodeType";
  
class IconGadget :public Gadget {
public:
  typedef enum {
    FOLDER=1,
    FILE,
  } NODE_TYPE;

private:
  LinkedList list;
  View*      container;
  IconGadget* parent;
  NODE_TYPE   nodeType;

public:
  IconGadget(View* container, const char* name, Args& args)
  :Gadget(container, name, xmIconGadgetClass, args),
  container(container),
  parent(NULL),
  nodeType(FILE)
  {
    //Constructor
  }
	
public:
  IconGadget()
  :Gadget(),
  container(NULL),
  parent(NULL),
  nodeType(FILE)
  {
    //Constructor
  }

public:
  void create(View* container, const char* name, Args& args)
  {
    Gadget::create(container, name, xmIconGadgetClass, args);
    this->container = container;
  }


public:
  IconGadget* add(const char* name, Args& args)
  {
    args.set(XmNentryParent, this->getWidget());
    //ar.set(XmNoutlineState, XmEXPANDED);
    args.set(XmNlayoutType, XmDETAIL);

    IconGadget* gadget = new IconGadget(container, name, args);
    gadget -> setParent(this);

    gadget -> nondestructive();
    list.add(gadget);
  	
    return gadget;
  }
	
  ~IconGadget()
  {
    unmanage();
  }

  void setNodeType(NODE_TYPE type)
  {
     nodeType = type;
  }

  NODE_TYPE getNodeType()
  {
     return nodeType;
  }

  void setParent(IconGadget* gadget)
  {
     parent = gadget;
  }

  IconGadget* getParent()
  {
     return parent;
  }

  //2015/08/28

  int getHierarchy(StringBufferT<char>& path, const char* delim="/")
  {
    LinkedList nodes(false); 

    int count = getHierarchy(nodes);

    ListEntryT<CommonObject>* ptr  = nodes.getEntry();

    while (ptr) {
      IconGadget* object = (IconGadget*)ptr -> getObject();
      if (object) {
        XmString xms; 
        object -> get(XmNlabelString, &xms);
        StringT<char> label;
        CompoundString cs(xms, label);
        path.append((const char*)label);
        printf("getParentLabels label %s\n", (const char*)label);

        path.append(delim);
      }
      ptr = ptr -> getNext();
    }
  }

  //2015/08/28
  int getHierarchy(LinkedList& nodes)
  {
    nodes.addFirst(this); 
    IconGadget* parent = getParent();
    if (parent) {
      return parent -> getHierarchy(nodes);
    } else {
      return nodes.getLength();
    }
  }

  //2015/08/28
  IconGadget* find(Widget widget)
  {
    IconGadget* found = NULL;
    ListEntryT<CommonObject>* ptr  = list.getEntry();

    while (ptr) {
      IconGadget* object = (IconGadget*)ptr -> getObject();
      if (object) {
        Widget w = object -> getWidget();
        if (widget == w) {
          found = object;
          break;
        }
      }
      ptr = ptr -> getNext();
    }
    return found;
  }

 //2017/05/29
  IconGadget* find(const char* name)
  {
    IconGadget* found = NULL;
    ListEntryT<CommonObject>* ptr  = list.getEntry();

    while (ptr) {
      IconGadget* object = (IconGadget*)ptr -> getObject();
      if (object) {
        XmString xms;
        object -> get(XmNlabelString, &xms);
        StringT<char> label;
        CompoundString cs(xms, label);
        const char* objectName = (const char*)label;
        printf("%s %s\n", objectName, name); 
        if (name && objectName && 
            strcmp(name, objectName) == 0) {
           found = object;
           break;
        }
      }
      ptr = ptr -> getNext();
    }
    return found;
  }

  bool hasChildren()
  {
    bool rc = false;
    if (list.getLength() > 0) {
       rc = true;
    }
    return rc;
  }
};

}

