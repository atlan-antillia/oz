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
 *  PixmapCursor.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/PixmapCursor.h>
#include <oz++/motif/Color.h>

namespace OZ {
 
class MainView :public ApplicationView {
private:
  SmartPtr<PixmapCursor> cursor;
  Pixmap       source;
  Pixmap       mask;

  void    draw(Event& event)
  {
    Display* display = getDisplay();
    Screen* screen = XDefaultScreenOfDisplay(display);
    Window root    = RootWindowOfScreen(screen);

    if(cursor == NULL) {
      int x, y;
      unsigned int w, h;
      XReadBitmapFile(display, root, 
				"/usr/include/X11/bitmaps/star", 
				//"star", 
				&w, &h, &source, &x, &y);
      XReadBitmapFile(display, root, 
				//"starMask", 
				"/usr/include/X11/bitmaps/starMask", 
				&w, &h, &mask,   &x, &y);
      XColor  fg, bg;
      Color   color(display);
      color.allocNamedColor("red",  &fg);
      color.allocNamedColor("blue", &bg);

      cursor = new PixmapCursor(display,
                          source, mask, &fg, &bg, 0, 0);
      XDefineCursor(display, getWindow(), cursor->get());
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
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
    args.set(XmNgeometry, "200x200");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  } 
  return 0;
}

