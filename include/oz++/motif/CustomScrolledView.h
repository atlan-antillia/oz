/****************************************************************************
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
 *  CustomScrolledView.h
 *
 *****************************************************************************/

//2017/10/10 Updated to use create method.

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/ScrolledWindow.h>
#include <oz++/motif/ScrollBar.h>
#include <oz++/motif/DC.h>

namespace OZ {

class CustomScrolledView :public ScrolledWindow {
private:
  SmartPtr<BulletinBoard>  workArea;
  SmartPtr<ScrollBar>      hscrollBar;
  SmartPtr<ScrollBar>      vscrollBar;

public:
  CustomScrolledView(View* parent, const char* name, Args& args)
  :ScrolledWindow()
  {
    CustomScrolledView::create(parent, name, args);
  }

  
  CustomScrolledView()
  :ScrolledWindow()
  {
  }

  virtual void create(View* parent, const char* name, Args& args)
  {
    ScrolledWindow::create(parent, name, 
	args.set(XmNscrollingPolicy, XmAPPLICATION_DEFINED));
  
    Args ar; 
    ar.reset();
    workArea = new BulletinBoard(this, "",ar);

    ar.reset();
    ar.set(XmNvalue, 0);
    ar.set(XmNsliderSize, 100);
    ar.set(XmNmaximum,    1000);
    ar.set(XmNorientation, XmVERTICAL);
    vscrollBar = new ScrollBar(this, "", ar);
    vscrollBar -> addCallback(XmNvalueChangedCallback, this,
           (Callback)&CustomScrolledView::vscValueChanged, NULL);

    vscrollBar -> addCallback(XmNdragCallback, this,
           (Callback)&CustomScrolledView::vscDrag, NULL);
    vscrollBar -> addCallback(XmNincrementCallback, this,
           (Callback)&CustomScrolledView::vscIncrement, NULL);
    vscrollBar -> addCallback(XmNdecrementCallback, this,
           (Callback)&CustomScrolledView::vscDecrement, NULL);

    ar.reset();
    ar.set(XmNvalue, 0);
    ar.set(XmNsliderSize, 100);
    ar.set(XmNmaximum,   1000);
    ar.set(XmNorientation, XmHORIZONTAL);
    hscrollBar = new ScrollBar(this, "", ar);
    hscrollBar -> addCallback(XmNvalueChangedCallback, this,
           (Callback)&CustomScrolledView::hscValueChanged, NULL);

    hscrollBar -> addCallback(XmNdragCallback, this,
           (Callback)&CustomScrolledView::hscDrag, NULL);
    hscrollBar -> addCallback(XmNincrementCallback, this,
           (Callback)&CustomScrolledView::hscIncrement, NULL);
    hscrollBar -> addCallback(XmNdecrementCallback, this,
           (Callback)&CustomScrolledView::hscDecrement, NULL);

    addEventHandler(StructureNotifyMask, this,
          (Handler)&CustomScrolledView::structureNotify, NULL);

    workArea -> addEventHandler(ExposureMask, this, 
          (Handler)&CustomScrolledView::expose, NULL);
 
    setArea(hscrollBar->getWidget(), vscrollBar->getWidget(), 
		workArea->getWidget());
  }

  int  getVertScrollBarValue()
  {
    return (int)vscrollBar->get(XmNvalue);
  }

  int  getHorizScrollBarValue()
  {
    return (int)hscrollBar->get(XmNvalue);
  }

  void setVertScrollBarRange(int min, int max) 
  {
    vscrollBar->set(XmNminimum, min);
    vscrollBar->set(XmNmaximum, max);
  }

  void setHorizScrollBarRange(int min, int max) 
  {
    hscrollBar->set(XmNminimum, min);
    hscrollBar->set(XmNmaximum, max);
  }

  int  setVertScrollBarSliderSize(int size)
  {
    int max = vscrollBar->get(XmNmaximum);
    if (size>max) {
      size = max;
    }
    vscrollBar->set(XmNsliderSize, size );
  }

  int  setHorizScrollBarSliderSize(int size)
  {
    int max = hscrollBar->get(XmNmaximum);
    if (size>max) {
      size = max;
    }
    hscrollBar->set(XmNsliderSize, size);
  }

  void getScrollBarMaximum(Dimension& hmax, Dimension& vmax)
  {
    hmax = hscrollBar->get(XmNmaximum);
    vmax = vscrollBar->get(XmNmaximum);
  }

  void setScrollBarMaximum(Dimension hmax, Dimension vmax)
  {
    hscrollBar->set(XmNmaximum, hmax);
    vscrollBar->set(XmNmaximum, vmax);
  }

  void setScrollBarSliderSize(int w, int h)
  {
    setHorizScrollBarSliderSize(w);
    setVertScrollBarSliderSize(h);
  }

  void getScrollBarSliderSize(Dimension& w, Dimension& h)
  {
    w = hscrollBar->get(XmNsliderSize);
    h = vscrollBar->get(XmNsliderSize);
  }

  BulletinBoard* getWorkArea()
  {
    return workArea;
  }

  ScrollBar* getHorizScrollBar()
  {
    return hscrollBar;
  }

  ScrollBar* getVertScrollBar()
  {
    return vscrollBar;
  }

  void  getScrollBarPosition(Position& hpos, Position& vpos)
  {
    hscrollBar->get(XmNvalue, (XtArgVal)&hpos);
    vscrollBar->get(XmNvalue, (XtArgVal)&vpos);
  }

  void  setScrollBarPosition(Position hpos, Position vpos)
  {
    hscrollBar->set(XmNvalue, (XtArgVal)hpos);
    vscrollBar->set(XmNvalue, (XtArgVal)vpos);
  }

  void  expose(Event& event)
  {
    //2017/08/20
    display();
  }

  //Define your own display method in a subclass derived from this class.
  virtual void display()
  {
    Position hpos, vpos;
    getScrollBarPosition(hpos, vpos);
  }
 
  virtual void resize(Dimension w, Dimension h)
  {
 //   printf("CustomScrolledView resize w=%d h=%d\n", w, h);
    BulletinBoard* bb = getWorkArea();
    int ww = bb->width();
    int hh = bb->height();

    Position hpos, vpos;
    getScrollBarPosition(hpos, vpos);

    Dimension hmax, vmax;
    getScrollBarMaximum(hmax, vmax);
    if (hpos >= (hmax-ww)) {
      hpos = (hmax -ww);
      if (hpos <0) hpos = 0;
      hscrollBar->set(XmNvalue, hpos);
    }

    if (vpos >= (vmax-hh)) {
      vpos = (vmax -hh);
      if (vpos <0) vpos = 0;
      vscrollBar->set(XmNvalue, vpos);
    }

    setHorizScrollBarSliderSize(ww); //(w);
    setVertScrollBarSliderSize(hh);  //h);
  }

  virtual void configureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    XConfigureEvent* xce = (XConfigureEvent*)xe;
    dprintf("configureNotify w=%d h=%d\n", xce->width, xce->height);

    resize(xce->width, xce->height);
  }

  virtual void mapNotify(Event& event)
  {
    //Call mapped method.
    mapped();
  }
  //If needed, please define your own method in a subclass derived from this class
  virtual void mapped()
  {
    // Window mapped, you can create GC or Pixmap inside this method. 
  }

  virtual void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    switch(xe->type) {
    case ConfigureNotify:
        configureNotify(event);
        break;
    case MapNotify:
        mapNotify(event);
        break;
    default:
       break;
    }
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
    XmScrollBarCallbackStruct* cbs = 
		(XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("vscValueChanged reason=%d %d\n", cbs->reason, cbs->value);
    workArea->sendExposeEvent();
  }

  void vscDrag(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = 
		(XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("vscDrag reason=%d %d\n", cbs->reason, cbs->value);
    workArea->sendExposeEvent();
  }

  void vscIncrement(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = 
		(XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("vscIncrement reason=%d %d\n", cbs->reason, cbs->value);
    workArea->sendExposeEvent();
  }

  void vscDecrement(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = 
		(XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("vscDecrement reason=%d %d\n", cbs->reason, cbs->value);
    workArea->sendExposeEvent();
  }

  void hscValueChanged(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = 
		(XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("hscValueChanged reason=%d %d\n", cbs->reason, cbs->value);
    workArea->sendExposeEvent();
  }

  void hscDrag(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = 
		(XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("hscDrag reason=%d %d\n", cbs->reason, cbs->value);
    workArea->sendExposeEvent();
  }

  void hscIncrement(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = 
		(XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("hscIncrement reason=%d %d\n", cbs->reason, cbs->value);
    workArea->sendExposeEvent();
  }

  void hscDecrement(Action& action)
  {
    XmScrollBarCallbackStruct* cbs = 
		(XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("hscDecrement reason=%d %d\n", cbs->reason, cbs->value);
    workArea->sendExposeEvent();
  }
};

}

