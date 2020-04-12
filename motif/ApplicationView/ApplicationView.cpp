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
 *  ApplicaionView.cpp
 *
 *****************************************************************************/

//2015/08/02 Updated menu items.

#include <oz++/motif/ApplicationView.h>

#include <oz++/motif/MainWindow.h>
#include <oz++/motif/MenuItem.h>
#include <oz++/motif/MenuBar.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/CascadeButton.h>
#include <oz++/motif/PulldownMenu.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/motif/FileDialog.h>
#include <oz++/Exception.h>
#include <oz++/CharString.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  //FILE_MENU_ID
  enum {NEW=100, OPEN, SAVE, SAVE_AS, EXIT};

  int             menuId;
  CharString      filePath;

  SmartPtr<MainWindow>     mainw;
  SmartPtr<MenuBar>        menubar;
  SmartPtr<CascadeButton>  file;
  SmartPtr<PulldownMenu>   pulldownm;
  SmartPtr<ScrolledText>   sctext;
  SmartPtr<FileDialog>     fileDialog;

  void    fileMenu(Action& action)
  {
    //this->menuId = (int)action.getClientData();
    this->menuId = (long)action.getClientData();

    switch(menuId) {
    case NEW: 
       sctext -> getText() -> setString("");
        break;
        
    case OPEN: 
        fileDialog->manage();
        break;
        
    case SAVE:
      sctext -> getText() -> save((const char*)filePath);
        break;

    case SAVE_AS: 
        fileDialog->manage();
        break;
        
    case EXIT: 
        confirm(action);
        break;
    }
  }


  void  open(Action& action)
  {
    XmFileSelectionBoxCallbackStruct* cbs =
           (XmFileSelectionBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->value);
    char* filename = NULL;
    cs.get(&filename);
    printf("filename: %s\n", filename);
    if (menuId == OPEN) {
      sctext -> getText() -> load(filename);
      filePath = filename;
    }
    if (menuId == SAVE_AS) {
      sctext -> getText() -> save(filename);
      filePath = filename;
    }
    XtFree(filename);
    fileDialog ->unmanage();
  }

  void  cancel(Action& action)
  {
    fileDialog -> unmanage();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args) 
  {
    static MenuItem items[] = {
     {PUSHBUTTON,  "new",   "New",      NEW,  ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "open",  "Open",     OPEN, ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "save",  "Save",     SAVE, ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "saveas", "Save As", SAVE_AS, ZERO,NULL, NULL, NULL},
     {SEPARATOR,   "sep",    "",        0,      ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "exit",  "Exit",     EXIT, ZERO, NULL, NULL, NULL},
    };

    Args ar;
    mainw   = new MainWindow(this, "mainw", ar);

    ar.reset();
    menubar = new MenuBar(mainw, "menubar", ar);

    ar.reset();
    sctext = new ScrolledText(mainw, "sctext", ar);
    filePath = "./ApplicationView.cpp";
    sctext->getText()
          ->load((const char*)filePath);

    CompoundString cs("File");
    ar.reset();
    ar.set(XmNlabelString, cs);
    file = new CascadeButton(menubar, "File", ar);

    ar.reset();
    //ar.set(XmNtearOffModel, XmTEAR_OFF_ENABLED);
    pulldownm = new PulldownMenu(menubar, "pulldownm", ar);
    pulldownm -> addItem(items, XtNumber(items),
                    this, (Callback)&MainView::fileMenu);
    file -> set(XmNsubMenuId, pulldownm);

    ar.reset();
    fileDialog = new FileDialog(this, "filed", ar);
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
  try {
    const char*  appclass = argv[0]; 
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "500x500");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  } 
  return 0;
}

