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
 *  ColoredLabel.h
 *
 *****************************************************************************/

//2017/10/10 Updated to use create method.

#pragma once

#include <oz++/motif/Label.h>

#include <oz++/motif/RenderTable.h>
#include <oz++/motif/DC.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/StrDef.h>

namespace OZ {

class ColoredLabel :public Label {
private:
  static const int      PIXMAP_WIDTH  = 60;
  static const int      PIXMAP_HEIGHT = 20;
  
  Pixmap                pixmap;
public:
  ColoredLabel(View* parent, const char* name, Args& args)
  :Label()
  ,pixmap(None)
  {
    ColoredLabel::create(parent, name, args);
  }

  ColoredLabel()
  :Label()
  ,pixmap(None)
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    pixmap = None;
    Label::create(parent, name, 
                       args.set(XmNlabelType,       XmPIXMAP_AND_STRING)
                           .set(XmNalignment,       XmALIGNMENT_END) 
                           .set(XmNpixmapPlacement, XmPIXMAP_LEFT)
                           .set(XmNtraversalOn,     TRUE)
                           .set(XmNhighlightOnEnter,TRUE)
                           .set(XmNhighlightThickness, 1) );  

    Pixel pixel = (Pixel)0;
    if (args.has(XmNpixel)) {
      pixel = (Pixel)args.get(XmNpixel);
    }
    
    char value[128];
    sprintf(value, "%06x", (unsigned int)pixel);
    CompoundString colorName(value);
    Display* display = getDisplay();
    Screen* screen   = XDefaultScreenOfDisplay(display);
    int depth        = DefaultDepthOfScreen(screen);

    pixmap = XCreatePixmap(display,
                      RootWindowOfScreen(screen),
                      PIXMAP_WIDTH, PIXMAP_HEIGHT, depth);

    if (pixmap == BadAlloc || pixmap ==  BadDrawable ||
        pixmap ==  BadValue) {
      throw IException("Failed to XCreatePixmap");
    }
    
    DC back(display, pixmap);

    back.setForeground(pixel);   //pixel may be 24bit rgb color
    back.fillRectangle(0, 0, PIXMAP_WIDTH, PIXMAP_HEIGHT);
    
    set(XmNlabelString, colorName);
    set(XmNlabelPixmap, pixmap);
    set(XmNwidth, PIXMAP_WIDTH*2);

    set(XmNrecomputeSize, TRUE);
  }

  ~ColoredLabel()
  {
    if (pixmap != None) {
      XFreePixmap(getDisplay(), pixmap);
      pixmap = None;
    }
  }

  void setColor(Pixel pixel)
  {
    char value[128];
    sprintf(value, "%06x", (unsigned int)pixel);
    CompoundString colorName(value);

    DC back(getDisplay(), pixmap);

    back.setForeground(pixel);   //pixel may be 24bit rgb color
    back.fillRectangle(0, 0, PIXMAP_WIDTH, PIXMAP_HEIGHT);
    set(XmNlabelString, colorName);
    set(XmNlabelPixmap, pixmap);
    set(XmNrecomputeSize, TRUE);
  }
};

}
      
