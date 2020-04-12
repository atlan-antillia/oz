/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      ApplicationView.h
 *
 *****************************************************************************/

//2015/01/18 Update to add terminate method to break the event loop of Application class.

#pragma once

#define _MOTIF_ 

#include <oz++/StringT.h>
#include <oz++/File.h>
#include <oz++/ModuleFileName.h>
#include <oz++/motif/ZException.h>

#include <oz++/motif/TopLevelView.h>
#include <oz++/motif/ConfirmationDialog.h>
#include <oz++/motif/CompoundString.h>

#include <oz++/motif/ErrorDialog.h>

namespace OZ {
  
class ConfirmationDialog;

class ApplicationView :public TopLevelView {
private:
  SmartPtr<ConfirmationDialog> confirmd;
  StringT<char> appname;

protected:
  void exit(Action& action)
  {
    Application* applet = getApplication();
    applet-> terminate();
  }

  void confirm(Action& action)
  {
    confirmd -> manage();
  }
  
public:
  ApplicationView(Application& applet, const char* name, Args& args)
  :TopLevelView(applet, name, 
                applicationShellWidgetClass,   
                args.set(XmNdeleteResponse, XmDO_NOTHING)) 
  {
    appname = name;

    ErrorDialog::createStaticDialog(this);

    Atom protocol = XInternAtom (applet.getDisplay(), 
        "WM_DELETE_WINDOW", FALSE);
    addWMProtocolCallback(protocol, this,
                         (Callback)&ApplicationView::confirm, NULL);
    //2016/12/28
    CompoundString title("Confirmation");
    CompoundString string("Do you really want to exit?");

    Args ar;
    ar.set(XmNdialogTitle, title);
    ar.set(XmNdialogStyle, XmDIALOG_APPLICATION_MODAL);
    ar.set(XmNmessageString, string);

    confirmd = new ConfirmationDialog(this, "confirm", ar);
    confirmd -> getMessageBox()
           -> addCallback(XmNokCallback, this, 
      (Callback)&ApplicationView::exit, NULL);
  }


  ~ApplicationView()
  {
  }

  ConfirmationDialog* getConfirmDialog()
  {
    return confirmd;
  }
	
  //2015/01/18	
  void terminate()
  {
    Application* applet = getApplication();
    applet-> terminate();
  }

  static void showErrorDialog(const char* text) 
  {
    printf("Error: %s\n", text);
    //Call static show method of ErrorDialog
    ErrorDialog::show(text); 
  }

  const char* getAppName()
  {
    return (const char*)appname;
  } 
  
  void setTitle(const char* title)
  {
    set(XmNtitle, title);
  }
  
  StringT<char> getFullpath(const char* filename)
  {
    ModuleFileName module;
    return module.getFullpath(filename);
  }

  StringT<char> getFullpath(const StringT<char>& filename)
  {
    return ApplicationView::getFullpath((const char*)filename);
  }
};
}


