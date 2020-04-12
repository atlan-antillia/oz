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
 *  BufferedColoredRegularIcosahedron.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>

#include <oz++/opengl/OpenGLColoredRegularIcosahedron.h>

#include <oz++/openglarb/OpenGLBufferedShape.h>


namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC gc;
    SmartPtr<OpenGLColoredRegularIcosahedron>   hedron;
    SmartPtr<OpenGLBufferedShape>   shape;
 
  public:
  virtual void display()
  {
    if (shape) {
      gc.matrixMode(GL_PROJECTION);
      gc.loadIdentity();
      gc.perspective(16.0, (double)width() / (double)height(), 0.5, 100.0);

      gc.matrixMode(GL_MODELVIEW); 
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();
      gc.clearColor(0.8, 0.8, 0.8, 1.0);

      try {
        //Draw the first shape1.
        gc.pushMatrix();
          gc.translate(0.0f,0.0f,1.0f);  
          gc.lookAt(4.0, 12.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
          shape->draw(&gc);
        gc.popMatrix();

      } catch (Exception& ex) {
        caught(ex); 
      }
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
    gc.enable( GL_DEPTH_TEST);

    try {
      Color3<GLfloat> vertexColors[] = {
        
        { 1.0f, 0.0f, 0.0f},
        { 0.0f, 1.0f, 0.0f}, 
        { 0.0f, 0.0f, 1.0f}, 
        { 1.0f, 1.0f, 0.0f},
        { 0.0f, 1.0f, 1.0f},
        { 1.0f, 0.0f, 1.0f},
        { 1.0f, 0.0f, 0.0f},
        { 0.0f, 1.0f, 0.0f}, 
        { 0.0f, 0.0f, 1.0f}, 
        { 1.0f, 1.0f, 0.0f},
        { 0.0f, 1.0f, 1.0f},
        { 1.0f, 0.0f, 1.0f},        
      };
      hedron = new OpenGLColoredRegularIcosahedron(vertexColors, SizeOf(vertexColors));
      
      shape = new OpenGLBufferedShape(hedron);
    
    } catch (Exception& ex) {
      caught(ex);
    }
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

