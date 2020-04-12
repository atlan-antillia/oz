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
 *  Texture.cpp
 *
 *****************************************************************************/

//2015/08/15

//If pngconf.h caused a compile error, 
// please see the following web site:
// https://bugs.launchpad.net/ubuntu/+source/libpng/+bug/218409 
//2016/12/27 Added the following line.
#define PNG_SKIP_SETJMP_CHECK

#include <oz++/xft/FTDC.h>
#include <oz++/xft/FTFont.h>
#include <oz++/motif/Image.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLTexture2D.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC        gc;
    OpenGLTexture2D texture;
    SmartPtr<Image>           image;
    SmartPtr<FTFont>          font20;
    SmartPtr<FTFont>          font24;
    SmartPtr<FTFont>          font40;
    Colormap         colormap;
    
  private:
    void imageToTexture()
    {
      FTDC ftdc(getWidget());
     
      unsigned int width  = 0; 
      unsigned int height = 0;
      get(XmNwidth, (XtArgVal)&width);
      get(XmNheight, (XtArgVal)&height);
      printf("width=%d height=%d\n", width, height);

      DC dc(this);

      Color color(this, colormap);
      Pixel white =  color.allocNamedColor("white");
      dc.setForeground(white);
      dc.fillRectangle(0, 0, width, height);

      Pixel red = color.allocNamedColor("red");
      dc.setForeground(red);
      dc.fillRectangle(0, 0, 40, 40);

      Pixel green = color.allocNamedColor("green");
      dc.setForeground(green);
      dc.fillEllipse(20, 40, 50, 50);
      Pixel blue = color.allocNamedColor("blue");
      dc.setForeground(blue);
      dc.fillEllipse(40, 80, 90, 60);

      ftdc.setFont(font20);
      ftdc.drawStringUtf8(10, 80, "The Red Mars Attacks! 赤い火星の襲撃", "red");
      ftdc.setFont(font24);
      ftdc.drawStringUtf8(10, 140, "The DarkGreen Man Strikes Back", "darkgreen");

      ftdc.setFont(font40);
      ftdc.drawStringUtf8(10, 200, "The Navy Awakens 海軍の覚醒", "navy");
      flush();

      Display* display = getDisplay();
      Window  window=  getWindow();

      image = new Image(display, window,
          0, 0, width, height, AllPlanes, ZPixmap);
      if (image) {
        XImage* ximage = image -> get();

        if (ximage) {
          printf("XImage depth=%d\n", ximage->depth);
          texture.image(0, GL_RGBA, width, height, 0,
             GL_BGRA,  //GL_RGBA, 
             GL_UNSIGNED_BYTE, (void*)(ximage->data));
        }
      }
    }

  public:
    virtual void display()
    {
      if (image == NULL) {
         imageToTexture();
      }

      gc.clearColor(1.0, 1.0, 1.0, 1.0);
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.rotate(1.0, 0.5, 1.0, 0.25);


      gc.begin(GL_QUADS);
      texture.coord(0.0, 0.0); 
      gc.vertex(-1.0,  1.0, 0.0);

      texture.coord(1.0, 0.0); 
      gc.vertex( 1.0,  1.0, 0.0);

      texture.coord(1.0, 1.0); 
      gc.vertex( 1.0, -1.0, 0.0);

      texture.coord(0.0, 1.0); 
      gc.vertex(-1.0, -1.0, 0.0);
      gc.end();
      flush();
    }
 
    virtual void resize(int w, int h)
    {
      if (w == 0 || h == 0) {
        return;
      }
      gc.matrixMode(GL_PROJECTION);
      gc.loadIdentity();
      gc.ortho(-1.25, 1.25, -1.25, 1.25, 1., 20.);

      gc.matrixMode(GL_MODELVIEW);
      gc.loadIdentity();
      gc.lookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
    }

    virtual void initialize()
    {
      gc.enable(GL_DEPTH_TEST);
      gc.enable(GL_TEXTURE_2D);

      texture.bind();

      texture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      texture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      texture.env(GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    }

  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args)
    {
      if (args.has(XmNcolormap)) {
        colormap = (Colormap)args.get(XmNcolormap);
      }

      image   = NULL;

      Args ar;
      ar.reset();
      ar.set(XftNfamily, "Times-12");
      ar.set(XftNsize, 20.0);
      ar.set(XftNslant, 1);
      font20 = new FTFont(getDisplay(), ar);

      ar.reset();
      ar.set(XftNfamily, "FreeSerif");
      ar.set(XftNsize, 24.0);
      ar.set(XftNslant, 1);
      font24 = new FTFont(getDisplay(), ar);


      ar.reset();
      ar.set(XftNfamily, "Times-12");
      ar.set(XftNsize, 40.0);
      ar.set(XftNslant, 1);
      font40 = new FTFont(getDisplay(), ar);
    } 

    ~SimpleView()
    {
    }
  };
  //Inner class ends.
	
private:
  SmartPtr<SimpleView>  view;

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, args)
       //args.set(XmNopenGLBufferType, OpenGL_DOUBLE_BUFFER)) 
       //args.set(XmNopenGLBufferType, OpenGL_SINGLE_BUFFER)) 
  {
    Args ar;
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    ar.set(XmNcolormap,       getColormap());
    view = new SimpleView(this, "", ar);
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
    OpenGLApplication applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "800x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

