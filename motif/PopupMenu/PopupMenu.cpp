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
 *  PopupMenu.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/motif/PopupMenu.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  enum {CUT, COPY, PASTE};

  SmartPtr<ScrolledText>  sctext;
  SmartPtr<PopupMenu>     popupmenu;

  void  edit(Action& action)
  {
    int menuId = action.getClientIntData();
    Text* text = sctext->getText();
    switch(menuId)
    {
      case CUT: {
        text->cut(CurrentTime);
      }
      break;

      case COPY:{
        text->copy(CurrentTime);
      }
      break;

      case PASTE: {
        text->paste();
      }
      break;
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    static MenuItem items[] = {
      {PUSHBUTTON,  "cut",   "Cut",    CUT,   ZERO},
      {PUSHBUTTON,  "copy",  "Copy",   COPY,  ZERO},
      {PUSHBUTTON,  "paste", "Paste",  PASTE, ZERO} 
     };

    Args ar;
    ar.set(XmNscrollBarDisplayPolicy, XmVARIABLE);
    sctext = new ScrolledText(this, "sctext", ar);

    ar.reset();
    popupmenu = new PopupMenu(sctext, "popupm", ar);
    popupmenu -> addItem(items, XtNumber(items),
                    this, (Callback)&MainView::edit);

    sctext -> addEventHandler(ButtonPressMask,  popupmenu,
                                (Handler)&PopupMenu::popup, NULL);
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
    args.set(XmNgeometry, (XtArgVal)"300x200");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

