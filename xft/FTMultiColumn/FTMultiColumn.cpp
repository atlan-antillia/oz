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
 *  FTMultiColumn.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/PixmapFile.h>
#include <oz++/motif/Color.h>
#include <oz++/xft/FTFont.h>
#include <oz++/xft/FTMultiColumn.h>
#include <oz++/SmartArray.h>

namespace OZ {


class MainView :public ApplicationView {
private:
  SmartPtr<FTFont>       font;
  SmartPtr<PixmapFile>   icon;
  SmartPtr<RowColumn>    rowColumn;
  SmartArray<FTMultiColumn*> columns;
  int index;

  void press(Event& event)
  {
    int id = event.getClientIntData();

    XEvent* xev = event.getXEvent();
    FTMultiColumn* sender = (FTMultiColumn*)event.getSender();
    Color color(this);
    Pixel lgray = color.allocNamedColor("lightgray"); 
    Pixel white = color.allocNamedColor("white"); 
    if (xev ->type == ButtonPress || event.isEnterKeyPressed() ) {
      sender -> set(XmNbackground, lgray);
      printf("pressed row=%d item=[%s]\n", id, sender->getItem(0) );
    }
   
    if (xev ->type == ButtonRelease || event.isEnterKeyReleased() ) {
      sender -> set(XmNbackground, white);
      printf("released row=%d\n", id);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    ar.reset();
    ar.set(XftNfamily, "Times-12");
    ar.set(XftNweight, FC_WEIGHT_THIN);//FONT_WEIGHT_THIN);
    ar.set(XftNsize, 12.0);
    font = new FTFont(getDisplay(), ar);

    Color color(this);
    Pixel white = color.whitePixel();
    

    ar.reset();
    ar.set(XmNnumColumns, 1);
    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNbackground, white);
    rowColumn = new RowColumn(this, "", ar);

    static FTColumnLayout columnLayout[] = {
      {110, XmALIGNMENT_BEGINNING, "red"},
      {110, XmALIGNMENT_END,       "darkgreen"},
      {180, XmALIGNMENT_BEGINNING, "blue"},
      {200, XmALIGNMENT_CENTER,     "darkpink"},
      {80,  XmALIGNMENT_BEGINNING,  "orange"},
      {200, XmALIGNMENT_BEGINNING,  "black"},
    };
    const int numColumns = XtNumber(columnLayout);

    const int numRows = 4;
    const char* items[numRows][numColumns] = {
      {"11 Hello world", "12 Goodbye world", "13 Good morning", 
       "14 Good afternoon", "15 Good evening", "16 Good night" },

      {"21 Hello world", "22 Goodbye world", "23 Good morning", 
       "24 Good afternoon", "25 Good evening", "26 Good night" },

      {"31 Hello world", "32 Goodbye world", "33 Good morning", 
       "34 Good afternoon", "35 Good evening", "36 Good night" },

      {"41 Hello world", "42 Goodbye world", "43 Good morning", 
       "44 Good afternoon", "45 Good evening", "46 Good night" },
    };
    icon = new PixmapFile(getDisplay(), "../../motif/xpm/folder.xpm", white, white);
 
    for (int i = 0; i< numRows; i++) {
      ar.reset();
      ar.set(XmNcolumnLayout, (XtArgVal)columnLayout);
      ar.set(XmNnumColumns, numColumns);
      ar.set(XftNfont, (XtArgVal)(FTFont*)font);
      if (icon) {
        ar.set(XmNlabelType, XmPIXMAP);
        ar.set(XmNlabelPixmap, icon);
      }

      FTMultiColumn* col = new FTMultiColumn(rowColumn, "", ar);
      col -> setItems(items[i], numColumns);

      col -> addEventHandler(ButtonPressMask|ButtonReleaseMask|
                   KeyPressMask|KeyReleaseMask, 
                   this,
                  (Handler)&MainView::press, to_voidptr(i));
      columns[i] = col;
    }
  }

  ~MainView()
  {
    columns.clear();
  }
};
}

//
int main(int argc, char** argv)
{
  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "760x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

