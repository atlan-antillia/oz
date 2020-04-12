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
 *  DropSink.cpp
 *
 *****************************************************************************/
//See http://www.ist.co.uk/motif/books/vol6A/ch-23.fm.html

//The XmNdestinationCallback is directly implemented into the List, 
//Container, DrawingArea, Text and TextField widget classes

#include <oz++/motif/ApplicationView.h>
#include <oz++/StringT.h>
#include <oz++/motif/DC.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/RenderTable.h>
#include <oz++/motif/DrawingArea.h>
#include <oz++/motif/ValueTransfer.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  Atom        COMPOUND_TEXT;
  SmartPtr<RenderTable>    rtable;
  StringT<char>            text;
  SmartPtr<DrawingArea>    drawingArea;
  SmartPtr<ValueTransfer>  valueTransfer;

  void transfer(Action& action)
  {
    XmSelectionCallbackStruct *scs =
       (XmSelectionCallbackStruct *) action.getCallData();
    if (valueTransfer) { 
      printf("Target %ld \n", (long)scs -> target);

      if ((scs->target == COMPOUND_TEXT)) {
        printf("COMPOUND_TEXT transfer done; value=%s\n", (const char*)scs->value); 
        valueTransfer -> succeeded();
        text = (const char*)scs -> value;
        printf("New text %s\n", (const char*)text);
        drawingArea -> update();

      } else {
       valueTransfer -> failed(); 
      }
      delete valueTransfer;
      valueTransfer = NULL;
    }
  }

  void destinationCallback(Action& action)
  {
    printf("MainView::destinationCallback \n");

    XmDestinationCallbackStruct* dcs = 
        (XmDestinationCallbackStruct *)action.getCallData() ;

    if (valueTransfer) {
      delete valueTransfer; 
    } 
    valueTransfer = new ValueTransfer(this, dcs->transfer_id);
    valueTransfer -> registerTransferProc(COMPOUND_TEXT, 
          this, (Callback)&MainView::transfer, NULL);
  }

  void expose(Action& action)
  {
    if (text.getLength()>0) {

      CompoundString cs((const char*)text);
      DC dc(drawingArea);
      dc.setRenderTable(rtable->get());
      cs.draw(10, 40, &dc);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args) ,
  text("This is a drawingArea as a DropSink")
  {
    Args ar;

    ar.reset();
    //CompoundString cs("Drop Receiver");
    //ar.set(XmNlabelString, cs);
    drawingArea = new DrawingArea(this, "drawingArea", ar);
    drawingArea -> addCallback(XmNexposeCallback, this,
            (Callback)&MainView::expose, NULL);
    ar.reset();
    ar.set(XmNfontSize, 20);
    ar.set(XmNfontName, "Times-12");
    rtable = new RenderTable(drawingArea, "", ar );

    COMPOUND_TEXT = XmInternAtom(getDisplay(), (char*)"COMPOUND_TEXT", FALSE);

    Atom targets[1];
    targets[0] = COMPOUND_TEXT;
    ar.reset();
    ar.set(XmNimportTargets, (XtArgVal)targets);
    ar.set(XmNnumImportTargets, 1);
    ar.set(XmNdropSiteOperations, XmDROP_COPY);
    drawingArea -> dropSink(ar);
    drawingArea -> addCallback(XmNdestinationCallback, this,
                (Callback)&MainView::destinationCallback, NULL);
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
    args.set(XmNgeometry, "600x100");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

