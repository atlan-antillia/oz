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
 *  FTMultiColumn.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/DrawnButton.h>
#include <oz++/motif/PixmapFile.h>
#include <oz++/SmartArray.h>
#include <oz++/CharString.h>

#include <oz++/xft/FTPrimitive.h>
#include <oz++/xft/FTDC.h>
#include <oz++/xft/FTColumnLayout.h>



namespace OZ {
  
/*
typedef struct {
  int   width;
  int   alignment;
  const char* color;
} FTColumnLayout;
*/
  
class FTMultiColumn :public FTPrimitive {
private:  
  FTColumnLayout*  columns;
  SmartArray<CharString*> labels;
  
  int          numColumns;
  XRectangle*  rectangles;
  int itemNum;
  
private:
  static const int MARGIN = 4;
  
public:
  FTMultiColumn(View* parent, const char* name, Args& args)
  :FTPrimitive(parent, name, args.set(XmNpushButtonEnabled, False)
                                 .set(XmNalignment, XmALIGNMENT_BEGINNING )
                                 .set(XmNhighlightColor, parent->color("blue"))
                                 .set(XmNhighlightThickness, 1)
                                 .set(XmNbackground, parent->whitePixel())
                                 .set(XmNshadowThickness, 0)
                                 .set(XmNtraversalOn, True) ),
  columns(NULL),
  numColumns(0),
  rectangles(NULL),
  itemNum(0)
  {
    if (args.has(XmNcolumnLayout) && args.has(XmNnumColumns) ) {
      //Shallow copy
      columns = (FTColumnLayout*) args.get(XmNcolumnLayout);
      numColumns = (int) args.get(XmNnumColumns);
    } else {
      throw IException("Invalid argument");  
    }
    
    short x = 0;
    short y = 0;
    
    rectangles = new XRectangle[numColumns];
    
    for (int i = 0; i<numColumns; i++) {
      unsigned short w = (unsigned short)columns[i].width;
      XRectangle* r = &rectangles[i];
      r -> x = x;
      r -> y = y;
      r -> width = w;// + MARGIN*2;
      r -> height = 30;
     //printf("%d %d %d %d\n", x, y, w, h);
      x += w;
    }
  }

  ~FTMultiColumn()
  {
    columns = NULL;
 
    delete [] rectangles;
    rectangles = NULL;
  }

  void setItems(const char** items, int num)
  {
    if (items && num >0 && num <=numColumns) {
      labels.removeAllItems();
      itemNum = num;
      for (int i = 0; i<num; i++) {
        //printf("%s \n", items[i]);
        labels[i] = new CharString(items[i]);
      }
    } else {
      printf("Invalid argument\n");  
    }
  }
  
  const char* getItem(int pos)
  {
    const char* item = NULL;
  
    if (pos >=0 && pos < itemNum) {
      CharString* cstring = labels[pos];
      if (cstring) {
        item = (const char*)(*cstring);
      }
    } else {
      printf("Invalid argument %d\n", pos);  
    }
    return item;
  }

  void expose(Action& action)
  {
    Display* display = getDisplay();
    FTDC dc(this -> getWidget());
    FTFont* font = getFont();
    if (font == NULL) {
      printf("Font is NULL");
      return;  
    }
    if (labels.getSize() == 0) {
      return;  
    }
    dc.setFont(font);
    int th = 0;    
    //dc.setClipRectangles(0, 0, rectangles, numColumns);
    int height = 0;
    for (int i = 0; i < numColumns; i++) {
      CharString* cstring = NULL;
      try {
        cstring = labels[i];
      } catch (Exception& ex) {
        ex.display();
      }
      if (cstring == NULL) {
      break;
      }
      const char* string = (const char*)(*cstring);
      //printf("%d %s \n", i, string);
      XGlyphInfo ex;    
      font -> textExtentsUtf8(string, strlen(string), &ex);
      int asc = ex.y;
      int des = ex.height - ex.y;
      int x = rectangles[i].x + MARGIN;
      int y = rectangles[i].y;
    
      int width = rectangles[i].width;
      int alignment = columns[i].alignment;
 
      if (alignment == XmALIGNMENT_BEGINNING) {
        ;//Do nothing here
      } 
      if (alignment == XmALIGNMENT_CENTER) { 
         x = x + (width - ex.width)/2;
      }
      if (alignment == XmALIGNMENT_END) {
        x = x + width  - ex.width - MARGIN - height;  
      }
   
     if (i == 0) {
       get(XmNheight, (XtArgVal)&height);
        //set(XmNheight, ex.height + MARGIN*2);
        //height = ex.height + MARGIN*2;
      }
      XRectangle* r = &rectangles[i];
      //r -> height = ex.height + MARGIN*2;
      r -> height = height;// + MARGIN*2;

      y = y + (height - ex.height)/2 + ex.height - des/2;
      dc.setClipRectangles(height, 0, &rectangles[i], 1);
      dc.drawStringUtf8(x+height, y, string, strlen(string), columns[i].color );
     // printf("%d %d %s\n", x, y, columns[i].label);
      //y += MARGIN*2;
    }
  }
};

}

