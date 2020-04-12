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
 *  MessageBox.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/DialogView.h>
#include <oz++/motif/MessageBox.h>
#include <oz++/motif/PushButton.h>

namespace OZ {

class HelperDialog :public DialogView {
private:
  SmartPtr<MessageBox>  messagebox;

public:
  HelperDialog(View* parent, const char* name, Args& args)
  :DialogView(parent, name, args)
  {
    messagebox = new MessageBox(this, "messagebox", args);

    messagebox -> unmanage(XmDIALOG_CANCEL_BUTTON);
    messagebox -> unmanage(XmDIALOG_HELP_BUTTON);
  }

  ~HelperDialog()
  {
  }

  void    manage()
  {
     messagebox -> manage();
  }

  void setMessage(const char* message)
  {
    CompoundString cs(message);
    messagebox -> set(XmNmessageString, cs);
  }
};


class MainView :public ApplicationView {
private:
  SmartPtr<HelperDialog>  helperDialog;
  SmartPtr<PushButton>    pushb;
  
  void  help(Action& action)
  {
    helperDialog -> manage();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    pushb = new PushButton(this, "Show HelperDialog", ar);
    pushb -> addCallback(XmNactivateCallback, this,
        (Callback)&MainView::help, NULL);

    CompoundString cs("Oz++ Class Library for Motif");
    ar.reset(); 
    ar.set(XmNmessageString, cs);
    helperDialog = new HelperDialog(this, "Helper", ar);
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
    Application applet(appclass, argc, argv);

    Args args;
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

