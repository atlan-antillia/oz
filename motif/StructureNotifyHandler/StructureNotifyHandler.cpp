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
 *  StructureNotifyHandler.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/ScrollBar.h>
#include <oz++/motif/RenderTable.h>
#include <oz++/motif/DC.h>

/*
#define KeyPress                2
#define KeyRelease              3
#define ButtonPress             4
#define ButtonRelease           5
#define MotionNotify            6
#define EnterNotify             7
#define LeaveNotify             8
#define FocusIn                 9
#define FocusOut                10
#define KeymapNotify            11
#define Expose                  12
#define GraphicsExpose          13
#define NoExpose                14
#define VisibilityNotify        15
#define CreateNotify            16
#define DestroyNotify           17
#define UnmapNotify             18
#define MapNotify               19
#define MapRequest              20
#define ReparentNotify          21
#define ConfigureNotify         22
#define ConfigureRequest        23
#define GravityNotify           24
#define ResizeRequest           25
#define CirculateNotify         26
#define CirculateRequest        27
#define PropertyNotify          28
#define SelectionClear          29
#define SelectionRequest        30
#define SelectionNotify         31
#define ColormapNotify          32
#define ClientMessage           33
#define MappingNotify           34
#define GenericEvent            35
 */
namespace OZ {

class MainView  :public ApplicationView {
private:
  SmartPtr<BulletinBoard>  bboard;
  SmartPtr<BulletinBoard>  tablet;
  SmartPtr<ScrollBar>      hscrollBar;
  SmartPtr<ScrollBar>      vscrollBar;
  SmartPtr<RenderTable>    renderTable;
  int  width;
  int  height;

 
  void    draw(Event& event)
  {
    XEvent* xevent = (XEvent*)event.getXEvent();
    DC dc(this);
    dc.setRenderTable(renderTable->get());

    const char* string = (const char*)event.getClientData();
    CompoundString cs(string);
    cs.move(50, 100);
    cs.draw(&dc);

    printf("%s\n", string);
  }

/*
typedef struct
{
  int      reason;
  XEvent   * event;
  int      value;
  int      pixel;
} XmScrollBarCallbackStruct;
*/
  void vscValueChanged(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = (XmScrollBarCallbackStruct*)action.getCallData();
    printf("vscValueChanged reason=%d %d\n", cbs->reason, cbs->value);
  }

  void vscDrag(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = (XmScrollBarCallbackStruct*)action.getCallData();
    printf("vscDrag reason=%d %d\n", cbs->reason, cbs->value);
  }

  void vscIncrement(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = (XmScrollBarCallbackStruct*)action.getCallData();
    printf("vscIncrement reason=%d %d\n", cbs->reason, cbs->value);
  }

  void vscDecrement(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = (XmScrollBarCallbackStruct*)action.getCallData();
    printf("vscDecrement reason=%d %d\n", cbs->reason, cbs->value);
  }

  void hscValueChanged(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = (XmScrollBarCallbackStruct*)action.getCallData();
    printf("hscrolled reason=%d %d\n", cbs->reason, cbs->value);
  }

  void hscDrag(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = (XmScrollBarCallbackStruct*)action.getCallData();
    printf("hscDrag reason=%d %d\n", cbs->reason, cbs->value);
  }

  void hscIncrement(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = (XmScrollBarCallbackStruct*)action.getCallData();
    printf("hscIncrement reason=%d %d\n", cbs->reason, cbs->value);
  }

  void hscDecrement(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = (XmScrollBarCallbackStruct*)action.getCallData();
    printf("hscDecrement reason=%d %d\n", cbs->reason, cbs->value);
  }

  void structureNotify(Event& event)
  {
    printf("structureNotify");
    XEvent* xe = event.getXEvent();
    printf("Type %d\n", xe->type);
   
    if (xe -> type == MapNotify) {
      layout();
    }
    if (xe -> type == ConfigureNotify) {
      XConfigureEvent xce = xe->xconfigure;
        // if (xce.width != width ||
        //        xce.height != height) {
           width = xce.width;
           height = xce.height;
          printf("Resized width=%d height=%d\n",
           width, height);
          bboard -> set(XmNwidth, width);
          bboard -> set(XmNheight,height);
      layout();
      //}
    }
  }

  void layout()
  {
    Dimension w = bboard -> width();
    Dimension h = bboard -> height();
    printf("w=%d h=%d\n", w, h);
    tablet -> resize(4, 4, (w-22), (h-22), 1);

    vscrollBar -> resize ((w-20), 4, 
                         14,  (h - 20 - 4), 0);
    hscrollBar -> resize( 4, (h-20),
                         (w - 20 -4),  14, 0);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    width = 0;
    height = 0;

    Args ar;
    ar.set(XmNfontName, "Times-a");
    ar.set(XmNfontSize, 24);
    renderTable = new RenderTable(this, "", ar);

    ar.reset();
    ar.set(XmNborderWidth, 1);
    bboard = new BulletinBoard(this, "", ar);

    bboard -> addEventHandler(StructureNotifyMask, this, 
          (Handler)&MainView::structureNotify, NULL);
    ar.reset();
    tablet = new BulletinBoard(this, "",ar);

    ar.reset();
    ar.set(XmNsliderSize, 100);
    ar.set(XmNmaximum,  1000+100);
    ar.set(XmNorientation, XmVERTICAL);
    vscrollBar = new ScrollBar(bboard, "", ar);
    vscrollBar -> addCallback(XmNvalueChangedCallback, this,
           (Callback)&MainView::vscValueChanged, NULL);

    vscrollBar -> addCallback(XmNdragCallback, this,
           (Callback)&MainView::vscDrag, NULL);
    vscrollBar -> addCallback(XmNincrementCallback, this,
           (Callback)&MainView::vscIncrement, NULL);
    vscrollBar -> addCallback(XmNdecrementCallback, this,
           (Callback)&MainView::vscDecrement, NULL);

    ar.reset();
    ar.set(XmNsliderSize, 10);
    ar.set(XmNmaximum, 20+10);
    ar.set(XmNorientation, XmHORIZONTAL);
    hscrollBar = new ScrollBar(bboard, "", ar);
    hscrollBar -> addCallback(XmNvalueChangedCallback, this,
           (Callback)&MainView::hscValueChanged, NULL);

    hscrollBar -> addCallback(XmNdragCallback, this,
           (Callback)&MainView::hscDrag, NULL);
    hscrollBar -> addCallback(XmNincrementCallback, this,
           (Callback)&MainView::hscIncrement, NULL);
    hscrollBar -> addCallback(XmNdecrementCallback, this,
           (Callback)&MainView::hscDecrement, NULL);

    bboard -> addEventHandler(StructureNotifyMask, this, 
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
    args.set(XmNgeometry, "300x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
   caught(ex);
  }
  return 0;
}

