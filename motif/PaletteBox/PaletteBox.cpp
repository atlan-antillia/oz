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
 *  PaletteBox.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/RadioBox.h>
#include <oz++/motif/CompoundStringList.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<BulletinBoard>  bboard;
  SmartPtr<RadioBox>       paletteb;

  void  valueChanged(Action& action)
  {
    int id = action.getClientIntData();
    printf("Button%d valueChanged\n", id);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
    :ApplicationView(applet, name, args)
  {
    static const char *items[] = {
      "Up", "Down", "Left", "Right" };
    Args ar;
    bboard = new BulletinBoard(this, "bboard", ar);

    ar.reset();
    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNbuttons,     (XtArgVal)items);
    ar.set(XmNbuttonCount, XtNumber(items));
    ar.set(XmNlabelType,   XmPIXMAP);
    ar.set(XmNpaletteBehavior,  TRUE);
    ar.set(XmNspacing,  0);
    paletteb = new RadioBox(bboard, "radiob", ar);
    paletteb -> addCallbackToKids(XmNvalueChangedCallback, this,
        (Callback)&MainView::valueChanged, NULL);
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
    const char* appclass = argv[0];
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


