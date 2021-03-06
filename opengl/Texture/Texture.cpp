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
 *  Texture.cpp
 *
 *****************************************************************************/

#include <oz++/motif/PixmapFile.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLTexture2D.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC        gc;
    SmartPtr<OpenGLTexture2D> texture;
    SmartPtr<PixmapFile>      pixmapFile; 

  public:
    virtual void display()
    {
      gc.clearColor(1.0, 1.0, 1.0, 1.0);
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.color(1.0, 1.0, 1.0);
      gc.rotate(1.0, 0.5, 1.0, 0.25);

      gc.begin(GL_QUADS);
      texture -> coord(0.0, 0.0); 
      gc.vertex(-1.0,  1.0, 0.0);

      texture -> coord(1.0, 0.0); 
      gc.vertex( 1.0,  1.0, 0.0);

      texture -> coord(1.0, 1.0); 
      gc.vertex( 1.0, -1.0, 0.0);

      texture -> coord(0.0, 1.0); 
      gc.vertex(-1.0, -1.0, 0.0);
      gc.end();
    }
 
    virtual void resize(int w, int h)
    {
      if (w == 0 || h == 0) {
        return;
      }
      gc.matrixMode(GL_PROJECTION);
      gc.loadIdentity();
      gc.ortho(-1.25, 1.25, -1.25, 1.25, 1., 20.);

      gc.matrixMode(GL_MODELVIEW);
      gc.loadIdentity();
      gc.lookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
    }

    virtual void initialize()
    {
      gc.enable(GL_DEPTH_TEST);
      gc.enable(GL_TEXTURE_2D);

      texture = new OpenGLTexture2D();
      texture -> bind();
      texture -> parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      texture -> parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      texture -> env(GL_TEXTURE_ENV_MODE, GL_MODULATE); 

      //const char* jpgfile = "./images/fudasho_18.jpg";
      //Ummh, XmGetPixmap seems to be not able to load jpg files.

      ModuleFileName module;
      const char* path = module.getPath();

      const char* jpgfile = "./images/NiohLiken.png";
      char fullpath[PATH_MAX];
      sprintf(fullpath, "%s/%s", path, jpgfile);
      try {
        pixmapFile = new PixmapFile(getDisplay(), fullpath); 
        flush(); 
        XImage*  ximage = pixmapFile -> getImage(); 
        unsigned int width, height;
        pixmapFile -> getGeometry(&width, &height);
        printf("Pixmap w=%d h=%d\n", width, height);
        if (ximage) {
          printf("XImage depth=%d\n", ximage->depth);
          texture -> image(0, GL_RGB, width, height, 0, 
              GL_BGRA, GL_UNSIGNED_BYTE, (void*)(ximage->data));
              //GL_RGBA, GL_UNSIGNED_BYTE, (void*)(ximage->data));

          XDestroyImage(ximage);
        } else {
          printf("ximage is NULL\n");
        }
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
    const char*  appclass = argv[0];
    OpenGLApplication applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "500x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

