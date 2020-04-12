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
 *  FolderSelectionBox.cpp
 *
 *****************************************************************************/

//2015/08/27

#include <oz++/motif/FolderSelectionBox.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/SashlessPanedWindow.h>
#include <oz++/motif/TextField.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<SashlessPanedWindow> panedw;
  SmartPtr<TextField>           textf;
  SmartPtr<FolderSelectionBox>  fsb;

private:
  void selected(Action& action)
  {
    const char* path = (const char*)action.getClientData();
    textf -> setString(path);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    
    ar.set(XmNorientation, XmVERTICAL);
    panedw = new SashlessPanedWindow(this, "", ar);

    ar.reset();
    textf = new TextField(panedw, "", ar);
    textf -> setEditable(FALSE);

    const char* rootFolder = "/";
    ar.reset();
    if (args.has(XmNrootFolderName)) {
      rootFolder = (const char*)args.get(XmNrootFolderName);
    }   
    ar.set(XmNrootFolderName, rootFolder);
    fsb = new FolderSelectionBox(panedw, "folderSelectionBox", ar);

    fsb -> setSelectCallback(this, 
            (Callback)&MainView::selected, NULL);
  }

  ~MainView()
  {
  }
};
}

//
int main(int argc, char** argv)
{
  const char* rootFolder = "/";
  if (argc == 2) {
    rootFolder = argv[1];
  }
  const char*  appclass = argv[0];
  try {
    
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "600x500");
    args.set(XmNrootFolderName, rootFolder);

    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

