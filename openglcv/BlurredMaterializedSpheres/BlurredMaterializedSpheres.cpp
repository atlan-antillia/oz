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
 *  BlurredMaterializedSpheres.cpp
 *
 *****************************************************************************/

//2019/01/03
#include <opencv2/stitching.hpp>
#define String _XtString

#include <oz++/StringT.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/PushButton.h>
#include <oz++/opengl/OpenGLApplication.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLTexture2D.h>
#include <oz++/opengl/OpenGLBulletinBoard.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>
#include <oz++/opengl/OpenGLSolidSphere.h>
#include <oz++/opengl/OpenGLWireSphere.h>

#undef String

#include <oz++/opencv/OpenCVImageInfo.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/opencv/OpenCVImageFileReader.h>


namespace OZ {

class MainView :public OpenGLMainView {
public:

private:
  //Inner OpenGLView class starts.
  class SimpleGLView: public OpenGLView {
  private:
    static const int        SPHERES=4;
    OpenGLGC      gc;
    SmartPtr<OpenGLGeometry>  sphere[SPHERES];
    float                    angle;
 
  public:
    virtual void display()
    {
      if (sphere[0]) {
        setPerspective();

        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gc.lookAt(4.0, 8.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        gc.clearColor(0.0, 0.0, 0.0, 1.0);
        gc.enable(GL_CULL_FACE); 
        gc.enable(GL_LIGHTING);

        OpenGLLight light(GL_LIGHT0);
        GLfloat lightPosition[] = {10, 10, 10, 1.0};  
        light.position(lightPosition);

        for (int i = 0; i<SPHERES; i++) {
          gc.pushMatrix();
          gc.rotate(angle, 0.0f, 1.0f, 0.0f);
          sphere[i]->draw(&gc, -2.0f+1.3f*i, 0.5f,  0.0f+ 0.3f*i);
          gc.popMatrix();
        }
      }
    }
  
    cv::Mat capture()
    {
      Dimension w = width();
      Dimension h = height();
/*
      get(XmNwidth,  (XtArgVal)&w);
      get(XmNheight, (XtArgVal)&h);
*/
      w = (w/8)*8;

      glReadBuffer(GL_FRONT);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      unsigned char* pixels = new unsigned char[3 * w * h];
      glReadPixels(0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, pixels);
        
      return cv::Mat(h, w, CV_8UC3, pixels);
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
      Color4  black         = { 0.0, 0.0, 0.0, 1.0 };
      Color4  ambient       = { 0.5, 0.5, 0.5, 1.0 };
      Color4  diffuse       = { 0.2, 0.4, 0.8, 1.0 };
      Color4  specular      = { 1.0, 1.0, 1.0, 1.0 };
      Color4  emission      = { 0.8, 0.0, 0.0, 0.0 };
      GLfloat lowShining    = { 10.0 };
      GLfloat highShining   = {100.0 };

      OpenGLMateria mat1(GL_FRONT, ambient, diffuse, specular, emission, lowShining);
      OpenGLMateria mat2(GL_FRONT, black,   diffuse, specular, emission, lowShining);
      OpenGLMateria mat3(GL_FRONT, black,   diffuse, black,    emission, highShining);
      OpenGLMateria mat4(GL_FRONT, ambient, diffuse, specular, black,    highShining);
      OpenGLMateria materias[] = {mat1, mat2, mat3, mat4};
      for (int i = 0; i<SPHERES; i++) {
        if (i%2 ==0) {
          sphere[i] = new OpenGLWireSphere(materias[i], 0.5f,  40, 40);
        } else {
          sphere[i] = new OpenGLSolidSphere(materias[i], 0.5f,  40, 40);
        }
      }
    }

  public:
    SimpleGLView(OpenGLBulletinBoard* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args),
    angle(100.0f)
    {
    } 
  };

  //Inner OpenCVImageView class start
  class SimpleCVView: public OpenCVImageView {
  private:
    cv::Mat        originalImage; 
    cv::Mat        blurredImage;
    cv::Mat        scaledImage;

  public:

    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    SimpleCVView(OpenGLBulletinBoard* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
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

    void setImage(cv::Mat mat, bool flip=false)
    {
      try {
        cv::Mat flipped = mat;
        
        if (flip) {;
          cv::flip(mat, flipped, 0); 
          originalImage = flipped;
        }
        originalImage = flipped;

        blur(originalImage);

        scaleImage(blurredImage, scaledImage, 80);  
        invalidate();

        refresh();
       
      } catch (OZ::Exception& ex) {
        caught(ex);
      }
    }    
  };
  //Inner class ends.
	
private:
  SmartPtr<OpenGLBulletinBoard> bboard;
  SmartPtr<RowColumn>     controlPane;

  SmartPtr<SimpleGLView>  glView;
  SmartPtr<SimpleCVView>  cvView;
  SmartPtr<PushButton>    captureButton;
 
  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 100;
    int ww = (w - CP_WIDTH)/2;
    if (glView && cvView && controlPane) { 
      glView -> reshape(0, 0, ww-1, h);
      cvView -> reshape(ww+1, 0, ww-2, h);
      controlPane -> reshape(w-CP_WIDTH, 0,CP_WIDTH-2, h);
    }
  }

  void captureImage(Action& action)
  {
    cv::Mat image =  glView->capture();
    cvView->setImage(image, true);
  }

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, 
       args.set(XmNopenGLBufferType, OpenGL_DOUBLE_BUFFER)) 
  {
    Args ar;    
    bboard = new OpenGLBulletinBoard(this, "", ar);


    ar.reset();
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    glView = new SimpleGLView(bboard, "", ar);

    ar.reset();
    cvView = new SimpleCVView(bboard, "", ar);
      
    ar.reset();
    controlPane = new RowColumn(bboard, "", ar);
    
    ar.reset();
    captureButton = new PushButton(controlPane, "Capture", ar);
    captureButton->addCallback(XmNactivateCallback, this,
       (Callback)&MainView::captureImage, NULL);

    sendConfigureEvent(); 

    
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
    glutInit(&argc, argv);

    const char*  appclass = argv[0];
    OpenGLApplication applet(appclass, argc, argv);

    Args args;
    args.set(XmNwidth, 900);
    args.set(XmNheight, 400);
    MainView view(applet, argv[0], args);
    view.realize();
    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

