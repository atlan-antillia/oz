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
 *      Icon.h
 *
 *****************************************************************************/

//2015/04/11 Modified not to use XmFontList or OZ::FontList class
#pragma once

#include <oz++/motif/Resource.h>
#include <oz++/motif/SimpleString.h>
#include <oz++/motif/CompoundString.h>

namespace OZ {
  
class Icon :public Resource {
private:
  Screen*         screen;
  Boolean         cached;
  Position        x;
  Position        y;
  SimpleString*  name;
  CompoundString* label;
  
public:
  Icon(Display* display, const char* imagename, Pixel fg, Pixel bg)
  :Resource(display)
  {
    screen = DefaultScreenOfDisplay(display);
        set((XtPointer)XmGetPixmap(screen, (char*)imagename, fg, bg));
        cached = TRUE;
    name   = new SimpleString(imagename);
    label  = new CompoundString(imagename);
    x      = 0;
    y      = 0;
  }

  Icon(IView* view, const char* imagename, Pixel fg, Pixel bg)
  :Resource(view->getDisplay())
  {
    screen = DefaultScreenOfDisplay(getDisplay());
    set((XtPointer)XmGetPixmap(screen, (char*)imagename, fg, bg));
    cached = TRUE;
    name   = new SimpleString(imagename);
    label  = new CompoundString(imagename);
    x      = 0;
    y      = 0;
  }

  Icon(IView* view, Pixmap pixmap, const char* name)
  :Resource(view->getDisplay())
  {
    screen = DefaultScreenOfDisplay(getDisplay());
        set(XtPointer(pixmap));
        cached = False;
        label  = new CompoundString(name);
        x      = 0;
        y      = 0;
  }
 
  Icon(Display* display, Pixmap pixmap, const char* name)
  :Resource(display)
  {
    screen = DefaultScreenOfDisplay(display);
        set(XtPointer(pixmap));
        cached = False;
        label  = new CompoundString(name);
        x      = 0;
        y      = 0;
  }
 
  ~Icon()
  {
    delete label;
    delete name;
    if(cached)
      XmDestroyPixmap(screen, get());
    else
      XFreePixmap(getDisplay(), get());
  }

  Pixmap get() 
  {
    return (Pixmap)Resource::get();
  }

  void  getGeometry(unsigned int* width, unsigned int* height)
  {
	Window root;
        int    x, y;
        unsigned int bw, d;
        XGetGeometry(getDisplay(), get(), &root,
                                &x, &y, width, height, &bw, &d);
  }


  void    move(int x1, int y1) {
    x = x1;
    y = y1;
  }

  void    draw(DC* dc)
  {
	unsigned int width = 0;
    unsigned int height = 0;
   if(get() != XmUNSPECIFIED_PIXMAP) {
		getGeometry(&width, &height);
                dc -> copyArea(get(), 0, 0, width, height, x, y);
    }
	Dimension w = label ->getWidth(dc -> getRenderTable());
	int x1 = (width - w)/2;
    label -> move(x+x1, y+height+4);
    label -> draw(dc);
  }

  void    setLabel(const char* string)
  {
	if(label) delete label;
    label = new CompoundString(string);
  }

};

}


