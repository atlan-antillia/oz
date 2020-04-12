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
 *  OpenGLCVImageViews.cpp
 *
 *****************************************************************************/

//2019/01/03
#include <opencv2/stitching.hpp>

#define String _XtString

#include <oz++/StringT.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>

#include <oz++/opengl/OpenGLTexture2D.h>
#include <oz++/opengl/OpenGLBulletinBoard.h>
#undef String

#include <oz++/opencv/OpenCVImageInfo.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/opencv/OpenCVImageFileReader.h>


namespace OZ {

class MainView :public OpenGLMainView {
public:

private:
  //Inner OpenCVImageView class start
  class SimpleCVView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat scaledImage;

  public:

    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    SimpleCVView(OpenGLBulletinBoard* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
      try {
        const char* filename = (const char*)args.get(XmNimageFileName);
        int imageLoadingFlag = args.get(XmNimageLoadingFlag);
        loadImage(filename, imageLoadingFlag);

      } catch (OZ::Exception ex) {
        caught(ex);
      }
    } 

    ~SimpleCVView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR)
    {
      try {
        originalImage = readImage(filename, imageLoadingFlag);

        scaleImage(originalImage, scaledImage, 60);  

        refresh();
      } catch (OZ::Exception& ex) {
        caught(ex);
      }
    }    
  };

  //Inner OpenGLView class starts.
  class SimpleGLView: public OpenGLView {
  private:
    OpenGLGC         gc;
    OpenGLTexture2D  texture;
    bool             textured; 
    cv::Mat          blurredImage;
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
        cvImageInfo.getImageInfo(blurredImage, imageInfo); 
        
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
      //Set a proportional perspective.
      gc.matrixMode(GL_PROJECTION);
      gc.loadIdentity();
      gc.perspective(16.0, (double)width() / (double)height(), 0.3, 90.0); 
      gc.matrixMode(GL_MODELVIEW);

      gc.clearColor(1.0, 1.0, 1.0, 1.0);
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


      gc.begin(GL_QUADS);
      texture.coord(0.0, 0.0); 
      gc.vertex(-1.0,  1.0, 1.0);

      texture.coord(1.0, 0.0); 
      gc.vertex( 1.0,  1.0, 0.0);

      texture.coord(1.0, 1.0); 
      gc.vertex( 1.0, -1.0, 1.0);

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
      gc.rotate(20.0, 0.0, 0.0, 1.0);

      texture.bind();

      texture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      texture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      texture.env(GL_TEXTURE_ENV_MODE, GL_MODULATE); 
      
    }

    void blur(cv::Mat& originalImage)
    {
      int ksize = 13;
      int sigma = 10;
      ksize = (ksize/2)*2 + 1;
      blurredImage = cv::Mat::zeros(originalImage.size(), 
                                originalImage.type() );
      //Blur operation is applied to the original image.
      cv::GaussianBlur(originalImage, blurredImage, cv::Size(ksize, ksize), 
            (double)sigma, //sigmaX, 
            (double)sigma); //sigmaY 
    }

  public:
    SimpleGLView(OpenGLBulletinBoard* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args)
    {
      textured = false;
      const char* filename = (const char*)args.get(XmNimageFileName);
      int   imageLoadingFlag = args.get(XmNimageLoadingFlag);

      if (filename) {
        OpenCVImageFileReader reader;
        cv::Mat image = reader.load(filename, imageLoadingFlag);
        blur(image);
      }
    } 

    ~SimpleGLView()
    {
    }

  };
  //Inner class ends.
	
private:
  SmartPtr<OpenGLBulletinBoard> bboard;
  SmartPtr<SimpleGLView>  glView;
  SmartPtr<SimpleCVView>  cvView;

  void resize(Dimension w, Dimension h)
  {
    if (cvView && glView) { 
      cvView -> reshape(0, 0, w/2-1, h);
      glView -> reshape(w/2+1, 0, w/2-2, h);
    }
  }

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, 
       args.set(XmNopenGLBufferType, OpenGL_DOUBLE_BUFFER)) 
  {
    Args ar;    
    bboard = new OpenGLBulletinBoard(this, "", ar);

    const char* filename = "../../opencv/images/flower.png"; 

    ar.reset();
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    ar.set(XmNimageFileName, filename);
    ar.set(XmNimageLoadingFlag, cv::IMREAD_COLOR);
    glView = new SimpleGLView(bboard, "", ar);

    ar.reset();
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    ar.set(XmNimageFileName, filename);
    ar.set(XmNimageLoadingFlag, cv::IMREAD_COLOR);
    cvView = new SimpleCVView(bboard, "", ar);
      
    sendConfigureEvent(); 

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
    args.set(XmNwidth, 800);
    args.set(XmNheight, 400);
    MainView view(applet, argv[0], args);
    view.realize();
    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

