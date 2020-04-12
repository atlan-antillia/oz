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
 *  DropReceiver.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/DropSite.h>
#include <oz++/motif/DropTransfer.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  Atom            COMPOUND_TEXT;
  SmartPtr<Label>          label;
  SmartPtr<DropSite>       dropSite;
  SmartPtr<DropTransfer>   dropTransfer;

  void drop(Action& action) 
  {
    XmDropProcCallback dropData =
          (XmDropProcCallback)action.getCallData();
    if(dropData -> dropAction == XmDROP &&
       dropData -> operation == XmDROP_COPY) {
       printf("DROP and DROP_COPY\n");
       dropTransfer->start(dropData->dragContext);
    } else {
      printf("drop cancel\n");
      dropTransfer->cancel(dropData->dragContext);
    }
  }


  void move(Transfer& transfer) 
  {
    printf("move\n");
    Atom* type   = transfer.getType();
    if(*type == COMPOUND_TEXT) {
      printf("type: COMPOUND_TEXT\n");
      String value = (String)transfer.getValue();
      XmString xms = XmCvtCTToXmString(value);
      label -> set(XmNlabelString, (XtArgVal)xms);
    }
  }


public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args) 
  {
    Args ar;
    CompoundString cs("Drop Receiver");
    ar.set(XmNlabelString, cs);
    label = new Label(this, "receiver", ar);

    COMPOUND_TEXT = XmInternAtom(getDisplay(), (char*)"COMPOUND_TEXT", FALSE);
    Atom targets[1];
    targets[0] = COMPOUND_TEXT;
    ar.reset();
    ar.set(XmNimportTargets, (XtArgVal)targets);
    ar.set(XmNnumImportTargets, 1);
    ar.set(XmNdropSiteOperations, XmDROP_COPY);

    dropSite = new DropSite(label, &ar);
    dropSite -> registerDropProc(this, (Callback)&MainView::drop, NULL);

    XmDropTransferEntryRec entry[1];
    entry[0].target  = COMPOUND_TEXT;
    ar.reset();
    ar.set(XmNdropTransfers, (XtArgVal)entry);
    ar.set(XmNnumDropTransfers, 1);
    dropTransfer = new DropTransfer(label, &ar);
    dropTransfer -> registerTransferProc(this, (Callback)&MainView::move, NULL);
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
    args.set(XmNgeometry, "300x100");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

