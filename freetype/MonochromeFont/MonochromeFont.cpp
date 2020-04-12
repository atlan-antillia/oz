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
 *  MonochromeFont.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/DC.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/Pixelmap.h>

#include <oz++/freetype/FreeTypeMonochromeFont.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<FreeTypeLibrary>         library;
  SmartPtr<FreeTypeMonochromeFont>  sans;
  SmartPtr<FreeTypeMonochromeFont>  serif;
  SmartPtr<FreeTypeMonochromeFont>  takao;
  SmartPtr<FreeTypeMonochromeFont>  takaog;
  SmartPtr<Pixelmap>                pixelmap;
  SmartPtr<DC>                      back;

  int      width;
  int      height;

  void    draw(Event& event)
  {
    DC  dc(this);
    dc.copyArea(pixelmap->get(), 0, 0, width, height, 0, 0);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Display* display = getDisplay();
    width  = 700;
    height = 400;

    pixelmap = new Pixelmap(display, width, height);

    back = new DC(display, pixelmap->get());
    Color   color(display);
    back -> setForeground(color.allocNamedColor("white"));
    back -> fillRectangle(0, 0, width, height);

    try {
      library  = new FreeTypeLibrary();
      sans     = new FreeTypeMonochromeFont(library, 
                    "/usr/share/fonts/truetype/freefont/FreeSans.ttf");
      serif     = new FreeTypeMonochromeFont(library, 
                    "/usr/share/fonts/truetype/freefont/FreeSerif.ttf");
      takao     = new FreeTypeMonochromeFont(library, 
                    "/usr/share/fonts/truetype/takao-mincho/TakaoMincho.ttf");
      takaog     = new FreeTypeMonochromeFont(library, 
                    "/usr/share/fonts/truetype/takao-gothic/TakaoGothic.ttf");

      Pixel red = color.allocNamedColor("red");
      Pixel blue = color.allocNamedColor("blue");
      Pixel dgreen = color.allocNamedColor("darkgreen");
      Pixel black = color.allocNamedColor("black");

      const char* string = "Hello World. Goodbye World";
      const wchar_t* jstring = L"Hello World こんにちは世界";
      const wchar_t* goodbye = L"さよなら世界";

      //Draw the strings above as a bitmap format, not outline. 
      sans -> setPixelHeight(16);
      sans -> drawString(back, 10, 20, string, red);

      sans -> setPixelHeight(24);
      sans -> drawString(back, 10, 60, string, red);

      serif -> setPixelHeight(48);
      serif -> drawString(back, 10, 120, string, red);

      takao -> setPixelHeight(32);
      takao -> drawString(back, 10, 180, jstring, blue);

      takao -> setPixelHeight(48);
      takao -> drawString(back, 10, 250, jstring, dgreen);

      takaog -> setPixelHeight(64);
      takaog -> drawString(back, 10, 340, goodbye, black);

    } catch (Exception& ex) {
      caught(ex);
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
    args.set(XmNgeometry, "710x410");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

