/*
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
 *  DragSource.cpp
 *
 *****************************************************************************/

//2015/08/15
//See http://www.ist.co.uk/motif/books/vol6A/ch-23.fm.html

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Label.h>

namespace OZ {

class MainView :public ApplicationView {
private:
   Atom   TARGETS;
   Atom   MOTIF_EXPORT_TARGETS;
   Atom   COMPOUND_TEXT;
   SmartPtr<Label> label;

public:
  void convertCallback(Action& action)
  {
    printf("MainView::convertCallback \n");
    XmConvertCallbackStruct* ccs = (XmConvertCallbackStruct*)action.getCallData();
 
    if ((ccs->target == MOTIF_EXPORT_TARGETS) ||
        (ccs->target == TARGETS)) {
      Atom* targs = (Atom *) XtMalloc(sizeof(Atom) );
      targs[0] = COMPOUND_TEXT;
      ccs->value = (XtPointer) targs;
      ccs->type  = XA_ATOM;
      ccs->length = 1;
      ccs->format = 32;
      ccs->status = XmCONVERT_MERGE;
    } 
    else if (ccs->target == COMPOUND_TEXT) {
      XmString  cstring;

      printf("COMPOUND_TEXT \n");
      label -> get(XmNlabelString, &cstring);
 
      char* ctext = XmCvtXmStringToCT(cstring);
      char* passtext = strdup(ctext);
      printf("labelString %s\n", ctext); 
      ccs->value  = (XtPointer)passtext;
      ccs->type   = XA_STRING;
      ccs->length = strlen(passtext);
      ccs->format = 8;
      ccs->status = XmCONVERT_DONE;
    }
    else  {
      printf("Unexpected target\n");
     ccs->status = XmCONVERT_REFUSE;
    }
  }

  virtual void buttonPress(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe -> type == ButtonPress) {
      Args ar;
      ar.set(XmNdragOperations, XmDROP_COPY|XmDROP_MOVE);
      label -> dragSource(NULL, xe, ar);
      printf("Label has been set as a dragSource\n");
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Display* display = getDisplay();
    TARGETS = XInternAtom(display, (char*)"TARGETS", FALSE);
    MOTIF_EXPORT_TARGETS = XInternAtom(display, XmS_MOTIF_EXPORT_TARGETS, FALSE);
    COMPOUND_TEXT = XmInternAtom(display, (char*)"COMPOUND_TEXT", FALSE);

    CompoundString cs("Hello world. This is a DragSource.");
    Args ar;
    ar.set(XmNlabelString, cs);
    label = new Label(this, "label", ar);

    label -> addCallback(XmNconvertCallback, this,
          (Callback)&MainView::convertCallback, NULL);
 
    label -> addEventHandler(ButtonPressMask, this,
          (Handler)&MainView::buttonPress, NULL);
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
    args.set(XmNgeometry, "400x100");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

