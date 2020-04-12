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
 *  TextureSubImageRotationByKey.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLTexture2D.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  class SimpleView :public OpenGLView {
    OpenGLGC        gc;
    OpenGLTexture2D texture;
    float            angle;

  public:
    virtual void display()
    {
      gc.clearColor(1.0, 1.0, 1.0, 1.0);
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.color(1.0, 1.0, 1.0);

      gc.rotate(5.0, 0.5, 1.0+angle, 0.25+angle);

      glNormal3d(0.0, 0.0, 1.0);
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

    virtual void keyPress(Event& event)
    {
      KeySym k = event.getKeySym();
      if (k == XK_Left) {
        angle -= 0.5f;
        sendExposeEvent();
      }
      if (k == XK_Right) {
        angle += 0.5f;
        sendExposeEvent();
      }
    }

    virtual void keyRelease(Event& event)
    {
      //Do nothing here
    }

    virtual void initialize()
    {
      gc.enable(GL_DEPTH_TEST);
      gc.enable(GL_TEXTURE_2D);

      createTexture("./images/Flower.png");
      //createTexture("./images/Dragonfly.png");

      setSubImage("./images/Nanten.png", 0, 20, 20);
    }

  public:
    void createTexture(const char* filename)
    {
      texture.bind();
      texture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      texture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      texture.env(GL_TEXTURE_ENV_MODE, GL_MODULATE); 

      try {
        texture.imageFromPNGFile(filename);

      } catch (Exception& ex) {
        caught(ex);
      }
    }

  public:
    void setSubImage(const char* filename, GLint level, GLint offset, GLint yoffset)
    {
       try {
        texture.subImageFromPNGFile(filename, level, offset, yoffset);

      } catch (Exception& ex) {
        caught(ex);
      }
    }

  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args),
    angle(0.5f)
    {
    }
 
  ~SimpleView()
    {
    }
  };
  //Inner class ends.
	
private:
  SmartPtr<SimpleView> view;
	
public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, args)
  {
    Args ar;
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
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
    args.set(XmNgeometry, "400x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}
