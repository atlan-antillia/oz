/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ScrolledTableView.cpp
 *
 *****************************************************************************/


#define DEBUG

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/ScrolledTableView.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<Form>      form;
  SmartPtr<ScrolledTableView> sctable;

  void activate(Action& action)
  {
     View* sender = (View*)action.getSender();
     XmString xms;
     sender -> get(XmNlabelString, (XtArgVal)&xms);
     CompoundString cs(xms);
     char* item = cs.unparse();
     printf("activate %s\n", item);
     XtFree(item);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args) ,
  sctable(NULL)
  {
    Color color(this);

    static ColumnData columnData[]  = {
      {"id", 30, XmALIGNMENT_END}, 
      {"object", 80, XmALIGNMENT_BEGINNING},
      {"score", 40, XmALIGNMENT_END   },
      {"x", 40, XmALIGNMENT_END      },
      {"y", 40, XmALIGNMENT_END   },
      {"w", 40, XmALIGNMENT_END      },
      {"h", 40, XmALIGNMENT_END      },
	};

    Args ar;
    form = new Form(this, "", ar);

    int numColumns = XtNumber(columnData);
    ar.reset();
    ar.set(XmNtopAttachment,    XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM);
    ar.set(XmNleftAttachment,   XmATTACH_FORM);

//  	ar.set(XmNtableColumnHeight, 20);
    ar.set(XmNtableNumColumns, numColumns);
    ar.set(XmNtableColumnData, (XtArgVal)columnData);
    ar.set(XmNtableNumRows, 10);
    ar.set(XmNtableRowWidth, 60);
    ar.set(XmNtableShowColumnHeader,False);

    sctable = new ScrolledTableView(form, "", ar);
    TableView* table = sctable->getTableView();

    table  -> removeAllItems();    	

    for (int row = 0; row<100; row++) {
      const char* items[numColumns];
      SmartArray<char*> labels(numColumns);

      for(int i = 0; i< numColumns; i++) {
         labels[i] = new char[128];
         sprintf(labels[i], "%3d", 1000+row+i);
         items[i] = labels[i];
      }
      table -> addItems( items, XtNumber(items));
    }
  }
 
  ~MainView() 
  {
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
    args.set(XmNgeometry, "760x400");
    MainView view(applet, argv[0], args);
    view.realize();
    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);    
  }
  return 0;
}

