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
 *  ModalFileDialog.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/CascadeMenuButton.h>
#include <oz++/motif/DefaultMenu.h>

#include <oz++/motif/MainWindow.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/motif/ModalFileDialog.h>
#include <oz++/Exception.h>
#include <oz++/CharString.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  int                         menuId;
  CharString                  filePath;

  SmartPtr<MainWindow>        mainw;
  SmartPtr<MenuBar>           menubar;
  SmartPtr<CascadeMenuButton> file;

  SmartPtr<ScrolledText>      sctext;
  SmartPtr<ModalFileDialog>   fileDialog;

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
        
    case SAVE: 
        sctext -> getText()
               -> save((const char*)filePath);
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
    char* filename;
    cs.get(&filename);
    printf("filename: %s\n", filename);
    if (menuId == OPEN) {
      sctext -> getText() -> load(filename);
    }
    if (menuId == SAVE_AS) {
      sctext -> getText() -> save(filename);
    }
    filePath = filename;
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
    filePath = "./ModalFileDialog.txt";
    sctext->getText()
          ->load((const char*)filePath);

    //Use the default file menu in DefaultMenu.h
    ar.reset();
    file = new CascadeMenuButton(menubar, "File", ar);
    file -> addItems(FileItems, XtNumber(FileItems),
                    this, (Callback)&MainView::fileMenu);

    //Create a modal FileDialog
    fileDialog = new ModalFileDialog(mainw, "FileDialog", ar);
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

