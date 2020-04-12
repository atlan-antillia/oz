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
 *  ScreenCapture.cpp
 *
 *****************************************************************************/

//2018/04/10 Added the following line to avoid INT32 duplicate definition
// in jpmorecfg.h and Xmd.h

#include <X11/Xmd.h>

#include <oz++/motif/Image.h>
#include <oz++/motif/ScreenCapture.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLTexture2D.h>

namespace OZ {

class MainView :public OpenGLMainView {
public:

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC        gc;
    OpenGLTexture2D texture;
    bool             textured; 
    ScreenCapture*   captured; //Shallow copy

  private:
    void imageToTexture()
    {
      XImage* ximage = captured -> getImage() -> get();;

      if (ximage) {
        printf("XImage depth=%d\n", ximage->depth);
        texture.image(0, GL_RGBA, captured ->getWidth(), captured -> getHeight(), 0,
             GL_BGRA,  //
             GL_UNSIGNED_BYTE, (void*)(ximage->data));
      }
    }

  public:
    virtual void display()
    {
      if (textured == false) {
        imageToTexture();
        textured = true;
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
      textured = false;
    } 

    ~SimpleView()
    {
    }

    void set(ScreenCapture* capture)
    {
      captured = capture;
    }
  };
  //Inner class ends.
	
private:
  SmartPtr<SimpleView>  view;

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, 
       args.set(XmNopenGLBufferType, OpenGL_DOUBLE_BUFFER)) 
       //args.set(XmNopenGLBufferType, OpenGL_SINGLE_BUFFER)) 
  {
    Args ar;
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    //ar.set(XmNcolormap,       getColormap());
    view = new SimpleView(this, "", ar);
  }

  ~MainView()
  {
  }

  void set(ScreenCapture* screenCapture)
  {
    view -> set(screenCapture); 
  }
};
}

//
int main(int argc, char** argv) 
{
  try {
    const char*  appclass = argv[0];
    OpenGLApplication applet(appclass, argc, argv);

    //Capture a region of a default root window.    
    ScreenCapture capture(applet);
    capture.capture(0, 0, 600, 400);

    //Create a MainView of the ScreenCature application.
    Args args;
    args.set(XmNgeometry, "600x400+40+40");
    MainView view(applet, argv[0], args);
    view.set(&capture);
    view.realize();
    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

