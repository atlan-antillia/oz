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
 *      FontList.h
 *
 *****************************************************************************/

#pragma once


#include <oz++/motif/Resource.h>
#include <oz++/motif/FontSet.h>

namespace OZ {
  
class FontList :public Resource {
public:
  FontList(class IView* view, const char* fontname)
  :Resource(view->getDisplay())
  {
    XmFontListEntry entry = XmFontListEntryLoad(view->getDisplay(),
                                (char*)fontname,
                                XmFONT_IS_FONTSET,
                                XmFONTLIST_DEFAULT_TAG);
    if (entry) {
      set(XmFontListAppendEntry(NULL, entry));
    } else {
      Exception("Failed to XmFontListEntryLoad, fontname=%s", fontname).display();
      ::exit(0);
    }
  }

  FontList(Display* display, const char* fontname) 
    :Resource(display) 
  {
    XmFontListEntry  entry = XmFontListEntryLoad(display,
        (char*)fontname, 
        XmFONT_IS_FONTSET, 
        XmFONTLIST_DEFAULT_TAG);
    if (entry) {
      set(XmFontListAppendEntry(NULL, entry));
    } else {
      Exception("Failed to XmFontListEntryLoad, fontname=%s", fontname).display();
      ::exit(0);  
    }
  }
  
  FontList(Display* display, const char* name, XmFontType type, char* tag)
    :Resource(display) 
  {
    XmFontListEntry entry = XmFontListEntryLoad(display,
        (char*)name, type, tag);
    set(XmFontListAppendEntry(NULL, entry));  
  }

  FontList(FontSet* fontset) 
    :Resource(fontset->getDisplay()) 
  {
    XmFontListEntry entry = XmFontListEntryCreate(XmFONTLIST_DEFAULT_TAG,
        XmFONT_IS_FONTSET, fontset->get());
    if (entry) {
      set(XmFontListAppendEntry(NULL, entry));
    } else {
      Exception("Failed to XmFontListEntryCreate").display();
      ::exit(0);
    }
  } 

  ~FontList() 
  {
    XmFontListFree(get());
  }
  
  void  append(XmFontListEntry entry1) 
  {
    set(XmFontListAppendEntry(get(), entry1));
  }
  XmFontList get() {
    return (XmFontList)Resource::get();
  }
};

}

