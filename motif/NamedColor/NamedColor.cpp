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
 *  NamedColor.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/DC.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/Pixelmap.h>
#include <oz++/motif/NamedColor.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<Pixelmap>   pixelmap;
  SmartPtr<DC>         back;
  SmartPtr<NamedColor> namedColor;
  int                  pixwidth;
  int                  pixheight;

  void    draw(Event& event)
  {
    DC  dc(this);
    dc.copyArea(pixelmap->get(), 0, 0, pixwidth, pixheight, 10, 10);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Display* display = getDisplay();
    namedColor = new NamedColor(display);
    size_t size = namedColor -> getSize();
    pixwidth = size*5;
    pixheight = 60;
    pixelmap = new Pixelmap(display, pixwidth, pixheight);

    back = new DC(display, pixelmap->get());
    Color   color(display);
    back -> setForeground(color.allocNamedColor("white"));
    back -> fillRectangle(0, 0, pixwidth, pixheight);
    for (size_t i = 0; i<size; i++) {
      Pixel pixel = namedColor -> getNth(i);
      back->setForeground(pixel);
      back->fillRectangle(5*i, 2, 5, pixheight); 
    }

    addEventHandler(ExposureMask, this, 
                  (Handler)&MainView::draw, NULL);
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
    args.set(XmNgeometry, "740x240");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

