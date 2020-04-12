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
 *  LightedSphereOrbiter.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>
#include <oz++/opengl/OpenGLSphere.h>
#include <oz++/opengl/OpenGLCircle.h>
#include <oz++/motif/RenderingTimer.h>
#include <oz++/Mutex.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC      gc;
    SmartPtr<OpenGLQuadric> quadric;
    SmartPtr<OpenGLSphere>  sphere;
    SmartPtr<OpenGLCircle>  circle;
    SmartPtr<RenderingTimer> timerThread;
    int                     renderingInterval;
    Mutex                   mutex;
    float angle;
    static const int    CIRCLE_ANGLE = 360;
    static const int INCREMENT = 1;
    
  public:
    virtual void display()
    {
      mutex.lock();
      if (angle <(CIRCLE_ANGLE - INCREMENT) ) {
          angle += INCREMENT;
      } else {
        angle = INCREMENT;
      }

      if (sphere) {
        setPerspective();

        Vertex3 pos = circle->getOrbitPosition(angle);

        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gc.loadIdentity();
        gc.lookAt(-5.0, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        gc.clearColor(0.0, 0.0, 0.0, 1.0);
        gc.enable(GL_CULL_FACE); 
        gc.enable(GL_LIGHTING);

        gc.translate(0.0f, 0.0f, 0.0f);
        gc.color(1.0f, 1.0f, 1.0f); //green

        circle->draw(&gc);
       
        gc.translate(pos.x, pos.y, pos.z);
        
        OpenGLLight light(GL_LIGHT0);
        GLfloat lightPosition[] = {-2.0f, 0.0f, -1.0f, 1.0};  
        light.position(lightPosition);

        GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat blue[]  = {0.0f, 0.0f, 1.0f, 1.0f};
        GLfloat green[] = {0.0f, 1.0f, 0.0f, 1.0f};

        GLfloat shininess[] = {100.0};

        OpenGLMaterial material(GL_FRONT);
        material.diffuse(green);
        material.specular(white);
        material.shininess(shininess);
        
        sphere-> draw(0.5f, 40, 40);
      }
      mutex.unlock();
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
      quadric = new OpenGLQuadric();
      quadric -> drawStyle(GLU_FILL);
      quadric -> normals(GLU_SMOOTH);
      sphere = new OpenGLSphere(quadric);
      circle = new OpenGLCircle(0.0f, 0.0f, 0.0f, 1.3f);
      timerThread  = new RenderingTimer(this, renderingInterval);
      timerThread->start();
    }

  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args),
    angle(0.0f),




    renderingInterval(10)
    {
      renderingInterval = (const int)args.get(XmNrenderingInterval); 
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
    int interval = args.get(XmNrenderingInterval); 
    Args ar;
    ar.set(XmNrenderingInterval, interval);  //20 millisec
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
    args.set(XmNwidth,  400);
    args.set(XmNheight, 400);
    args.set(XmNrenderingInterval, 20);  //20 millisec

    MainView view(applet, name, args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

