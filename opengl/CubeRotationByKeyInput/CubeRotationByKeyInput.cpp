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
 *  CubeRotationByKeyInput.cpp
 *
 *****************************************************************************/


#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>

#include <oz++/opengl/OpenGLClientState.h>


namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC gc;
    float    angle;
    
  public:
    virtual void display()
    {
      static GLfloat vertices[] = {
      -0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,

      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      };
    
      //Colors(RGBAs) for the 5 vertices
      static GLfloat colors[] = { 
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
      };
  
      //Indices for the 4 triangles.
      static GLint indices[] = { 
        0, 1, 2,  2, 3, 0,
        3, 2, 6,  6, 7, 3,
        7, 6, 5,  5, 4, 7,
        4, 5, 1,  1, 0, 4,
        4, 0, 3,  3, 7, 4,
        1, 5, 6,  6, 2, 1,
      };
      gc.matrixMode(GL_PROJECTION);
      gc.loadIdentity();
      gc.perspective(16.0, (double)width() / (double)height(), 0.5, 100.0);

      gc.matrixMode(GL_MODELVIEW);
    
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();
      gc.translate(0.0f,0.0f,-1.0f); 
      gc.lookAt(2.0, 4.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

      //Rotation around y-axis.
      gc.rotate(angle, 0.0, 1.0, 0.0);
      
      try {        
          OpenGLClientState state1(GL_VERTEX_ARRAY); 
          OpenGLClientState state2(GL_COLOR_ARRAY);

          gc.vertexPointer(3, GL_FLOAT, 0, vertices);
          gc.colorPointer (4, GL_FLOAT, 0, colors);
                 
          gc.drawElements(GL_TRIANGLES, SizeOf(indices), GL_UNSIGNED_INT, indices);
          gc.flush();
      
      } catch (Exception& ex) {
        caught(ex); 
      }
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

    virtual void keyPress(Event& event)
    {
      KeySym k = event.getKeySym();
      if (k == XK_Left) {
        angle -= 1.0f;
        sendExposeEvent();
      } 
      if (k == XK_Right) {
        angle += 1.0f;
        sendExposeEvent();
      } 
    }

    virtual void keyRelease(Event& event)
    {
      //Do nothing here
    }
    virtual void initialize()
    {
      gc.enable( GL_DEPTH_TEST );
    }

    
  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args),
    angle(0.0)
    {
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
    const char*  name = argv[0];
    OpenGLApplication applet(name, argc, argv);

    Args args;
    args.set(XmNwidth, 400);
    args.set(XmNheight, 300);
    
    MainView view(applet, name, args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

