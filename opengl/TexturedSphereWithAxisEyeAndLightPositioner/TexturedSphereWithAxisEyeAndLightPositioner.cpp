/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  TexturedSphereWithAxisEyeAndLightPositioner.cpp
 *
 *****************************************************************************/

#include <oz++/Vector3f.h>
#include <oz++/Vertex.h>

#include <oz++/motif/RowColumn.h>

#include <oz++/motif/LabeledTrackBar.h>
#include <oz++/motif/AxisPositioner.h>
#include <oz++/motif/EyePositioner.h>
#include <oz++/motif/LightPositioner.h>
#include <oz++/motif/FileOpenDialog.h>

#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLBulletinBoard.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLLight.h>
#include <oz++/opengl/OpenGLMaterial.h>
#include <oz++/opengl/OpenGLSphere.h>
#include <oz++/opengl/OpenGLTexturedSphere.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView: public OpenGLView {
  private:
    OpenGLGC          gc;
    SmartPtr<OpenGLTexturedSphere> texture;
    float             angle;
    
    Vector3f          axis;
    Vector3f          veye;
    Vector3f          vlight;
    
  public:
    virtual void display()
    {
      if (texture) {
        gc.matrixMode(GL_PROJECTION);
        gc.loadIdentity();
        gc.perspective(18.0, (double)width() / (double)height(), 0.5, 110.0);
        gc.matrixMode(GL_MODELVIEW);

        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gc.loadIdentity();

        gc.enable(GL_CULL_FACE);
        gc.cullFace(GL_FRONT);

        gc.enable(GL_LIGHTING);

        gc.enable(GL_DEPTH_TEST);
        gc.color(1.0f, 1.0f, 1.0f);
        gc.lookAt(veye[0], veye[1], veye[2], 0.0, 0.0, 0.0, 0.0, 6.0, 0.0);

        gc.translate(0.0f, 0.0f, 0.0f);

        gc.rotate(angle, axis[0], axis[1], axis[2]);

        OpenGLLight light(GL_LIGHT0);
        light.position(vlight[0] , vlight[1], vlight[2], 0.0f);
    
        GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat blue[]  = {0.0f, 0.0f, 1.0f, 1.0f};
        GLfloat shininess[] = {100.0};
        OpenGLMaterial material(GL_FRONT);
        material.diffuse(blue);
        material.specular(white);
        material.shininess(shininess);
            
        texture -> draw(&gc);
      }
    }

    virtual void resize(int w, int h)
    {
       if (w == 0 || h == 0) {
          return;
       }
       gc.matrixMode(GL_PROJECTION);
       gc.loadIdentity();
       gc.perspective(18.0, (double)w / (double)h, 0.5, 110.0); 

       gc.matrixMode(GL_MODELVIEW);
    }


    virtual void initialize()
    {
      gc.enable(GL_DEPTH_TEST);
      gc.enable(GL_TEXTURE_2D);
    }

  public:
    SimpleView(OpenGLBulletinBoard* parent, const char* name, Args& args)
    :OpenGLView(parent, name, args)
    ,angle(120)
    {
    } 

    ~SimpleView()
    {
    }

    void createTexture(const char* filename)
    {
      try {
        texture = NULL;
        texture = new OpenGLTexturedSphere(filename, 2.5f, 40, 40); 
        printf("created texture %s\n", filename);
      } catch (Exception& ex) {
       caught(ex);
      }
    }

    void setAngle(int angle)
    {
       this->angle = angle;
    } 

    void setAxisPosition(int x, int y, int z)
    {
       axis[0]= x;
       axis[1]= y;
       axis[2]= z;
    } 
    void setEyePosition(int x, int y, int z)
    {
       veye[0]= x;
       veye[1]= y;
       veye[2]= z;
    } 
 
    void setLightPosition(int x, int y, int z)
    {
       vlight[0]= x;
       vlight[1]= y;
       vlight[2]= z;
    } 
 
  };
  //Inner class ends.
	
private:
  SmartPtr<OpenGLBulletinBoard>   bboard;
  SmartPtr<SimpleView>      view;
  SmartPtr<BulletinBoard>   controlPane;
  SmartPtr<LabeledTrackBar> rotator;
  SmartPtr<AxisPositioner>  axisPositioner;
  SmartPtr<EyePositioner>   eyePositioner;
  SmartPtr<LightPositioner> lightPositioner;
  SmartPtr<FileOpenDialog>  fileDialog;

  virtual void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe->type == ConfigureNotify) {
      configureNotify(event);
    } else {
      //Add here your own nofity event handlers.
    }
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

    if (controlPane && view) {
      Dimension CONTROLPANE_WIDTH = eyePositioner->getPositionerWidth();

      Dimension w = width-CONTROLPANE_WIDTH -4;

      view ->reshape(0, 0, w, height); 
      controlPane->reshape(w-4, 0, w+2,  height);
      controlPane->unmap();
      controlPane->map();
    }
  }

  void angleChanged(Action& action)
  {
    int angle = rotator->getPosition();
    //printf("angleChanged %d\n", angle);
    if (view) {
      view->setAngle(angle);
      view->sendExposeEvent();
    }
  }

  void axisChanged(Action& action)
  {
    int x, y, z;
    axisPositioner->getPosition(x, y, z);
    //printf("AxisChanged x=%d, y=%d, z=%d\n", x, y, z);
    if (view) {
       view->setAxisPosition(x, y, z);
       view->sendExposeEvent();
    }
  }

  void eyeChanged(Action& action)
  {
    int x, y, z;
    eyePositioner->getPosition(x, y, z);
    // printf("eyeChanged x=%d, y=%d, z=%d\n", x, y, z);
    if (view) {
       view->setEyePosition(x, y, z);
       view->sendExposeEvent();
    }
  }

  void lightChanged(Action& action)
  {
    int x, y, z;
    lightPositioner->getPosition(x, y, z);
    //printf("lightChanged x=%d, y=%d, z=%d\n", x, y, z);
    if (view) {
       view->setLightPosition(x, y, z);
       view->sendExposeEvent();
    }
  }

  virtual void fileMenu(Action& action)
  {
    int menuId = action.getClientIntData(); 
    printf("fileMenu %d\n", menuId);
    if (menuId == OPEN) {
      fileDialog->popup();
    } else {
      OpenGLMainView::fileMenu(action);
    }
  }
 
  void fileOpen(const char* filename)
  {
    try {
      if (view) {
        printf("filename: %s\n", filename);
        view->createTexture(filename);
        view->sendExposeEvent();
        const char* name = filename;
        const char* slash = strrchr(filename, '/');
        if (slash) {
          name = ++slash;
        }
        const char* appName = getAppName();
        char title[PATH_MAX];
        sprintf(title, "%s - %s", name, appName);
        setTitle(title); 
        flush();
      }
    } catch (Exception& ex) {
      caught(ex);
    }
  }

  void ok(Action& action)
  {
    try {  
      const char* filename  = fileDialog->getFileName();
      fileDialog->popdown();
      fileOpen(filename);

    } catch (Exception& ex) {
       caught(ex);
    } 
  }

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, args) 
  {
    Args ar;
    bboard = new OpenGLBulletinBoard(this, "", ar);
 
    ar.reset();

    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    view = new SimpleView(bboard, "", ar);

    ar.reset();
    //ar.set(XmNorientation, XmVERTICAL);
    controlPane = new BulletinBoard(bboard, "", ar);

    CompoundString rcs("Rotator:[-180,-180]");
    ar.reset();
    ar.set(XmNx,      0);
    ar.set(XmNy,      0);
    ar.set(XmNwidth, 200);
    ar.set(XmNheight, 80);
    ar.set(XmNvalue, 120);
    ar.set(XmNminimum, -180);
    ar.set(XmNmaximum, 180);
    ar.set(XmNtitleString, rcs);
    rotator = new LabeledTrackBar(controlPane, "", ar);
    rotator->addCallback(XmNvalueChangedCallback, this,
           (Callback)&MainView::angleChanged, NULL);

    int axisX = -10;
    int axisY = -50;
    int axisZ =  80;

    ar.reset();
    ar.set(XmNx, 0);
    ar.set(XmNy, 100);
    ar.set(XmNminimum, -100);
    ar.set(XmNmaximum,  100);
    axisPositioner = new AxisPositioner(controlPane, "", ar);
    axisPositioner->setPosition(axisX, axisY, axisZ);
    view->setAxisPosition(axisX, axisY, axisZ);

    axisPositioner->addCallback(XmNpositionChangedCallback, this,
           (Callback)&MainView::axisChanged, NULL);
    int ph = Positioner::getPositionerHeight();

    int eyeX = 0;
    int eyeY =18;
    int eyeZ = 4;
    ar.reset();
    ar.set(XmNx, 0);
    ar.set(XmNy, 100+ph+4);
    ar.set(XmNminimum, -200);
    ar.set(XmNmaximum,  200);
    eyePositioner = new EyePositioner(controlPane, "", ar);
    eyePositioner->setPosition(eyeX, eyeY, eyeZ);
    view->setEyePosition(eyeX, eyeY, eyeZ);

    eyePositioner->addCallback(XmNpositionChangedCallback, this,
           (Callback)&MainView::eyeChanged, NULL);

    int lightX = -31;
    int lightY = -10;
    int lightZ =  2;
    ar.reset();
    ar.set(XmNx, 0);
    ar.set(XmNy, 100+ph*2+4);
    ar.set(XmNminimum, -200);
    ar.set(XmNmaximum,  200);
    lightPositioner = new LightPositioner(controlPane, "", ar);
    lightPositioner->setPosition(lightX, lightY, lightZ);
    view->setLightPosition(lightX, lightY, lightZ);
    lightPositioner->addCallback(XmNpositionChangedCallback, this,
           (Callback)&MainView::lightChanged, NULL);

    this -> addEventHandler(StructureNotifyMask, this, 
         (Handler)&MainView::structureNotify, NULL);
    ar.reset();
    fileDialog = new FileOpenDialog(this, "FileOpenDialog", ar);
    fileDialog  -> getOkButton()
                -> addCallback(XmNactivateCallback, this,
                          (Callback)&MainView::ok, NULL); 
     //Open an inital image file.
     const char* filename = "./images/ven0aaa2.jpg";
     fileOpen(filename);
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
    args.set(XmNheight, 480);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

