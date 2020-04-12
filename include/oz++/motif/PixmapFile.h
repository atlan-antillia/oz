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
 *      PixmapFile.h
 *
 *****************************************************************************/

//2015/05/12 Added getImage method.

#pragma once

#include <oz++/motif/Resource.h>
#include <oz++/Exception.h>

namespace OZ {

class PixmapFile :public Resource {
private:
  Screen* screen;

public:
  PixmapFile(Display* display, const char* imagename, 
       Pixel fg = XmUNSPECIFIED_PIXEL, Pixel bg = XmUNSPECIFIED_PIXEL)
    :Resource(display)
  {
    screen = DefaultScreenOfDisplay(display);
    Pixmap pixmap = XmGetPixmap(screen, (char*)imagename, fg, bg);

     if (pixmap != XmUNSPECIFIED_PIXMAP) {
       set((XtPointer)pixmap);
     } else {
       throw IException("Failed to XmGetPixmap %s", imagename);    
     }
  }

  PixmapFile(Display* display, const char* imagename, 
         Pixel fg, Pixel bg, int depth)
    :Resource(display)
  {
    screen = DefaultScreenOfDisplay(display);
    Pixmap pixmap = XmGetPixmap(screen, (char*)imagename, fg, bg);
   
    if (pixmap != XmUNSPECIFIED_PIXMAP) {
      set((XtPointer)pixmap);
    } else {
      throw IException("Failed to XmGetPixmap %s", imagename); 
    }
  }
 
  ~PixmapFile() 
  {
    XmDestroyPixmap(screen, get());
  }

  Pixmap  get() 
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
 
  void  getGeometry(unsigned int* width, unsigned int* height, 
          unsigned int* borderwidth, unsigned int* depth) 
  {
    Window root;
    int    x, y;

    XGetGeometry(getDisplay(), get(), &root,
                                &x, &y, width, height, borderwidth, depth);
  }

  XImage* getImage()
  { 
    unsigned int width  = 0;
    unsigned int height = 0;
    getGeometry(&width, &height);
    return XGetImage(getDisplay(), get(), 0, 0, width, height, 
                AllPlanes, ZPixmap);
  }
};
}


