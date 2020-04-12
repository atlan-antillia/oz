/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  CustomScrolledView.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/CustomScrolledView.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/DC.h>
#include <oz++/motif/RenderTable.h>

namespace OZ {

class MainView : public ApplicationView {

private:
  /////////////////////////////////////////////////////////////////
  //Inner class starts.
  class InnerView :public CustomScrolledView {
  private:
    Pixmap pixmap;
    int pmWidth;
    int pmHeight;

    void configureNotify(Event& event)
    {
      XEvent* xe = event.getXEvent();
      XConfigureEvent* cev = (XConfigureEvent*)xe;
      printf("configureNotify w=%d, h=%d\n", cev->width, cev->height);

      BulletinBoard* workArea = getWorkArea();
 
      uint ww = workArea->width();
      uint wh = workArea->height();
      printf("workArea w=%d, h=%d\n", ww, wh);

      setScrollBarSliderSize(ww, wh);
    }

    virtual Pixmap getPixmap()
    {
      return pixmap; 
    }

    virtual Pixmap createPixmap()
    {
      Args ar;
      ar.set(XmNfontName, "Times-a");
      ar.set(XmNfontSize, 28);
      RenderTable renderTable(this, "", ar);

      Display* dsp = getDisplay();
      pmWidth  = 640;
      pmHeight = 480;
      Color color(this);
      BulletinBoard* workArea = getWorkArea();

      Window window = workArea->getWindow();

      pixmap = XCreatePixmap(dsp, window, pmWidth, pmHeight,
                        DefaultDepth(dsp,0));
      DC dc(dsp, pixmap);
      dc.setForeground(color.color256(0, 40, 40));
      dc.fillRectangle(0, 0, pmWidth, pmHeight);

      dc.setRenderTable(renderTable.get());

      for (int i = 0; i<100; i++) {
        dc.setForeground(color.color256(255, 100+i, i)); 
        dc.drawRectangle(40+i, 20+i, 200, 100);
      }
 
      dc.setForeground(color.color256(0, 255, 0));  //green
      dc.setArcMode(ArcPieSlice);
      dc.fillArc(60, 140, 130, 130, 45*360, 120*360);

      dc.setForeground(color.color256(0, 0, 255));  //blue
      dc.fillArc(260, 140, 130, 130, 45*360, 120*360);

      const char* string = "Is Motif dead?\n Could be.\n"
                           "Don't mind.\nYou have been living\n"
                           "in the world after death.\n";
                           //"You can reach any star.\n";
      dc.setForeground(color.color256(255, 255, 255)); //white
      CompoundString cs(string);
      cs.move(50, 50);
      cs.draw(&dc);

      setHorizScrollBarRange(0, pmWidth);
      setVertScrollBarRange(0,  pmHeight);

      uint ww = workArea->width();
      uint wh = workArea->height();
      setScrollBarSliderSize(ww, wh);
      return pixmap;
    }

  public:
     InnerView(View* parent, const char* name, Args& args)
     :CustomScrolledView(parent, name, args)
     {
        pixmap = None;
     }

     virtual void display()
     {
        Position hpos, vpos;
        getScrollBarPosition(hpos, vpos);

        Pixmap pixmap = getPixmap();         
        if (pixmap == None) {
          pixmap = createPixmap();
        }
        if (pixmap != None) {
          BulletinBoard* dest = getWorkArea();
          Dimension width  = dest->width();
          Dimension height = dest->height(); 
          Pixmap    source = pixmap;
          DC destDC(dest);
          destDC.copyArea(source, 0, 0, width, height, -hpos, -vpos); 
          //You may need to clear something garbage area.
        }
     }
  };
  //Inner class ends.
  /////////////////////////////////////////////////////////////////

  SmartPtr<InnerView>  innerView;

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    innerView = new InnerView(this, "", ar);
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
    args.set(XmNwidth,  500);
    args.set(XmNheight, 400);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
   caught(ex);
  }
  return 0;
}

