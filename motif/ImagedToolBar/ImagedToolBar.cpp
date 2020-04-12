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
 *  ImagedToolBar.cpp
 *
 *****************************************************************************/

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/MainWindow.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/ToolBar.h>
#include <oz++/motif/DrawingArea.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/FontCursor.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  enum {NEW, OPEN, SAVE, EXIT};

  int                      menuId;
  SmartPtr<MainWindow>     mainw;
  SmartPtr<MenuBar>        menubar;
  SmartPtr<CascadeButton>  file;
  SmartPtr<PulldownMenu>   pulldown;
  SmartPtr<Form>           form;  
  enum {FILL_RECTANGLE, FILL_ELLIPSE, LINE, RECTANGLE, ELLIPSE};
  
  SmartPtr<ToolBar>       toolBar;
  int                     toolBarId;	
  SmartPtr<DrawingArea>   drawingArea;

  int                     x,  y;
  int                     zx, zy;

  SmartPtr<DC>            drawingAreaDC;
  SmartPtr<FontCursor>    cursor;
  Pixel                   fgColor;
 
 void  fileMenu(Action& action)
  {
     this->menuId = action.getClientIntData();
     Color color(drawingArea);

     switch(menuId) {
     case NEW: 
        drawingArea -> set(XmNbackground, color.whitePixel()); 
        drawingArea -> update();
        break;
       
     case EXIT: 
         terminate();
         break;
     }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args),
  menuId(0),
  toolBarId(0),
  x(0),
  y(0),
  fgColor(0)
  {
    Display* display = getDisplay();

    Args ar;
    mainw   = new MainWindow(this, "mainw", ar);

    static MenuItem items[] = {
      {PUSHBUTTON,  "new",   "New",  NEW,  ZERO},
      {SEPARATOR,   "sep",    "",    0,    ZERO},
      {PUSHBUTTON,  "exit",  "Exit", EXIT, ZERO} 
    };

    ar.reset();
    menubar = new MenuBar(mainw, "menubar", ar);

    CompoundString cs("File");
    ar.reset();
    ar.set(XmNlabelString, cs);
    file = new CascadeButton(menubar, "File", ar);

    ar.reset();
    pulldown = new PulldownMenu(menubar, "pulldown", ar);
    pulldown -> addItem(items, XtNumber(items),
                     this, (Callback)&MainView::fileMenu);
    file -> set(XmNsubMenuId, pulldown);

    ar.reset();
    form = new Form(mainw, "...", ar);

    static const char *png[] = {
        "./images/new_64.png", 
        "./images/file_64.png",   
        "./images/folder_64.png",         
        "./images/4circle_64.png",   
        "./images/4rect_64.png",     
    };

    ar.reset();
    ar.set(XmNheight,      76);
    ar.set(XmNbuttons,     (XtArgVal)png);
    ar.set(XmNbuttonCount, XtNumber(png));
    ar.set(XmNlabelType,   XmPIXMAP);
    ar.set(XmNtopAttachment,   XmATTACH_FORM);
    ar.set(XmNrightAttachment, XmATTACH_FORM); 
    ar.set(XmNleftAttachment,  XmATTACH_FORM); 
    toolBar = new ToolBar(form, "toolbar", ar);

    Color color(form);
    ar.reset();
    ar.set(XmNtopAttachment, XmATTACH_WIDGET);
    ar.set(XmNtopWidget,     toolBar); 
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM); 
    ar.set(XmNleftAttachment,   XmATTACH_FORM); 
    ar.set(XmNbackground, color.whitePixel());
    drawingArea = new DrawingArea(form, "drawingArea" ,ar);
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
    args.set(XmNgeometry, "400x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

