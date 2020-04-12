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
 *  Draw.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/ScrolledDrawingArea.h>
#include <oz++/motif/ColorListView.h>
#include <oz++/motif/Pixelmap.h>
#include <oz++/motif/MainWindow.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  static const int WIDTH = 600;
  static const int HEIGHT = 600;

  enum {NEW, OPEN, SAVE, EXIT};

  int             menuId;
	
  SmartPtr<MainWindow>     mainw;
  SmartPtr<BulletinBoard>  bboard; 
  SmartPtr<MenuBar>        menubar;
  SmartPtr<CascadeButton>  file;
  SmartPtr<PulldownMenu>   pulldown;
  SmartPtr<ScrolledDrawingArea> scrolledDrawingArea;
  SmartPtr<ColorListView>    colorListView;
  SmartPtr<Pixelmap>  pixelmap;
  SmartPtr<DC>        back;

  Pixel  currentPixel;
  int    x, y;
 
  void selected(Event& event)
  {
    if (event.isEnterKey() || event.isButtonPressed() ) {

      int width =  40;
      int height = 40;
      currentPixel = (Pixel)event.getClientData();
      printf("Pixel %lx\n", currentPixel);
      DrawingArea* area = scrolledDrawingArea->getDrawingArea();
      back->setForeground(currentPixel);
      back->fillRectangle(x, y, x + width, y+ height); 
      x += 10;
      y += 10;
      area -> update();
      View* sender = (View*)event.getSender();
      sender -> processTraversal();
    }
  }

  void    draw(Event& event)
  {
     DrawingArea* area = scrolledDrawingArea->getDrawingArea();
     DC dc(area);
     dc.copyArea(pixelmap->get(), 0, 0, WIDTH, HEIGHT, 0, 0);
  }

  void    fileMenu(Action& action)
  {
     this->menuId = action.getClientIntData();

     switch(menuId) {
     case NEW: {
        x = 0;
        y = 0;
        Color color(scrolledDrawingArea); 
        back -> setForeground(color.whitePixel());
        back -> fillRectangle(0, 0, WIDTH, HEIGHT);
       
        DrawingArea* area = scrolledDrawingArea->getDrawingArea();
        area->update();
        break;
       }
     case EXIT: {
         confirm(action);
         break;
       } 
     }
  }

  void resize(Dimension w, Dimension h)
  {
    colorListView -> reshape(0, 0, 200, h);
    scrolledDrawingArea -> reshape(200, 0, w-200, h);
  }

  virtual void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe->type == MapNotify) {
      //mapNotify(event);
    } else if (xe->type == ConfigureNotify) {
      XConfigureEvent* xce = (XConfigureEvent*)xe;
      resize(xce->width, xce->height);
    } else {
      //Add here your own nofity event handlers.
    }
  }
 
public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    mainw   = new MainWindow(this, "mainw", ar);

    static MenuItem items[] = {
      {PUSHBUTTON,  "new",   "New",  NEW,  ZERO},
            //PUSHBUTTON,  "open",  "Open", OPEN, ZERO},
            //PUSHBUTTON,  "save",  "Save", SAVE, ZERO},
      {SEPARATOR,   "sep",    "",      0, ZERO},
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
    bboard = new BulletinBoard(mainw, "bboard", ar);
 
    ar.reset();
    ar.set(XmNwidth, 140);
    ar.set(XmNheight, 400);

    colorListView = new ColorListView(bboard, "colorlistbox", ar);
    colorListView ->addEventHandler(ButtonPressMask|KeyPressMask, this,
             (Handler)&MainView::selected, NULL); 

    ar.reset();
    ar.set(XmNwidth, 400);
    ar,set(XmNheight, 400);
    ar.set(XmNscrollingPolicy, XmAUTOMATIC);
    ar.set(XmNscrollBarDisplayPolicy, XmAS_NEEDED);
    scrolledDrawingArea = new ScrolledDrawingArea(bboard,
                  "..", ar);
    DrawingArea* area = scrolledDrawingArea->getDrawingArea();
    area -> set(XmNwidth,  WIDTH);
    area -> set(XmNheight, HEIGHT);
    Display* display = getDisplay();
    Color color(area);
    Pixel white = color.whitePixel();
    area -> set(XmNbackground, white);
    pixelmap = new Pixelmap(display, WIDTH, HEIGHT);
    back = new DC(display, pixelmap->get());
    back -> setForeground(white);
    back -> fillRectangle(0, 0, WIDTH, HEIGHT);
    x = 0;
    y = 0;

    area -> addEventHandler(ExposureMask, this, 
                   (Handler)&MainView::draw, NULL);

    addEventHandler(StructureNotifyMask, this,
         (Handler)&MainView::structureNotify, NULL);
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
    args.set(XmNgeometry, "600x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

