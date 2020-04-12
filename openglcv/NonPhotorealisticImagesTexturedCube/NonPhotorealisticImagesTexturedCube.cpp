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
 *  NonPhotorealisticImagesTexturedCube.cpp 
 *
 *****************************************************************************/

//2019/01/03
#include <opencv2/stitching.hpp>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLMultiTexturedCube.h>
#include <oz++/opencv/OpenCVImageInfo.h>
#include <oz++/opencv/OpenCVImageFileReader.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.

  class SimpleView: public OpenGLView {
  private:
    OpenGLGC         gc;
    static const int COUNT = 6;     //The number of faces of a cube.
    cv::Mat          faces[COUNT];  //Mat images for 6 faces of a cube.
    OpenGLImageInfo  imageInfos[COUNT];

    SmartPtr<OpenGLMultiTexturedCube> cube;
    float  angle;
    
  public:
    virtual void display()
    {
      if (cube) {  
       //Set a proportional perspective.
       gc.matrixMode(GL_PROJECTION);
       gc.loadIdentity();
       gc.perspective(16.0, (double)width() / (double)height(), 0.5, 100.0); 

       gc.matrixMode(GL_MODELVIEW);  
        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gc.clearColor(1.0f, 1.0f, 1.0f, 1.0f);

        gc.loadIdentity();
        gc.translate(0.0f,0.0f,-1.0f); 
        gc.lookAt(2.0, 6.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

        //Rotation around y-axis.
        gc.rotate(angle, 0.0, 1.0, 0.0);

        cube -> draw(&gc);
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
      gc.enable(GL_TEXTURE_2D);
      try {
        
        cube = new OpenGLMultiTexturedCube(COUNT, imageInfos);
        
      } catch (Exception& ex) {
        caught(ex);
      }
    }

    virtual void keyPress(Event& event)
    {
      KeySym k = event.getKeySym();
      if (k == XK_Left) {
        angle -= 0.5f;
        sendExposeEvent();
      } 
      if (k == XK_Right) {
        angle += 0.5f;
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
    angle(0.0)
    {
      OpenCVImageFileReader reader;
      cv::Mat originalImage = reader.load("../../opencv/images/flower.png");
      int n = 0;
      cv::stylization(originalImage,             faces[n++]);
      cv::Mat img2;
      cv::pencilSketch(originalImage, img2,      faces[n++], 10 , 0.1f, 0.03f); 
      cv::edgePreservingFilter(originalImage,    faces[n++], 1);
      cv::edgePreservingFilter(originalImage,    faces[n++], 2);
      cv::detailEnhance(originalImage,           faces[n++]);
      cv::Mat img1;
      cv::pencilSketch(originalImage,            faces[n++], img1, 10 , 0.1f, 0.03f); 
     
      for (int i = 0; i<COUNT; i++) {
        OpenCVImageInfo cvImageInfo;
        cvImageInfo.getImageInfo(faces[i], imageInfos[i], true);  
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
    args.set(XmNwidth,  480);
    args.set(XmNheight, 480);

    MainView view(applet, name, args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

