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
 *      DC.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/Resource.h>
#include <oz++/motif/FontStruct.h>
#include <oz++/motif/FontSet.h>

namespace OZ {


class DC :public Resource {
private:
  XmRenderTable renderTable;
	
  XFontSet   fontset;
  GC     gc;

public:
  DC(IView* view)
  :Resource(view->getDisplay())
  {
    if (view->getDisplay() == NULL) {
      printf("DC is getDisplay is NULL\n");
    }
    Display* display = getDisplay();
    set((XtPointer)view->getWindow());
    renderTable = NULL;
  	
    fontset  = NULL;
    gc       = XCreateGC(display, getDrawable(), 0, 0);
    int  screen = DefaultScreen(display);
    Pixel fg = BlackPixel(display, screen);
    setForeground(fg);
  }

  DC(Widget widget)
  :Resource(XtDisplay(widget))
  {
    Display* display = getDisplay();
    set((XtPointer)XtWindow(widget));
    renderTable = NULL;
    fontset  = NULL;
    gc       = XCreateGC(display, getDrawable(), 0, 0);
    int  screen = DefaultScreen(display);
    Pixel fg = BlackPixel(display, screen);
    setForeground(fg);
  }

  DC(Display* display, Drawable drawable)
  :Resource(display)
  {
    set((XtPointer)drawable);
    renderTable = NULL;
    fontset  = NULL;
    gc       = XCreateGC(display, drawable, 0, 0);
    int  screen = DefaultScreen(display);
    Pixel fg = BlackPixel(display, screen);
    setForeground(fg);
  }

  ~DC()
  {
    if(gc) {
      XFreeGC(getDisplay(), gc);
    }
  }

  void  changeGC(unsigned long mask, XGCValues* values) {
    XChangeGC(getDisplay(), gc, mask, values);
  }

  void  copyArea(Drawable source, int sx, int sy,
      Dimension width, Dimension height, int destx, int desty)
  {
    XCopyArea(getDisplay(), source, getDrawable(), gc, sx, sy,
        width, height, destx, desty);
  }

  void  copyPlane(Drawable source, int sx, int sy,
      Dimension width, Dimension height, int destx, int desty, 
                             //int plane=AllPlanes) 2017/08/20 
                             unsigned long plane=AllPlanes)
  {
    XCopyPlane(getDisplay(), source, getDrawable(), gc, sx, sy,
        width, height, destx, desty, plane);
  }

  void  copyGC(unsigned long mask, GC gc2)
  {
    XCopyGC(getDisplay(), gc, mask, gc2);
  }

  void  drawArc(int x, int y, Dimension width, Dimension height,
        int beg, int end)
  {
    XDrawArc(getDisplay(), getDrawable(), gc, x, y, width, height,
        beg, end);
  }

  void  drawEllipse(int x, int y, Dimension width, Dimension height)
  {
    XDrawArc(getDisplay(), getDrawable(), gc, x, y, width, height,
        0, 360*64);
  }

  void  drawLine(int x1, int y1, int x2, int y2)
  {
    XDrawLine(getDisplay(), getDrawable(), gc,
      x1, y1, x2, y2);
  }

  void  drawPoint(int x, int y)
  {
    XDrawPoint(getDisplay(), getDrawable(), gc, x, y);
  }
	
  //2015/01/14 Added the following drawString methods
  void  drawString(int x, int y, char* string)
  {
    if (string) {
      XDrawString(getDisplay(), getDrawable(), gc, x, y, string, strlen(string));
    }
  }
/*
  void drawString(
           const char* string, Position x, Position y, //Dimension width,
           unsigned char alignment=XmALIGNMENT_BEGINNING , 
           unsigned char layoutDirection=XmSTRING_DIRECTION_L_TO_R,
          XRectangle* clip=NULL)
  {
    if (renderTable == NULL) {
      //Do nothing
      printf("DC::drawString, error renderTable is NULL\n");
      return;
    }

    if (renderTable && string) {
      drawString(renderTable, string, x, y, 
         alignment, layoutDirection, clip);
    } 
  }

  
  //alignmnet will take one of the values:
  //XmALIGNMENT_BEGINNING, XmALIGNMENT_CENTER, or XmALIGNMENT_END.

  void drawString(XmRenderTable renderTable,
           const char* string, Position x, Position y, 
           unsigned char alignment=XmALIGNMENT_BEGINNING , 
           unsigned char layoutDirection=XmSTRING_DIRECTION_L_TO_R,
           XRectangle* clip=NULL)
  {
    if (renderTable && string) {
      CompoundString cs(string);
      XmString xmstring = cs.get();
      Dimension width = 0;
      Dimension height = 0;
      XmStringExtent(renderTable, xmstring, &width, &height);

      XmStringDraw (
          getDisplay(),
          getDrawable(),
          renderTable,
          xmstring,
          gc,
          x,
          y,
          width,
          alignment,
          layoutDirection;
          clip);
    }
  }
*/
  void  drawString16(int x, int y, XChar2b* string, int len)
  {
    if (string) {
      XDrawString16(getDisplay(), getDrawable(), gc, x, y, string, len);
    }
  }
 
  void drawText(int x, int y, XTextItem *items, int nitems)
  {
    if (items) {
      XDrawText(getDisplay(), getDrawable(), gc, x, y, items, nitems);
    }
  }
	
  void drawText16(int x, int y, XTextItem16 *items, int nitems)
  {
    if (items) {
      XDrawText16(getDisplay(), getDrawable(), gc, x, y, items, nitems);
    }
  }
  	
  void drawString(int x, int y, XFontSet fontSet, char* string)
  {
    if (string) {
      XmbDrawString(getDisplay(), getDrawable(), fontSet, gc, x, y, string, strlen(string) );
    }
  }

  void drawString(int x, int y, XFontSet fontSet, wchar_t* string)
  {
    if (string) {
      XwcDrawString(getDisplay(), getDrawable(), fontSet, gc, x, y, string, wcslen(string) );
    }
  }

  void  drawRectangle(int x, int y, Dimension width, Dimension height)
  {
    XDrawRectangle(getDisplay(), getDrawable(), gc, x, y, width, height);
  }

  void  fillArc(int x, int y, Dimension width, Dimension height,
      int beg, int end)
  {
    XFillArc(getDisplay(), getDrawable(), gc, x, y, width, height,
        beg, end);
  }

  void  fillEllipse(int x, int y, Dimension width, Dimension height)
  {
    XFillArc(getDisplay(), getDrawable(), gc, x, y, width, height,
        0, 360*64);
  }

  void  fillRectangle(int x, int y, Dimension width, Dimension height)
  {
    XFillRectangle(getDisplay(), getDrawable(), gc, x, y, width, height);
  }

  Drawable getDrawable()
  {
    return (Drawable)Resource::get();
  }

  XmRenderTable getRenderTable()
  {
    return renderTable;
  }

  XFontSet getFontSet()
  {
    return fontset;
  }

  GC  getGC()
  {
    return gc;
  }

  void  put(XImage* image, int srcx, int srcy,
        int destx, int desty, Dimension width, Dimension height)
  {
    XPutImage(getDisplay(), getDrawable(), gc, image, srcx, srcy,
      destx, desty, width, height);
  }

  void    setArcMode(int mode)
  {
    XSetArcMode(getDisplay(), gc, mode);
  }

  void    setBackground(Pixel bg)
  {
    XSetBackground(getDisplay(), gc, bg);
  }

  void    setClipMask(Pixmap mask)
  {
    XSetClipMask(getDisplay(), gc, mask);
  }

  void    setClipOrigin(int x, int y)
  {
    XSetClipOrigin(getDisplay(), gc, x, y);
  }

  void    setClipRectangle(int x, int y, XRectangle& rectangle, 
                                int ordering = 0)
  {
    XSetClipRectangles(getDisplay(), gc, x, y, &rectangle,
                                1, ordering);
  }
	
  void    setClipRectangles(int x, int y, XRectangle rectangles[], int n,
                                int ordering)
  {
    XSetClipRectangles(getDisplay(), gc, x, y, rectangles,
                                n, ordering);
  }
  void  setDashes(int offset, char* dashList, int n)
  {
    XSetDashes(getDisplay(), gc, offset, dashList, n);
  }

  void  setFillRule(int rule)
  {
    XSetFillRule(getDisplay(), gc, rule);
  }

  void  setFillStyle(int style)
  {
    XSetFillStyle(getDisplay(), gc, style);
  }

  void  setFont(Font font)
  {
    XSetFont(getDisplay(), gc, font);
  }

  void  setFontStruct(FontStruct* fonts)
  {
    XSetFont(getDisplay(), gc, fonts->getFont());
  }


  void  setRenderTable(XmRenderTable renderTable1)
  {
    renderTable = renderTable1;
  }

  void   setFontSet(FontSet* fontset1)
  {
    fontset = fontset1->get();
  }

  void  setForeground(Pixel fg)
  {
    XSetForeground(getDisplay(), gc, fg);
  }

  void  setFunction(int function)
  {
    XSetFunction(getDisplay(), gc, function);
  }

  void  setLineAttributes(int lineWidth, int lineStyle,
      int capStyle, int joinStyle)
  {
    XSetLineAttributes(getDisplay(), gc, lineWidth, lineStyle,
      capStyle, joinStyle);
  }

  void  setPlaneMask(unsigned long mask)
  {
    XSetPlaneMask(getDisplay(), gc, mask);
  }

  void  setGC(GC gc1)
  {
    gc = gc1;
  }

  void  setSubwindowMode(int mode)
  {
    XSetSubwindowMode(getDisplay(), gc, mode);
  }

  void  setStipple(Pixmap stipple)
  {
    XSetStipple(getDisplay(), gc, stipple);
  }

  void  setState(unsigned long fg, unsigned long bg, int function,
      unsigned long mask)
  {
    XSetState(getDisplay(), gc, fg, bg, function, mask);
  }

  void  setTile(Pixmap tile)
  {
    XSetTile(getDisplay(), gc, tile);
  }

  void  setTSOrigin(int x, int y)
  {
    XSetTSOrigin(getDisplay(), gc, x, y);
  }
};

}

