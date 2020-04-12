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
 *  Cube.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLClientState.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class start
  class SimpleView: public OpenGLView {
    OpenGLGC gc;
 
  public:
    virtual void display()
    {
      static GLfloat points[] ={
        0.0, 0.0, 0.0, 
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0, 
        0.0, 0.0,-1.0, 
        1.0, 0.0,-1.0,
        1.0, 1.0,-1.0,
        0.0, 1.0,-1.0,
      };

      static GLuint index[]={
        0, 1, 2, 3,
        1, 5, 6, 2,
        5, 4, 7, 6,
        4, 0, 3, 7,
        4, 5, 1, 0,
        3, 2, 6, 7
      };
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      gc.loadIdentity();
      gc.lookAt(2.0, 4.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
      OpenGLClientState client(GL_VERTEX_ARRAY);

      gc.polygonMode(GL_FRONT_AND_BACK,GL_LINE);
      //gc.polygonMode(GL_FRONT_AND_BACK,GL_FILL);
      gc.color(0.0, 0.0, 1.0);
      gc.lineWidth(2.0);
      gc.vertexPointer(3, GL_FLOAT, 0, points); 
      gc.drawElements(GL_QUADS, SizeOf(index), GL_UNSIGNED_INT, index);
    }
 
    virtual void resize(int w, int h)
    {
       if (w == 0 || h == 0) {
          return;
       }
       gc.matrixMode(GL_PROJECTION);
       gc.loadIdentity();
       gc.perspective(16.0, (double)w / (double)h, 0.5, 40.0); 

       gc.matrixMode(GL_MODELVIEW);
    }

    virtual void initialize()
    {
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
  //Inner class end
	
private:
  SimpleView*  view;

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, args) 
  {
    Args ar;
    //ar.set(XmNvisualInfo, (XtArgVal)getVisualInfo());
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    view = new SimpleView(this, "", ar);
  }

  ~MainView()
  {
    delete view;
  }
};
}

//
int main(int argc, char** argv) {
  try {
    const char*  appclass = "Cube";
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

