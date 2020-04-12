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
 *  LocalizedString.cpp
 *
 *****************************************************************************/

#include <oz++/Locale.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/FontSet.h>

namespace OZ {

class MainView :public ApplicationView {
private:
   SmartPtr<Form>         form;
   SmartPtr<FontSet>      fontSet;
   SmartPtr<Label>        label;
   SmartPtr<PushButton>   pushb;

  void pressed(Action* action)
  {
    terminate();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    form = new Form(this, "", ar);

    CompoundString cs("永遠の終わり　The End of Eternity \n"
                      "火星帝国　Imperial Mars");
    //Create a fontList
    fontSet = new FontSet(getDisplay(),
                                "-*-*-medium-r-normal--24-*-*");

    ar.reset();
    ar.set(XmNfontSize, 20);
    ar.set(XmNlabelString, cs);
    label = new Label(form, "label", ar);

    CompoundString pb("了解/OK");

    ar.reset();
    ar.set(XmNx, 10);
    ar.set(XmNy, 200);
    ar.set(XmNlabelType,   XmSTRING);
    ar.set(XmNlabelString, pb);
    pushb = new PushButton(form, "pushb", ar);
    pushb -> addCallback(XmNactivateCallback,
            this, (Callback)&MainView::pressed, NULL);
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
    Locale locale;

    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

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

