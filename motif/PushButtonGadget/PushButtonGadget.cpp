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
 *  PushButtonGadget.cpp
 *
 *****************************************************************************/

//2015/01/26 Modified to use Xft true type.

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/PushButtonGadget.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/Rendition.h>
#include <oz++/motif/RenderTable.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  static const size_t         COUNT = 10;
  SmartPtr<RowColumn>         rowColumn; 
  SmartPtr<PushButtonGadget>  pushb[COUNT];
  SmartPtr<RenderTable>       rtable;

  void   activate(Action& action)
  {
    XmPushButtonCallbackStruct* cbs =
        (XmPushButtonCallbackStruct*)action.getCallData();
    printf("activated\n");
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Color color(this);
    Pixel white = color.allocNamedColor("white");
    Pixel navy = color.allocNamedColor("navy");
 
    Args ar;
    ar.reset();
    rowColumn = new RowColumn(this, "", ar);

    ar.reset();
    ar.set(XmNfontName, "Arial");
    ar.set(XmNfontSize, 20);
    rtable = new RenderTable(rowColumn, "", ar);

    for (int i = 0; i< COUNT; i++) {
      char label[256];
      sprintf(label, "PushButtonGadget %d", i);
      CompoundString cs(label);
      ar.reset();
      ar.set(XmNlabelType, XmSTRING);
      ar.set(XmNlabelString, cs);
      ar.set(XmNbackground,  navy);
      ar.set(XmNforeground,  white);

      ar.set(XmNrenderTable, (XtArgVal)rtable->get());

      pushb[i] = new PushButtonGadget(rowColumn, "pushb", ar);

      pushb[i] -> addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::activate, NULL);
    }
  }

  ~MainView()
  {
  }
};

}

//
int main(int argc, char** argv)
{
  const char*  appclass = "PushButtonSample";
  Application applet(appclass, argc, argv);
  try {
    Args args;
    //args.set(XmNgeometry, "400x400");
    MainView view(applet, argv[0], args);
    view.realize();

     applet.run();
  } catch (Exception& ex) {
     caught(ex);
  }
  return 0;
}
