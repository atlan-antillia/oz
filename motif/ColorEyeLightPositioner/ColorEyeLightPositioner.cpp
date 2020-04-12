/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ColorEyeLightPositioner.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/ColorEyeLightPositioner.h>
#include <oz++/motif/Canvas.h>
#include <oz++/motif/DC.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  class SimpleView :public BulletinBoard {
  private:
    ColorEyeLightPositioner* positioner;
    SmartPtr<Label>          eyePosition;
    SmartPtr<Label>          lightPosition;

  public:
    void expose(Event& ev) 
    {
      DC dc(this);
      clear();
      if (positioner) {
        int r, g, b;
        positioner->getColorPosition(r, g, b);
      
        Color color(this);
        Pixel pixel = color.color256(r, g, b);
        dc.setForeground(pixel);
        dc.fillRectangle(10, 10, 100, 100); 
      }
     
      if (positioner) {
        int x, y, z;
        positioner->getEyePosition(x, y, z);
        char string[128];
        sprintf(string, "EyePositioner(%2d, %2d, %2d)", x, y, z);
        CompoundString cs(string);
        eyePosition->set(XmNlabelString, cs);
      }
      if (positioner) {
        int x, y, z;
        positioner->getLightPosition(x, y, z);
        char string[128];
        sprintf(string, "LightPositioner(%2d, %2d, %2d)", x, y, z);
        CompoundString cs(string);
        lightPosition->set(XmNlabelString, cs);
      }
    }

  public:
    SimpleView(View* parent, const char* name, Args& args)
    :BulletinBoard(parent, name, args)
    {
      Color color(this);
      Pixel white = color.whitePixel();
      set(XmNbackground, (XtArgVal)white);

      Args ar;
      CompoundString cs("EyePosition");
      ar.set(XmNx, 0);
      ar.set(XmNy, 180);
      ar.set(XmNlabelString, cs);
      ar.set(XmNbackground, (XtArgVal)white);
      eyePosition = new Label(this, "", ar);

      ar.reset();
      CompoundString cs2("LightPosition");
      ar.set(XmNx, 0);
      ar.set(XmNy, 210);
      ar.set(XmNlabelString, cs2);
      ar.set(XmNbackground, (XtArgVal)white);
      lightPosition = new Label(this, "", ar);

      addEventHandler(ExposureMask, this,
        (Handler)&SimpleView::expose, NULL);
    } 
    ~SimpleView()
    {
    }

    void setPositioner(ColorEyeLightPositioner* pos)
    {
      positioner = pos;
    }
  };
  //Inner class ends.

  SmartPtr<BulletinBoard> bboard;
  SmartPtr<SimpleView>   canvas;
  SmartPtr<ColorEyeLightPositioner> positioner;

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    bboard = new BulletinBoard(this, "", ar);

    ar.reset();
    ar.set(XmNx, 0);
    ar.set(XmNy, 10);
    ar.set(XmNwidth, 250);
    ar.set(XmNheight, 260);
    ar.set(XmNnoResize, TRUE);
    ar.set(XmNresizePolicy, XmRESIZE_NONE); 
    canvas = new SimpleView(bboard, "", ar);
    
    ar.reset();
    ar.set(XmNx, 264);
    ar.set(XmNy,  0);
    positioner = new ColorEyeLightPositioner(bboard, "", ar);
    positioner->setColorPosition(10, 128, 255);
    positioner->setEyePosition(-5, 0, 10);
    positioner->setLightPosition(-10, 5, 10);

    positioner->setExposeEventListener(canvas);

    canvas->setPositioner(positioner);
  }
  
  ~MainView() 
  {
  }
};
}

//
int main(int argc, char** argv){

  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "540x360");
    MainView view(applet, argv[0], args);
    view.realize();
    applet.run();
    
  } catch (Exception& ex) {
    
  }
  return 0;
}

