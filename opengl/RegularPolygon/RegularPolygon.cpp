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
 *  RegularPolygon.cpp
 *
 *****************************************************************************/

//2017/01/16 Updated display method.

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLRegularPolygon.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC      gc;
    SmartPtr<OpenGLRegularPolygon>    polygon;
    SmartPtr<OpenGLRegularPolygon>    pentagon;
    
  public:
    virtual void display()
    {
      if (polygon && pentagon) {
       //Set a proportional perspective.
       gc.matrixMode(GL_PROJECTION);
       gc.loadIdentity();
       gc.perspective(16.0, (double)width() / (double)height(), 0.5, 100.0); 

       gc.matrixMode(GL_MODELVIEW);  
        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gc.loadIdentity();
 
        gc.pushMatrix();
        gc.lookAt(2.0, 4.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        gc.translate(-1.0, -1.0, 0.0);
        gc.polygonMode(GL_FRONT_AND_BACK, GL_FILL);//GL_LINE);
        gc.color(0.0, 1.0, 0.0);
        //Draw a polygon
        polygon->draw(&gc);
        gc.popMatrix();
      
        gc.pushMatrix();
        gc.lookAt(2.0, 4.0, 14.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        gc. translate(1.0, 1.0, 1.5);
        gc.polygonMode(GL_FRONT_AND_BACK, GL_LINE);
        gc.color(1.0, 0.0, 0.0);
        //Draw a pentagon.
        pentagon->draw(&gc);
        gc.popMatrix();
      }
    }
 
    virtual void resize(int w, int h)
    {
      if (w == 0 || h == 0) {
          return;
      }
      gc.matrixMode(GL_PROJECTION);
      gc.loadIdentity();
        
      gc.perspective(20.0, (double)w / (double)h, 0.5, 40.0); 

      gc.matrixMode(GL_MODELVIEW);
    }

    virtual void initialize()
    {
      polygon  = new OpenGLRegularPolygon(8);
      pentagon = new OpenGLRegularPolygon(5);
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

