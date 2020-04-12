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
 *  FTDrawingArea.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/xft/FTFont.h>
#include <oz++/xft/FTDrawingArea.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  class SimpleDrawingArea :FTDrawingArea {
  private:
    SmartPtr<FTFont>  font20;
    SmartPtr<FTFont>  font30;
    SmartPtr<FTFont>  font18;
  public:
    virtual void expose(Action& action)
    {
       FTDC ftdc(getWidget());
       DC dc(this);

       Color color(this);
       Pixel pink = color.allocNamedColor("lightpink");
       dc.setForeground(pink);

       dc.fillRectangle(10, 10, 700, 140);
       ftdc.setFont(font18);
       ftdc.drawStringUtf8(20, 30, "Hello world: こんにちは世界", "red");

       ftdc.setFont(font20);
       ftdc.drawStringUtf8(20, 60, "Goodbye world: さようなら世界", "darkgreen");
       ftdc.setFont(font30);
       ftdc.drawStringUtf8(20, 100, "The Force Awakens:フォースの覚醒", "blue");

       Pixel red = color.allocNamedColor("red");
       dc.setForeground(red);
       dc.fillRectangle(20, 150, 100, 60); 
       Pixel navy = color.allocNamedColor("navy");
       dc.setForeground(navy);
       dc.fillEllipse(120, 150, 100, 60); 
       for (int i = 0; i<50; i++) {
         dc.drawRectangle(220, 150, 10+ 3*i, 10+ 3*i);
       }
  
       Pixel green = color.allocNamedColor("darkgreen");
       dc.setForeground(green);
       for (int i = 0; i<50; i++) {
         dc.drawEllipse(420, 150, 10+ 3*i, 10+ 3*i);
       }
    }

    public:
      SimpleDrawingArea(View* parent, const char* name, Args& args)
      :FTDrawingArea(parent, name, args)
    {
      Args ar;
      ar.reset();
      ar.set(XftNfamily, "VL ゴシック");
      ar.set(XftNsize, 18.0);
      ar.set(XftNslant, 1);
      font18 = new FTFont(getDisplay(), ar);

      ar.reset();
      ar.set(XftNfamily, "Times-12");
      ar.set(XftNsize, 20.0);
      font20 = new FTFont(getDisplay(), ar);

      ar.reset();
      ar.set(XftNfamily, "Times-12");
      ar.set(XftNsize, 30.0);
      ar.set(XftNslant, 1);
      font30 = new FTFont(getDisplay(), ar);
    }

    ~SimpleDrawingArea()
    {
    }
  };

private:
  SmartPtr<SimpleDrawingArea> drawingArea;

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    drawingArea = new SimpleDrawingArea(this, "", ar);
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
    args.set(XmNgeometry, "760x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

