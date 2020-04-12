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
 *  FTPrimitive.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/DrawnButton.h>
#include <oz++/xft/FTFont.h>
#include <oz++/xft/FTDC.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/Color.h>

namespace OZ {
		
class FTPrimitive :public DrawnButton {
private:
  FTFont*      font;
  int          margin;
  CharString   label;
  CharString   foreground;

  int          alignment;
	
public:
  virtual FTFont*   getFont()
  {
    return font;
  }

	
public:
  //2015/02/02 Modified to be virtua
  //2015/03/20 Modified argument type from Action* to Action&
  virtual void expose(Action& action)
  {
    FTDC dc(this->getWidget());
  	const char* string = (const char*)label;
  	//printf("expose %s\n", string);

  	int width  = 0;
  	int height = 0;
  	get(XmNwidth, (XtArgVal)&width);
  	get(XmNheight, (XtArgVal)&height);
  	
  	XGlyphInfo ex;
    font -> textExtentsUtf8(string, strlen(string), &ex);
    int asc = ex.y;
  	int des = ex.height - ex.y;

  	if (width < ex.width + margin*2) {
      set(XmNwidth, ex.width + margin*2);	
  	}
  	if (height < ex.height + margin*2) {
      set(XmNheight, ex.height +margin*2);
  	}
  	get(XmNwidth, (XtArgVal)&width);
  	get(XmNheight, (XtArgVal)&height);

  	int x = (width - ex.width)/2;
  	int y = (height - ex.height)/2 + ex.height - des/2;
  	
  	if (alignment == XmALIGNMENT_BEGINNING) {
  	  x = margin;	
  	}
  	if (alignment == XmALIGNMENT_END) {
  	  x = width - margin - ex.width;	
  	}

    dc.setFont(getFont());
	
  	const char* fgcolor = (const char*)foreground;

  	if (strlen(fgcolor)> 0) {
  	  dc.drawStringUtf8(x, y, string, strlen(string), fgcolor);
  	} else {
  	  dc.drawStringUtf8(x, y, string, strlen(string));
  	}
  }

public:
  FTPrimitive(View* parent, const char* name, Args& args)
  :DrawnButton(parent, name, args),
  font(NULL),
  margin(4),
  label(""),
  foreground(""),
  alignment(XmALIGNMENT_CENTER)
  {
    margin = 4;
  //XmALIGNMENT_BEGINNING, XmALIGNMENT_END
  	int align = (int)args.get(XmNalignment);
  	if (align >=0 ) {
  		alignment = align;
  	}
  	
  	label = name;
  	const char* lab = (const char*)args.get(XftNlabelString);
  	if (lab) {
  	  label = lab;
  	}

  	foreground = "black";
  	const char* fgcolor = (const char*)args.get(XftNforeground);
  	if (fgcolor) {
  	  foreground = fgcolor;
  	}
  	
    font = (FTFont*)args.get(XftNfont);
  
    addCallback(XmNexposeCallback, this,
         (Handler)&FTPrimitive::expose, NULL); 
  } 

  ~FTPrimitive()
  {
  }
	
  virtual void get(const char* name, XtArgVal value)
  {
  	if (strcmp(name, XftNlabelString) == 0) {
  	  char** string = (char**)value;
  		*string = (char*)(const char*)label;
  	} else {
  		View::get(name, value);	
  	}
  }
};

}

