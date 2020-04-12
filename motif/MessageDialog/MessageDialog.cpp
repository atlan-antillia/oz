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
 *  MessageDialog.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/MessageDialog.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/CompoundStringList.h>
#include <oz++/SmartPtrs.h>

namespace OZ {

class Template :public MessageDialog {
private:
  int                    num;
  SmartPtrs<PushButton*>  pushb;

  void    createButton(View* box, XmString* xmsl)
  {
    Args  ar;
    char name[20];
    for(int i = 0; i<num; i++) {
      ar.reset();
      sprintf(name, "button%d", i);
      if(xmsl) ar.set(XmNlabelString, xmsl[i]);
        pushb[i] = new PushButton(box, name, ar);
     }   
     box -> set(XmNdefaultButton, pushb[0]);
  }

public:
  Template(View* parent, const char* name, Args& args)
  :MessageDialog(parent, name,
                args.set(XmNdialogType, XmDIALOG_TEMPLATE))
  {
    XmString* xmsl = (XmString*)args.get(XmNbuttons);
    num   = (int)args.get(XmNbuttonCount);
    pushb = new PushButton*[num];
    if(num) {
      createButton(getMessageBox(), xmsl);
    }
  }

  ~Template()
  {
    for(int i = 0; i<num; i++) {
      delete pushb[i];
    }
   }
};


class MainView :public ApplicationView {
private:
  SmartPtr<Template>  dialog;
  SmartPtr<PushButton> pushb;

  void  confirm(Action& action)
  {
    dialog -> manage();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    pushb = new PushButton(this, "Show ConfirmationDialog", ar);
    pushb -> addCallback(XmNactivateCallback, this,
      (Callback)&MainView::confirm, NULL);

    static const char* items[] = {"Yes", "No", "Cancel", "Help"};
    CompoundStringList csl(items, XtNumber(items));

    CompoundString cs("File already exists. Do you overwrite it?");
    ar.reset();
    ar.set(XmNmessageString, cs);
    ar.set(XmNbuttons, csl);

    ar.set(XmNbuttonCount, XtNumber(items));
    dialog = new Template(this, "File", ar);
  }

  ~MainView()
  {
  }
};
}

//
int main(int argc, char** argv)
{
  const char*  appclass = argv[0];
  Application applet(appclass, argc, argv);
  try {
    Args args;
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

