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
 *      FontSet.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/Exception.h>
#include <oz++/motif/Resource.h>

namespace OZ {
  
class View;

class FontSet :public Resource {
private:
  XFontSet create(Display* display, const char* fontname)
  {
    char** missingCharSets = NULL;
    int    numMissingCharSets = 0;
    char* defaultString = NULL;
    XFontSet xfontset = XCreateFontSet(display,
                        fontname,
                        &missingCharSets,
                        &numMissingCharSets,
                        &defaultString);
    if(numMissingCharSets > 0) {
      for(int i = 0; i<numMissingCharSets; i++) {
        fprintf(stderr, "Missing :%s\n", missingCharSets[i]);
      }
      XFreeStringList(missingCharSets);
    }
    return xfontset;
  }
  
  
public:
  FontSet(IView* view, const char* fontname)
  :Resource(view->getDisplay())
  {
  	XFontSet fontSet = create(view->getDisplay(), fontname);
  	if (fontSet) {
      set(fontSet);
  	} else {
  		throw IException("Failed to create XFontSet %s", fontname);	
  	}
  }

  FontSet(Display* display, const char* fontname)
  :Resource(display)
  {
  	XFontSet fontSet = create(display, fontname);
  	if (fontSet) {
      set(fontSet);
  	} else {
  		throw IException("Failed to create XFontSet %s", fontname);	
  	}
  }


  ~FontSet() 
  { 
    XFreeFontSet(getDisplay(), get()); 
  }

  char*  baseFontNameList() 
  { 
    return XBaseFontNameListOfFontSet(get());
  }

  XFontSetExtents* extents() 
  {
    return XExtentsOfFontSet(get());
  }

  XFontSet get() 
  {
    return (XFontSet)Resource::get();
  } 

  int  fontList(XFontStruct*** fontlist, char*** namelist) 
  {
    return XFontsOfFontSet(get(), fontlist, namelist);
  }

  char* locale() 
  {
    return XLocaleOfFontSet(get());
  }
};

}


