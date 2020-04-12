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
 *  Polygon.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC gc;

  public:
    //This method will be called from an expose callback of OpenGLView.
    virtual void display()
    {
      const GLfloat lightPosition[] = { 5.0 , 0.0 , -8.0 , 1.0 };
      const GLfloat lightColor[]    = { 0.5 , 1 , 0.5 , 1 };
      const GLfloat materialColor[] = { 0.2 , 0.6 , 0.2 , 1};
  	
      OpenGLLight light(GL_LIGHT0);
      light.position(lightPosition);
      light.diffuse(lightColor);
      light.enable(); 
    	
      OpenGLMaterial material(GL_FRONT_AND_BACK );
      material.diffuse(materialColor);
    	
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.begin(GL_POLYGON);

      gc.normal(3.0 , 0.0 , -2.0);
      gc.vertex(0.0 , -0.9 , -2.0);
      gc.vertex(3.0 , -0.9 , -7.0);
      gc.vertex(0.0 , 0.9 , -2.0 );

      gc.normal(-3.0 , 0.0 , -2.0);
      gc.vertex(0.0 , -0.9 , -2.0);
      gc.vertex(-3.0, -0.9 , -7.0);
      gc.vertex(0.0, 0.9 , -2.0);

      gc.end();
    }
 
    //This method will be called from a structureNotiy event handler of OpenGLView.
    virtual void resize(int widht, int height)
    {
       //Write your own resize procedure.
    }

    //This method will be called from a mapNotify event handler of OpenGLView.
    virtual void initialize()
    {
      gc.enable(GL_LIGHTING);
      gc.matrixMode(GL_PROJECTION);
      gc.loadIdentity();
      gc.frustum(1, -1, -1, 1, 2, 10);
    }

  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args)
    {
    } 
  };
  //Inner class ends.
	
private:
  SmartPtr<SimpleView>  view;

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, args ) 
  {
    Args ar;
    ar.set(XmNopenGLBufferType, getBufferType());
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

