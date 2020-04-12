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
 *  Triangle.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLList.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {

  private:
    OpenGLGC gc;
    GLuint LISTID;
 
  public:
    virtual void display()
    {
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      gc.loadIdentity();
      gc.pushMatrix();
       
      OpenGLList list(LISTID);
      list.call();
      gc.popMatrix();
    }
 
    virtual void resize(int w, int h)
    {
      //Write your own resize procedure
    }

    void createList(GLuint id)
    {
      OpenGLList list(id, GL_COMPILE);

      gc.begin(GL_QUADS);

      gc.translate(-1.5f, 0.0f, -6.0f);  
      // Front
      gc.redVertex( 0.0, 1.0, 0.0);
      gc.greenVertex(-1.0, -1.0, 1.0);
      gc.blueVertex(1.0, -1.0, 1.0);

      // Right
      gc.redVertex(0.0, 1.0, 0.0);
      gc.blueVertex(1.0, -1.0, 1.0);
      gc.greenVertex(1.0, -1.0, -1.0);

      // Back
      gc.redVertex(0.0, 1.0, 0.0);
      gc.greenVertex(1.0, -1.0, -1.0);
      gc.blueVertex(-1.0, -1.0, -1.0);

      // Left
      gc.redVertex( 0.0, 1.0, 0.0);
      gc.blueVertex(-1.0,-1.0,-1.0);
      gc.greenVertex(-1.0,-1.0, 1.0);

      gc.end();
    }

    virtual void initialize()
    {
      try {
        createList(LISTID);
      } catch (Exception& ex) {
        caught(ex);
      }
    }

  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args),
     LISTID(1)
    {
    } 
  };
  //Inner class ends.
	
private:
  SmartPtr<SimpleView>  view;

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

