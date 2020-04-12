/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  FileSystemBrowser.cpp
 *
 *****************************************************************************/

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/FileSystemBrowser.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/TextField.h>

namespace OZ {

class MainView :public ApplicationView {

private:
  SmartPtr<FileSystemBrowser>  fileSystemBrowser;
  SmartPtr<PushButton>         pushb;
  SmartPtr<BulletinBoard>      bboard;
  SmartPtr<TextField>          folderName;
  SmartPtr<TextField>          fileName;

  void    ok(Action& action)
  {
    fileSystemBrowser->popdown();
    StringT<char> fname;
    fileSystemBrowser -> getFolderName(fname);

    StringT<char> name;
    fileSystemBrowser -> getFileName(name);

    printf("folderName: %s\n", (const char*)fname);
    folderName -> setString((const char*)fname);

    printf("fileName: %s\n", (const char*)name);
    fileName -> setString((const char*)name);
  }

  void   cancel(Action& action)
  {
    fileSystemBrowser -> popdown();
  }

  void   activate(Action& action)
  {
    if (fileSystemBrowser == NULL) {
      Args ar;
      ar.reset();
      ar.set(XmNwidth, 500);
      ar.set(XmNheight, 400);
      fileSystemBrowser = new FileSystemBrowser(this, "FileSystemBrowser", ar);

      fileSystemBrowser->okButtonCallback(XmNactivateCallback,
             this,
             (Callback)&MainView::ok, NULL);

      fileSystemBrowser->cancelButtonCallback(XmNactivateCallback,
             this,
             (Callback)&MainView::cancel, NULL);
    }
    fileSystemBrowser-> popup();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    bboard = new BulletinBoard(this, "", ar);
    ar.reset();
    ar.set(XmNy, 20);
    ar.set(XmNwidth, 200);
    ar.set(XmNheight, 35);
    pushb = new PushButton(bboard, "Show FileSystemBrowser", ar);
    pushb->addCallback(XmNactivateCallback, this,
      (Callback)&MainView::activate, NULL);
    ar.reset();
    ar.set(XmNy,  100);
    ar.set(XmNwidth, 400);
    ar.set(XmNheight, 35);
    folderName = new TextField(bboard, "", ar);
    ar.reset();
    ar.set(XmNy,  140);
    ar.set(XmNwidth, 400);
    ar.set(XmNheight, 35);
    fileName = new TextField(bboard, "", ar);
  }

  ~MainView()
  {
  }
};
}


int main(int argc, char** argv)
{
  const char* rootFolder = "/";
  if (argc == 2) {
    rootFolder = argv[1];
  }
  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNwidth, 600);
    args.set(XmNheight,200);
    args.set(XmNrootFolderName, rootFolder);

    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

