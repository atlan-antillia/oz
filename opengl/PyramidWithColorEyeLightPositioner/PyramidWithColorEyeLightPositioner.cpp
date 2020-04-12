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
#include <oz++/opengl/Normal3.h>
#include <oz++/opengl/FaceIndices.h>

#include <oz++/opengl/OpenGLClientState.h>
#include <oz++/opengl/OpenGLTriangleSurfaces.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class start
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC gc;
    SmartPtr<OpenGLTriangleSurfaces> surfaces;
    Normal3* normals; //Shallow copy
    size_t numNormals;
    ColorEyeLightPositioner* positioner;//Shallow copy
 
  public:
  virtual void display()
  {
    static Vertex<3> vertices[] = {
      { -1.0, -1.0,  1.0},
      {  1.0, -1.0,  1.0},
      {  1.0, -1.0, -1.0},
      { -1.0, -1.0, -1.0},
      {  0.0,  1.0,  0.0},
    };
              
    static FaceIndices<3> indices[] = {
      {0, 1, 4}, //Triangle: face1
      {1, 2, 4}, //Triangle: face2
      {2, 3, 4}, //Triangle: face3
      {3, 0, 4}, //Triangle] face4
    };
       
    if (surfaces == NULL) {
      surfaces = new OpenGLTriangleSurfaces(vertices, SizeOf(vertices), indices, SizeOf(indices));
      normals = surfaces->calculateSurfaceNormals(numNormals);
      //surfaces->displayNormals();
    }
    
    if (normals && positioner) { 
      gc.frustum(1 , -1 , -1 , 1 , 1 , 10);

      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();
      gc.matrixMode(GL_MODELVIEW);
      gc.clearColor(0.0, 1.0, 1.0, 1.0);
      
      gc.translate(0.0f,0.0f,-1.0f);
      float eyeX, eyeY, eyeZ;
      positioner->getEyePosition(eyeX, eyeY, eyeZ); 
      gc.lookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
      gc.rotate(0.0, 0.0, 1.0, 0.0);
      
      try {
        gc.enable(GL_LIGHTING);
 
        OpenGLLight light(GL_LIGHT0);
        float lightX, lightY, lightZ;
        positioner->getLightPosition(lightX, lightY, lightZ);

        light.position(lightX, lightY, lightZ, 0.0);  

        OpenGLMaterial material(GL_FRONT);
        float r, g, b;
        positioner->getColorPosition(r, g, b);
        material.diffuse (r, g, b, 0.0f);
        material.specular(1.0f, 1.0f, 1.0f, 0.0f);
        material.shininess(100.0f);
  
        gc.frontFace(GL_CCW);

        gc.enable(GL_CULL_FACE);
        gc.cullFace(GL_BACK);
        gc.enable(GL_NORMALIZE);
        
        for (int i = 0; i< SizeOf(indices); i++) {
          gc.begin(GL_TRIANGLES);
          FaceIndices<3> tri = indices[i];
          gc.draw( normals[i], 
                    vertices[ tri.index[0] ], 
                    vertices[ tri.index[1] ], 
                    vertices[ tri.index[2] ]);
          gc.flush();
          gc.end();
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
       gc.perspective(16.0, (double)w / (double)h, 0.5, 40.0); 

       gc.matrixMode(GL_MODELVIEW);
    }


    virtual void initialize()
    {
      gc.enable(GL_DEPTH_TEST);
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
      positioner = pos; //shallow copy.
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
    positioner = new ColorEyeLightPositioner(form, "", ar);
     
    positioner->setColorPosition(255, 0, 128);
    positioner->setEyePosition(3, 6, 12);
    positioner->setLightPosition(-4, -8, 10);
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

