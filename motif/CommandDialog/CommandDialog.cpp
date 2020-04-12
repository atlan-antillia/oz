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
 *  CommandDialaog.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/CommandDialog.h>
#include <oz++/motif/MessageBox.h>
#include <oz++/motif/PushButton.h>

namespace OZ {

class SampleCommand :public CommandDialog {
private:

  void    execute(Action& action) 
  {
    XmCommandCallbackStruct* cbs =
      (XmCommandCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->value);
    char* string;
    cs.get(&string);
    system(string);
    XtFree(string);
  }
  
public:
  SampleCommand(View* parent, const char* name, Args& args)
  :CommandDialog(parent, name, args) 
  {
    Args ar;
    CompoundString cs("Enter UNIX command");
    Command* command = getCommand();
    command -> set(XmNpromptString, cs);

    command -> addCallback(XmNcommandEnteredCallback, this,
                                (Callback)&SampleCommand::execute, NULL);
  }
};


class MainView :public ApplicationView {
private:
  SmartPtr<CommandDialog> commandd;
  SmartPtr<PushButton> pushb;

  void  command(Action& action)
  {
    commandd -> manage();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args) 
  {
    Args ar;
    pushb = new PushButton(this, " Please push me to launch a command dialog. ", ar);
    pushb -> addCallback(XmNactivateCallback, this,
      (Callback)&MainView::command, NULL);

    ar.reset(); 
    commandd= new SampleCommand(this, "CommandDialog", ar);
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

