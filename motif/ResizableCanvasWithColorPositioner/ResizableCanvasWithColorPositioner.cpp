/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ResizableCanvasWithColorPositioner.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/MainWindow.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/MenuBar.h>
#include <oz++/motif/Canvas.h>
#include <oz++/motif/ColorPositioner.h>
#include <oz++/motif/CascadeButton.h>
#include <oz++/motif/PulldownMenu.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  enum {NEW, OPEN, SAVE, EXIT,
       CUT, COPY, PASTE,FIND, REPLACE };

  SmartPtr<MainWindow>      mainw;
  SmartPtr<Form>            form;
  SmartPtr<Canvas>          canvas;
  SmartPtr<ColorPositioner> positioner;
  SmartPtr<MenuBar>         menubar;
  SmartPtr<CascadeButton>   file;
  SmartPtr<CascadeButton>   edit;
  SmartPtr<PulldownMenu>    pulldownm;
  SmartPtr<PulldownMenu>    epulldownm;


  void    fileMenu(Action& action)
  {
    int menuId = action.getClientIntData();
    printf("fileMenu %d\n", menuId);
    if(menuId == EXIT) {
      confirm(action);
    }
  }

  void    editMenu(Action& action)
  {
    int menuId = action.getClientIntData();
    printf("editMenu %d\n", menuId);
  }

  void expose(Event& event)
  {
    DC dc(canvas);
    Color color(canvas);
    int r, g, b;
    positioner->getPosition(r, g, b);
    Pixel pixel = color.color256(r, g, b);
    dc.setForeground(pixel);
    Dimension w, h;
    canvas->get(XmNwidth, (XtArgVal)&w);
    canvas->get(XmNheight, (XtArgVal)&h);

    dc.fillEllipse(2, 2, w/2-4, h/2-4);
  }

  virtual void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe->type == ConfigureNotify) {
      configureNotify(event);
    } else {
      //Add here your own nofity event handlers.
    }
    sendExposeEvent();
  }
  
  //Your own resize handler.
  virtual void configureNotify(Event& event)
  {
    Dimension width, height;
    get(XmNwidth, (XtArgVal)&width);
    get(XmNheight, (XtArgVal)&height);
    if (positioner && canvas) {
      int w = width-positioner->getPositionerWidth()-4; 
      canvas->set(XmNwidth, w);
      canvas->set(XmNheight, height-4);
    }
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
     mainw   = new MainWindow(this, "mainw", ar);

     ar.reset();
     form   = new Form(mainw, "form", ar);

     Color color(this);
     ar.reset();
     ar.set(XmNx, 2);
     ar.set(XmNy, 2);
     ar.set(XmNtopAttachment,   XmATTACH_FORM);
     ar.set(XmNleftAttachment,  XmATTACH_FORM);
     ar.set(XmNbottomAttachment, XmATTACH_FORM);
     ar.set(XmNbackground, color.whitePixel());
     canvas = new Canvas(form, "canvas", ar);
     ar.reset();
     ar.set(XmNtopAttachment,   XmATTACH_FORM);
     ar.set(XmNrightAttachment, XmATTACH_FORM);
     ar.set(XmNleftAttachment,  XmATTACH_WIDGET);
     ar.set(XmNbottomAttachment, XmATTACH_FORM);
     ar.set(XmNleftWidget, canvas->getWidget());
     positioner = new ColorPositioner(form, "positioner", ar);
     positioner->setPosition(80, 0, 128);
     positioner->setExposeEventListener(this);
     
     ar.reset();
     menubar = new MenuBar(mainw, "menubar", ar);

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

     this -> addEventHandler(StructureNotifyMask, this, 
         (Handler)&MainView::structureNotify, NULL);

     this -> addEventHandler(ExposureMask, this, 
         (Handler)&MainView::expose, NULL);
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
    args.set(XmNgeometry, "600x370");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

