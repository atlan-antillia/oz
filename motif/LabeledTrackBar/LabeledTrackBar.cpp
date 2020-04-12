/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  LabeledTrackBar.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/LabeledTrackBar.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<BulletinBoard>    bboard;
  SmartPtr<RowColumn>        rowColumn;

  SmartPtr<LabeledTrackBar>  trackBar1;
  SmartPtr<LabeledTrackBar>  trackBar2;

  void    valueChanged(Action& action)
  {
     XmScaleCallbackStruct* cbs =
                (XmScaleCallbackStruct*) action.getCallData();
     printf("value %d\n", cbs->value);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    bboard = new BulletinBoard(this, "", ar);
    ar.reset();
    int w = bboard->width();
    ar.set(XmNx, 200);
    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNy, 10);
    rowColumn = new RowColumn(bboard, "", ar);
 
    ar.reset();
    CompoundString cs("Temparature [0, 100]");
    ar.set(XmNminimum, 0);
    ar.set(XmNmaximum,  100);
    ar.set(XmNvalue,   30);
    ar.set(XmNtitleString, cs);
    trackBar1 = new LabeledTrackBar(rowColumn, "scale", ar);

    trackBar1 -> addCallback(XmNdragCallback, this,
              (Callback)&MainView::valueChanged, NULL);
    ar.reset();
   CompoundString cs2("Month [1, 12]");
    ar.set(XmNminimum, 1);
    ar.set(XmNmaximum,  12);
    ar.set(XmNvalue,   5);
    ar.set(XmNtitleString, cs2);
    trackBar2 = new LabeledTrackBar(rowColumn, "scale2", ar);

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
    args.set(XmNgeometry, "800x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

