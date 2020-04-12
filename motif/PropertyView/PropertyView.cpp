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
 *  PropertyView.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/TableView.h>
#include <oz++/motif/Color.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<Form>      form;
  SmartPtr<TableView> table;

  void buttonPress(Event& event)
  {
    event.processTraversal();

    Color color(this);
    Pixel black = color.blackPixel();
    Pixel white = color.whitePixel();

    View* sender = (View*)event.getSender();
    int   index = event.getClientIntData();
    XEvent* xevent = event.getXEvent();
    if (xevent->type == ButtonPress) {
      printf("button pressed index=%d\n", index);
      sender -> set(XmNbackground, black);
      sender -> set(XmNforeground, white);
    }
    if (xevent->type == ButtonRelease) {
      printf("button released index=%d\n", index);
      sender -> set(XmNbackground, white);
      sender -> set(XmNforeground, black);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Color color(this);
    Pixel white = color.whitePixel();

    static ColumnData columnData[]  = {
      {"Value", 410, XmALIGNMENT_BEGINNING}, 
	};

    static RowData rowData[]  = {
      {"Name1", 110, XmALIGNMENT_END}, 
      {"Name2", 0, XmALIGNMENT_END}, 
      {"Name3", 0, XmALIGNMENT_END}, 
      {"Name4", 0, XmALIGNMENT_END}, 
      {"Name5", 0, XmALIGNMENT_END}, 
      {"Name6", 0, XmALIGNMENT_END}, 
      {"Name7", 0, XmALIGNMENT_END}, 
      {"Name8", 0, XmALIGNMENT_END}, 
      {"Name9", 0, XmALIGNMENT_END}, 
      {"Name10", 0, XmALIGNMENT_END}, 
	};

    Args ar;
    form = new Form(this, "", ar);

    int numColumns = XtNumber(columnData);
    int numRows    = XtNumber(rowData);
    ar.reset();
    ar.set(XmNtopAttachment,    XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM);
    ar.set(XmNleftAttachment,   XmATTACH_FORM);

  	ar.set(XmNtableColumnHeight, 20);
    ar.set(XmNtableNumColumns, numColumns);
    ar.set(XmNtableColumnData, (XtArgVal)columnData);
    ar.set(XmNtableNumRows, numRows);
    ar.set(XmNtableRowData, (XtArgVal)rowData);

    table = new TableView(form, "", ar);
    table -> removeAllItems();    	
    for (int i = 0; i<numRows; i++) {
      char item[256];
      sprintf(item, "%d Value1234567890 1234567890 12345678890 ABCDEFGHIJKLMN", i); 
      table -> setItem(i, 0, item);
      table -> addEventHandler(i, 0, ButtonPressMask|ButtonReleaseMask, this, 
            (Handler)&MainView::buttonPress, to_voidptr(i));
    }
  }
  
	
  ~MainView() 
  {
  }
};
}

//
int main(int argc, char** argv){

  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "760x400");
    MainView view(applet, argv[0], args);
    view.realize();
    applet.run();
    
  } catch (Exception& ex) {
    
  }
  return 0;
}

