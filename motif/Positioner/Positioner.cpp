/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  BulletinBoard.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/Positioner.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<BulletinBoard> bboard;
  SmartPtr<Label>         label;
  SmartPtr<Positioner>    positioner;

private:
  void expose(Event& ev) 
  {
    if (positioner) {
      int x, y, z;
      positioner->getPosition(x, y, z);
      char pos[128];
      sprintf(pos, "Position(%d, %d, %d)", x, y, z);
      CompoundString cs(pos);
      label->set(XmNlabelString, cs);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args) 
  {
    Args ar;
    bboard = new BulletinBoard(this, "", ar);

    CompoundString cs("Hello");

    ar.reset();
    ar.set(XmNx, 10);
    ar.set(XmNy, 10);
    ar.set(XmNlabelString, cs);
    label = new Label(bboard, "", ar);

    ar.reset();
    ar.set(XmNx, 220);
    ar.set(XmNy,  2);
    positioner = new Positioner(bboard, "", ar);
    positioner->setPosition(10, 20, 30);
    positioner->setExposeEventListener(this);
    addEventHandler(ExposureMask, this,
      (Handler)&MainView::expose, NULL);
  }
  
  ~MainView() 
  {
  }
};
}

//
int main(int argc, char** argv){

  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "500x300");
    MainView view(applet, argv[0], args);
    view.realize();
    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

