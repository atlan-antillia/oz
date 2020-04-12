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
 *  DrawnButton.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/DrawnButton.h>
#include <oz++/motif/PixmapFile.h>
#include <oz++/motif/RenderTable.h>


namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<RenderTable> rtable;
  SmartPtr<RowColumn>   rowColumn;
  SmartPtr<PixmapFile>  pixmapf;
  SmartPtr<DrawnButton> drawnb;

private:
  void activate(Action& action)
  {
    printf("activate\n");
  }

private:
   void expose(Action& action)
   {
      Color color(this);
      //Pixel white = color.whitePixel();
      Pixel red = color.allocNamedColor("red");
      Pixel navy = color.allocNamedColor("navy");
      DrawnButton* view = (DrawnButton*)action.getSender();
      Dimension h = 0;
      view -> get(XmNheight, (XtArgVal)&h); 
      DC dc(view);
      const char* string[] = {
		"Good morning おはようabcdefghijiklmn",
		"Hello world こんにちは1234567890 ABCDEF",
        "Goodbye world さようなら ABCDEF 1234567890"};
      Pixel colors[XtNumber(string)];
      int n = 0;
      colors[n++] = color.allocNamedColor("lightgreen");
      colors[n++] = color.allocNamedColor("lightblue");
      colors[n++] = color.allocNamedColor("lightpink");

      XRectangle r;
      r.x = h; 
      const int WIDTH = 280;
      dc.setRenderTable(*rtable);
      for (int i = 0; i < XtNumber(string); i++) {
        r.y = 0;
        r.width = WIDTH;
        r.height = h;
        dc.setForeground(colors[i]);
        dc.setClipRectangles(0, 0, &r, 1, 0);
        dc.fillRectangle(r.x, 4, WIDTH, h-8);
        dc.setForeground(red);
        CompoundString cs(string[i]);
        cs.setClipRectangle(&r);
        cs.move(r.x + 2, 8);
        cs.draw(&dc);
        dc.setClipRectangles(0, 0, &r, 1, 0);

        r.x += WIDTH + 10; 
      }
   }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;

    ar.reset();
    ar.set(XmNfontSize, 20);
    ar.set(XmNfontName, "Times-12");
    rtable = new RenderTable(this, "", ar); 

    Color color(this);
    Pixel white = color.allocNamedColor("white");
    ar.reset();
    ar.set(XmNbackground, white);
    rowColumn = new RowColumn(this, "", ar);

    Pixel fg = white;
    Pixel bg = white;
    pixmapf = new PixmapFile(getDisplay(), "../xpm/folder48.xpm", fg, bg);

    ar.reset();
    ar.set(XmNbackground, white);
    ar.set(XmNlabelType,   XmPIXMAP);
    ar.set(XmNshadowThickness, 0);
    ar.set(XmNhighlightThickness, 1);

    ar.set(XmNlabelPixmap, (XtArgVal)pixmapf->get());
    drawnb = new DrawnButton(rowColumn, "label", ar);

    drawnb -> addCallback(XmNexposeCallback, this,
             (Callback)&MainView::expose, NULL);
    drawnb -> addCallback(XmNactivateCallback, this,
             (Callback)&MainView::activate, NULL);
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
    args.set(XmNgeometry, "800x100");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

