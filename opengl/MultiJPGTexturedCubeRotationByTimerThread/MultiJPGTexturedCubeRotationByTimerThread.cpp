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
 *  MultiJPGTexturedCubeRotationByTimerThread.cpp
 *
 *****************************************************************************/

//2016/07/25
//2017/01/15 Updated display method.

#include <oz++/Mutex.h>
#include <oz++/motif/RenderingTimer.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLMultiTexturedCube.h>


namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.

  class SimpleView: public OpenGLView {
  private:
    OpenGLGC        gc;
    SmartPtr<OpenGLMultiTexturedCube> cube;
    
    Mutex            mutex;
    SmartPtr<RenderingTimer>  timerThread;
    float              angle;
    
  public:
    virtual void display()
    {
      mutex.lock();

      if (cube) {
       //Set a proportional perspective.
       gc.matrixMode(GL_PROJECTION);
       gc.loadIdentity();
       gc.perspective(16.0, (double)width() / (double)height(), 0.5, 100.0); 

       gc.matrixMode(GL_MODELVIEW);
        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gc.loadIdentity();

        gc.translate(0.0f,0.0f,-1.0f); 
        gc.lookAt(2.0, 8.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

        gc.rotate(angle, 0.0, 1.0, 0.0);
        angle -= 0.4f;

        cube -> draw(&gc);
      }
      mutex.unlock();
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
      gc.enable(GL_DEPTH_TEST);
      gc.enable(GL_TEXTURE_2D);
      try {
        static const char* jpgFiles[] = {
           "./images/1.jpg", "./images/2.jpg", "./images/3.jpg", 
           "./images/4.jpg", "./images/5.jpg", "./images/6.jpg", 
         };
        
        cube = new OpenGLMultiTexturedCube(6, jpgFiles);
        
      } catch (Exception& ex) {
        caught(ex);
      }
     //Create a renderingTimerThread
      timerThread = new RenderingTimer(this, 30);
      timerThread -> start();
    }


  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args),
    angle(0.0f)
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
    XInitThreads();

    const char*  name = argv[0];
    OpenGLApplication applet(name, argc, argv);

    Args args;
    args.set(XmNwidth,  480);
    args.set(XmNheight, 480);

    MainView view(applet, name, args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

