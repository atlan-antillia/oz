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
 *  WideChartring.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/DrawingArea.h>
#include <oz++/motif/FontSet.h>
#include <oz++/motif/WideCharString.h>
#include <oz++/motif/Color.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<DrawingArea> drawingArea;
  SmartPtr<FontSet>     fontSet;

  void    expose(Action& event)
  {
    DC dc(drawingArea);
    dc.setFontSet(fontSet);
    XRectangle r = {20, 10, 400, 80};
    dc.setClipRectangle(0, 0, r, 0);
    Color color(this);
    Pixel blue = color.allocNamedColor("blue");
    Pixel red = color.allocNamedColor("red");
    Pixel green = color.allocNamedColor("green");
    dc.setBackground(blue);
    dc.setForeground(red);

    WideCharString ss(L"Clipped WideChar string ワイド文字列 1234567890");
    ss.move(24, 40);
    ss.drawImage(&dc);

    WideCharString s(L"Nonclipped WideChar string ワイド文字列 1234567890");
    DC d(drawingArea);
    d.setFontSet(fontSet);
    d.setBackground(green);
    s.move(24, 100);
    s.drawImage(&d);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    fontSet = new FontSet(this,  "-*-*-medium-r-normal--24-*-*"); 
    drawingArea = new DrawingArea(this, "", ar);

    drawingArea -> addCallback(XmNexposeCallback, this,
      (Callback)&MainView::expose, NULL);
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
    args.set(XmNgeometry, "500x200");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);  
  } catch (int ex) {
  }
  
}
