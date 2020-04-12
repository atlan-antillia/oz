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
 *  NotifierBox.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/SashlessPanedWindow.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/motif/Notifier.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/FileDialog.h>

#include <oz++/Pair.h>
#include <oz++/SmartArray.h>

namespace OZ {
	
class MainView  :public ApplicationView {
private:
  typedef enum {
    NEW,
    OPEN,
    SAVE,
    QUIT
  } MENU;

private:
  SmartPtr<SashlessPanedWindow> panedw;
  SmartPtr<RowColumn>           rowColumn;
  SmartArray<Notifier*>         labels;
  SmartPtr<ScrolledText>        scrolledText;
  SmartPtr<FileDialog>          fileDialog;
  MENU          menuId;

  void activate(Event& event)
  {
    View* sender = (View*)event.getSender();

    XEvent* xe = event.getXEvent();
    XmString xms;
    sender -> get(XmNlabelString, (XtArgVal)&xms);
    CompoundString cs(xms);
    char* label = cs.unparse();
    int id = 0;
    sender -> get(XmNuserData, (XtArgVal)&id);
    printf("acitvate name=%s, id=%d\n", label, id);

    XtFree(label);
    menuId = (MENU)id;
    switch(id) {
    case NEW:
       scrolledText->getText() -> setString("");
       break;

    case OPEN:
       fileDialog->manage();
       break;

    case SAVE:
       fileDialog->manage();
       break;

    case QUIT:
       terminate();
    }
  }

  void  open(Action& action)
  {
    XmFileSelectionBoxCallbackStruct* cbs =
           (XmFileSelectionBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->value);
    char* filename;
    cs.get(&filename);
    printf("filename: %s\n", filename);
    if (menuId == OPEN) {
      scrolledText -> getText() -> load(filename);
    }
    if (menuId == SAVE) {
      scrolledText -> getText() -> save(filename);
    }
    XtFree(filename);
    fileDialog ->unmanage();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Color color(this);
    Pixel white = color.whitePixel();

    static const Pair<const char*, MENU> items[] = {
      {"New", NEW},
      {"Open", OPEN},
      {"Save", SAVE},
      {"Quit", QUIT},
    };

    Args ar;
    panedw = new SashlessPanedWindow(this, "", ar);

    ar.reset();
    ar.set(XmNrows, 1);
    ar.set(XmNcolumns, XtNumber(items));
    ar.set(XmNorientation, XmHORIZONTAL);
  	rowColumn = new RowColumn(panedw, "", ar);

    for (int i = 0; i<XtNumber(items); i++) {
      CompoundString cs(items[i].first);
      ar.reset();
      ar.set(XmNlabelString, cs);

      ar.set(XmNbackground, white);
      ar.set(XmNmarginHeight, 4);
      ar.set(XmNmarginWidth, 10);
      ar.set(XmNuserData,  items[i].second);
      ar.set(XmNalignment, XmALIGNMENT_CENTER); //BEGINNING);
 
      labels[i] = new Notifier(rowColumn, "", ar);

      labels[i] -> addEventHandler(ButtonPressMask, this, 
          (Handler)&MainView::activate, NULL);
    }
    ar.reset();
    scrolledText = new ScrolledText(panedw, "", ar);
    ar.reset();
    fileDialog = new FileDialog(this, "filed", ar);
    fileDialog -> getFileBox()
               -> addCallback(XmNokCallback, this,
                      (Callback)&MainView::open, NULL);
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
    args.set(XmNgeometry, "500x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

