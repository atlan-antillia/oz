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
 *  TextureSubImage.cpp
 *
 *****************************************************************************/

#include <oz++/StringT.h>
#include <oz++/motif/XtSmartPtr.h>
#include <oz++/motif/FileDialog.h>
#include <oz++/motif/PopupMenu.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLView.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLTexture2D.h>

namespace OZ {

class MainView :public OpenGLMainView {

private:
  //Inner class starts.
  class SimpleView :public OpenGLView {
    OpenGLGC        gc;
    SmartPtr<OpenGLTexture2D> texture;

  public:
    virtual void display()
    {
      gc.clearColor(1.0, 1.0, 1.0, 1.0);
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.color(1.0, 1.0, 1.0);
//      gc.rotate(5.0, 0.5, 1.0, 0.25);


      gc.normal(0.0, 0.0, 1.0);
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

      createTexture("./images/Flower.png");
      //createTexture("./images/Dragonfly.png");

      setSubImage("./images/Nanten.png", 0, 20, 20);
      setSubImage("./images/Ring.png", 0, 480, 100);
    }

  public:
    void createTexture(const char* filename)
    {
      texture = new OpenGLTexture2D();
      texture -> bind();
      texture -> parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      texture -> parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      texture -> env(GL_TEXTURE_ENV_MODE, GL_MODULATE); 

      try {
        texture -> imageFromPNGFile(filename);

      } catch (Exception& ex) {
        caught(ex);
      }
    }

  public:
    void setSubImage(const char* filename, GLint level, GLint offset, GLint yoffset)
    {
       try {
        texture -> subImageFromPNGFile(filename, level, offset, yoffset);

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
  SmartPtr<SimpleView> view;
  SmartPtr<PopupMenu>  popupMenu;
  SmartPtr<FileDialog> saveDialog;
  int menuId;
  StringT<char> fileName;

  typedef enum {
    SAVEAS_JPG = 300,
    SAVEAS_PNG,
    HELP
  } POPUPMENU_ID;

  void  edit(Action& action)
  {
    menuId = action.getClientIntData();

    switch(menuId)
    {
      case SAVEAS_JPG: {
       printf("SaveAs JPG\n");
       saveDialog -> manage();
      }
      break;

      case SAVEAS_PNG:{
       printf("SaveAs PNG\n");
       saveDialog -> manage();
      }
      break;

      case HELP: {
       printf("Not implemented yet\n");
      }
      break;
    }
  }

  void ok(Action& action)
  {
    XmFileSelectionBoxCallbackStruct* cbs =
                    (XmFileSelectionBoxCallbackStruct*)action.getCallData();
    StringT<char> charString;
    CompoundString cs(cbs->value, charString);
    printf("charString %s\n", (const char*)charString);
    const char* filename = (const char*)charString;
    printf("filename: %s\n", filename);
    if (menuId == SAVEAS_JPG && 
        charString.endsWithIgnoreCase(".jpg")) {
      fileName = filename;
      view -> save(filename);
      saveDialog -> unmanage();
    }
    else if (menuId == SAVEAS_PNG && 
        charString.endsWithIgnoreCase(filename, ".png")) {
      fileName = filename;
      view -> save(filename);
      saveDialog -> unmanage();
    }  else {
      printf("Invalid filename %s\n", filename);
    }
  }
	
  void cancel(Action& action)
  {
    saveDialog -> unmanage();
  }

public:
  MainView(OpenGLApplication& applet, const char* name, Args& args)
  :OpenGLMainView(applet, name, args),
  menuId(0)
  {
    Args ar;
    ar.set(XmNopenGLBufferType, (XtArgVal)getBufferType());
    view = new SimpleView(this, "", ar);

    static MenuItem items[] = {
      {PUSHBUTTON,  "saveasJPG",  "SaveAS JPG",  SAVEAS_JPG,   ZERO},
      {PUSHBUTTON,  "saveasPNG",  "SaveAS PNG",  SAVEAS_PNG,  ZERO},
      {PUSHBUTTON,  "help",       "Help",  HELP, ZERO}
     };

    ar.reset();
    popupMenu = new PopupMenu(view, "popupm", ar);
    popupMenu -> addItem(items, XtNumber(items),
                    this, (Callback)&MainView::edit);

    view -> addEventHandler(ButtonPressMask,  popupMenu,
                                (Handler)&PopupMenu::popup, NULL);
    ar.reset();
    saveDialog = new FileDialog(view, "FileSaveDialog", ar);
    saveDialog -> getFileBox()
               -> addCallback(XmNokCallback, this,
                 (Callback)&MainView::ok, NULL);
    saveDialog -> getFileBox()
               -> addCallback(XmNcancelCallback, this,
                 (Callback)&MainView::cancel, NULL);
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
