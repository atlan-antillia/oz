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
 *  OpenGLView.h
 *
 *****************************************************************************/

//2015/08/26 Added save, saveASJPG, saveASPNG methods.
//2016/09/17 Added new Contructor takes as parent parameter of OpenGLForm.
#pragma once

#include <oz++/Exception.h>
#include <oz++/motif/Image.h>
#include <oz++/motif/Canvas.h>
#include <oz++/opengl/OpenGLVisualBuffer.h>
#include <oz++/opengl/OpenGLVisual.h>
#include <oz++/opengl/OpenGLContext.h>
#include <oz++/opengl/OpenGLBitmap.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLForm.h>
#include <oz++/opengl/OpenGLImageInfo.h>
#include <oz++/opengl/OpenGLBulletinBoard.h>

namespace OZ {

class OpenGLView: public Canvas {
private:
  OpenGLContext*   context;
  int              bufferType;
	
public:
  //Please redefine a virtual display method in your own class derived from this OpenView class.
  virtual void display()
  {
     printf("OpenGLView::display\n");
  }

  virtual void expose(Action& action)
  {
    setViewPort();
    //Call a virtual display method to display OpenGL figures.
    display();
  	
    glFlush();

    if (bufferType == OpenGL_DOUBLE_BUFFER) {
      swapBuffers();
    }
  }

  void setViewPort()
  {
    Dimension w, h;
    get(XmNwidth, (XtArgVal)&w);
    get(XmNheight, (XtArgVal)&h);
       
    //printf("OpenGLView::setViewPort w=%d h=%d\n", w, h);
    glViewport(0, 0, w, h);
  }

  //2017/03/01
  void setPerspective(GLdouble fovy=16.0,
        GLdouble zNear=0.5,
        GLdouble zFar =100.0f)
  {
     //Set a proportional perspective.
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     int w = width();
     int h = height();
     if (h > 0) {
       GLdouble aspect = (GLdouble)w / (GLdouble)h;
       gluPerspective(fovy, aspect, zNear, zFar);
     }
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
  }

  //If needed, please redefine a virtual intialize method in your own class derived from this Canvas class.
  virtual void initialize()
  {
     //printf("OpenGLView::initialize\n");
  }

  void mapNofity(Event& event)
  {
    //printf("OpenGLView::mapNotify\n");
    Window window = this->getWindow();
    context -> makeCurrent(window);
    setViewPort();
  	
    Dimension width, height;
    get(XmNwidth, (XtArgVal)&width);
    get(XmNheight, (XtArgVal)&height);

    resize(width, height);

    //Call a virtual initialize method   	
    initialize();
  }

  virtual void resize(int width, int height)
  {
  }
	
  void configureNotify(Event& event)
  {
    //printf("OpenGLView::ConfigureNotify\n");
    setViewPort();
    //Call resize
  	
    Dimension width, height;
    get(XmNwidth, (XtArgVal)&width);
    get(XmNheight, (XtArgVal)&height);
    //Call a virtual resize method
    resize(width, height);
  }

  void swapBuffers()
  {
    glXSwapBuffers(getDisplay(), getWindow());
  }
	
public:
  OpenGLView(OpenGLMainView* parent, const char* name, Args& args)
  :Canvas(parent->getMainWindow(), name,// args),
                        args.set(XmNdepth, (XtArgVal)parent->getDepth())
                            .set(XmNvisual, (XtArgVal)parent->getVisual())
                            .set(XmNcolormap, (XtArgVal)parent->getColormap()) ),
       
  context(NULL)
  {
    //Shallow copy of a openGLContext from the parent.
    context =  parent->getOpenGLContext(); 

    bufferType = parent->getBufferType();

    if (args.has(XmNopenGLBufferType)) {
      bufferType = (int)args.get(XmNopenGLBufferType);
    }
  }

  OpenGLView(OpenGLForm* parent, const char* name, Args& args)
  :Canvas(parent, name, args.set(XmNdepth, (XtArgVal)parent->getDepth())
                            .set(XmNvisual, (XtArgVal)parent->getVisual())
                            .set(XmNcolormap, (XtArgVal)parent->getColormap()) ),

  context(NULL)
  {
    //Shallow copy of a openGLContext from the parent.
    context =  parent->getOpenGLContext();

    bufferType = parent->getBufferType();

    if (args.has(XmNopenGLBufferType)) {
      bufferType = (int)args.get(XmNopenGLBufferType);
    }
  }

  //2017/09/02
  OpenGLView(OpenGLBulletinBoard* parent, const char* name, Args& args)
  :Canvas(parent, name, args.set(XmNdepth, (XtArgVal)parent->getDepth())
                            .set(XmNvisual, (XtArgVal)parent->getVisual())
                            .set(XmNcolormap, (XtArgVal)parent->getColormap()) ),

  context(NULL)
  {
    //Shallow copy of a openGLContext from the parent.
    context =  parent->getOpenGLContext();

    bufferType = parent->getBufferType();

    if (args.has(XmNopenGLBufferType)) {
      bufferType = (int)args.get(XmNopenGLBufferType);
    }
  }

  ~OpenGLView()
  {
    context = NULL;
  }
	
  OpenGLContext* getOpenGLContext()
  {
    return context;
  }

  //2015/08/22
  virtual void save(const char* filename)
  {
    if (StringT<char>::endsWithIgnoreCase(filename, ".jpg")) {
      saveAsJPG(filename);
    } 
    else if (StringT<char>::endsWithIgnoreCase(filename, ".png")) {
      saveAsPNG(filename);
    } else {
        //Ignore
    }
  }

  virtual void saveAsJPG(const char* filename)
  {
    try {
      Dimension width = 0;
      Dimension height = 0;
      get(XmNwidth,  (XtArgVal)&width);
      get(XmNheight, (XtArgVal)&height);
      int rw = (width / 8) * 8;
          
      OpenGLBitmap bitmap(0, 0, rw, height, 24, GL_RGB);

      bitmap.read(GL_FRONT);
      bitmap.saveAsJPG(filename);
     printf("saveAsJPG %s\n", filename);
    } catch (Exception& ex) {
      ex.display();
    }      
  }

  virtual void saveAsPNG(const char* filename)
  {
    try {
      Dimension width = 0;
      Dimension height = 0;
      get(XmNwidth,  (XtArgVal)&width);
      get(XmNheight, (XtArgVal)&height);
      int rw = (width / 8) * 8;
          
      OpenGLBitmap bitmap(0, 0, width, height, 8, GL_RGBA);
      bitmap.read(GL_FRONT);
      bitmap.saveAsPNG(filename);
     printf("saveAsPNG %s\n", filename);
    } catch (Exception& ex) {
      ex.display();
    }      
  }

  void refresh()
  {
    unmap();
    map();
  }

};

}

