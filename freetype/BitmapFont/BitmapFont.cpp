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
 *  FreeType.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/DC.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/Pixelmap.h>

#include <oz++/freetype/FreeTypeLibrary.h>
#include <oz++/freetype/FreeTypeBitmapFont.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<FreeTypeLibrary>    library;
  SmartPtr<FreeTypeBitmapFont> bitmapFont;

  SmartPtr<Pixelmap>           pixelmap;
  SmartPtr<DC>                 back;

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
    width  = 1000;
    height = 400;

    pixelmap = new Pixelmap(display, width, height);

    back = new DC(display, pixelmap->get());
    Color   color(display);
    back -> setForeground(color.allocNamedColor("white"));
    back -> fillRectangle(0, 0, width, height);

    try {
      library  = new FreeTypeLibrary();
      bitmapFont     = new FreeTypeBitmapFont(library, 
                    "/usr/share/fonts/truetype/freefont/FreeSans.ttf");
      //bitmapFont     = new FreeTypeBitmapFont(library, 
      //    "/usr/share/fonts/truetype/takao-mincho/TakaoMincho.ttf");
      int size = 24;
      bitmapFont -> setPixelSizes(0, size);
      Pixel dgreen = color.allocNamedColor("darkgreen");

      //const wchar_t* string = L"Hello世界";
      const char* string = "Hello world";

      //Draw the string as a bitmap format, not outline. 
      bitmapFont -> drawString(back, 20, 200, string, dgreen);

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
    args.set(XmNgeometry, "1010x410");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

