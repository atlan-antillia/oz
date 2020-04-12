/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      FTFont.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/CharString.h>
#include <oz++/Exception.h>
#include <oz++/xft/FTResource.h>
#include <oz++/xft/FTPattern.h>

#include <oz++/xft/FTNames.h>

/*
typedef struct _XftFont {
    int     ascent;
    int     descent;
    int     height;
    int     max_advance_width;
    FcCharSet   *charset;
    FcPattern   *pattern;
} XftFont;

*/

namespace OZ {

class FTFont :public FTResource {
private:
  XftFont*   font;
  CharString string;  //string for fontconfigpattern or xlfd

  FTPattern* pattern;
	
public:
  FTFont(Display* display, Args& args)
  :FTResource(display),
   font(NULL),
   string(""),
   pattern(new FTPattern())
  { 
  	const char* name = (const char*)args.get(XftNfamily);
  	double size      = (double)args.get(XftNsize);
  	
  	if (name == NULL) {
  		name = "Times";
  		args.set(XftNfamily, name);
  	}
  	if (size <= 0.0) {
  		size = 10.0;
  		args.set(XftNsize, size);
  	}
  	pattern -> add(args);
  	
  	FcPattern* matchedPattern = pattern->match(display);
  	//printf("FreeTypeFont %lx %lx \n", (long)pattern->get(), (long)matchedPattern);

  	this -> font = XftFontOpenPattern(display, matchedPattern);
  	
  	if (this -> font ==NULL) {
  		FcPatternDestroy(matchedPattern);
    	throw IException("Failed to XftFontOpen family=%s, size%f", name, size);
    }
  	delete pattern;
  	pattern = new FTPattern(matchedPattern);
  }
	
public:
  //name is for fontconfig or xlfd
  FTFont(Display* display, const char* name)
  :FTResource(display),
	font(NULL),
	string(""),
	pattern(NULL)
  {
  	if (name) {
  	  string = name;
	  int screen = defaultScreen();
	  this -> font = XftFontOpenName(display, screen, name);  //string for config pattern;
      if (this -> font == NULL) {
      	printf("Failed to XftFontOpenName=%s", name);
      	this -> font = XftFontOpenXlfd(display, screen, name); //string for an X Logical Font Description (XLFD), "fixed"?
      	if (this -> font == NULL) {
      	  throw IException("Failed to XftFontOpenXlfd name=%s", name);
      	}
  	  }
  	} else {
  		throw IException("Invalid argument");
  	}
  }
	
  ~FTFont()
  {
  	if (pattern) {
  	  delete pattern;
  	  pattern = NULL;
  	}

  	if (font) {
      XftFontClose (getDisplay(), font);
  	  font = NULL;
  	}
  }
	
  XftFont* get()
  {
    return font;
  }
  
	
/*  XftFont* operator (XftFont*)()
  {
    return font;
  }
 */

  XftFont* copy()
  {
    return XftFontCopy(getDisplay(), font);
  }
	
  FT_Face lockFace()
  {
	 return XftLockFace (font);
  }
	
  void unlockFace()
  {
    XftUnlockFace(font);
  }
	
/*
typedef struct _XGlyphInfo {
    unsigned short  width;
    unsigned short  height;
    short	    x;
    short	    y;
    short	    xOff;
    short	    yOff;
} XGlyphInfo;
	*/
  void glyphExtents(
         const unsigned int    *glyphs,
         int            nglyphs,
         XGlyphInfo     *extents)
  {
	XftGlyphExtents(getDisplay(),
         font,
         (_Xconst FT_UInt*)glyphs,
         nglyphs,
         extents);
  }
  
  void textExtents8(
         _Xconst FcChar8    *string,
         int            len,
         XGlyphInfo     *extents)
  {
    XftTextExtents8 (getDisplay(),
         font,
         (_Xconst FcChar8*)string,
         len,
         extents);
  }
  
  void textExtents16(
          const unsigned short  *string,
          int           len,
          XGlyphInfo        *extents)
  {
    XftTextExtents16 (getDisplay(),
          font,
          (_Xconst FcChar16*)string,
          len,
          extents);
  }
  
  void textExtents32(
          const int32_t  *string,
          int           len,
          XGlyphInfo        *extents)
  {
     XftTextExtents32 (getDisplay(),
          font,
          (_Xconst FcChar32 *)string,
          len,
          extents);
          
  }
         
  void textExtentsUtf8(
            const char *string,
            int         len,
            XGlyphInfo      *extents)
  {
    XftTextExtentsUtf8 (getDisplay(),
            font,
            (_Xconst FcChar8 *)string,
            len,
            extents);
  }
  
  void textExtentsUtf16(
             const char  *string,
             FcEndian       endian,
             int        len,
             XGlyphInfo     *extents)
  {
    XftTextExtentsUtf16 (getDisplay(),
             font,
             ( _Xconst FcChar8*)string,
             endian,
             len,
             extents);
  }
  

};

}

