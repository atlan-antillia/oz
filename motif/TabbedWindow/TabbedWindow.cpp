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
 *  TabbedWindow.cpp
 *
 *****************************************************************************/


#include <oz++/CommonObject.h>
#include <oz++/Locale.h>
#include <oz++/Exception.h>
#include <oz++/HashTable.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/MenuBar.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/TabbedWindow.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>
#include <oz++/SmartArray.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  static const int MAX = 100;
  static const char DIR_PREFIX  = '+';
  static const char FILE_PREFIX = ' ';

private:
  SmartPtr<Form>            form;
  SmartPtr<TabbedWindow>    tabbedWindow;
  SmartArray<ScrolledListBox*> scrolledListBoxes;
  int max;
  int count;

  void browse(Action& action)
  {
    XmListCallbackStruct* cbs =
       (XmListCallbackStruct*)action.getCallData();
    int index = action.getClientIntData();
    printf("Index %d\n", index);
 
    CompoundString csitem(cbs -> item);
    char* item = csitem.unparse();
    char* rname = item + 2;
    printf("item: item=%s\n",item );
    if (item[0] != DIR_PREFIX) {
      XtFree(item);
      return;    
    }
 
    ScrolledListBox* sc= scrolledListBoxes[index+1];
    CompoundString* cs = new CompoundString(rname);
    if (sc == NULL) {
      printf("Adding new Tab item=%s\n", rname);
      Args ar;
      ar.reset();
      ar.set(XmNtabAlignment, XmALIGNMENT_BEGINNING);
      sc =  new ScrolledListBox(tabbedWindow, "", ar);
      scrolledListBoxes[index+1] = sc;
    } else {
      //Managing an existing tab of index+1.
      scrolledListBoxes[index+1] -> manage();
      for (int n = index +2; n<count; n++) {
        if (scrolledListBoxes[n] != NULL) {
      printf("Unmanaging already existing tabs index=%d\n", n);
          scrolledListBoxes[n] -> unmanage();
          scrolledListBoxes[n] = NULL;         
        }
      }
    }

    tabbedWindow -> selectTab(sc, TRUE);
    sc -> set(XmNtabLabelString, (XtArgVal)cs->get());
    char path[PATH_MAX]= {0};
    for(int i = 0; i<= index+1; i++) {
      XmString xms = NULL;
      scrolledListBoxes[i] ->get(XmNtabLabelString, (XtArgVal)&xms);
     
      if (xms) {
        CompoundString css(xms);
        char* label = css.unparse();
        strcat(path, label);
        strcat(path, "/");
        XtFree(label);
      }
    }
    printf("path %s\n", path);
    sc -> getList() -> unmap();
    sc -> getList() -> deleteAllItems();
    sc -> getList() -> listupDirectories(path, DIR_PREFIX);
    sc -> getList() -> listupFiles(path, FILE_PREFIX);
    sc -> getList() -> map();

    count = index+1;
    printf("Count %d\n", count); 
    sc -> getList()
              ->addCallback(XmNbrowseSelectionCallback, this,
                (Callback)&MainView::browse, to_voidptr(count));  

    count++; 
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args),
  form(NULL),
  tabbedWindow(NULL),
  count(0)
  {
    Args ar;
    form = new Form(this, "form", ar);

    ar.reset();
    ar.set(XmNresizePolicy, XmRESIZE_GROW);
    ar.set(XmNtopAttachment,   XmATTACH_FORM);
    ar.set(XmNrightAttachment, XmATTACH_FORM);
    ar.set(XmNleftAttachment,  XmATTACH_FORM);
    ar.set(XmNbottomAttachment,  XmATTACH_FORM);

    ar.set(XmNtabStyle, XmTABS_SQUARED);
    ar.set(XmNuniformTabSize, FALSE);

    tabbedWindow = new TabbedWindow(form, "tabbedWindow", ar);

    const char* home = getenv("HOME");

    CompoundString* cs = new CompoundString(home);
    ar.reset();
    ar.set(XmNtabAlignment, XmALIGNMENT_BEGINNING);
    ar.set(XmNtabLabelString, cs->get());

    ScrolledListBox* sc = new ScrolledListBox(tabbedWindow, "ScrolledList", ar);

    scrolledListBoxes[count] = sc;
      
    sc -> getList()
             -> listupDirectories(home, DIR_PREFIX);
    sc -> getList()
             -> listupFiles(home, FILE_PREFIX);
    sc -> getList()
              ->addCallback(XmNbrowseSelectionCallback, this,
                (Callback)&MainView::browse, to_voidptr(count));  
  
     count++;
  }


  ~MainView()
  {
    scrolledListBoxes.clear();
  }
};

}

//
int main(int argc, char** argv)
{
  const char*  appclass = argv[0];
  Application applet(appclass, argc, argv);
  try {
    Args args;
    args.set(XmNgeometry, "600x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}


