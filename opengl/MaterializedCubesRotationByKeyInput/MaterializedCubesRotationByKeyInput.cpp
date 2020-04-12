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
 *  MaterializedCubesRotationByKeyInput.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>
#include <oz++/opengl/OpenGLSolidCube.h>
#include <oz++/opengl/OpenGLWireCube.h>

namespace OZ {

class MainView :public OpenGLMainView {
private:
  //Inner class starts
  class SimpleView: public OpenGLView {
  private:
    static const int        CUBES=4;
    OpenGLGC                gc;
    SmartPtr<OpenGLGeometry>  cubes[CUBES];
    float                   angle; 
  public:
   virtual void display()
    {
      if (cubes[0]) {
        setPerspective();

        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gc.lookAt(4.0, 8.0, 14.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        gc.clearColor(0.0, 0.0, 0.0, 1.0);
        gc.enable(GL_CULL_FACE); 
        gc.enable(GL_LIGHTING);

        OpenGLLight light(GL_LIGHT0);
        GLfloat lightPosition[] = {10, 10, 10, 1.0};  
        light.position(lightPosition);

        for (int i = 0; i<CUBES; i++) {
          gc.pushMatrix();
          gc.rotate(angle, 0.0f, 1.0f, 0.0f);
          cubes[i]->draw(&gc, -2.0f+1.3f*i, 0.5f,  0.0f+ 0.3f*i);
          gc.popMatrix();
        }
      }
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
      Color4  black         = { 0.0, 0.0, 0.0, 1.0 };
      Color4  ambient       = { 0.5, 0.5, 0.5, 1.0 };
      Color4  diffuse       = { 0.2, 0.4, 0.8, 1.0 };
      Color4  specular      = { 1.0, 1.0, 1.0, 1.0 };
      Color4  emission      = { 0.8, 0.0, 0.0, 0.0 };
      GLfloat lowShining    = { 10.0 };
      GLfloat highShining   = {100.0 };

      OpenGLMateria mat1(GL_FRONT, ambient, diffuse, specular, emission, lowShining);
      OpenGLMateria mat2(GL_FRONT, black,   diffuse, specular, black,    lowShining);
      OpenGLMateria mat3(GL_FRONT, black,   diffuse, black,    emission, highShining);
      OpenGLMateria mat4(GL_FRONT, ambient, diffuse, specular, black,    highShining);
      OpenGLMateria materias[] = {mat1, mat2, mat3, mat4};
      for (int i = 0; i<CUBES; i++) {
        if (i%2 ==0) {
          cubes[i] = new OpenGLWireCube(materias[i], 0.5f+0.2f*i);
        } else {
          cubes[i] = new OpenGLSolidCube(materias[i], 0.5f+0.2f*i);
        }
      }
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
    
  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args),
    angle(2.0f)
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
    glutInit(&argc, argv);
    
    const char*  name = argv[0];
    OpenGLApplication applet(name, argc, argv);

    Args args;
    args.set(XmNwidth,  500);
    args.set(XmNheight, 500);
    MainView view(applet, name, args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

