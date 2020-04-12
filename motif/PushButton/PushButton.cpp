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
 *  PushButton.cpp
 *
 *****************************************************************************/

//2015/0/26 Modified to use Xft true type.

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/Rendition.h>
#include <oz++/motif/RenderTable.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<PushButton>  pushb;
  SmartPtr<RenderTable> rtable;

  void   activate(Action& action)
  {
    XmPushButtonCallbackStruct* cbs =
        (XmPushButtonCallbackStruct*)action.getCallData();
    if(cbs->click_count == 2) {
      exit(action);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Color color(this);
    Pixel red = color.allocNamedColor("navy");
 
    //1. Create an ordinay instance of PushButton
    Args ar;
    CompoundString cs("Please click twice this button to exit\n"
                      "終了するには、2回押してください");

    ar.set(XmNlabelType, XmSTRING);
    ar.set(XmNlabelString, cs);
    ar.set(XmNforeground,  red);
    pushb = new PushButton(this, "pushb", ar);

    pushb -> addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::activate, NULL);

    //2. Create an instance of RenderTable based on a Rendition object.
    ar.reset();
    ar.set(XmNfontName, "Times-12");
    ar.set(XmNfontSize, 20);
    Rendition  rendition(pushb, NULL, ar);
    rtable = new RenderTable(&rendition);

    //3. Set the renderTable to the PushBitton instance.
    pushb -> set(XmNrenderTable, rtable);
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
