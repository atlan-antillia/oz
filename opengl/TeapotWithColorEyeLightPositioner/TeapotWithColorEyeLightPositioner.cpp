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
 *  SphereWithColorEyeLightPositioner.cpp
 *
 *****************************************************************************/

#include <oz++/motif/ColorEyeLightPositioner.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLForm.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>
#include <oz++/opengl/OpenGLSphere.h>
#include <oz++/opengl/OpenGLGeometry.h>


namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class start
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC gc;
    ColorEyeLightPositioner* positioner;
 
  public:
    virtual void display()
    {
      if (positioner) {
        gc.enable(GL_DEPTH_TEST);
        gc.frustum(1 , -1 , -1 , 1 , 1 , 10);
        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gc.loadIdentity();
        gc.matrixMode(GL_MODELVIEW);

        float eyeX, eyeY, eyeZ;
        positioner->getEyePosition(eyeX, eyeY, eyeZ);
        gc.lookAt(eyeX, eyeY, eyeZ, 
            0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        gc.clearColor(0.0, 0.0, 0.0, 1.0);
        gc.enable(GL_CULL_FACE); 
        gc.enable(GL_LIGHTING);

        OpenGLLight light(GL_LIGHT0);
        float lightX, lightY, lightZ;
        positioner->getLightPosition(lightX, lightY, lightZ);
        light.position(lightX, lightY, lightZ, 1.0f);

        float r, g, b;
        positioner->getColorPosition(r, g, b);
       
        OpenGLMaterial material(GL_FRONT);
        material.diffuse((GLfloat)r, (GLfloat)g, (GLfloat)b, 1.0f);
        material.specular(1.0f, 1.0f, 1.0f, 1.0f);
        material.shininess(100.0);
        OpenGLGeometry geometry;
        geometry.solidTeapot(3.0);
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
    }

  public:
    SimpleView(OpenGLForm* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args)
    ,positioner(NULL)
    {
    } 

    ~SimpleView()
    {
    }

    void setPositioner(ColorEyeLightPositioner* pos)
    {
      positioner = pos; //shallow copy
    } 
  };
  //Inner class end
	
private:
  SmartPtr<SimpleView>  view;
  SmartPtr<OpenGLForm>  form;
  SmartPtr<ColorEyeLightPositioner> positioner;
 
  virtual void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe->type == ConfigureNotify) {
      configureNotify(event);
    } else {
      //Add here your own nofity event handlers.
    }
    //sendExposeEvent();
  }
  
  //Your own resize handler.
  virtual void configureNotify(Event& event)
  {
    configure();
  }

  virtual void configure()
  {
    Dimension width, height;
    get(XmNwidth, (XtArgVal)&width);
    get(XmNheight, (XtArgVal)&height);

    if (positioner && view) {
      Dimension w = width-positioner->getPositionerWidth()-4;

      view->set(XmNwidth, w);
      view->set(XmNheight, height);
      view->resize(w, height);
      positioner->resize((Position)(w-4), (Position)0,
                w+2, (Dimension)(height-2), (Dimension)0);
    }
  }

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, args) 
  {
    Args ar;
    form  = new OpenGLForm(this, "", ar);
 
    ar.reset();
    ar.set(XmNtopAttachment,   XmATTACH_FORM);
    ar.set(XmNleftAttachment,  XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNresizePolicy,     XmRESIZE_ANY);
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    view = new SimpleView(form, "", ar);

    ar.reset();
    ar.set(XmNtopAttachment,   XmATTACH_FORM);
    ar.set(XmNrightAttachment, XmATTACH_FORM);
    ar.set(XmNleftAttachment,  XmATTACH_WIDGET);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNleftWidget, view->getWidget());
    ar.set(XmNeyePositionerMinimum, -30);
    ar.set(XmNeyePositionerMaximum,  30);
    positioner = new ColorEyeLightPositioner(form, "", ar);
     
    positioner->setColorPosition(230, 200, 255);
    positioner->setEyePosition(-20, 4, 30);

    positioner->setLightPosition(20, 12, -20);
    positioner->setExposeEventListener(view);

    view->setPositioner(positioner);

    this -> addEventHandler(StructureNotifyMask, this, 
         (Handler)&MainView::structureNotify, NULL);

    //Call configure layaout method. 
    configure();
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
    args.set(XmNgeometry, "600x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

