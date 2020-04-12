/******************************************************************************
 *
 * Copyright (c) 2015   TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  FTDrawingArea.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/DrawingArea.h>
#include <oz++/xft/FTFont.h>
#include <oz++/xft/FTDC.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/DC.h>

namespace OZ {
		
class FTDrawingArea :public DrawingArea {
private:
  FTFont*      font;
	
public:
  virtual FTFont*   getFont()
  {
    return font;
  }
	
public:
  //Simple drawString method based on FTFont.
  void drawStringUtf8(FTFont* font, int x, int y, const char* string, 
      const char* fgcolor=NULL)
  {
    if (font && string) {
      FTDC dc(this->getWidget());
      dc.setFont(font);

      if (fgcolor != NULL && strlen(fgcolor)> 0) {
        dc.drawStringUtf8(x, y, string, strlen(string), fgcolor);
      } else {
        dc.drawStringUtf8(x, y, string, strlen(string));
      }
    }
  }

public:
  //Please redifine your own expose method in a subclass derived from this class
  virtual void expose(Action& action)
  {
    printf("FDDrawingArea::expose\n");
  }

public:
  FTDrawingArea(View* parent, const char* name, Args& args)
  :DrawingArea(parent, name, args),
  font(NULL)
  {
  	
    font = (FTFont*)args.get(XftNfont);
  
    addCallback(XmNexposeCallback, this,
         (Handler)&FTDrawingArea::expose, NULL); 
  } 

  ~FTDrawingArea()
  {
  }
	
};

}

