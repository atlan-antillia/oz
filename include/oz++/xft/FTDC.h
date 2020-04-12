/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com  TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      FTDC.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/motif/IView.h> //2015/05/21
#include <oz++/Exception.h>
#include <oz++/xft/FTResource.h>
#include <oz++/xft/FTColor.h>
#include <oz++/xft/FTFont.h>

//XFTDC is a class to represent a device(graphics) context (XftDraw)
 
namespace OZ {

//XftDraw is an opaque object which holds information 
// used to render to an X drawable using either the core protocol or the X Rendering extension.
// from http://www.x.org/archive/X11R7.5/doc/man/man3/Xft.3.html
	
class FTDC :public FTResource {
private:
  XftDraw*  drawable;

private:
  FTFont* font; 

  FTColor color;

public:
  FTDC(Widget widget)
  :FTResource(widget),
    drawable(NULL),
    font(NULL),
    color(XtDisplay(widget))
  {
    Display* display = getDisplay();
    int screen        = defaultScreen();
    Colormap cmap    = defaultColormap();
    Visual*  visual  = defaultVisual();
  	
    Window window    = XtWindow(widget);
    this -> drawable = XftDrawCreate(display,
        window, visual, cmap);
    if (this -> drawable == NULL) {
       throw IException("Failed to XftDrawCreate");
    }
  }

public:
  FTDC(IView* view)
  :FTResource(view -> getWidget()),
    drawable(NULL),
    font(NULL),
    color(XtDisplay(view -> getWidget()))
  {
    Display* display = getDisplay();
    int screen        = defaultScreen();
    Colormap cmap    = defaultColormap();
    Visual*  visual  = defaultVisual();

    Window window    = XtWindow(view ->getWidget());
    this -> drawable = XftDrawCreate(display,
        window, visual, cmap);
    if (this -> drawable == NULL) {
       throw IException("Failed to XftDrawCreate");
    }
  }

public:
  FTDC(Display* display, Pixmap pixmap)
  :FTResource(display),
    drawable(NULL),
    font(NULL),
    color(display) 
  {
    this -> drawable = XftDrawCreateBitmap(display, pixmap);
  	
    if (this -> drawable == NULL) {
       throw IException("Failed to XftDrawCreateBitmap");
    }
  }
	

public:
  FTDC(Display* display, Pixmap pixmap, int depth)
  :FTResource(display),
    drawable(NULL),
    font(NULL),
    color(display)
  {
    this -> drawable = XftDrawCreateAlpha(display, pixmap, depth);
  	
    if (this -> drawable == NULL) {
       throw IException("Failed to XftDrawCreateAlpha");
    }
  }

  void setFont(FTFont* ftFont)
  {
    font = ftFont;
  }

  XftDraw* getDrawable() 
  {
    return drawable;
  }

  XftFont* getFont()
  {
    XftFont* ftfont = NULL;
    if (font) {
       ftfont = font->get();		
    }
    ftfont;
  }

  //2015/02/02
  void rectangle(int x, int y, 
	     unsigned int   width,
	     unsigned int   height,
	     const char* colorname = "black")
  {
    XftColor c = color.alloc(colorname);
    XftDrawRect (drawable,
	     &c,
	     x, 
	     y,
	     width,
	     height);
  }

  Bool setClip(Region region)
  {
    return XftDrawSetClip (drawable,
		region);
  }
	
  Bool setClipRectangles (
			  int			xOrigin,
			  int			yOrigin,
			  const XRectangle	*rects,
			  int			n)
  {
    return XftDrawSetClipRectangles (drawable,
			  xOrigin,
			  yOrigin,
			  rects,
			  n);
  }
	
  void setSubwindowMode (int mode)
  {
    XftDrawSetSubwindowMode (drawable, mode);
  }
	
  void drawGlyphs (
           int      x,
           int      y,
           const int* glyphs,
           int      nglyphs,
           const char* colorname = "black")
  {
    XftColor c = color.alloc(colorname);
    XftDrawGlyphs (drawable,
           &c,
    	   getFont(),
           x,
           y,
           (_Xconst FT_UInt*)glyphs,
           nglyphs);
  }
  
  void drawString8 (
        int         x,
        int         y,
        const char*  string,
        int         len,
        const char* colorname = "black")

  {
  	XftColor c = color.alloc(colorname);

  	XftDrawString8 (drawable,
           &c,
    	   getFont(),
           x,
           y,
           (_Xconst FcChar8 *)string,
          len);
  }
  
  void drawString8 (
        int         x,
        int         y,
        const char*  string,
        const char* colorname = "black")

  {
    if (string) {
        XftColor c = color.alloc(colorname);

        XftDrawString8 (drawable,
           &c,
           getFont(),
           x,
           y,
           (_Xconst FcChar8 *)string,
          strlen(string) );
    }
  }

  void drawString16 (
         int            x,
         int            y,
         const short* string,
         int            len,
         const char* colorname = "black")
  {
  	XftColor c = color.alloc(colorname);

  	XftDrawString16 (drawable,
           &c,
    	   getFont(),
           x,
           y,
           (_Xconst FcChar16 *)string,
           len);
  }
  
 void drawString16 (
         int            x,
         int            y,
         const short* string,
         const char* colorname = "black")
  {
    if (string) {
        XftColor c = color.alloc(colorname);

        XftDrawString16 (drawable,
           &c,
           getFont(),
           x,
           y,
           (_Xconst FcChar16 *)string,
           wcslen((wchar_t*)string) );
    }
  }

  void drawString32 (
         int            x,
         int            y,
         const uint32_t* string,
         int            len,
         const char* colorname = "black")
  {
    XftColor c = color.alloc(colorname);
  
    XftDrawString32 (drawable,
           &c,
    	   getFont(),
           x,
           y,
           (_Xconst FcChar32 *)string,
          len);
  }

 void drawString32 (
         int            x,
         int            y,
         const uint32_t* string,
         const char* colorname = "black")
  {
    if (string) { 
      XftColor c = color.alloc(colorname);

      XftDrawString32 (drawable,
           &c,
           getFont(),
           x,
           y,
           (_Xconst FcChar32 *)string,
          wcslen((wchar_t*)string) );
    }
  }

  void drawStringUtf8 (
           int          x,
           int          y,
           const char* string,
           int          len,
           const char* colorname = "black")
  {
  	XftColor c = color.alloc(colorname);

  	XftDrawStringUtf8 (drawable,
           &c,
    	   getFont(),
           x,
           y,
           (_Xconst FcChar8 *)string,
           len);
  }
	
  void drawStringUtf8 (
           int          x,
           int          y,
           const char* string,
           const char* colorname = "black")
  {
    if (string) { 
        XftColor c = color.alloc(colorname);

        XftDrawStringUtf8 (drawable,
           &c,
           getFont(),
           x,
           y,
           (_Xconst FcChar8 *)string,
           strlen(string) );
    }
  }

//
  void drawStringUtf8 (
           int          x,
           int          y,
           const char* string,
           int          len,
           XftColor&    color)
  {
  	XftDrawStringUtf8 (drawable,
           &color,
    	   getFont(),
           x,
           y,
           (_Xconst FcChar8 *)string,
           len);
  }

 void drawStringUtf8 (
           int          x,
           int          y,
           const char* string,
           XftColor&    color)
  {
    if (string) {
        XftDrawStringUtf8 (drawable,
           &color,
           getFont(),
           x,
           y,
           (_Xconst FcChar8 *)string,
           strlen(string) );
    }
  }

  void drawStringUtf16 (
            int         x,
            int         y,
            const char* string,
            FcEndian        endian,
            int         len,
            const char* colorname = "black")
  {
  	XftColor c = color.alloc(colorname);

  	XftDrawStringUtf16 (drawable,
            &c,
    	    getFont(),
            x,
            y,
            (_Xconst FcChar8 *)string,
            endian,
            len);
  }
};

}

