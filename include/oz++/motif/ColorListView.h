/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ColorListView.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/ScrolledWindow.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/ColoredLabel.h>

#include <oz++/motif/NamedColor.h>
#include <oz++/motif/Color.h>

namespace OZ {

class ColorListView :public ScrolledWindow {
  
private:
  SmartPtr<RowColumn> rowColumn;
  SmartPtr<NamedColor>  namedColor;
  size_t            size; 
  ColoredLabel**    colors;

public:
  ColorListView(View* parent, const char* name, Args& args)
  :ScrolledWindow(),
  size(0),
  colors(NULL)
  {
    ColorListView::create(parent, name, args);
  }

  ColorListView()
  :ScrolledWindow(),
  size(0),
  colors(NULL)
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    ScrolledWindow::create(parent, name, 
                             args.set(XmNscrollingPolicy, XmAUTOMATIC)
                                 .set(XmNtraversalOn, TRUE)
                                 .set(XmNscrollBarDisplayPolicy, XmVARIABLE));
    Args ar;
    ar.set(XmNorientation, XmVERTICAL);
    rowColumn = new RowColumn(this, "", ar);
    
    Display* display = getDisplay();
    namedColor = new NamedColor(display);

    size = namedColor->getSize();
    colors = new ColoredLabel*[size];
    
    for (size_t i = 0; i < size; i++) {
      Pixel pixel = namedColor->getNth(i);
      
      ar.reset();
      ar.set(XmNpixel, pixel);
      colors[i] = new ColoredLabel(rowColumn, "", ar);
    }

    Widget hscrollBar = (Widget)get(XmNhorizontalScrollBar);
    if (hscrollBar) {
      XtUnmanageChild(hscrollBar);
    }
  }

  ~ColorListView()
  {
    for(size_t i = 0; i<size; i++) {
      delete colors[i];
    }
    delete [] colors;
  }

  virtual void addEventHandler(EventMask mask, CommonObject* object, 
      Handler eventHandler, XtPointer data)
  {
    for(size_t i = 0; i<size; i++) {
      colors[i] -> addEventHandler(mask, object, eventHandler, 
         (XtPointer)namedColor->getNth(i));
    }
  }

  RowColumn* getRowColumn()
  {
    return rowColumn;
  }
};
}

