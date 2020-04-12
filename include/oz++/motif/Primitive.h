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
 *      Primitive.h
 *
 *****************************************************************************/

//2017/10/10 Modified to use create method.

#pragma once

#include <oz++/motif/View.h>
#include <oz++/motif/ErrorDialog.h>

namespace OZ {
  
class Primitive :public View {
private:
  View* parent;

protected:
  Primitive(View* parent, const char* name, 
      const WidgetClass widgetClass, Args& args)
  :parent(parent)
  {
    Primitive::create(parent, name, widgetClass, args);
  }

  Primitive(Widget widget)
  :View(widget),
  parent(NULL)
  {
  }

  Primitive()
  :View(),
  parent(NULL)
  {
  }
  
  ~Primitive()
  {
    parent=NULL;
  }

  virtual void create(View* parent, const char* name, 
      const WidgetClass widgetClass, Args& args)
  {
    this->parent = parent;

    if (parent == NULL) {
      showErrorDialog("Primitive: parent is NULL\n");
      ::exit(0);
    }
    Widget primitive = XtCreateWidget((char*)name, widgetClass,
                                parent -> getWidget(),
                                args.getArgList(),
                                args.count() );
    if (primitive == NULL) {
      throw IException("Failed to XtCreateWidget.\n");
    }
    setWidget(primitive);

    if(!parent->isDialog()) {
      manage();
    }
  }

  View* getParent()
  {
    return parent;
  }

  virtual void showErrorDialog(const char* message)
  {
     ::printf("Primitive::showErrorDialog %s\n", message);
  }
};

}

