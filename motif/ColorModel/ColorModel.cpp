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
 *  ColorModel.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/DC.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/Pixelmap.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<Pixelmap> pixelmap;
  SmartPtr<DC>       back;

  void    draw(Event& event)
  {
    DC  dc(this);
    dc.copyArea(pixelmap->get(), 0, 0, 150,150, 20, 20);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Display* display = getDisplay();
    const int num = 64;//256;
    const int US = 65535;
    pixelmap = new Pixelmap(display, num*2, num*2);

    back = new DC(display, pixelmap->get());
    Color   color(display);
    back -> setForeground(color.allocNamedColor("white"));
    back -> fillRectangle(0, 0, num*2, num*2);
    for (int r = 0; r<num; r+=2 ) {
      for(int g = 0; g<num; g+=2) {
        for(int b = 0; b<num; b+=2) {
          XColor c;
          c.red   = US-(num-r)*US/num;;
          c.green = US-(num-g)*US/num;  
          c.blue  = US-(num-b)*US/num; 

          back->setForeground(color.allocColor(&c));
			   back->fillRectangle(b + r, g + r, 2, 2); 
        }
      }
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
    args.set(XmNgeometry, "300x300");
    MainView view(applet, argv[0], args);
    view.realize();
    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

