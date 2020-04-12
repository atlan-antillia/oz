/******************************************************************************
 *
 * Copyright (c) 2014-2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      ShellView.h
 *
 *****************************************************************************/

//2017/10/10 Modified to use create method.

#pragma once

#include <oz++/motif/View.h>
#include <oz++/motif/Application.h>

namespace OZ {
  
class ShellView :public View {
private:
  Application* application;
  
protected:
  ShellView()
  :application(NULL) 
  { 
    //Constructor
  }

  ShellView(Application& applet, const char* name, 
    const WidgetClass widgetClass, Args& args)
  :application(&applet) 
  {      
    Widget shell = XtAppCreateShell((char*)name, 
          applet.getAppClass(),
          widgetClass, 
          applet.getDisplay(),
          args.getArgList(), 
          args.count() );
    if (shell == NULL) {
      throw IException("Failed to XtAppCreateShell.");
    }
    setWidget(shell);
  }
  
  ShellView(View* parent, const char* name,
    const WidgetClass widgetClass, Args& args)
  :application(NULL)
  {
    ShellView::create(parent, name, widgetClass, args);
  }

  virtual void create(View* parent, const char* name, 
    const WidgetClass widgetClass, Args& args)
  {
    if (parent == NULL) {
      throw IException("Invalid parent parameter: NULL.");
    }
    Widget popupShell = XtCreatePopupShell((char*)name, widgetClass,
        parent -> getWidget(),
        args.getArgList(),
        args.count() );
    if (popupShell == NULL) {
      throw IException("Failed to XtCreatePopupShell.");
    }
    setWidget(popupShell);
  }

  Application* getApplication() 
  {
    return application;
  }
};

}


