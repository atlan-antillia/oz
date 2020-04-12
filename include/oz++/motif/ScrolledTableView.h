/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      ScrolledTableView.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/motif/ScrolledWindow.h>
#include <oz++/motif/TableView.h>
#include <oz++/motif/ScrollBar.h>

namespace OZ {

class ScrolledTableView :public ScrolledWindow {
private:
  SmartPtr<TableView> tablev;
  SmartPtr<ScrollBar> hscrollBar;
  SmartPtr<ScrollBar> vscrollBar;

public:
  ScrolledTableView(View* parent, const char* name, Args& args)
  :ScrolledWindow()
  {
    ScrolledTableView::create(parent, name, args);
  }

  ScrolledTableView()
  :ScrolledWindow()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Args ar; 
    ScrolledWindow::create(parent, name, 
                             args.set(XmNscrollingPolicy, XmAUTOMATIC)
                                 .set(XmNscrollBarDisplayPolicy, XmAS_NEEDED)); 
    tablev = new TableView(this, "tablev", args);

    Widget hs, vs;
    hs = (Widget)get(XmNhorizontalScrollBar);
    vs = (Widget)get(XmNverticalScrollBar);

    if (hs) {
      hscrollBar = new ScrollBar(hs); 
      hscrollBar -> addCallback(XmNvalueChangedCallback, this,
           (Callback)&ScrolledTableView::hScrollValueChanged, NULL);

      hscrollBar -> addCallback(XmNdragCallback, this,
           (Callback)&ScrolledTableView::hScrollValueChanged, NULL);
      hscrollBar -> addCallback(XmNincrementCallback, this,
           (Callback)&ScrolledTableView::hScrollValueChanged, NULL);
      hscrollBar -> addCallback(XmNdecrementCallback, this,
           (Callback)&ScrolledTableView::hScrollValueChanged, NULL);

      hscrollBar -> addCallback(XmNpageDecrementCallback, this,
           (Callback)&ScrolledTableView::hScrollValueChanged, NULL);
      hscrollBar -> addCallback(XmNpageIncrementCallback, this,
           (Callback)&ScrolledTableView::hScrollValueChanged, NULL);
    }
    if (vs) {
      vscrollBar = new ScrollBar(vs); 
      vscrollBar -> addCallback(XmNvalueChangedCallback, this,
           (Callback)&ScrolledTableView::vScrollValueChanged, NULL);
      vscrollBar -> addCallback(XmNdragCallback, this,
           (Callback)&ScrolledTableView::vScrollValueChanged, NULL);
      vscrollBar -> addCallback(XmNincrementCallback, this,
           (Callback)&ScrolledTableView::vScrollValueChanged, NULL);
      vscrollBar -> addCallback(XmNdecrementCallback, this,
           (Callback)&ScrolledTableView::vScrollValueChanged, NULL);

      vscrollBar -> addCallback(XmNpageDecrementCallback, this,
           (Callback)&ScrolledTableView::vScrollValueChanged, NULL);
      vscrollBar -> addCallback(XmNpageIncrementCallback, this,
           (Callback)&ScrolledTableView::vScrollValueChanged, NULL);
    }
  }

  ~ScrolledTableView()
  {
  }

  void manage()
  {
    View::manage();
    if (tablev) {
      tablev->manage();
    }
  }

  TableView* getTableView()
  {
    return tablev;
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
  void vScrollValueChanged(Action& action)
  {
    XmScrollBarCallbackStruct* cbs =
                (XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("vScrollValueChanged reason=%d %d\n", cbs->reason, cbs->value);
    //workArea->sendExposeEvent();
  }

  void hScrollValueChanged(Action& action)
  {
    XmScrollBarCallbackStruct* cbs =
                (XmScrollBarCallbackStruct*)action.getCallData();
    dprintf("hScrollValueChanged reason=%d %d\n", cbs->reason, cbs->value);
    //workArea->sendExposeEvent();
  }

};

}


