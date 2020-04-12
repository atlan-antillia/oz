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
 *  LightedTriangleRotationByKeyInput.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>

#include <oz++/opengl/Vertex3.h>
#include <oz++/opengl/Color4.h>
#include <oz++/opengl/FaceIndices.h>

#include <oz++/opengl/OpenGLClientState.h>
#include <oz++/opengl/OpenGLTriangleSurfaces.h>


namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC gc;
    float     angle;
    SmartPtr<OpenGLTriangleSurfaces> surfaces;
    Normal3* normals; 
    size_t numNormals;
    
  virtual void display()
  {
    static Vertex<3> vertices[] = {
      //Face 1:
      {-1.0,-1.0, 1.0 }, //0
      { 1.0,-1.0, 1.0 }, //1
      { 0.0, 1.0, 1.0 }, //2      
    };
        
    static Color4 colors[] = {
      {1.0f, 0.4f, 0.2f, 1.0f}, 
    };
       
    static FaceIndices<3> indices[] = {
      {0, 1, 2}   //Triangle
    };
   
    
    if (surfaces == NULL) {
      surfaces = new OpenGLTriangleSurfaces(vertices, SizeOf(vertices), indices, SizeOf(indices));
      normals = surfaces->calculateSurfaceNormals(numNormals);
    }
    
    if (normals) { 
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();
      gc.matrixMode(GL_MODELVIEW);
      gc.clearColor(0.0, 1.0, 1.0, 1.0);
      
      gc.translate(0.0f,0.0f,-1.0f); 
      gc.lookAt(2.0, 4.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
      
      gc.rotate(angle, 0.0, 1.0, 0.0);
      
      try {
        gc.enable(GL_LIGHTING);
 
        OpenGLLight light(GL_LIGHT0);
  
        light.position(0, 0.0, 1.0, 0.0);  

        GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat blue[]  = {0.0f, 0.0f, 1.0f, 1.0f};
        GLfloat shininess[] = {40.0};

        OpenGLMaterial material(GL_FRONT);
        material.diffuse(blue);
        material.specular(white);
        material.shininess(shininess);
  
        gc.frontFace(GL_CCW);

        gc.enable(GL_CULL_FACE);
        //Draw the front.
        gc.cullFace(GL_BACK);
        for (int i = 0; i< SizeOf(indices); i++) {
          gc.begin(GL_TRIANGLES);
          FaceIndices<3> tri = indices[i];
          gc.draw(colors[i], normals[i], 
               vertices[ tri.index[0] ], vertices[ tri.index[1] ], vertices[ tri.index[2] ]);
          gc.end();  
        }
        
        //Draw the back.
        gc.cullFace(GL_FRONT);
        for (int i = 0; i< SizeOf(indices); i++) {
          gc.begin(GL_TRIANGLES);
          FaceIndices<3> tri = indices[i];
          gc.draw(colors[i], normals[i], 
               vertices[ tri.index[0] ], vertices[ tri.index[1] ], vertices[ tri.index[2] ]);
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
    gc.enable( GL_DEPTH_TEST );
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

    virtual void keyRelease(Event& event)
    {
      //Do nothing here
    }

  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args),
    angle(0.0f)
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
    args.set(XmNwidth, 400);
    args.set(XmNheight, 300);
    
    MainView view(applet, name, args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

