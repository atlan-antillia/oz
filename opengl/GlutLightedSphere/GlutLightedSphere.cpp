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
 *  GlutLightedSphere.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>
#include <oz++/opengl/OpenGLGeometry.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC gc;
 
  public:
    virtual void display()
    {
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();
      gc.lookAt(2.0, 4.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

      gc.clearColor(0.0, 0.0, 0.0, 1.0);
      gc.enable(GL_CULL_FACE); 
      gc.enable(GL_LIGHTING);

      gc.enable(GL_DEPTH_TEST);     
      OpenGLLight light(GL_LIGHT0);
      light.position(10.0, 10.0, 10.0, 1.0);

      OpenGLMaterial material(GL_FRONT);
      material.specular(1.0f, 1.0f, 1.0f, 1.0f); //white);
      material.shininess(100.0); 

      OpenGLGeometry geometry;
      gc.pushMatrix();
      material.diffuse(0.0f, 0.0f, 1.0f, 1.0f);  //blue;
      gc.translate(-2.0, 1.0, -6.0);
      gc.rotate(-40.0, 1.0, 0.0, 0.0);
      geometry.solidSphere(2.0, 40.0, 40.0); 
      gc.popMatrix();

      gc.pushMatrix();
      material.diffuse(1.0f, 0.0f, 0.0f, 1.0f);  //red
      gc.translate(-2.2,-6.0,-6.0);
      gc.rotate(10.0, 1.0, 0.0, 0.0);
      geometry.wireSphere(2.0, 40.0, 40.0); 
      gc.popMatrix();
    }

    virtual void resize(int w, int h)
    {
      if (w == 0 || h == 0) {
        return;
      }
        const float r = (float) w / (float) h;

        gc.matrixMode(GL_PROJECTION);
        gc.loadIdentity();
        gc.frustum(-r, r, -1.0, 1.0, 2.0, 100.0);

        gc.matrixMode(GL_MODELVIEW);
        gc.loadIdentity() ;
    }


    virtual void initialize()
    {
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
  glutInit(&argc, argv);

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

