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
 *  BulletinBoardWithMenuBar.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>
//#include <oz++/motif/MainWindow.h>
#include <oz++/motif/MenuBar.h>
#include <oz++/motif/CascadeButton.h>
#include <oz++/motif/PulldownMenu.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  enum {NEW, OPEN, SAVE, EXIT,
       CUT, COPY, PASTE,FIND, REPLACE };

  //MainWindow*     mainw;
  SmartPtr<BulletinBoard>  bboard;
  SmartPtr<MenuBar>        menubar;
  SmartPtr<CascadeButton>  file;
  SmartPtr<CascadeButton>  edit;
      
  SmartPtr<PulldownMenu>   pulldownm;
  SmartPtr<PulldownMenu>   epulldownm;

  void    fileMenu(Action& action)
  {
    int menuId = action.getClientIntData();
    printf("fileMenu %d\n", menuId);
    if(menuId == EXIT) {
       exit(action);
    }
  }

  void    editMenu(Action& action)
  {
    int menuId = action.getClientIntData();
    printf("editMenu %d\n", menuId);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    static MenuItem items[] = {
      {PUSHBUTTON,  "new",   "New",  NEW,   ZERO},
      {PUSHBUTTON,  "open",  "Open", OPEN,  ZERO},
      {PUSHBUTTON,  "save",  "Save", SAVE,  ZERO},
      {PUSHBUTTON,  "exit",  "Exit", EXIT, ZERO}
     };

     static MenuItem eitems[] = {
       {PUSHBUTTON,  "cut",    "Cut",  CUT,  ZERO},
       {PUSHBUTTON,  "copy",  "Copy", COPY,  ZERO},
       {PUSHBUTTON,  "paste",  "Paste", PASTE, ZERO},
       {PUSHBUTTON,  "find",  "Find", FIND, ZERO},
       {PUSHBUTTON,  "replace",  "Replace", REPLACE, ZERO}
      };

     Args ar;
     bboard   = new BulletinBoard(this, "bboard", ar);

     ar.reset();
     menubar = new MenuBar(bboard, "menubar", ar);

     CompoundString cs("File");
     ar.reset();
     ar.set(XmNlabelString, cs);
     file = new CascadeButton(menubar, "File", ar);

     ar.reset();
     ar.set(XmNtearOffModel, XmTEAR_OFF_ENABLED);
     pulldownm = new PulldownMenu(menubar, "pulldownm", ar);
     pulldownm -> addItem(items, XtNumber(items),
                          this, (Callback)&MainView::fileMenu);
     file -> set(XmNsubMenuId, pulldownm);

     CompoundString ecs("Edit");
     ar.reset();
     ar.set(XmNlabelString, ecs);
     edit = new CascadeButton(menubar, "Edit", ar);

     ar.reset();
     ar.set(XmNtearOffModel, XmTEAR_OFF_ENABLED);
     epulldownm = new PulldownMenu(menubar, "epulldownm", ar);
     epulldownm -> addItem(eitems, XtNumber(eitems),
                          this, (Callback)&MainView::editMenu);
     edit -> set(XmNsubMenuId, epulldownm);
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

  Args args;
  args.set(XmNgeometry, "300x300");
  MainView view(applet, argv[0], args);
  view.realize();

  applet.run();
  return 0;
}

