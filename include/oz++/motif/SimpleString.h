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
 *      SimpleString.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/CharString.h>
#include <oz++/motif/DC.h>


namespace OZ {
  
class SimpleString :public CharString {
private:
  Position x;
  Position y;

public:
  SimpleString(const char* string) 
  :CharString(string)
  {
  }

  void   draw(DC* dc)
  {
    const char* string = (const char*)(*this);
  	XFontSet fontSet = dc -> getFontSet();
  	if (fontSet) {
	  XmbDrawString(dc->getDisplay(), dc->getDrawable(),  fontSet,
		   dc->getGC(), x, y, string, strlen(string));
  	} else {
	  XDrawString(dc->getDisplay(), dc->getDrawable(),
		   dc->getGC(), x, y, string, strlen(string));
  	}
  }

  void   drawImage(DC* dc) 
  {
    const char* string = (const char*)(*this);
  	XFontSet fontSet = dc -> getFontSet();
  	if (fontSet) {
	  XmbDrawImageString(dc->getDisplay(), dc->getDrawable(),  fontSet,
		   dc->getGC(), x, y, string, strlen(string));
  	} else {
	  XDrawImageString(dc->getDisplay(), dc->getDrawable(),
		   dc->getGC(), x, y, string, strlen(string));
  	}
  	
  }

  void  move(int x1, int y1) 
  {
	x = x1;	
	y = y1;
  }
};

}

