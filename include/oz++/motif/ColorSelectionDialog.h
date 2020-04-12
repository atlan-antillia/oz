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
 *  ColorSelectionDialog.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/CustomDialog.h>
#include <oz++/motif/ColorListView.h>
#include <oz++/motif/DrawingArea.h>
#include <oz++/motif/PushButton.h>
#include <oz++/StrDef.h>

namespace OZ {

class ColorSelectionDialog :public CustomDialog {
private:
  static const int          WIDTH = 400;
  static const int          HEIGHT= 400;
  SmartPtr<BulletinBoard>   bboard;  
  SmartPtr<ColorListView>   colorListView;
  SmartPtr<DrawingArea>     area;
  SmartPtr<Label>           value;
  Pixel                     pixel; 

  static const int          COLOR_WIDTH = 160;
  static const int          COLOR_HEIGHT = 40;
  SmartPtr<Pixelmap>        pixelmap;
  SmartPtr<DC>              back;

public:
  enum {OK_BUTTON =0 , CANCEL_BUTTON, FILTER_BUTTON, HELP_BUTTON};
    
private:
  void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe -> type == MapNotify || xe -> type == ConfigureNotify) {
      layout();
    }
  }
    
  void    draw(Event& event)
  {
     DC dc(area);
     dc.copyArea(pixelmap->get(), 0, 0, 
           COLOR_WIDTH, COLOR_HEIGHT, 0, 0);
  }

  void layout()
  {
    int margin = 2;
    Dimension w = bboard -> width();
    Dimension h = bboard -> height();
    Dimension th = 40;

    colorListView -> reshape(0, margin, w-margin, h-th -margin*2);
    area     -> reshape(18, h-th-margin*2, COLOR_WIDTH, th);
    value    -> reshape(COLOR_WIDTH+20, h-th-margin*2, w-COLOR_WIDTH-20, th);
  }

private:
  void selected(Event& event)
  {
    if (event.isEnterKey() ||  event.isButtonPressed() ) {
      event.processTraversal();

      pixel = (Pixel)event.getClientData();
      char text[128];
      sprintf(text, "%06lx", pixel);
      CompoundString cs(text);
      value -> set(XmNlabelString, cs);

      back->setForeground(pixel);
      back->fillRectangle(0, 0, COLOR_WIDTH, COLOR_HEIGHT);
      area->update();
    }
  }

public:
  ColorSelectionDialog(View* parent, const char* name, Args& args)
  :CustomDialog()
  { 
    ColorSelectionDialog::create(parent, name, args);
  }

  ColorSelectionDialog()
  :CustomDialog()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    CustomDialog::create(parent, name, 
                              args.setIfNotSet(XmNwidth,  WIDTH) 
                                  .setIfNotSet(XmNheight, HEIGHT));
    int W = args.get(XmNwidth);
    int H = args.get(XmNheight);

    set(XmNminWidth, W); 
    set(XmNmaxWidth, W) ;
    set(XmNminHeight,H);
    set(XmNmaxHeight, H);

    Form* workArea = getWorkArea();
    
    Color color(workArea);

    Args ar;
    ar.reset();
    ar.set(XmNtopAttachment,    XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM);
    ar.set(XmNleftAttachment,   XmATTACH_FORM);

    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNmarginHeight, 0);
    ar.set(XmNmarginWidth,  0);

    bboard = new BulletinBoard(workArea, "", ar);
 
    ar.reset();
    ar.set(XmNshowColorName, true);
    ar.set(XmNhighlightThickness, 1); 

    colorListView = new ColorListView(bboard, "",ar);
    colorListView ->addEventHandler(ButtonPressMask|KeyPressMask, this,
             (Handler)&ColorSelectionDialog::selected, NULL);

    ar.reset();
    area = new DrawingArea(bboard, "", ar);

    area -> set(XmNwidth,  COLOR_WIDTH);
    area -> set(XmNheight, COLOR_HEIGHT);

    ar.reset();
    ar.set(XmNalignment,  XmALIGNMENT_BEGINNING);
    ar.set(XmNrecomputeSize, False);
    value = new Label(bboard, "", ar);

    Display* display = getDisplay(); 
    Pixel white = color.whitePixel();
    area -> set(XmNbackground, white);

    pixelmap = new Pixelmap(display, COLOR_WIDTH, COLOR_HEIGHT);

    back = new DC(display, pixelmap->get());
    back -> setForeground(white);
    back -> fillRectangle(0, 0, COLOR_WIDTH, COLOR_HEIGHT);
    
    addEventHandler(StructureNotifyMask, this,
          (Handler)&ColorSelectionDialog::structureNotify, NULL);

    area -> addEventHandler(ExposureMask, this,
           (Handler)&ColorSelectionDialog::draw, NULL);

    //Add OK, Cancel

    static const char* buttons[] = { "OK", "Cancel"}; 
    CompoundStringList csl(buttons, SizeOf(buttons) );
    addButton(csl);

    //CancelButton
    getNthButton(CANCEL_BUTTON) -> addCallback(XmNactivateCallback, this,
                  (Callback)&PopupView::popdown, NULL);

  }

  ~ColorSelectionDialog()
  {
  }


  PushButton* getOkButton()
  {
    return getNthButton(OK_BUTTON);
  }

  PushButton* getCancelButton()
  {
    return getNthButton(CANCEL_BUTTON);
  }

  PushButton* getFilterButton()
  {
    return getNthButton(FILTER_BUTTON);
  }

  PushButton* getHelpButton()
  {
    return getNthButton(HELP_BUTTON);
  }

  Pixel  getPixel()
  {
    return pixel;
  }


};
}

