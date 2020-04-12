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
 *  FileSelection.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/SashlessPanedWindow.h>

#include <oz++/motif/PushButton.h>
#include <oz++/motif/TextField.h>
//#include <oz++/motif/FileDialog.h>
#include <oz++/motif/FileSelectionBox.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<PanedWindow>      panedw;
  SmartPtr<TextField>        textf;
  SmartPtr<FileSelectionBox> fsb;

  void    activate(Action& action)
  {
    StringT<char> string;

    textf -> getString(string);
    printf("%s \n", (const char*)string);
    fsb -> clear();
    fsb -> buildFileList((const char*)string);
  }

  void selected(Action& action)
  {
    XmContainerSelectCallbackStruct* cbs =
        (XmContainerSelectCallbackStruct*)action.getCallData();
    const char* folderName = fsb -> getFolderName();

    int itemCount = cbs -> selected_item_count;
    Container* container = fsb -> getContainer();
 
    for (int i = 0; i < itemCount; i++) {
      Widget w =  cbs->selected_items[i];
      IconGadget* selected = (IconGadget*)container->lookup(w);
      if (selected) {
        XmString xms;
        selected -> get(XmNlabelString, &xms);
        StringT<char> filename;
        CompoundString cs(xms, filename);
        const char* name = (const char*)filename;
        char fullPath[PATH_MAX];

        sprintf (fullPath, "%s/%s\n", (const char*)folderName, name);
        printf("selected %s\n", fullPath);
      }
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    ar.set(XmNorientation, XmVERTICAL);

    panedw = new SashlessPanedWindow(this, "", ar);

    ar.reset();
    textf  = new TextField(panedw, "", ar);

    ar.reset();
    ar.set(XmNfolderPixmapFile, "../xpm/folder.xpm");
    ar.set(XmNfilePixmapFile,   "../xpm/file.xpm");
    fsb = new FileSelectionBox(panedw, "FileSelectionBox", ar);
    fsb -> getContainer()
        -> addCallback(XmNselectionCallback, this,
                (Callback)&MainView::selected, NULL);

    const char* rootFolder = getenv("HOME");
    if (args.has(XmNrootFolderName)) {
      rootFolder = (const char*)args.get(XmNrootFolderName);
    }
    fsb -> buildFileList(rootFolder);
    textf -> setString(rootFolder);
    textf -> addCallback(XmNactivateCallback, this,
          (Callback)&MainView::activate, NULL);
  }

  ~MainView()
  {
  }
};
}


int main(int argc, char** argv)
{
  const char* rootFolder = getenv("HOME");
  printf("RootFolder %s\n", rootFolder);

  if (argc == 2) {
    rootFolder = argv[1];
  }
  try {
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

