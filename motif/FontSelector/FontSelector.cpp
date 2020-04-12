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
 *  ColorListView.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/FontSelector.h>

namespace OZ {

class MainView :public ApplicationView {

private:
  SmartPtr<Form>          form; 
  SmartPtr<FontSelector>  fontSelector;
  SmartPtr<Label>         label;

  void selected(Event& event)
  {
    if (event.isEnterKey() ||  event.isButtonPressed() ) {
      event.processTraversal();

      Pixel pixel = (Pixel)event.getClientData();
      char text[128];
      sprintf(text, "Pixel=%lx", pixel);
      CompoundString cs(text);
      label -> set(XmNlabelString, cs);
    }
  }


public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    form = new Form(this, "form", ar);
 
    ar.reset();
    ar.set(XmNwidth, 400);
    ar.set(XmNheight, 400);
    ar.set(XmNtopAttachment, XmATTACH_FORM); 
    ar.set(XmNleftAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment, XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);

    fontSelector = new FontSelector(form, "fontselector", ar);
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
    args.set(XmNgeometry, "500x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

