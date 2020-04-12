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
#include <oz++/motif/LabeledFrame.h>
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
  class InnerView :public ScrolledImageView {
  private:
    //This is a mandatory method.
    virtual void createPixmap()
    {
      Args ar;
      ar.set(XmNfontName, "Times-a");
      ar.set(XmNfontSize, 26);
      RenderTable renderTable(this, "", ar);

      Display* dsp = getDisplay();
      uint width  = 1200;
      uint height =  800;
      Color color(this);

      BulletinBoard* workArea = getWorkArea();

      Window window = workArea->getWindow();

      Pixmap pixmap = XCreatePixmap(dsp, window, width, height,
                        DefaultDepth(dsp,0));
      
      DC dc(dsp, pixmap);
      Pixel backc = color.color256(0, 128, 100); 
      dc.setForeground(backc);
      dc.fillRectangle(0, 0, width, height);

      dc.setRenderTable(renderTable.get());

      const char* string = "This is a simple example of \n LabeledScrolledImageView\n"
                           " in OZ++ class library,\n"
                           "which is based on OpenMotif-2.3.7.\n";

      for (int i = 0; i<100; i++) {
        dc.setForeground(color.color256(i, 0, i)); //green
        dc.drawRectangle(20, 20, i, i);
      }
 
      dc.setArcMode(ArcPieSlice);
      for (int i= 0; i<100; i++) {
        dc.setForeground(color.color256(255, i, i));  //red
        dc.drawArc(200, 200, 10+i, 10+i, 45*360, 120*360);
      }

      dc.setForeground(color.color256(0, 0, 255));  //blue
      dc.fillArc(20, 200, 130, 130, 45*360, 120*360);

      dc.setForeground(color.color256(255, 255, 255)); //white
      CompoundString cs(string);
      cs.move(50, 50);
      cs.draw(&dc);
      
      setPixmap(pixmap);

      showImageGeometry();
    }

  public:
     InnerView(View* parent, const char* name, Args& args)
     :ScrolledImageView(parent, name, args)
     {
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
  SmartPtr<LabeledFrame>   frame;  
  SmartPtr<InnerView>      view;  

  void layout()
  {
    if (bboard && frame ) {
     int w = bboard->width();
     int h = bboard->height();
     frame ->reshape(0, 0, w, h);
    }
  }

  void mapNotify(Event& event)
  {
    //Do nothing here.
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
    int menuId = action.getClientIntData();
    printf("editMenu %d\n", menuId);
  }
  
public:
  //////////////////////////////////////////////////////////
  //Constructor

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

     CompoundString labelName("ラベル付きスクロールImageView");
     ar.reset();
     ar.set(XmNlabelString, labelName);

     frame = new LabeledFrame(bboard, "", ar);
   
     ar.reset();
     view = new InnerView(frame, "", ar);

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
    args.set(XmNwidth,  600);
    args.set(XmNheight, 400);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

