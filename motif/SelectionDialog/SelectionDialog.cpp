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
 *  SeletionDialog.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/SelectionDialog.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/CompoundStringList.h>

namespace OZ {

class ColorSelection :public SelectionDialog {
private:
  char*  colorname;

  void    selected(Action& action)
  {
     XmSelectionBoxCallbackStruct* cbs =
                 (XmSelectionBoxCallbackStruct*)action.getCallData();
     CompoundString cs(cbs->value);
     if(colorname) {
        XtFree(colorname);
     }
     cs.get(&colorname);
     printf("%s\n", colorname);
  }

public:
  ColorSelection(View* parent, const char* name, Args& args)
  :SelectionDialog(parent, name, args)
  {
    colorname = NULL;
    static const char*  items[] = {
            "white", "gray", "yellow", "green", "cyan",
            "blue",  "red",  "brown", "navy", "black" };
    CompoundStringList csl(items, XtNumber(items));

    SelectionBox* selb = getSelectionBox();
    Args ar;
    ar.set(XmNlistVisibleItemCount, 5);
    ar.set(XmNlistItemCount,  XtNumber(items));
    ar.set(XmNlistItems,  csl);

    selb -> set(&ar);
    selb -> addCallback(XmNokCallback, this, 
                  (Callback)&ColorSelection::selected, NULL);
  }

  const char* getColorName()
  {
    return colorname;
  }
};


class MainView :public ApplicationView {
private:
  SmartPtr<ColorSelection> dialog;
  SmartPtr<PushButton>     pushb;

  void  confirm(Action& action)
  {
    dialog -> manage();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
    :ApplicationView(applet, name, args)
  {
    Args ar;
    pushb = new PushButton(this, "Click me to show ColorSelectionDialog.", ar);
    pushb -> addCallback(XmNactivateCallback, this,
      (Callback)&MainView::confirm, NULL);

    ar.reset();
    dialog = new ColorSelection(this, "Color", ar);
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

