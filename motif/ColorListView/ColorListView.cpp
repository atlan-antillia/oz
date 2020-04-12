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
 *  ColorListView.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/ColorListView.h>
#include <oz++/motif/CompoundString.h>

namespace OZ {

class MainView :public ApplicationView {

private:
  SmartPtr<BulletinBoard>  bboard; 
  SmartPtr<ColorListView>  colorListView;
  SmartPtr<Label>          label;

  void selected(Event& event)
  {
    if (event.isEnterKey() ||  event.isButtonPressed() ) {
      event.processTraversal();

      Pixel pixel = (Pixel)event.getClientData();
      char text[128];
      sprintf(text, "Pixel=%lx", pixel);
      CompoundString cs(text);
      label -> set(XmNlabelString, cs);
    }
  }

  virtual void resize(Dimension w, Dimension h)
  {
    label ->move(10, 10);

    colorListView->reshape(10, 60, w-20, h-70); 
  }

  void configureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    XConfigureEvent* xce = (XConfigureEvent*)xe;
    dprintf("configureNotify w=%d h=%d\n", xce->width, xce->height);

    resize(xce->width, xce->height);
  }

  void structureNotify(Event& event)
  {
     XEvent* xe = event.getXEvent();
     if (xe->type == ConfigureNotify) {
        configureNotify(event);
     }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    bboard = new BulletinBoard(this, "bboard", ar);
 
    ar.reset();
    ar.set(XmNwidth, 400);
    ar.set(XmNheight, 400);
    colorListView = new ColorListView(bboard, "colorlistbox", ar);
    colorListView ->addEventHandler(ButtonPressMask|KeyPressMask, this,
             (Handler)&MainView::selected, NULL); 


    ar.reset();
    ar.set(XmNalignment, XmALIGNMENT_CENTER);
    label = new Label(bboard, "label", ar);

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
    args.set(XmNgeometry, "300x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

