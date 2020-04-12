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
 *   FTPattern.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/CommonObject.h>
#include <oz++/Exception.h>

#include <oz++/motif/Args.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <string.h>
#include <oz++/xft/FTNames.h>

namespace OZ {
	
class FTPattern :public CommonObject{
private:
  FcPattern* pattern;
  bool       destructive;
public:
  FTPattern()
  :pattern(FcPatternCreate()),
	destructive(true)
  {
    if (pattern == NULL) {
    	throw IException("Failed in FcPatternCreate");
    }
  }

public:
  FTPattern(FcPattern* pat)
  :pattern(pat),
	destructive(false)
  {
    if (pattern == NULL) {
    	throw IException("Invalid argument");
    }
  }

public:
  ~FTPattern()
  {
  	if (pattern && destructive) {
	  FcPatternDestroy(pattern);
  	}
  	pattern = NULL;
  }
	
public:
  FcPattern* get()
  {
  	return pattern;
  }
	
	
  void addPattern(const char* name, XtArgVal val, XFCTYPE type)
  {
  	switch(type) {
  	case XFCTYPE_STRING:
  	  add(name, (const char*)val);
  	  break;
  		
  	case XFCTYPE_INTEGER:
  	  add(name, (int)val);
  	  break;
  		
  	case XFCTYPE_CHARSET:
  	  add(name, (FcCharSet*)val);
  	  break;
  		
  	case XFCTYPE_DOUBLE:
  	  add(name, (double)val);
  	  break;
	
  	case XFCTYPE_BOOL:
  	  add(name, (FcBool)val);
  	  break;

   	case XFCTYPE_MATRIX:
  	  add(name, (FcMatrix*)val);
  	  break;
  		
  	}
  }
	
public:
  void add(Args& args)
  {
  	int size = args.count();
  	ArgList ar = args.getArgList();
    FTNames ftNames;
  	
  	for (int i = 0; i<size; i++) {
  		const char* name = ar[i].name;
  		XtArgVal val     = ar[i].value;
  		try {
  		  if (strncmp(name, XftNamePrefix, strlen(XftNamePrefix)) == 0) {
  		  	XFCTYPE type = ftNames.getType(name);
  		  
  			const char* fcname = strchr(name, '_');
  		  	if (fcname) {
  		      addPattern(++fcname, val, type);
  		  	}
  		  }
  		} catch (...) {
  			//Ignore an IException
  		}
  	}
  }
	
public:
  void display()
  {
    FcPatternPrint(pattern);
  }
	
public:
  
  void add(const char* name, const char* value)
  {
    FcPatternAddString(pattern, name, (FcChar8*)value);
  }
  
  void add(const char* name, int value)
  {
    FcPatternAddInteger(pattern, name, value);
  }
  
  void add(const char* name, double value)
  {
    FcPatternAddInteger(pattern, name, value);
  }

  void add(const char* name, FcMatrix* value)
  {
    FcPatternAddMatrix(pattern, name, value);
  }
  	
  void add(const char* name, FcCharSet* value)
  {
    FcPatternAddCharSet(pattern, name, value);
  }

  void del(const char* name)
  {
    FcPatternDel(pattern, name);
  }
  
  FcPattern* match(Display* display)
  {
	FcResult result;
  	int screen = DefaultScreen(display);
	FcPattern* matchedPattern = XftFontMatch(display, screen, pattern, &result);
  	if (result != FcResultMatch) {
  		throw IException("Failed to XftFontMatch");
  	}
    return matchedPattern;

  }
  	
  int isEqual(FTPattern* fontPattern)
  {
    return FcPatternEqual(this -> pattern, fontPattern ->get() );
  }
};

}

//
/*
Usage example:
	
FTPattern* pattern = new FTPattern();
	pattern -> add(FC_FMMILIY, "Courier");
	pattern -> add(FC_SLANT, FC_SLANT_ITALIC); //FC_SLANT_ROMAN
	pattern -> add(FC_SIZE,  pointsize);// 12.0
	//pattern -> add(FC_PIXEL_SIZE, pixelsize);  
	pattern -> add(FC_WEIGHT, FC_WEIGHT_BOLD); //FC_WEIGHT_NORMAL
	pattern -> add(FC_MINSPACE, 1);
	pattern -> add(FC_ANTIALIAS, FcTrue);   //FcFalse
*/
	
//


