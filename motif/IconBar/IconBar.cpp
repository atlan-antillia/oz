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
 *  IconBar.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/SashlessPanedWindow.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/motif/IconBar.h>
#include <oz++/motif/FileDialog.h>
#include <oz++/Pair.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<SashlessPanedWindow> panedw;
  SmartPtr<FileDialog>          fileDialog;
  SmartPtr<IconBar>             iconBar;
  SmartPtr<ScrolledText>        sctext;
  typedef enum {NEW, OPEN, SAVE, QUIT} MENU;
  	
  int menuId;

  void   activate(Action& action)
  {
     View* sender = (View*) action.getSender();
     XmString xms;
     sender -> get(XmNlabelString, (XtArgVal)&xms);
     sender -> get(XmNuserData, (XtArgVal)&menuId);

     CompoundString cs(xms);
     char* name = cs.unparse();
     printf("activate %s menuId=%d\n", name, menuId);
     XtFree(name);
     handleMenu(action);
  }

  void handleMenu(Action& action)
  {
    switch(menuId) { 
     case NEW:
       sctext -> getText() -> setString("");
       break;

     case OPEN:
       fileDialog -> manage();
       break;

     case SAVE:
       fileDialog -> manage();
       break;
    
     case QUIT:
        confirm(action);
     }
  }

  void  cancel(Action& action)
  {
    fileDialog ->unmanage();
  }

  void  open(Action& action)
  {
    XmFileSelectionBoxCallbackStruct* cbs =
           (XmFileSelectionBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->value);
    char* filename;
    cs.get(&filename);
    printf("filename: %s\n", filename);
    if (menuId == OPEN) {
      sctext -> getText() -> load(filename);
    }
    if (menuId == SAVE) {
      sctext -> getText() -> save(filename);
    }
    XtFree(filename);
    fileDialog ->unmanage();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    menuId = -1;
    Args ar;
    panedw = new SashlessPanedWindow(this, "", ar);

    ar.reset();
    ar.set(XmNpaneMinimum, 60);
    ar.set(XmNskipAdjust,FALSE);
    iconBar = new IconBar(panedw, "", ar);
  	
    static const IconBarItem items[] = 
  	{
  		{"New", NEW,         "../xpm/file.xpm"},
  		{"Open", OPEN,       "../xpm/open.xpm"},
  		{"Save", SAVE,       "../xpm/save.xpm"},
  		{"Quit", QUIT,       "../xpm/close.xpm" },
  	};
  	
    for (int  i = 0; i<XtNumber(items); i++) {
      CompoundString cs(items[i].name);
                      
      ar.reset();
      ar.set(XmNuserData, items[i].menuId);
      args.set(XmNminimumCellHeight, 48);    	
      ar.set(XmNiconFileName, items[i].iconFile); 
      ar.set(XmNlabelString, cs);
      ar.set(XmNshadowThickness, 0);
    	
      iconBar -> addButton(ar, 
                  XmNactivateCallback, this,
                 (Callback)&MainView::activate, NULL);
    }
    ar.reset();
    sctext = new ScrolledText(panedw, "", ar);

    ar.reset();
    fileDialog = new FileDialog(this, "FileDialog", ar);
    fileDialog -> getFileBox()
               -> addCallback(XmNokCallback, this,
                      (Callback)&MainView::open, NULL);

    fileDialog -> getFileBox()
               -> addCallback(XmNcancelCallback, this,
                      (Callback)&MainView::cancel, NULL);

  }

  ~MainView()
  {
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
    args.set(XmNgeometry, "600x500");
    MainView view(applet, argv[0], args);
    view.realize();

     applet.run();
  } catch (Exception& ex) {
     caught(ex);
  }
  return 0;
}
