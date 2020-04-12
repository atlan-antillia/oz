/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  LightedCubes.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>

#include <oz++/opengl/OpenGLIndexedCube.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts
  class SimpleView: public OpenGLView {
  private:
    static const int   CUBES = 4;
    OpenGLGC           gc;
    SmartPtr<OpenGLIndexedCube> cubes[CUBES];
    
  public:
  virtual void display()
  {
    if (cubes[0]) { 
      gc.frustum(1 , -1 , -1 , 1 , 1 , 10);

      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();
      gc.matrixMode(GL_MODELVIEW);

      gc.clearColor(0.2, 0.0, 0.2, 1.0);      
      gc.translate(-0.5f, -0.5f, 0.0f); 
      gc.lookAt(3.0f, 8.0f, 14.0f, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

      try {
        gc.enable(GL_LIGHTING);
 
        OpenGLLight light(GL_LIGHT0);
        light.position(0.0f, 6.0f, 2.0f, 0.0);  
          
        OpenGLMaterial material(GL_FRONT);
        material.diffuse (0.2f, 1.0f, 0.2f, 0.0f);
        material.specular(1.0f, 1.0f, 1.0f, 0.0f);
        material.shininess(100.0f);
  
        for (int i = 0; i<CUBES; i++) {
          cubes[i]->draw(&gc); 
        }
       
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
      gc.perspective(20.0, (double)w / (double)h, 0.5, 100.0); 

      gc.matrixMode(GL_MODELVIEW);
    }

    virtual void initialize()
    {
      for (int i = 0; i<CUBES; i++) {
        cubes[i] = new OpenGLIndexedCube(1.0f, -2.0f+1.2f*i, -2.0f+1.2f*i, 1.0f-0.5f*i);
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
int main(int argc, char** argv) 
{
  try {
    const char*  name = argv[0];
    OpenGLApplication applet(name, argc, argv);

    Args args;
    args.set(XmNwidth,  400);
    args.set(XmNheight, 400);
    MainView view(applet, name, args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

