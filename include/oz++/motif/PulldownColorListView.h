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
 *  PulldownColorListView.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/PopupView.h>
#include <oz++/motif/ColorListView.h>
#include <oz++/StrDef.h>

namespace OZ {

class PulldownColorListView :public PopupView {
private:
  static const int          WIDTH = 400;
  static const int          HEIGHT= 400;
  SmartPtr<BulletinBoard>   bboard;  
  SmartPtr<ColorListView>   colorListView;
  Pixel                     pixel; 
  ColoredLabel*               notifier;

  static const int          COLOR_WIDTH = 160;
  static const int          COLOR_HEIGHT = 40;

private:
  void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe -> type == MapNotify || xe -> type == ConfigureNotify) {
      layout();
    }
  }
    
  void layout()
  {
    int margin = 2;
    Dimension w = bboard -> width();
    Dimension h = bboard -> height();
    Dimension th = 40;

    colorListView -> reshape(0, 0, w, h);
  }

private:
  void selected(Event& event)
  {
    View* sender = (View*)event.getSender();
    sender -> processTraversal();

    if (event.isEscapeKeyPressed()) {
      ungrab();
      popdown();
      return;
    }

    if (event.isEnterKeyPressed() ||  
        event.isButtonPressed() ) {
      pixel = (Pixel)event.getClientData();
      printf("selected: %06lx\n", pixel);
      if (notifier) {
        notifier->setColor(pixel);
      }
      ungrab();
      popdown();
    //  View* sender = (View*)event.getSender();
    //  sender -> processTraversal();
    }
  }

public:
  PulldownColorListView(View* parent, const char* name, Args& args)
  :PopupView()
  {
    PulldownColorListView::create(parent, name, args);
  }

  PulldownColorListView()
  :PopupView()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    PopupView::create(parent, 
            name, 
            //transientShellWidgetClass,
            xmDialogShellWidgetClass,
                              //args.set(XmNoverrideRedirect, TRUE)
                              args.set(XmNtraversalOn,     TRUE)
                                  .set(XmNmwmDecorations, 0)
                                  .setIfNotSet(XmNwidth,  WIDTH)
                                  .setIfNotSet(XmNheight, HEIGHT) );
    notifier = NULL;
   
    int W = args.get(XmNwidth);
    int H = args.get(XmNheight);

    set(XmNminWidth, W); 
    set(XmNmaxWidth, W) ;
    set(XmNminHeight,H);
    set(XmNmaxHeight, H);

    Color color(this);

    Args ar;
    ar.reset();
    ar.set(XmNtopAttachment,    XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM);
    ar.set(XmNleftAttachment,   XmATTACH_FORM);

//    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNmarginHeight, 0);
    ar.set(XmNmarginWidth,  0);

    bboard = new BulletinBoard(this, "", ar);
 
    ar.reset();
    ar.set(XmNshowColorName, true);
    ar.set(XmNhighlightThickness, 1); 

    colorListView = new ColorListView(bboard, "",ar);
    colorListView ->addEventHandler(ButtonPressMask, this,
             (Handler)&PulldownColorListView::selected, NULL);
    colorListView ->addEventHandler(KeyPressMask, this,
             (Handler)&PulldownColorListView::selected, NULL);

    //Widget hscb = (Widget)colorListView->get(XmNhorizontalScrollBar);
    //XtUnmanageChild(hscb);
    
    addEventHandler(StructureNotifyMask, this,
          (Handler)&PulldownColorListView::structureNotify, NULL);
  }

  ~PulldownColorListView()
  {
  }

  Pixel  getPixel()
  {
    return pixel;
  }

  void setNotifier(ColoredLabel* colorLabel)
  {
    notifier = colorLabel;
  }

  void grab()
  {
    RowColumn* rowColumn = colorListView->getRowColumn();
    rowColumn -> grabPointer(None);
  }

  void ungrab()
  {
    RowColumn* rowColumn = colorListView->getRowColumn();
    rowColumn->ungrabPointer(CurrentTime);
  }
/*
  Boolean isDialog() const
  {
    return TRUE;
  }
*/
};
}

