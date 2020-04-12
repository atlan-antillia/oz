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
 *  ColoredCube.cpp
 *
 *****************************************************************************/

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLClientState.h>
#include <oz++/opengl/OpenGLFace.h>
#include <oz++/ArrayT.h>

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

      gc.translate(-2.0f, 0.0f, -6.0f); 
      gc.clearColor(0, 0, 0, 0);
 
      gc.begin(GL_QUADS);         

      OpenGLFace top = {
          { 1.0f, 1.0f, -1.0f},
          {-1.0f, 1.0f, -1.0f},
          {-1.0f, 1.0f,  1.0f},
          { 1.0f, 1.0f,  1.0f} };

      OpenGLFace bottom = {
          { 1.0f, -1.0f,  1.0f},
          {-1.0f, -1.0f,  1.0f},
          {-1.0f, -1.0f, -1.0f},
          { 1.0f, -1.0f, -1.0f} };

      OpenGLFace front = {
          { 1.0f,  1.0f, 1.0f},
          {-1.0f,  1.0f, 1.0f},
          {-1.0f, -1.0f, 1.0f},
          { 1.0f, -1.0f, 1.0f} };
 
      OpenGLFace back = {
        { 1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f} };
 
      OpenGLFace left = {
        {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f} };
 
      OpenGLFace right = {
        { 1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f,  1.0f},
        { 1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f, -1.0f} };

      gc.color(0.0f, 1.0f, 0.0f);     
      gc.draw(top);

      gc.color(0.5f, 0.5f, 0.0f);    
      gc.draw(bottom);

      gc.color(1.0f, 0.0f, 0.0f);  
      gc.draw(front);

      gc.color(0.5f, 0.0f, 0.5f); 
      gc.draw(back);

      gc.color(0.0f, 0.0f, 1.0f);  
      gc.draw(left);

      gc.color(0.5f, 0.5f, 1.0f);  
      gc.draw(right);
      gc.end();
    }
 
    virtual void resize(int w, int h)
    {
      if (w == 0 || h == 0) {
        return;
      }
      gc.matrixMode(GL_PROJECTION); 
      gc.loadIdentity();
      gc.perspective(50.0f, (GLfloat)w/(GLfloat)h, 0.1f, 50.0f);
      gc.matrixMode(GL_MODELVIEW);
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
  //Inner class end
	
private:
  SmartPtr<SimpleView>  view;

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name,
               args.set(XmNopenGLBufferType, OpenGL_DOUBLE_BUFFER)
                   .set(XmNopenGLBufferDepth, 24)) 
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
int main(int argc, char** argv) {
  try {
    const char*  appclass = "ColoredCube";
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

