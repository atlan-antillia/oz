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
 *      Color.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/IView.h>
#include <oz++/motif/Resource.h>

namespace OZ {

class Color :public Resource {

public:
  Color(class IView* view)
  :Resource(view->getDisplay())
  {
    set((XtPointer)DefaultColormap(view->getDisplay(), 
      DefaultScreen(view->getDisplay())));
  }

public:
  //2015/05/21
  Color(class IView* view, Colormap colormap)
  :Resource(view->getDisplay())
  {
    set((XtPointer)colormap);
  }

  Color(Display* display)
    :Resource(display) 
  {
    set((XtPointer)DefaultColormap(display, 
        DefaultScreen(display)));
  }
  
  Color(Widget widget)
    :Resource(XtDisplay(widget)) 
  {
    set((XtPointer)DefaultColormap(XtDisplay(widget), 
              DefaultScreen(XtDisplay(widget))));
  }
  
  Colormap getColormap() 
  {
    return (Colormap)Resource::get();
  }

  Pixel blackPixel()
  {
    return BlackPixel(getDisplay(), XDefaultScreen(getDisplay()) );
  }	
	
  Pixel whitePixel()
  {
    return WhitePixel(getDisplay(), XDefaultScreen(getDisplay()) );
  }	

  Pixel colorPixel(const char* name) 
  {
    XColor color;
    XColor exact;
    XAllocNamedColor(getDisplay(), getColormap(), name, &color, &exact);
    return color.pixel;
  }

  Pixel allocNamedColor(const char* name) 
  {
    XColor color;
    XColor exact;
    XAllocNamedColor(getDisplay(), getColormap(), name, &color, &exact);
    return color.pixel;
  }
	

  Pixel allocNamedColor(const char* name, XColor* color) 
  {
    XColor exact;
    XAllocNamedColor(getDisplay(), getColormap(), name, color, &exact);
    return color->pixel;
  }
  
 //2016/09/12
  Pixel color256(int red, int green, int blue) 
  {
    const int MAX = 65535;
    float v = (float)MAX/(float)255;
    float r = v * (float)red;
    float g = v * (float)green;
    float b = v * (float)blue;
    XColor xcolor;
    memset(&xcolor, 0, sizeof(xcolor));
    xcolor.red   = (int)r;
    xcolor.green = (int)g;
    xcolor.blue  = (int)b;
    XAllocColor(getDisplay(), getColormap(), &xcolor);
    return xcolor.pixel;
  }

 //2016/09/12
  Pixel color(int red, int green, int blue) 
  {
    XColor xcolor;
    memset(&xcolor, 0, sizeof(xcolor));
    xcolor.red   = red;
    xcolor.green = green;
    xcolor.blue  = blue;
    XAllocColor(getDisplay(), getColormap(), &xcolor);
    return xcolor.pixel;
  }


  Pixel allocColor(XColor* color) 
  {
    XAllocColor(getDisplay(), getColormap(), color);
    return color->pixel;
  }
  
  int  allocColorCells(Bool contig, unsigned long* masks,
    unsigned int nplanes, unsigned long* pixels, 
    unsigned int ncolors) 
  {
    return XAllocColorCells(getDisplay(), getColormap(), contig, 
      masks, nplanes, pixels, ncolors); 
  }
  
  int  allocColorPlanes(Bool contig, unsigned long* pixels, 
    int ncolors, int nr, int ng, int nb, 
    unsigned long* rm, unsigned long* gm, unsigned long* bm) 
  {
    return XAllocColorPlanes(getDisplay(), getColormap(), contig, 
      pixels, ncolors, nr, ng, nb, rm, gm, bm);  
  }
  
  void storeColor(XColor* color) 
  {
    XStoreColor(getDisplay(), getColormap(), color);
  }
  
  void storeColors(XColor* color, int num) 
  {
    XStoreColors(getDisplay(), getColormap(), color, num);
  }
  
  void storeNamedColor(const char* name, unsigned long pixel, int mask) 
  {
    XStoreNamedColor(getDisplay(), getColormap(), name, pixel, mask);
  }

  void queryColor(XColor* color) 
  {
    XQueryColor(getDisplay(), getColormap(), color);
  }
};
}

