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
 *  PulldownPullrightMenu.cpp
 *
 *****************************************************************************/

//2015/08/10 Simple sample for Pulldown and Pullright menus.

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
  enum {NEW=100, OPEN, SAVE, SAVE_AS, COLOR,CODE, EXIT};

  //Dummy menu colorItems
  enum {RED=120, GREEN, BLUE};

  //Dummy menu codeItems
  enum {ASCII=140, UTF_8, SHIFT_JIS};

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
    this->menuId = action.getClientIntData();

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
        exit(action);
        break;
        
    case RED:
       printf("Color is Red\n");
       break;

    case GREEN:
       printf("Color is Green\n");
       break;
    
    case BLUE:
       printf("Color is Blue\n");
       break;

    case ASCII:
       printf("Code is ASCII\n");
       break;

    case UTF_8:
       printf("Code is UTF-8\n");
       break;
    
    case SHIFT_JIS:
       printf("Code is SHIFT_JIS\n");
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
    //Dummy colorItems;
    static MenuItem colorItems[] = {
     {PUSHBUTTON,  "red",   "Red",   RED,  ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "green", "Green", GREEN, ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "blue",  "Blue",  BLUE,   ZERO, NULL, NULL, NULL},
    };

    //Dummy codeItems;
    static MenuItem codeItems[] = {
     {PUSHBUTTON,  "asc",   "Ascii",       ASCII,  ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "utf-8",  "UFT-8",      UTF_8, ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "shiftjis",  "Shift-JIS",  SHIFT_JIS,   ZERO, NULL, NULL, NULL},
    };

    static MenuItem items[] = {
     {PUSHBUTTON,  "new",   "New",      NEW,  ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "open",  "Open",     OPEN, ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "save",  "Save",     SAVE, ZERO, NULL, NULL, NULL},
     {PUSHBUTTON,  "saveas", "Save As", SAVE_AS, ZERO,NULL, NULL, NULL},
     {CASCADEBUTTON,  "color",   "Color",  COLOR, ZERO,NULL, NULL, colorItems, SizeOf(colorItems)},
     {CASCADEBUTTON,  "code",   "Code",    CODE, ZERO,NULL, NULL, codeItems, SizeOf(codeItems)},
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

