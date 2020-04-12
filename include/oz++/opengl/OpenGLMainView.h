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
 *  OpenGLMainView.h
 *
 *****************************************************************************/

#pragma once

//2015/08/01 Modified to use MainWindow and DefaultMenu.
//2017/08/20 Modified to use getClientIntData instead of getClientData.

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/MainWindow.h>
#include <oz++/motif/MenuBar.h>
//#include <oz++/motif/DefaultMenu.h>
#include <oz++/motif/CascadeMenuButton.h>
#include <oz++/motif/HelpDialog.h>
#include <oz++/opengl/OpenGLApplication.h>
#include <oz++/opengl/OpenGLVisualBuffer.h>
#include <oz++/opengl/OpenGLGC.h>
#include <oz++/opengl/OpenGLDefaultMenu.h>

namespace OZ {

class OpenGLMainView :public ApplicationView {

private:
  OpenGLContext* context; //shallow copy
  Colormap     colormap;
  Visual*      visual;
  int          depth;
  int          bufferType;

protected:
  SmartPtr<MainWindow>  mainw;
  SmartPtr<MenuBar>     menubar;

  int            menuId;
  SmartPtr<CascadeMenuButton> file;
  SmartPtr<CascadeMenuButton> edit;
  SmartPtr<CascadeMenuButton> help;

  SmartPtr<HelpDialog> helpDialog;

  void exit(Action& action)
  {
    Application* applet = getApplication();
    applet-> terminate();
  }

  virtual void fileMenu(Action& action)
  {
    //menuId = (int)action.getClientData(); 
    menuId = action.getClientIntData(); 
    printf("fileMenu %d\n", menuId);

    switch(menuId) {
    case NEW: 
        helpDialog -> setMessage("Default menu: New");
        helpDialog -> manage();
        break;
        
    case OPEN: 
        helpDialog -> setMessage("Default menu: Open");
        helpDialog -> manage();
        break;
       
    case SAVE: 
        helpDialog -> setMessage("Default menu: Save");
        helpDialog -> manage();
        break;
        
    case SAVE_AS: 
        helpDialog -> setMessage("Default menu: SaveAs");
        helpDialog -> manage();
        break;
        
    case EXIT: 
        confirm(action); //ApplicationView
        break;
    }
  }

  virtual void editMenu(Action& action)
  {
    //menuId = (int)action.getClientData();
    menuId = action.getClientIntData();
    printf("editMenu %d\n", menuId);
    switch(menuId) {
    case CUT:
        helpDialog -> setMessage("Default menu: Cut");
        helpDialog -> manage();
        break;
    case COPY:
        helpDialog -> setMessage("Default menu: Copy");
        helpDialog -> manage();
        break;
    case PASTE:
        helpDialog -> setMessage("Default menu: Paste");
        helpDialog -> manage();
        break;
    case SELECT_ALL:
        helpDialog -> setMessage("Default menu: Find");
        helpDialog -> manage();
        break;
    case FIND:
        helpDialog -> setMessage("Default menu: Find");
        helpDialog -> manage();
        break;
    }
  }

  virtual void helpMenu(Action& action)
  {
    //menuId = (int)action.getClientData();
    menuId = action.getClientIntData();
    printf("helpMenu %d\n", menuId);
    switch (menuId) {
    case HELP:
        helpDialog -> setMessage("Default menu: Help");
        helpDialog -> manage();
        break;
    case VERSION:
        {
          char message[1024];
          memset(message, 0, sizeof(message));
          sprintf(message, "OpenGL Sample on: %s", XmVERSION_STRING);
        helpDialog -> setMessage(message);
        helpDialog -> manage();
        }
        break;
    case CONTEXT_VERSION:
        {
          char message[1024];
          memset(message, 0, sizeof(message));
          const GLubyte *vendor     = glGetString(GL_VENDOR);
          const GLubyte *renderer   = glGetString(GL_RENDERER);
          const GLubyte *version    = glGetString(GL_VERSION);
          const GLubyte *glsl       = glGetString(GL_SHADING_LANGUAGE_VERSION);

          sprintf(message, 
            "Context Version\n\n" 
            "GL_VENDOR                   :%s\n"
            "GL_RENDERER                 :%s\n"
            "GL_VERSION                  :%s\n"
            "GL_SHADING_LANGUAGE_VERSION :%s\n",
             
             (const char*)vendor,
             (const char*)renderer,
             (const char*)version,
             (const char*)glsl
             );
        helpDialog -> setMessage(message);
        helpDialog -> manage();
        }
        break;
    }
  }

  //2017/09/02
  virtual void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe->type == MapNotify) {
      mapNotify(event);
    } else if (xe->type == ConfigureNotify) {
      configureNotify(event);
    } else {
      //Add here your own nofity event handlers.
    }
    //sendExposeEvent();
  }
  
  virtual void mapNotify(Event& event)
  {

  }

  void configureNotify(Event& event)
  {
    configure();
  }

  //Define Your own resize  method in a subclass derived from this class.
  virtual void resize(Dimension w, Dimension h)
  {
  }

  void configure()
  {
    XRectangle  r;

    //DC dc(this);
    r.x = 0;
    r.y = 0;
    

    Dimension width, height;
    get(XmNwidth, (XtArgVal)&width);
    get(XmNheight, (XtArgVal)&height);

    r.width  = width;
    r.height = height;

    resize(width, height);
  }

public:
  OpenGLMainView(OpenGLApplication& applet, const char* name, Args& args)
  :ApplicationView(applet, name, 
                          args.set(XmNvisual,(XtArgVal)applet.getVisual())
                               .set(XmNdepth,  (XtArgVal)applet.getDepth())
                               .set(XmNcolormap, (XtArgVal)applet.getColormap()) ),
  context(NULL)
  {
    this -> bufferType = OpenGL_DOUBLE_BUFFER;  
    this -> depth = applet.getDepth();
    this -> visual = applet.getVisual();
    this -> colormap  = applet.getColormap();

    if (args.has(XmNopenGLBufferType)) {
    	bufferType = (int)args.get(XmNopenGLBufferType);
    }

    this->context = applet.getOpenGLContext();

    Args ar;
    ar.reset();
    mainw = new MainWindow(this, "mainw", ar);
    ar.reset();
    menubar = new MenuBar(mainw, "menubar", ar);
    ar.reset();
    ar.set(XmNmnemonic, 'F');
    file = new CascadeMenuButton(menubar, "File", ar);
    file -> addItems(FileItems, XtNumber(FileItems),
                    this, (Callback)&OpenGLMainView::fileMenu);

    ar.reset();
    ar.set(XmNmnemonic, 'E');
    edit = new CascadeMenuButton(menubar, "Edit", ar);
    edit -> addItems(EditItems, XtNumber(EditItems),
                    this, (Callback)&OpenGLMainView::editMenu);

    ar.reset();
    ar.set(XmNmnemonic, 'H');
    help = new CascadeMenuButton(menubar, "Help", ar);
    help -> addItems(HelpItems, XtNumber(HelpItems),
                    this, (Callback)&OpenGLMainView::helpMenu);

    
    CompoundString cs("OpenGL message");
    CompoundString title("Help Dialog");

    ar.reset();
    ar.set(XmNdialogTitle, title); //2016/12/26
    ar.set(XmNmessageString, cs);
    helpDialog = new HelpDialog(this, "", ar);
    this -> addEventHandler(StructureNotifyMask, this, 
         (Handler)&OpenGLMainView::structureNotify, NULL);

  }
  
  ~OpenGLMainView()
  {
    context = NULL;
    visual  = NULL;
  }

  OpenGLContext* getOpenGLContext() const 
  {
    return context;
  }    

  int getBufferType()
  {
    return bufferType;	
  }

  int     getDepth()
  {
    return depth;
  }
  
  Visual* getVisual()
  {
    return visual;
  }

  Colormap getColormap()
  {
    return colormap;
  }

  MainWindow* getMainWindow()
  {
    return mainw;
  }
};

}

