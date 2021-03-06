/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  DenoisedImageTexture.cpp
 *
 *****************************************************************************/

//2019/01/03
#include <opencv2/stitching.hpp>
#include <oz++/StringT.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLTexture2D.h>
#include <oz++/opencv/OpenCVImageInfo.h>
#include <oz++/opencv/OpenCVImageFileReader.h>

namespace OZ {

class MainView :public OpenGLMainView {
public:

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC         gc;
    OpenGLTexture2D  texture;
    bool             textured; 
    cv::Mat          denoisedImage;
    StringT<char>    imageFile;
    int              imageLoadingFlag;

  private:
    void imageToTexture()
    {
      try {
        int width  = 0;
        int height = 0;
        OpenGLImageInfo imageInfo;

        OpenCVImageInfo cvImageInfo;
        cvImageInfo.getImageInfo(denoisedImage, imageInfo);
        texture.image(imageInfo);

      } catch (OZ::Exception& ex) {
        caught(ex);
      }
    }

  public:
    virtual void display()
    {
      if (textured == false) {
        imageToTexture();
        textured = true;
      }

      gc.clearColor(1.0, 1.0, 1.0, 1.0);
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      gc.begin(GL_QUADS);
      texture.coord(0.0, 0.0);
      gc.vertex(-1.0,  1.0, 0.0);

      texture.coord(1.0, 0.0);
      gc.vertex( 1.0,  1.0, 0.0);

      texture.coord(1.0, 1.0);
      gc.vertex( 1.0, -1.0, 0.0);

      texture.coord(0.0, 1.0);
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
      gc.rotate(30.0, 0.0, 0.0, 1.0);

      texture.bind();

      texture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      texture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      texture.env(GL_TEXTURE_ENV_MODE, GL_MODULATE); 
      
    }

    void denoise(cv::Mat& originalImage)
    {
      denoisedImage = cv::Mat::zeros(originalImage.size(), 
                                originalImage.type() );
      
      int hParameter         = 27;
      int templateWindowSize = 15;
      int searchWindowSize   = 13;

      fastNlMeansDenoising(originalImage, denoisedImage, (float)hParameter, 
           templateWindowSize, searchWindowSize);

    }

  public:
    SimpleView(OpenGLMainView* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args)
    {
      textured = false;
      const char* filename = (const char*)args.get(XmNimageFileName);
      int   imageLoadingFlag = args.get(XmNimageLoadingFlag);

      if (filename) {
        OpenCVImageFileReader reader;
        cv::Mat image = reader.load(filename, imageLoadingFlag);
        denoise(image);
      }
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
  :OpenGLMainView(applet, name, 
       args.set(XmNopenGLBufferType, OpenGL_DOUBLE_BUFFER)) 
  {
    const char* filename = "../../opencv/images/MeshedNioh.png"; 

    Args ar;
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    ar.set(XmNimageFileName, filename);
    ar.set(XmNimageLoadingFlag, cv::IMREAD_COLOR);
    view = new SimpleView(this, "", ar);

    char title[PATH_MAX];
    sprintf(title, "%s - %s", filename, name);
    set(XmNtitle, title);
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
    args.set(XmNgeometry, "500x500+40+40");
    MainView view(applet, argv[0], args);
    view.realize();
    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

