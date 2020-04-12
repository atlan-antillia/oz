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
 *  CascadeMenuButton.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>

#include <oz++/motif/CascadeMenuButton.h>
#include <oz++/motif/HelpDialog.h>

#include <oz++/motif/MainWindow.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/motif/FileDialog.h>
#include <oz++/Exception.h>
#include <oz++/CharString.h>

#include <oz++/motif/DefaultMenu.h>


namespace OZ {

class MainView :public ApplicationView {
private:
  int             menuId;
  CharString      filePath;

  SmartPtr<MainWindow>        mainw;
  SmartPtr<MenuBar>           menubar;
  SmartPtr<CascadeMenuButton> file;
  SmartPtr<CascadeMenuButton> edit;
  SmartPtr<CascadeMenuButton> help;

  SmartPtr<ScrolledText>      sctext;
  SmartPtr<FileDialog>        fileDialog;
  SmartPtr<HelpDialog>        helpDialog;

  void    fileMenu(Action& action)
  {
    this->menuId = action.getClientIntData();
    printf("fileMenu %d\n", menuId);

    switch(menuId) {
    case NEW: 
        sctext -> getText()
               -> setString("");
        break;
        
    case OPEN: 
        fileDialog->manage();
        break;
        
    case SAVE_AS: 
        fileDialog->manage();
        break;
        
    case EXIT: 
        confirm(action);
        break;
    }
  }

 void    editMenu(Action& action)
  {
    this->menuId = action.getClientIntData();
    printf("editMenu %d\n", menuId);
    switch(menuId) {
    case CUT:
        helpDialog->setMessage("Default menu: Cut");
        helpDialog -> manage();
        break;

    case COPY:
        helpDialog->setMessage("Default menu: Copy");
        helpDialog -> manage();
        break;

    case PASTE:
        helpDialog->setMessage("Default menu: Paste");
        helpDialog -> manage();
        break;

    case SELECT_ALL:
        helpDialog->setMessage("Default menu: Select All");
        helpDialog -> manage();
        break;
    }
  }

 void    helpMenu(Action& action)
  {
    this->menuId = action.getClientIntData();
    printf("helpMenu %d\n", menuId);
    switch(menuId) {
    case HELP:
        helpDialog->setMessage("Default Help message");
        helpDialog -> manage();
        break;

    case VERSION:
        helpDialog->setMessage("OZ++ Class Library for Linux");
        helpDialog -> manage();
        break;
    }
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
    if (menuId == SAVE_AS) {
      sctext -> getText() -> save(filename);
    }
    XtFree(filename);
    fileDialog ->unmanage();
  }

  void cancel(Action& action)
  {
    fileDialog -> unmanage();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    mainw   = new MainWindow(this, "mainw", ar);

    ar.reset();
    menubar = new MenuBar(mainw, "menubar", ar);
    ar.reset();
    sctext = new ScrolledText(mainw, "sctext", ar);
    sctext->getText()
          ->load("./PulldownMenuButton.cpp");

    ar.reset();
    ar.set(XmNmnemonic, 'F');
    file = new CascadeMenuButton(menubar, "File", ar);
    file -> addItems(FileItems, XtNumber(FileItems),
                    this, (Callback)&MainView::fileMenu);

    ar.reset();
    ar.set(XmNmnemonic, 'E');
    edit = new CascadeMenuButton(menubar, "Edit", ar);
    edit -> addItems(EditItems, XtNumber(EditItems),
                    this, (Callback)&MainView::editMenu);

    ar.reset();
    ar.set(XmNmnemonic, 'H');
    help = new CascadeMenuButton(menubar, "Help", ar);
    help -> addItems(HelpItems, XtNumber(HelpItems),
                    this, (Callback)&MainView::helpMenu);

    ar.reset();
    //Modal FileDialog
    ar.set(XmNdialogStyle, XmDIALOG_APPLICATION_MODAL);
    fileDialog = new FileDialog(mainw, "FileDialog", ar);
    fileDialog -> getFileBox()
               -> addCallback(XmNokCallback, this,
                      (Callback)&MainView::open, NULL);
    fileDialog -> getFileBox()
               -> addCallback(XmNcancelCallback, this,
                      (Callback)&MainView::cancel, NULL);

    ar.reset();
    helpDialog = new HelpDialog(this, "Message", ar);
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
    args.set(XmNgeometry, "600x600");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  } 
  return 0;
}

