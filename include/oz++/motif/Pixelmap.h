/******************************************************************************
 *
 * Copyright (c) 2014-2017   TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      Pixelmap.h
 *
 *****************************************************************************/

//2017/05/10 Updated to add a new constructor
// Pixelmap(DC& dc, XImage& ximage) 
// Pixelmap(Display* display, Pixel bg, const char** xpmData)

// You have to specify -lXpm in link option. 

#pragma once

#include <oz++/Exception.h>
#include <oz++/motif/Resource.h>

namespace OZ {

class Pixelmap :public Resource {
private:
  Pixmap mask;
  bool destructive;

public:
  Pixelmap(Display* display, Dimension width, Dimension height)
    :Resource(display),
  mask(None),
  destructive(true)
  {
    Screen* screen = XDefaultScreenOfDisplay(display);

    Pixmap pixmap = XCreatePixmap(display,
       RootWindowOfScreen(screen),
       width, height,
       DefaultDepthOfScreen(screen));
  
    if (pixmap == BadAlloc || pixmap ==  BadDrawable ||
        pixmap ==  BadValue) {
      throw IException("Failed to XCreatePixmap");
    } else {
      set((XtPointer)pixmap);
    }
  }

  //2015/05/16
  Pixelmap(Display* display, Dimension width, Dimension height,
     unsigned int depth)
    :Resource(display),
   mask(None),
   destructive(true)
  {
    Screen* screen = XDefaultScreenOfDisplay(display);

    Pixmap pixmap = XCreatePixmap(display,
       RootWindowOfScreen(screen),
       width, height,
       depth);

    if (pixmap == BadAlloc || pixmap ==  BadDrawable ||
        pixmap ==  BadValue) {
      throw IException("Failed to XCreatePixmap");
    } else {
      set((XtPointer)pixmap);
    }
  }

  //2017/05/10
  Pixelmap(DC& dc, XImage& ximage) 
    :Resource(dc.getDisplay()),
   mask(None),
   destructive(true)
  {
    Dimension width  = ximage.width;
    Dimension height = ximage.height;
    unsigned int depth = ximage.depth;
    Display* display  = dc.getDisplay();
    Drawable drawable = dc.getDrawable();

    GC       gc       = dc.getGC();
    Screen* screen = XDefaultScreenOfDisplay(display);

    Pixmap pixmap = XCreatePixmap(display,
       RootWindowOfScreen(screen),
       width, height,
       depth);

    if (pixmap == BadAlloc || pixmap ==  BadDrawable ||
        pixmap ==  BadValue) {
      throw IException("Failed to XCreatePixmap");
    } else {
      XPutImage(display, pixmap, gc, &ximage, 0, 0, 0, 0, width, height); 
      set((XtPointer)pixmap);
    }
  }

public:
  Pixelmap(Display* display, Pixel bg, const char** data, 
  XpmAttributes* attr=NULL)
  :Resource(display),
  mask(None),
  destructive(true)
  {  
    XpmAttributes attributes;

    memset(&attributes, 0, sizeof(attributes));
    XpmColorSymbol bgColor;
    bgColor.name = NULL;
    bgColor.value = (char*)"none";
    bgColor.pixel = bg; 

    int screen =  DefaultScreen (display);
    attributes.colorsymbols = &bgColor;
    attributes.numsymbols   = 1;
    attributes.closeness    = 65535;

    attributes.depth     = DefaultDepth (display, screen);
    attributes.colormap  = DefaultColormap (display, screen);
    attributes.visual    = DefaultVisual (display, screen);
    attributes.valuemask = XpmCloseness|XpmColorSymbols|XpmDepth | XpmColormap | XpmVisual;
    
    if (attr == NULL) {
      attr = &attributes;
    }

    Pixmap pixmap = None;
    Status status = XpmCreatePixmapFromData (display,
                                       DefaultRootWindow (display), 
                                       (char**)data, 
                                       &pixmap, 
                                       &mask, 
                                       attr);
    if (status != XpmSuccess) {
      throw IException("Failed to XpmCreatePixmapFromData.");
    }
    if (pixmap == BadAlloc || pixmap ==  BadDrawable ||
        pixmap ==  BadValue) {
      throw IException("Failed to XCreatePixmap");
    } else {
      set((XtPointer)pixmap);
      //
      destructive=false;
    }
  }

  ~Pixelmap()
  {
    Pixmap pixmap= (Pixmap)get();
    if (pixmap != None && destructive) {
      XFreePixmap(getDisplay(), pixmap);
    }
    set((XtPointer)NULL);
    if (mask != None) {
      XFreePixmap(getDisplay(), mask );
    }
  }

  void nondestructive()
  {
    destructive = false; 
  }

  operator Pixmap()
  {
    return (Pixmap)Resource::get();
  }

  Pixmap get()
  {
    return (Pixmap)Resource::get();
  }

  Pixmap getMask()
  {
    return mask;
  }

  void  getGeometry(unsigned int* width, unsigned int* height) 
  {
    Window root;
    int    x, y;
    unsigned int bw, d;
    Pixmap pixmap = get();
    if (pixmap != None) {
      XGetGeometry(getDisplay(), pixmap, &root,
                                &x, &y, width, height, &bw, &d);
    } else {
      throw IException("Invalid pixmap");
    }
  }

  void  getGeometry(unsigned int* width, unsigned int* height, 
     unsigned int* borderwidth, unsigned int* depth) 
  {
    Window root;
    int    x, y;

    Pixmap pixmap = get();
    if (pixmap != None) {
      XGetGeometry(getDisplay(), pixmap, &root,
                                &x, &y, width, height, borderwidth, depth);
    } else {
      throw IException("Invalid pixmap");
    }
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


