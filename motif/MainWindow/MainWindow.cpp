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
 *  MainWindow.cpp
 *
 *****************************************************************************/


#include <oz++/motif/Application.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/MainWindow.h>
#include <oz++/motif/MenuBar.h>
#include <oz++/motif/CascadeButton.h>
#include <oz++/motif/DrawingArea.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/DC.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<MainWindow>     mainw;
  SmartPtr<MenuBar>        menubar;
  SmartPtr<CascadeButton>  file;
  SmartPtr<CascadeButton>  edit; 
  SmartPtr<DrawingArea>    drawinga;

  void  draw(Action& action)
  {
    DC dc(drawinga);
    Color   color(drawinga);
    dc.setForeground(color.allocNamedColor("green"));
    dc.fillEllipse(10, 10, 100, 50);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    ar.set(XmNscrollingPolicy, XmAUTOMATIC);
    mainw = new MainWindow(this, "mainw", ar);

    ar.reset();
    menubar = new MenuBar(mainw, "menubar", ar);

    ar.reset();
    file = new CascadeButton(menubar, "File", ar);

    ar.reset();
    edit = new CascadeButton(menubar, "Edit", ar);

    ar.reset();
    ar.set(XmNwidth,  300);
    ar.set(XmNheight, 300);
    drawinga = new DrawingArea(mainw, "drawinga", ar);
    drawinga -> addCallback(XmNexposeCallback, this,
                   (Callback)&MainView::draw, NULL);
    mainw -> setAreas(menubar->getWidget(), NULL,
                   NULL, NULL, drawinga->getWidget());
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
    args.set(XmNgeometry, "400x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

