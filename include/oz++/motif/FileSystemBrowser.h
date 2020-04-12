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
 *  FileSystemBrowser.h 
 *
 *****************************************************************************/

//2017/05/28  Simple file system browser dialog.
//2017/10/15 Modified to use create method.

#include <oz++/motif/PopupView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/BulletinBoard.h>

#include <oz++/motif/PushButton.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/FolderSelectionBox.h>
#include <oz++/motif/FileListView.h>

namespace OZ {

class FileSystemBrowser : public PopupView {
private:
  SmartPtr<BulletinBoard>         bboard;
  SmartPtr<PushButton>            ok;
  SmartPtr<PushButton>            cancel;
  SmartPtr<TextField>             folderName;
  SmartPtr<TextField>             fileName;
  SmartPtr<FolderSelectionBox>    selectionBox;
  SmartPtr<FileListView>          fileListView;
 
private:
  void fileSelected(Action& action)
  {
    char* dir = folderName -> getString();
    View* sender = (View*) action.getSender();
    int type = 0;
    sender -> get(XmNuserData, (XtArgVal)&type);
    XmString xms = NULL;
    sender -> get(XmNlabelString, (XtArgVal)&xms);
    CompoundString cs(xms);
    char* item = cs.unparse();
    char filename[PATH_MAX];
    if (__endsWith(dir, '/')) {
      sprintf(filename, "%s%s", dir, item);
    } else {
     sprintf(filename, "%s/%s", dir, item);
    }
    dprintf("selected filename=[%s]\n", filename);
    fileName -> setString((char*)"");

    if (type == FileListView::DIR) {
      if (strcmp(item, "..") == 0) {
        char* slash = strrchr(dir, '/');
        if (slash != dir) {
           *slash = '\0';
          strcpy(filename, dir);
        } else {
          if (strcmp(dir, "/") != 0) {
            slash++;
            *slash= '\0';
          }
          //dprintf("dir=[%s]\n", dir);
          strcpy(filename, dir);
        }
      }
      folderName   -> setString(filename);
      fileListView -> listup(filename);
      fileListView -> addCallback(XmNactivateCallback, this,
        (Callback)&FileSystemBrowser::fileSelected, NULL);
    } else {
        fileName -> setString(filename);
    }
    XtFree(item); 
    XtFree(dir);
  }

  void folderSelected(Action& action)
  {
    const char* dir = (const char*)selectionBox -> getFolderName();
    char path[PATH_MAX];

    if (StringT<char>::startsWith(dir, "/") == false) {
      sprintf(path, "/%s", dir);
    } else {
      sprintf(path, "%s", dir);
    }

    printf("path %s\n", path);
    folderName -> setString(path);
    fileListView->listup(path);

    fileName -> setString((char*)"");
    fileListView->addCallback(XmNactivateCallback, this,
         (Callback)&FileSystemBrowser::fileSelected, NULL);
  }

  void resize(Dimension w, Dimension h)
  {
    static int TH = 0;
    if (TH == 0) {
      TH = folderName->get(XmNheight);
    }
    if (bboard && selectionBox && fileListView)  {
      bboard     ->  reshape(0, 0, w, h);
      folderName ->  reshape(0, 0, w, TH);
      selectionBox-> reshape(2, TH, w/2-2, h-TH*3-16);
      selectionBox->sendExposeEvent();
      fileListView-> reshape(w/2, TH, w/2-2, h-TH*3-16);
      fileListView->sendExposeEvent();
      fileName -> reshape(0, h-TH*2-16, w, TH);
      ok        -> reshape(w/2, h-TH-6, 100, TH);
      cancel    -> reshape(w/2+120, h-TH-4, 120, TH);
    }
  }

  virtual void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe->type == ConfigureNotify) {
      XConfigureEvent* xce = (XConfigureEvent*)xe;
      resize(xce->width, xce->height);
    }
  }

public:
  void popup(XtGrabKind kind=XtGrabNonexclusive)
  {
    bboard->manage();
    PopupView::popup(kind);
  }

public:
  ////////////////////////////////////////////////////////////////
  // Constructor
  //
  FileSystemBrowser(View* parent, const char* name, Args& args)
  :PopupView()
  {
    FileSystemBrowser::create(parent, name, args);
  }

  FileSystemBrowser()
  :PopupView()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    //PopupView::create(parent, name, xmDialogShellWidgetClass, args)
    PopupView::create(parent, name, transientShellWidgetClass, args);
  
    set(XmNdeleteResponse, XmUNMAP);

    Args ar;
    ar.set(XmNwidth, 550);
    ar.set(XmNheight, 400);
    bboard = new BulletinBoard(this, "", ar);

    ar.reset();
    folderName = new TextField(bboard, "", ar);

    ar.reset();
    selectionBox = new FolderSelectionBox(bboard, "", ar);
    selectionBox -> setSelectCallback(this,
            (Callback)&FileSystemBrowser::folderSelected, NULL);

    ar.reset();
    fileListView = new FileListView(bboard, "", ar);
    fileListView->addCallback(XmNactivateCallback, this,
         (Callback)&FileSystemBrowser::fileSelected, NULL);

    ar.reset();
    fileName = new TextField(bboard, "", ar);

    CompoundString okcs("OK");
    ar.reset();
    ar.set(XmNlabelString, okcs);
    ok = new PushButton(bboard, "", ar);
    
    CompoundString cancelcs("Cancel");
    ar.reset();
    ar.set(XmNlabelString, cancelcs);
    cancel = new PushButton(bboard, "", ar);

    addEventHandler(StructureNotifyMask, this,
      (Handler)&FileSystemBrowser::structureNotify, NULL);
  }
 
  ~FileSystemBrowser()
  {
  }

  void okButtonCallback(const char* name, CommonObject* object,
               Callback callback, void* data)
  {
    ok->addCallback(name, object, callback, NULL);
  }

  void cancelButtonCallback(const char* name, CommonObject* object,
               Callback callback, void* data)
  {
    cancel->addCallback(name, object, callback, NULL);
  }

  void getFolderName(StringT<char>& string)
  {
    char* s = folderName -> getString();
    string = s;
    XtFree(s);
  } 

  void getFileName(StringT<char>& string)
  {
    char* s = fileName -> getString();
    string = s;
    XtFree(s);
  } 
};
}

