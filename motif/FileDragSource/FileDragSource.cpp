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
 *  FileDragSource.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/SashlessPanedWindow.h>

#include <oz++/motif/PushButton.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/FileSelectionBox.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<SashlessPanedWindow> panedw;
  SmartPtr<TextField>           textf;
  SmartPtr<FileSelectionBox>    fsb;
  StringT<char>     fileName;

  Atom   TARGETS;
  Atom   MOTIF_EXPORT_TARGETS;
  Atom   COMPOUND_TEXT;
  bool   selected;

  void    activate(Action& action)
  {
    StringT<char> string;

    textf -> getString(string);
    printf("%s \n", (const char*)string);

    fsb -> update((const char*)string);

    selected = false;
    fileName = ""; 
  }

  void select(Action& action)
  {
    XmContainerSelectCallbackStruct* cbs =
        (XmContainerSelectCallbackStruct*)action.getCallData();

    const char* folderName = fsb -> getFolderName();
    printf("select %d  folderName=%s\n", 
       cbs ->selected_item_count, folderName);

    int itemCount = cbs -> selected_item_count;
    Container* container = fsb -> getContainer();
    if (itemCount != 1) {
      printf("Please specify one file\n");
      return;
    }

    for (int i = 0; i < itemCount; i++) {
      Widget w =  cbs->selected_items[i];
      if (w == NULL) {
        continue;
      }
   
      IconGadget* gadget = (IconGadget*)container->lookup(w);
      if (gadget) {
        XmString xms;
        gadget -> get(XmNlabelString, &xms);
        StringT<char> filename;
        CompoundString cs(xms, filename);
        const char* name = (const char*)filename;
        char fullPath[PATH_MAX];
        selected = true; 
        sprintf (fullPath, "%s/%s\n", (const char*)folderName, name);
        printf("selected %s\n", fullPath);
        fileName = fullPath;
      }
    }
  }

  // The default convertProc of XmContainerWidget class seems to 
  // provide a filename only withoug path.

  // The following convertCallback will provide a fullpath name.
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
      //printf("COMPOUND_TEXT \n");
      const char* ctext = (const char*)fileName;
      char* passtext = strdup(ctext);
      int len = 0;
      if (passtext) {
        len = strlen(passtext);
      }
    
      //printf("selected IconGaget %s\n", ctext);
      ccs->value  = (XtPointer)passtext;
      ccs->type   = XA_STRING;
      ccs->length = len; //strlen(passtext);
      ccs->format = 8;
      ccs->status = XmCONVERT_DONE;
      
    }
    else  {
      printf("Unexpected target\n");
      ccs->status = XmCONVERT_REFUSE;
    }
  }

  virtual void motionNotify(Event& event)
  {
   // try {
   //   FileAttributes attr((const char*)fileName);
   //   Boolean isDirectory = attr.isDirectory();

      XEvent* xe = event.getXEvent();
      if (selected == true &&
    //      isDirectory == FALSE &&
          xe -> type == MotionNotify &&
          xe -> xmotion.state & Button1Mask) {
      
        Args ar;
        ar.set(XmNdragOperations, XmDROP_COPY|XmDROP_MOVE);
        fsb -> getContainer()
            -> dragSource(NULL, xe, ar);
        printf("Container has been set as a dragSource\n");
      }
    //} catch (Exception& ex) {
      //caught(ex);
    //}
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args),
   fileName(""),
   selected(false)
  {
    Display* display = getDisplay();
    TARGETS = XInternAtom(display, (char*)"TARGETS", FALSE);
    MOTIF_EXPORT_TARGETS = XInternAtom(display, XmS_MOTIF_EXPORT_TARGETS, FALSE);
    COMPOUND_TEXT = XmInternAtom(display, (char*)"COMPOUND_TEXT", FALSE);

    Args ar;
    ar.set(XmNorientation, XmVERTICAL);
    panedw = new SashlessPanedWindow(this, "", ar);

    ar.reset();
    textf  = new TextField(panedw, "", ar);

    ar.reset();
    ar.set(XmNfolderPixmapFile, "../xpm/folder.xpm");
    ar.set(XmNfilePixmapFile,   "../xpm/file.xpm");
    ar.set(XmNincludeFolder, FALSE);

    fsb = new FileSelectionBox(panedw, "FileSelectionBox", ar);

    const char* rootFolder = getenv("HOME");
    if (args.has(XmNrootFolderName)) {
      rootFolder = (const char*)args.get(XmNrootFolderName);
    }
    fsb -> buildFileList(rootFolder);
    Container* container = fsb -> getContainer();
    container -> addCallback(XmNselectionCallback, this,
                (Callback)&MainView::select, NULL);
    container -> addEventHandler(ButtonPressMask|ButtonMotionMask, this,
          (Handler)&MainView::motionNotify, NULL);

    container -> addCallback(XmNconvertCallback, this,
          (Callback)&MainView::convertCallback, NULL);

    textf -> setString(rootFolder);
    textf -> addCallback(XmNactivateCallback, this,
          (Callback)&MainView::activate, NULL);
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

    const char* rootFolder = getenv("HOME");
    printf("RootFolder %s\n", rootFolder);

    if (argc == 2) {
      rootFolder = argv[1];
    }
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNwidth, 660);
    args.set(XmNheight,400);
    args.set(XmNrootFolderName, rootFolder);

    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }

  return 0;
}

