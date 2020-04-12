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
 *  BufferedGradientColoredCube.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLColoredCube.h>
#include <oz++/openglarb/OpenGLBufferedShape.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC gc;
    SmartPtr<OpenGLColoredCube>     cube1;
    SmartPtr<OpenGLColoredCube>     cube2;
    SmartPtr<OpenGLBufferedShape>   shape1;
    SmartPtr<OpenGLBufferedShape>   shape2;
    
  public:
  virtual void display()
  {
    if (shape1 && shape2) { 
      gc.matrixMode(GL_PROJECTION);
      gc.loadIdentity();
      gc.perspective(16.0, (double)width() / (double)height(), 0.5, 100.0);

      gc.matrixMode(GL_MODELVIEW);
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();
      gc.clearColor(1.0, 1.0, 1.0, 1.0);

      try {
        //Draw the first shape1.
        gc.pushMatrix();
          gc.translate(-1.5f,0.0f,-1.0f);  
          gc.lookAt(2.0, 8.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
          shape1->draw(&gc);
        gc.popMatrix();

        //Draw the second shape2.
        gc.pushMatrix();
          gc.translate(1.8f,0.0f,-4.0f);  
          gc.lookAt(-4.0, -16.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
          shape2->draw(&gc);
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
    gc.enable( GL_DEPTH_TEST );

    try {
      cube1 = new OpenGLColoredCube();
      cube2 = new OpenGLColoredCube();
      
      Color3<GLfloat> colorsForEachVertex1[] = {
        //face 1:
        { 1.0f, 0.0f, 0.0f}, 
        { 0.0f, 1.0f, 0.0f}, 
        { 0.0f, 0.0f, 1.0f}, 
        { 1.0f, 1.0f, 0.0f},
        
        //face 2:
        { 1.0f, 0.0f, 0.0f}, 
        { 0.0f, 1.0f, 0.0f}, 
        { 0.0f, 0.0f, 1.0f}, 
        { 1.0f, 1.0f, 0.0f},
        
        //face 3:
        { 1.0f, 0.0f, 0.0f}, 
        { 0.0f, 1.0f, 0.0f}, 
        { 0.0f, 0.0f, 1.0f}, 
        { 1.0f, 1.0f, 0.0f},
        
        //face 4:
        { 1.0f, 0.0f, 0.0f}, 
        { 0.0f, 1.0f, 0.0f}, 
        { 0.0f, 0.0f, 1.0f}, 
        { 1.0f, 1.0f, 0.0f},
        
        //face 5:
        { 1.0f, 0.0f, 0.0f}, 
        { 0.0f, 1.0f, 0.0f}, 
        { 0.0f, 0.0f, 1.0f}, 
        { 1.0f, 1.0f, 0.0f},
        
        //face 6:
        { 1.0f, 0.0f, 0.0f}, 
        { 0.0f, 1.0f, 0.0f}, 
        { 0.0f, 0.0f, 1.0f}, 
        { 1.0f, 1.0f, 0.0f},
      };
      
      cube1->setVerticesColors(colorsForEachVertex1, SizeOf(colorsForEachVertex1));

      Color3<GLfloat> colorsForEachVertex2[] = {
        //face 1:
        { 0.0f, 0.2f, 0.4f}, 
        { 0.4f, 0.6f, 0.8f}, 
        { 0.8f, 0.6f, 0.4f}, 
        { 0.4f, 0.2f, 0.0f},
        
        //face 2:
        { 0.0f, 0.2f, 0.4f}, 
        { 0.4f, 0.6f, 0.8f}, 
        { 0.8f, 0.6f, 0.4f}, 
        { 0.4f, 0.2f, 0.0f},
        
        //face 3:
        { 0.0f, 0.2f, 0.4f}, 
        { 0.4f, 0.6f, 0.8f}, 
        { 0.8f, 0.6f, 0.4f}, 
        { 0.4f, 0.2f, 0.0f},
        
        //face 4:
        { 0.0f, 0.2f, 0.4f}, 
        { 0.4f, 0.6f, 0.8f}, 
        { 0.8f, 0.6f, 0.4f}, 
        { 0.4f, 0.2f, 0.0f},
        
        //face 5:
        { 0.0f, 0.2f, 0.4f}, 
        { 0.4f, 0.6f, 0.8f}, 
        { 0.8f, 0.6f, 0.4f}, 
        { 0.4f, 0.2f, 0.0f},
        
        //face 6:
        { 0.0f, 0.2f, 0.4f}, 
        { 0.4f, 0.6f, 0.8f}, 
        { 0.8f, 0.6f, 0.4f}, 
        { 0.4f, 0.2f, 0.0f},
      };
      cube2->setVerticesColors(colorsForEachVertex2, SizeOf(colorsForEachVertex2));
           
      shape1 = new OpenGLBufferedShape(cube1);
      shape2 = new OpenGLBufferedShape(cube2);
   
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

