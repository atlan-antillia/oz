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
 *  PulldownColorListView.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>

#include <oz++/motif/LabeledFrame.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/ColoredLabel.h>
#include <oz++/motif/ArrowButton.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/PulldownColorListView.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<BulletinBoard>    bboard;
  SmartPtr<RowColumn>        rowColumn;
  SmartPtr<ColoredLabel>     label;
  SmartPtr<ArrowButton>      pushb;
  SmartPtr<PulldownColorListView>   listView;

  void   activate(Action& action)
  {
    Position x = rowColumn->get(XmNx);
    Position y = rowColumn->get(XmNy);
    Dimension h = (Dimension)rowColumn->height();

    Position rx = 0;
    Position ry = 0;
    bboard ->translateCoords(x, y+h,
        &rx, &ry);

    listView -> popupAt(rx, ry, XtGrabNonexclusive );
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    bboard = new BulletinBoard(this, "", ar);
    ar.reset();
    ar.set(XmNx, 10);
    ar.set(XmNy, 40);
    ar.set(XmNorientation, XmHORIZONTAL);
    rowColumn = new RowColumn(bboard, "", ar);

    ar.reset();
    ar.set(XmNwidth, 140);
    label = new ColoredLabel(rowColumn, "", ar);
    ar.reset();
    ar.set(XmNwidth, 30);
    ar.set(XmNarrowDirection, XmARROW_DOWN);
    ar.set(XmNhighlightThickness,1);
    ar.set(XmNshadowThickness,1);
    pushb = new ArrowButton(rowColumn, "pushb", ar);
    pushb -> addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::activate, NULL);
    
    ar.reset();
    ar.set(XmNwidth,  160);
    ar.set(XmNheight, 200);
    listView = new PulldownColorListView(this, "", ar);
    listView->setNotifier(label);
  }

  ~MainView()
  {
  }
};
}


int main(int argc, char** argv)
{
  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNwidth, 200);
    args.set(XmNheight,300);

    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

