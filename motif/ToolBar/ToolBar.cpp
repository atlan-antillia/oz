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
 *  ToolBar.cpp
 *
 *****************************************************************************/
//2015/01/18 Simple sample program to use ToolBar class.
// Draw some typical figures onto DrawingArea with an XOR drawing function. 

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
  enum {FILL_RECTANGLE, FILL_ELLIPSE, LINE, RECTANGLE, ELLIPSE};

  int                      menuId;
  SmartPtr<MainWindow>     mainw;
  SmartPtr<MenuBar>        menubar;
  SmartPtr<CascadeButton>  file;
  SmartPtr<PulldownMenu>   pulldown;
  SmartPtr<Form>           form;   
  SmartPtr<ToolBar>        toolBar;
  int                      toolBarId;	
  SmartPtr<DrawingArea>    drawingArea;
  SmartPtr<DC>             drawingAreaDC;
  SmartPtr<FontCursor>     cursor;

  int         x,  y;
  int         zx, zy;
  Pixel       fgColor;
 
  void clicked(Action& action)
  {
    toolBarId = action.getClientIntData();
    printf("Button %d clicked\n", toolBarId);
  	
  	static const char* namedColors[] = {"blue", "yellow", "black", "green", "red"};
    	
    Color color(drawingArea);
  	fgColor = color.allocNamedColor(namedColors[toolBarId]);

    //Note that the background color of the drawingArea is white.
    //So, we set the following color as the foreground color
    drawingAreaDC -> setForeground(color.whitePixel()^fgColor);
  }

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

  void expose(Event& event)
  {
     if (drawingAreaDC == NULL) {
        drawingAreaDC = new DC(drawingArea);
        //Set XOR drawing function to drawingAreaDC(GC) 
        drawingAreaDC -> setFunction(GXxor);
     }
  }

  void buttonPress(Event& event)
  {
     drawingArea -> grabPointer(cursor->get());

     x = event.x();
     y = event.y();
     zx = event.x();
     zy = event.y();
     //printf("x=%d y=%d\n", x, y);
  }

  void buttonRelease(Event& event)
  {
     drawingArea -> ungrabPointer();
  }

  void motionNotify(Event& event)
  {
    switch(toolBarId) {
    case FILL_RECTANGLE:  
      drawingAreaDC->fillRectangle(x, y, zx - x, zy - y);
      drawingAreaDC->fillRectangle(x, y, event.x() - x, event.y() - y);
      break;
    case FILL_ELLIPSE:  
      drawingAreaDC->fillEllipse(x, y, zx - x, zy- y);
      drawingAreaDC->fillEllipse(x, y, event.x() - x, event.y() - y);
      break;

    case LINE:  
      drawingAreaDC->drawLine(x, y, zx, zy);
      drawingAreaDC->drawLine(x, y, event.x(), event.y());
      break;
    case RECTANGLE:  
      drawingAreaDC->drawRectangle(x, y, zx - x, zy - y);
      drawingAreaDC->drawRectangle(x, y, event.x() - x, event.y() - y);
      break;
    case ELLIPSE:  
      drawingAreaDC->drawEllipse(x, y, zx - x, zy - y);
      drawingAreaDC->drawEllipse(x, y, event.x() - x, event.y() - y);
      break;

    }
    zx = event.x();
    zy = event.y();
    //printf("motion x=%d y=%d\n", event.x(), event.y());
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

    static const char *xpms[] = {
        "./xpm/full_rectangle.xpm", 
        "./xpm/full_circle.xpm",   
        "./xpm/line.xpm",         
        "./xpm/rectangle.xpm",   
        "./xpm/circle.xpm",     
    };

    CompoundStringList csl(xpms, XtNumber(xpms));
    ar.reset();
    ar.set(XmNheight,      40);
    ar.set(XmNbuttons,     (XtArgVal)xpms);
    ar.set(XmNbuttonCount, XtNumber(xpms));
    ar.set(XmNlabelType,   XmPIXMAP);
    ar.set(XmNtopAttachment,   XmATTACH_FORM);
    ar.set(XmNrightAttachment, XmATTACH_FORM); 
    ar.set(XmNleftAttachment,  XmATTACH_FORM); 
    toolBar = new ToolBar(form, "toolbar", ar);
    toolBar -> addCallbackToKids(XmNvalueChangedCallback, this,
                     (Callback)&MainView::clicked, NULL);

    Color color(form);
    ar.reset();
    ar.set(XmNtopAttachment, XmATTACH_WIDGET);
    ar.set(XmNtopWidget,     toolBar); 
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM); 
    ar.set(XmNleftAttachment,   XmATTACH_FORM); 
    ar.set(XmNbackground, color.whitePixel());
    drawingArea = new DrawingArea(form, "drawingArea" ,ar);

    drawingAreaDC = NULL;
    cursor = new FontCursor(display, XC_crosshair);

    drawingArea -> addEventHandler(ExposureMask, this,
                   (Handler)&MainView::expose, NULL);
    drawingArea -> addEventHandler(ButtonPressMask, this,
                   (Handler)&MainView::buttonPress, NULL);
    drawingArea -> addEventHandler(ButtonReleaseMask, this,
                   (Handler)&MainView::buttonRelease, NULL);
    drawingArea -> addEventHandler(ButtonMotionMask, this,
                   (Handler)&MainView::motionNotify, NULL);         
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

