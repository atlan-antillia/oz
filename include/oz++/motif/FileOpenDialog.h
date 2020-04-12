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
 *  FileOpenDialog.h
 *
 *****************************************************************************/

//2018/04/10 Updated to support refresh operation.

#pragma once

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/CustomDialog.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/FileListView.h>
#include <oz++/motif/FlatButton.h>
#include <oz++/motif/TextField.h>
#include <oz++/StrDef.h>
#include <oz++/motif/xpm/upfolder.xpm>
//const char** upfolder_xpm;

namespace OZ {

class FileOpenDialog :public CustomDialog {
private:
  SmartPtr<BulletinBoard>   bboard;  
  SmartPtr<Pixelmap>        upfolder;
  SmartPtr<PushButton>      upButton;
  SmartPtr<TextField>       folderName;
  SmartPtr<FileListView>    fileListView;
  SmartPtr<TextField>       fileName;
public:
  enum {OK_BUTTON =0 , CANCEL_BUTTON, REFRESH_BUTTON, FILTER_BUTTON};
    
private:
  void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    if (xe -> type == MapNotify || xe -> type == ConfigureNotify) {
      layout();
    }
  }
    
  void layout()
  {
    int margin = 2;
    Dimension w = bboard -> width();
    Dimension h = bboard -> height();
    //printf("w=%d h=%d\n", w, h);
    Dimension th = 0;
    folderName -> get(XmNheight, (XtArgVal)&th);
    upButton     -> resize(0, 0, th, th, 1); 
    folderName   -> resize(th, 0, w-th-margin, th, 1);      
    fileListView -> resize(0, th+margin, w-margin, h-th*2 -margin-margin, 1);
    fileName     -> resize(0, h-th-margin-margin, w-margin, th, 1);
  }

private:
  void upFolder(Action& action)
  {
    char* dir = folderName -> getString();
    printf("upFolder current dir=[%s]\n", dir);
    fileName -> setString((char*)"");
    char filename[PATH_MAX];

    char* slash = strrchr(dir, '/');
    if (slash != dir) {
        *slash = '\0';
        strcpy(filename, dir);
    } else {
      if (strcmp(dir, "/") != 0) {
        slash++;
        *slash= '\0';
        }
      dprintf("dir=[%s]\n", dir);
      strcpy(filename, dir);
    }
    dprintf("Upfolder %s\n", filename);
    folderName   -> setString(filename);
    fileListView -> listup(filename);
    fileListView -> addCallback(XmNactivateCallback, this,
        (Callback)&FileOpenDialog::selected, NULL);
  }

  //2018/04/10
  void refresh(Action& action)
  {
    char* dir = folderName -> getString();
    printf("refresh current dir=[%s]\n", dir);
    fileName -> setString((char*)"");
    //folderName   -> setString(filename);
    fileListView -> listup(dir);
    fileListView -> addCallback(XmNactivateCallback, this,
        (Callback)&FileOpenDialog::selected, NULL);
  }

private:
  void selected(Action& action)
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
    printf("selected filename=[%s]\n", filename);
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
          //printf("dir=[%s]\n", dir);
          strcpy(filename, dir);
        }
      }
      folderName   -> setString(filename);
      fileListView -> listup(filename);
      fileListView -> addCallback(XmNactivateCallback, this,
        (Callback)&FileOpenDialog::selected, NULL);
    } else {
        fileName -> setString(filename);
    }
    XtFree(item); 
    XtFree(dir);
  }

public:
  FileOpenDialog(View* parent, const char* name, Args& args)
  :CustomDialog()
  {
    FileOpenDialog::create(parent, name, args);
  }

  FileOpenDialog()
  :CustomDialog()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    CustomDialog::create(parent, name, args.setIfNotSet(XmNwidth, 600)
                                  .setIfNotSet(XmNheight, 400));
  
    try {
    Form* workArea = getWorkArea();

    Args ar;
    ar.reset();
    ar.set(XmNtopAttachment,    XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM);
    ar.set(XmNleftAttachment,   XmATTACH_FORM);

    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNmarginHeight, 0);
    ar.set(XmNmarginWidth,  0);
    bboard = new BulletinBoard(workArea, "", ar);

    Pixel bg;
    bboard -> get(XmNbackground, (XtArgVal)&bg);
    upfolder= new Pixelmap(getDisplay(), bg, upfolder_xpm);
    ar.reset();
    ar.set(XmNlabelPixmap, upfolder->get());
    ar.set(XmNlabelType, XmPIXMAP_AND_STRING);
    ar.set(XmNwidth, 24);
    ar.set(XmNhighlightThickness, 1);
    ar.set(XmNshadowThickness, 0);
    upButton = new PushButton(bboard, "", ar);
    upButton->addCallback(XmNactivateCallback, this,
        (Callback)&FileOpenDialog::upFolder, NULL);

    ar.reset();
    ar.set(XmNeditable, FALSE);
    folderName = new TextField(bboard, "", ar);  
    
    ar.reset();
    ar.set(XmNnameWidth, 360);
    ar.set(XmNchangedTimeWidth, 180); 
    ar.set(XmNsizeWidth, 150);
    ar.set(XmNsizeUnitKB, TRUE);
    fileListView = new FileListView(bboard, "", ar);
    
    ar.reset();
    fileName = new TextField(bboard, "", ar);
    
    addEventHandler(StructureNotifyMask, this,
          (Handler)&FileOpenDialog::structureNotify, NULL);

    char home[PATH_MAX];
    strcpy(home, getenv("HOME"));

    const char* root = home;
    if (args.has(XmNrootFolderName)) {
      root = (const char*)args.get(XmNrootFolderName);
    }
    folderName -> setString((char*)root);
    
    fileListView -> listup(root);
    fileListView -> addCallback(XmNactivateCallback, this,
        (Callback)&FileOpenDialog::selected, NULL);

    //Add OK, Cancel, Refresh, and Filter buttons.

    static const char* buttons[] = { "OK", "Cancel", "Refresh", "Filter"};
    CompoundStringList csl(buttons, SizeOf(buttons) );
    addButton(csl);

    //FilterButton
    getNthButton(FILTER_BUTTON) -> setSensitive(FALSE);

    //CancelButton
    getNthButton(CANCEL_BUTTON) -> addCallback(XmNactivateCallback, this,
                  (Callback)&PopupView::popdown, NULL);

    //RefreshButton
    getNthButton(REFRESH_BUTTON) -> addCallback(XmNactivateCallback, this,
                  (Callback)&FileOpenDialog::refresh, NULL);

    } catch (OZ::Exception& ex) {
      caught(ex);
    }
  }

  ~FileOpenDialog()
  {
  }

  FileListView* getFileListView()
  {
    return fileListView;
  }

  PushButton* getOkButton()
  {
    return getNthButton(OK_BUTTON);
  }

  PushButton* getCancelButton()
  {
    return getNthButton(CANCEL_BUTTON);
  }

  PushButton* getFilterButton()
  {
    return getNthButton(FILTER_BUTTON);
  }

  PushButton* getRefreshButton()
  {
    return getNthButton(REFRESH_BUTTON);
  }

  const char* getFileName()
  {
    return fileName -> getString();
  }

  const char* getFolderName()
  {
    return folderName -> getString();
  }

  void unmanageFileNameField()
  {
    fileName -> unmanage();
  }
};
}

