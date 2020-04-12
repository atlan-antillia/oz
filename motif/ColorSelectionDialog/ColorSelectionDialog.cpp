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
 *  ColorSelectionDialog.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>

#include <oz++/motif/PushButton.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/ColorSelectionDialog.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<BulletinBoard>    bboard;
  SmartPtr<PushButton>       pushb;
  SmartPtr<TextField>        textf;
  SmartPtr<ColorSelectionDialog>   dialog;

  void   cancel(Action& action)
  {
    dialog->popdown();
  }

  void    ok(Action& action)
  {
    Pixel pixel = dialog -> getPixel();

    printf("fileName: %lx\n", pixel);
    char value[128];
    sprintf(value, "Pixel: %06lx", pixel);
    textf -> setString(value);
    dialog -> popdown();
  }

  void   activate(Action& action)
  {
    dialog -> popup();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    bboard = new BulletinBoard(this, "", ar);
    ar.reset();

    CompoundString cs("Show ColorSelectionDialog");
    ar.set(XmNlabelString, cs);
    ar.set(XmNx, 10);
    ar.set(XmNy, 10);
    pushb = new PushButton(bboard, "pushb", ar);
    pushb -> addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::activate, NULL);
    ar.reset();
    ar.set(XmNx, 10);
    ar.set(XmNy, 60);
    ar.set(XmNwidth, 512);
    textf = new TextField(bboard, "", ar);

    ar.reset();
    ar.set(XmNwidth,  340);
    ar.set(XmNheight, 340);
    dialog = new ColorSelectionDialog(this, "ColorSelectionDialog", ar);
    dialog -> getOkButton()
           -> addCallback(XmNactivateCallback, this,
                  (Callback)&MainView::ok, NULL);
    dialog -> getCancelButton()
           -> addCallback(XmNactivateCallback, this,
                  (Callback)&MainView::cancel, NULL);
  }

  ~MainView()
  {
  }
};
}


int main(int argc, char** argv)
{
  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNwidth, 600);
    args.set(XmNheight,200);

    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

