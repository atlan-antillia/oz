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
 *  Notifier.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/Notifier.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/DC.h>

#include <oz++/SmartArray.h>

namespace OZ {
	
class MainView  :public ApplicationView {
private:
  SmartPtr<BulletinBoard> bboard;
  SmartPtr<RowColumn>     rowColumn;
  SmartArray<Notifier*>   labels;

  int   width;
  int   height;

  Pixel white;
  Pixel blue;

  void activate(Event& event)
  {
    View* sender = (View*)event.getSender();

    XEvent* xe = event.getXEvent();
    XmString xms;
    sender -> get(XmNlabelString, (XtArgVal)&xms);
    CompoundString cs(xms);
    char* label = cs.unparse();
    printf("acitvate %s\n", label);
    XtFree(label);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Color color(this);
    white = color.whitePixel();
    static const char* items[] = {
      "Amazon アマゾン",
      "Apple アップル",
      "Google グーグル",
      "HP ヒューレット・パッカード",
      "IE (Imperial Earth) 地球帝国",
      "Microsoft マイクロソフト",
      "IBM アイビーエム",
      "Sony ソニー",  
    };

    Args ar;
    //ar.set(XmNbackground, white);
    bboard = new BulletinBoard(this, "", ar);

    ar.reset();
    ar.set(XmNcolumns, 1);
    ar.set(XmNrows, XtNumber(items));
    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNbackground, white);

  	rowColumn = new RowColumn(bboard, "", ar);

    for (int i = 0; i<XtNumber(items); i++) {
      CompoundString cs(items[i]);
      ar.reset();
      ar.set(XmNlabelString, cs);

      ar.set(XmNwidth, 260);

      ar.set(XmNbackground, white);
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING);
 
      labels[i] = new Notifier(rowColumn, "", ar);

      labels[i] -> addEventHandler(ButtonPressMask, this, 
          (Handler)&MainView::activate, NULL);
    }
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

