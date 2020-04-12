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
 *  ImageViewer.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/MainWindow.h>
#include <oz++/motif/SashlessPanedWindow.h>
#include <oz++/motif/MenuItem.h>
#include <oz++/motif/MenuBar.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/CascadeButton.h>
#include <oz++/motif/PulldownMenu.h>
#include <oz++/motif/Scrolled.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/FileDialog.h>
#include <oz++/Exception.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  enum {OPEN, EXIT};

  int                           menuId;
  SmartPtr<MainWindow>          mainw;
  SmartPtr<MenuBar>             menubar;
  SmartPtr<CascadeButton>       file;
  SmartPtr<PulldownMenu>        pulldownm;
  SmartPtr<SashlessPanedWindow> panedw;
  SmartPtr<Label>               title;
  SmartPtr<PixmapFile>          pixmapf;
  SmartPtr<Scrolled<Label> >     scrolledLabel;
  SmartPtr<FileDialog>          fileDialog;

  void    fileMenu(Action& action)
  {
    this->menuId = action.getClientIntData();

    switch(menuId) {
    case OPEN:
        fileDialog->manage();
        break;
       
    case EXIT: 
        confirm(action);
        break;
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
    //printf("filename: %s\n", filename);
    if (menuId == OPEN) {
      Label* label = scrolledLabel -> getItem(); 
      try {
        PixmapFile* pfile = new PixmapFile(getDisplay(), filename); 
        if (pfile) {
          unsigned int w, h, bw, d;
          pfile -> getGeometry(&w, &h, &bw, &d); 
          printf("Depth %d\n", d);
          label -> unmanage(); 
          pixmapf = NULL;
          pixmapf = pfile;    
          label->set(XmNlabelPixmap, (XtArgVal)pixmapf -> get());
          label -> manage(); 
          char* name = filename; 
          char* slash = strrchr(filename, '/');
          if (slash) {
            name = ++slash;
          }
          CompoundString cs(name);
          title -> set(XmNlabelString, cs);
        }
      } catch (Exception& ex) {
        caught(ex); 
      }
    }
    XtFree(filename);
    fileDialog ->unmanage();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args) 
  {
    static MenuItem items[] = {
      {PUSHBUTTON,  "open",  "Open", OPEN, ZERO},
      {SEPARATOR,   "sep",    "", 0, ZERO},
      {PUSHBUTTON,  "exit",  "Exit", EXIT, ZERO} 
    };

    Args ar;
    mainw   = new MainWindow(this, "mainw", ar);

    ar.reset();
    menubar = new MenuBar(mainw, "menubar", ar);

    CompoundString cs("File");
    ar.reset();
    ar.set(XmNlabelString, cs);
    file = new CascadeButton(menubar, "File", ar);

    ar.reset();
    pulldownm = new PulldownMenu(menubar, "pulldownm", ar);
    pulldownm -> addItem(items, XtNumber(items),
                    this, (Callback)&MainView::fileMenu);
    file -> set(XmNsubMenuId, pulldownm);

    ar.reset();
    ar.set(XmNorientation, XmVERTICAL);
    panedw = new SashlessPanedWindow(mainw, "", ar);

    CompoundString ls(" ");
    ar.reset();
    ar.set(XmNlabelType, XmSTRING);
    ar.set(XmNlabelString, ls);
    ar.set(XmNalignment, XmALIGNMENT_BEGINNING);
    title  = new Label(panedw, "", ar);
 
    pixmapf = NULL;

    ar.reset();
    ar.set(XmNscrollingPolicy, XmAUTOMATIC);
    ar.set(XmNscrollBarDisplayPolicy, XmVARIABLE);
    ar.set(XmNlabelType, XmPIXMAP);
    scrolledLabel = new Scrolled<Label>(panedw, "scrolledLabel", ar);

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
int main(int argc, char** argv) {
  
  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "400x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  } 
  return 0;
}

