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
 *  OpenCVMainView.h
 *
 *****************************************************************************/

//2017/05/03

#pragma once

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/MainWindow.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/MenuBar.h>
#include <oz++/motif/CascadeMenuButton.h>
#include <oz++/motif/HelpDialog.h>
#include <oz++/motif/FileDialog.h>

#include <oz++/opencv/OpenCVObject.h>
#include <oz++/opencv/OpenCVApplication.h>
#include <oz++/opencv/OpenCVDefaultMenu.h>

namespace OZ {

class OpenCVMainView :public ApplicationView {

protected:
  SmartPtr<MainWindow>        mainw;
  SmartPtr<BulletinBoard>     bboard;
  SmartPtr<MenuBar>           menubar;

  int            menuId;
  SmartPtr<CascadeMenuButton> file;
  SmartPtr<CascadeMenuButton> edit;
  SmartPtr<CascadeMenuButton> help;

  SmartPtr<HelpDialog> helpDialog;

  virtual void fileNew(Action& action)
  {
    helpDialog -> setMessage("Default menu: New");
    helpDialog -> manage();
  }

  virtual void fileOpen(Action& action)
  {
    helpDialog -> setMessage("Default menu: Open");
    helpDialog -> manage();
  }

  virtual void fileSave(Action& action)
  {
    helpDialog -> setMessage("Default menu: Save");
    helpDialog -> manage();
  }

  virtual void fileSaveAs(Action& action)
  {
    helpDialog -> setMessage("Default menu: SaveAs");
    helpDialog -> manage();
  }

  void exit(Action& action)
  {
    Application* applet = getApplication();
    applet-> terminate();
  }

  virtual void fileMenu(Action& action)
  {
    menuId = action.getClientIntData();
    printf("fileMenu %d\n", menuId);

    switch(menuId) {
    case NEW: 
        fileNew(action);
        break;
        
    case OPEN: 
        fileOpen(action);
        break;
       
    case SAVE: 
        fileSave(action);
        break;
        
    case SAVE_AS: 
        fileSaveAs(action);
        break;
        
    case EXIT: 
        confirm(action); //ApplicationView
        break;
    }
  }

  virtual void editMenu(Action& action)
  {
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
        helpDialog -> setMessage("Default menu: SelectAll");
        helpDialog -> manage();
        break;
    case FIND:
        helpDialog -> setMessage("Default menu: Find");
        helpDialog -> manage();
        break;
    }
  }

  virtual void onHelp(Action& action)
  {
    helpDialog -> setMessage("Default menu: Help");
    helpDialog -> manage();
  }

  virtual void helpMenu(Action& action)
  {
    menuId = action.getClientIntData();
    printf("helpMenu %d\n", menuId);
    switch (menuId) {
    case HELP:
        onHelp(action);
        break;

    case VERSION:
        {
         char message[1024];
         snprintf(message, CountOf(message), "OpenCV Sample on %s", XmVERSION_STRING);
         helpDialog -> setMessage(message);
         helpDialog -> manage();
        }
        break;

    case OPENCV_VERSION:
        {
         char message[1024];
         snprintf(message, CountOf(message), "Major Version: %d  Minor Version: %d  		    SubMinor_Version: %d",
           CV_MAJOR_VERSION,
           CV_MINOR_VERSION,
           CV_SUBMINOR_VERSION);

           helpDialog -> setMessage(message);
           helpDialog -> manage();
        }
        break;
    }
  }

  //Please redine your own addSomthingPulldowMenu if you need.

  virtual void addFilePulldownMenu()
  { 
    MenuBar* menubar = getMenuBar();
    getFileMenuButton() -> addItems(FileItems, XtNumber(FileItems),
                    this, (Callback)&OpenCVMainView::fileMenu);
  }

  virtual void addEditPulldownMenu()
  {
    MenuBar* menubar = getMenuBar();
    getEditMenuButton() -> addItems(EditItems, XtNumber(EditItems),
                    this, (Callback)&OpenCVMainView::editMenu);
  }

  virtual void addHelpPulldownMenu()
  {
    MenuBar* menubar = getMenuBar();
    getHelpMenuButton() -> addItems(HelpItems, XtNumber(HelpItems),
                    this, (Callback)&OpenCVMainView::helpMenu);
  }

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

  virtual void createNotify(Event& event)
  {
   
  }

public:
  //Constructor
  OpenCVMainView(OpenCVApplication& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    ar.reset();
    mainw = new MainWindow(this, "mainw", ar);

    ar.reset();
    menubar = new MenuBar(mainw, "menubar", ar);
  
    ar.reset();
    ar.set(XmNmnemonic, 'F');
    file = new CascadeMenuButton(menubar, "File", ar);
    addFilePulldownMenu();

    ar.reset();
    ar.set(XmNmnemonic, 'E');
    edit = new CascadeMenuButton(menubar, "Edit", ar);
    addEditPulldownMenu();

    ar.reset();
    ar.set(XmNmnemonic, 'H');
    help = new CascadeMenuButton(menubar, "Help", ar);
    addHelpPulldownMenu();
    
    ar.reset();
    ar.set(XmNmarginHeight,2);
    ar.set(XmNmarginWidth, 2);
    bboard = new BulletinBoard(mainw, "bboard", ar);

    CompoundString cs("OpenGL message");
    CompoundString title("Help Dialog");

    ar.reset();
    ar.set(XmNdialogTitle, title); 
    ar.set(XmNmessageString, cs);
    helpDialog = new HelpDialog(this, "", ar);

   this -> addEventHandler(CreateNotify, this, 
         (Handler)&OpenCVMainView::createNotify, NULL);

   this -> addEventHandler(StructureNotifyMask, this, 
         (Handler)&OpenCVMainView::structureNotify, NULL);

    //Call configure layaout method. 
    configure();
  }
  
  ~OpenCVMainView()
  {
  }

  MenuBar*    getMenuBar()
  {
    return menubar;
  }

  CascadeMenuButton* getFileMenuButton()
  {
     return file;
  }

  CascadeMenuButton* getEditMenuButton()
  {
     return edit;
  }

  CascadeMenuButton* getHelpMenuButton()
  {
     return help;
  }

  MainWindow* getMainWindow()
  {
    return mainw;
  }

  BulletinBoard* getBulletinBoard()
  {
    return bboard;
  }
};

}

