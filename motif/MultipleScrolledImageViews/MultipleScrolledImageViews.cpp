/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ScrolledImageView.cpp
 *
 *****************************************************************************/

//2017/05/10 This is a simple example to use ScrolledImageView in a MainWindow
//  with a menubar.

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/MainWindow.h>
#include <oz++/motif/MenuBar.h>
#include <oz++/motif/CascadeButton.h>
#include <oz++/motif/PulldownMenu.h>
#include <oz++/motif/ScrolledImageView.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/DC.h>
#include <oz++/motif/RenderTable.h>
#include <oz++/RECT.h>

namespace OZ {

class MainView :public ApplicationView {
      
private:
  //////////////////////////////////////////////////////////
  //Inner class starts.
  class SimpleView :public ScrolledImageView {
  private:
    CompoundString cs;

    virtual void createPixmap()
    {
      //We are ready to create a pixmap
      Args ar;
      ar.set(XmNfontName, "Times-a");
      ar.set(XmNfontSize, 28);
      RenderTable renderTable(this, "", ar);

      Display* dsp = getDisplay();
      uint width  = 1200;
      uint height =  800;
      Color color(this);
      Window window = getWindow();

      //Create a empty pixmap
      Pixmap pixmap = XCreatePixmap(dsp, window, width, height,
                        DefaultDepth(dsp,0));
      
      //Get a graphic context to the pixmap.
      DC dc(dsp, pixmap);

      //Draw something on the dc.
      Pixel backc = color.color256(18, 40, 100); 
      dc.setForeground(backc);
      dc.fillRectangle(0, 0, width, height);

      dc.setRenderTable(renderTable.get());

      dc.setForeground(color.color256(0, 255, 0)); //green
      dc.fillRectangle(240, 200, 200, 100);
        
      dc.setForeground(color.color256(255, 0, 0));  //red
      dc.setArcMode(ArcPieSlice);
      dc.fillArc(200, 200, 130, 130, 45*360, 120*360);

      dc.setForeground(color.color256(0, 0, 255));  //blue
      dc.fillArc(20, 200, 130, 130, 45*360, 120*360);

      dc.setForeground(color.color256(255, 255, 255)); //white
      cs.move(50, 50);
      cs.draw(&dc);

      //Set the pixmap to the member variable in ScrolledImageView.
      setPixmap(pixmap);

      showImageGeometry();
    }

  public:
     SimpleView(View* parent, const char* name, Args& args)
     :ScrolledImageView(parent, name, args)
     ,cs((const char*)args.get(XmNlabelString))
     {
       const char* string = cs.unparse();
       printf("strig %s\n", string);
     }
  };
  //Inner class ends.
  //////////////////////////////////////////////////////////

private:

  enum {NEW, OPEN, SAVE, EXIT,
       CUT, COPY, PASTE,FIND, REPLACE };

  SmartPtr<MainWindow>     mainw;
  SmartPtr<BulletinBoard>  bboard;
  SmartPtr<MenuBar>        menubar;
  SmartPtr<CascadeButton>  file;
  SmartPtr<CascadeButton>  edit;
  SmartPtr<PulldownMenu>   pulldownm;
  SmartPtr<PulldownMenu>   epulldownm;
  SmartPtr<SimpleView>     view1;  
  SmartPtr<SimpleView>     view2;  

  void layout()
  {
    if (bboard && view1 && view2) {
    int w = bboard->width();
    int h = bboard->height();
      view1 ->reshape(0, 0, w/2, h);
      view2 ->reshape(w/2, 0,  w/2, h);
    }
  }

  void mapNotify(Event& ev)
  {
  }

  void structureNotify(Event& event)
  {
    XEvent* xev = event.getXEvent();
    if (xev->type == MapNotify) {
      mapNotify(event);
    }
    if (xev->type == ConfigureNotify) {
      XConfigureEvent* xcv = (XConfigureEvent*)xev; 
      layout();
    } 
  }

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
    int menuId = (int)action.getClientIntData();
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
     mainw   = new MainWindow(this, "mainw", ar);

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

     ar.reset();
     bboard = new BulletinBoard(mainw, "", ar);

     const char* cs1 = "Kontakte mit der Ewigkeit\n"
                       "永遠とのコンタクト\n" 
                       "Gegner im Dunkel\n"
                       "闇に潜む敵\n";

     ar.reset();
     ar.set(XmNlabelString, cs1);
     view1 = new SimpleView(bboard, "", ar);

     const char* cs2 = "Imperial Earth\n"
                           "地球帝国\n"
                          "Axis of evil\n"
                           "悪の枢軸\n";
     ar.reset();
     ar.set(XmNlabelString, cs2);
     view2 = new SimpleView(bboard, "", ar);

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
    args.set(XmNwidth, 800);
    args.set(XmNheight, 300);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

