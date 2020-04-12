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
 *  MultiPNGTexturedBoxRotationByKeyInput.cpp
 *
 *****************************************************************************/

//2016/07/28
//2017/01/16 Updated display method.

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLMultiTexturedBox.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.

  class SimpleView: public OpenGLView {
  private:
    OpenGLGC        gc;
    SmartPtr<OpenGLMultiTexturedBox> box;
    float  angle;
    
  public:
    virtual void display()
    {
      if (box) {    
       //Set a proportional perspective.
       gc.matrixMode(GL_PROJECTION);
       gc.loadIdentity();
       gc.perspective(16.0, (double)width() / (double)height(), 0.5, 100.0); 

       gc.matrixMode(GL_MODELVIEW);
        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gc.clearColor(1.0f, 1.0f, 1.0f, 1.0f);

        gc.loadIdentity();
        gc.translate(0.0f,0.0f,-1.0f); 
        gc.lookAt(2.0, 4.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

        //Rotation around y-axis.
        gc.rotate(angle, 0.0, 1.0, 0.0);

        box -> draw(&gc);
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

    virtual void initialize()
    {
      gc.enable(GL_DEPTH_TEST);
      gc.enable(GL_TEXTURE_2D);
      try {
        static const char* pngFiles[] = {
           "./images/1.png", "./images/2.png", "./images/3.png", 
           "./images/4.png", "./images/5.png", "./images/6.png", 
         };
        
        box = new OpenGLMultiTexturedBox(1.0, 0.8f, 0.8f, 6, pngFiles);
        
      } catch (Exception& ex) {
        caught(ex);
      }
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

