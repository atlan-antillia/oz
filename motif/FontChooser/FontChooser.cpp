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
 *  FontChooser.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/PopupView.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/FontPreview.h>

namespace OZ {

class FontChooser :public FontPreview {
private:
  SmartPtr<RowColumn> commandArea;
  SmartPtr<PushButton> ok;
  SmartPtr<PushButton> cancel;
public:
  FontChooser(View* parent, const char* name, Args& args)
  :FontPreview(parent, name, args)
  {
    printf("Chooser\n");
    Args ar;
    ar.set(XmNorientation, XmHORIZONTAL);
    commandArea = new RowColumn(this, "", ar); 

    CompoundString csok("OK");
    ar.reset();
    ar.set(XmNlabelString, csok);
   // ar.set(XmNshowAsDefault, 2);
    ok = new PushButton(commandArea, "OK", ar);
  

    CompoundString cscancel("Cancel");
    ar.reset();
    ar.set(XmNlabelString, cscancel);
    cancel = new PushButton(commandArea, "Cancel", ar);
    commandArea -> set(XmNdefaultButton, ok->getWidget());
    
  }

  ~FontChooser()
  {
  }

  PushButton* getOKButton()
  {
     return ok;
  }

  PushButton* getCancelButton()
  {
     return cancel;
  }

};

class MainView :public ApplicationView {
private:
  SmartPtr<FontChooser> chooser;

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    chooser  = new FontChooser(this, "", ar);
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

