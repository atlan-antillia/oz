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
 *      GlyphCursor.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/Resource.h>
#include <X11/cursorfont.h>

namespace OZ {
  
class GlyphCursor :public Resource {

public:
  GlyphCursor(IView* view, Font sfont, Font mfont,
        char schar, char mchar, XColor* fg, XColor* bg) 
    :Resource(view->getDisplay()) 
  {
    set((XtPointer)XCreateGlyphCursor(view->getDisplay(), sfont, mfont, 
        schar, mchar, fg, bg));
  }

public:
  GlyphCursor(Display* display, Font sfont, Font mfont,
        char schar, char mchar, XColor* fg, XColor* bg) 
    :Resource(display) 
  {
    set((XtPointer)XCreateGlyphCursor(display, sfont, mfont, 
        schar, mchar, fg, bg));
  }
  
  ~GlyphCursor() 
  {
    XFreeCursor(getDisplay(), get());
  }
  
  Cursor  get() 
  {
    return (Cursor)Resource::get();
  }

  void    recolor(XColor* fg, XColor* bg) 
  {
      XRecolorCursor(getDisplay(), get(), fg, bg);
  }
};

}
