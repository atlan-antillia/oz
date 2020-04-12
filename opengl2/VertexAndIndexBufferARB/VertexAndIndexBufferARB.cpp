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
 *  IndexAndIndexBufferARB.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>
#include <oz++/opengl/ColorNormalVertex.h>

#include <oz++/opengl/OpenGLVertex3.h>
#include <oz++/opengl/OpenGLClientState.h>
#include <oz++/openglarb/OpenGLVertexBufferARB.h>
#include <oz++/openglarb/OpenGLIndexBufferARB.h>

#include <oz++/opengl2/OpenGLVertexAttribute.h>


namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC gc;
    SmartPtr<OpenGLVertexBufferARB> vertexBuffer;
    SmartPtr<OpenGLIndexBufferARB>  indexBuffer;

    int numVertices;
    int numIndices;
    
  public:
  virtual void display()
  {

    if (vertexBuffer && indexBuffer) { 
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();

      try {
        gc.translate(0.0f,0.0f,-1.0f);  
        gc.lookAt(2.0, 4.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
        gc.clearColor(1.0f, 0.8f, 0.8f, 1.0f);
        
        //OpenGLClientState constructor enables its argument, and the destructor disables its argument.  
        
        OpenGLClientState state1(GL_VERTEX_ARRAY); 
        OpenGLClientState state2(GL_COLOR_ARRAY);
        OpenGLClientState state3(GL_NORMAL_ARRAY);
        
        const GLfloat lightPosition[] = {  5.0,  5.0, -8.0 , 1.0 };
        const GLfloat lightColor[]    = {  1.0,  0.0,  0.0 , 1.0 };
        const GLfloat materialColor[] = {  0.8,  0.8,  0.4 , 1.0 };
    
        OpenGLLight light(GL_LIGHT0);
        light.position(lightPosition);
        light.diffuse(lightColor);
        light.enable(); 
      
        OpenGLMaterial material(GL_FRONT_AND_BACK );
        material.diffuse(materialColor);

        vertexBuffer -> bind();
        indexBuffer  -> bind();

        gc.interleavedArrays(GL_C4F_N3F_V3F, 0, NULL);
        gc.drawElements(GL_QUADS, numIndices, GL_UNSIGNED_INT, NULL);

        vertexBuffer -> unbind();
        indexBuffer  -> unbind();
      
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

      static ColorNormalVertex<float,4,3,3> vertices[] = {
	{ 0.5f, 0.0f, 0.0f, 0.5f,  -0.6f,  0.6f,  0.6f,  -0.5f,  0.5f,  0.5f },
	{ 0.5f, 0.0f, 0.0f, 0.5f,  -0.6f, -0.6f,  0.6f,  -0.5f, -0.5f,  0.5f },
	{ 0.5f, 0.0f, 0.0f, 0.5f,   0.6f,  0.6f,  0.6f,   0.5f,  0.5f,  0.5f },
	{ 0.5f, 0.0f, 0.0f, 0.5f,   0.6f, -0.6f,  0.6f,   0.5f, -0.5f,  0.5f },
	{ 0.5f, 0.0f, 0.0f, 0.5f,   0.6f,  0.6f, -0.6f,   0.5f,  0.5f, -0.5f },
	{ 0.5f, 0.0f, 0.0f, 0.5f,   0.6f, -0.6f, -0.6f,   0.5f, -0.5f, -0.5f },
	{ 0.5f, 0.0f, 0.0f, 0.5f,  -0.6f,  0.6f, -0.6f,  -0.5f,  0.5f, -0.5f },
	{ 0.5f, 0.0f, 0.0f, 0.5f,  -0.6f, -0.6f, -0.6f,  -0.5f, -0.5f, -0.5f },
      };
    
      static GLuint indices[] = {
       0, 2, 3, 1,	
       2, 4, 5, 3,	
       4, 6, 7, 5,	
       6, 0, 1, 7,	
       6, 4, 2, 0,	
       1, 3, 5, 7,	
      };

      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();

      numVertices = SizeOf(vertices);
     
      vertexBuffer = new OpenGLVertexBufferARB();

      vertexBuffer -> bind();
      vertexBuffer -> data(//sizeof(vertices),
            numVertices*sizeof(ColorNormalVertex<float,4,3,3>), 
            vertices, GL_STATIC_DRAW_ARB);
      vertexBuffer -> unbind();
              
      numIndices  = SizeOf( indices );

      indexBuffer  = new OpenGLIndexBufferARB();
      indexBuffer -> bind();
      indexBuffer -> data(numIndices*4, indices, GL_STATIC_DRAW_ARB);
      indexBuffer -> unbind();
      
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

