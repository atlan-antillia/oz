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
 *  TriangleMeshedXZPlane.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLMeshedXZPlane.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC  gc;
    SmartPtr<OpenGLMeshedXZPlane<GL_TRIANGLE_STRIP, 6, 10> > xzPlane;
    
  public:
    virtual void display()
    {
      setPerspective();

      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //gc->scale(10.0f, 10.0f, 10.0f);

      gc.translate(0.0f, 0.5f, -1.0f);  
      gc.rotate(0.0f, 0.0f, 0.0f, 0.0f);
      gc.lookAt(4.0, 4.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

      gc.polygonMode(GL_FRONT_AND_BACK,GL_LINE);
        
      xzPlane -> draw(&gc);
    }
 
    virtual void resize(int w, int h)
    {
     if (w == 0 || h == 0) {
        return;
      }
      setPerspective();
    }

    virtual void initialize()
    {
      //Write your own initialization procedure
      xzPlane = new OpenGLMeshedXZPlane<GL_TRIANGLE_STRIP, 6, 10>(0.5f);
    }

  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args)
    {
    } 
    
    ~SimpleView()
    {
    }
  };
  //Inner class ends
  
private:
  SmartPtr<SimpleView>  view;

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, args) 
  {
    Args ar;
    view = new SimpleView(this, "", ar);
  }
  ~MainView()
  {
  }
};

}

//
int main(int argc, char** argv) {
  try {
    const char*  name = argv[0];
    OpenGLApplication applet(name, argc, argv);

    Args args;
    args.set(XmNwidth, 500);
    args.set(XmNheight,500);

    MainView view(applet, name, args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

