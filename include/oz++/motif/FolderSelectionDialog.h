/******************************************************************************
 *
 * Copyright (c) 2015-2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  FolderSelectionDialog.h
 *
 *****************************************************************************/

//2015/08/29
//2017/10/15 Update to use create method.

#pragma once

#include <oz++/motif/FolderSelectionBox.h>
#include <oz++/motif/SashlessPanedWindow.h>
#include <oz++/motif/CustomDialog.h>

namespace OZ {

class FolderSelectionDialog :public CustomDialog {
public:
  enum {OK_BUTTON =0 , CANCEL_BUTTON, FILTER_BUTTON, HELP_BUTTON};

private:
  SmartPtr<SashlessPanedWindow> panedw;
  SmartPtr<TextField>           textf;
  SmartPtr<FolderSelectionBox> selectionBox;

  void selected(Action& action)
  {
    const char* path = (const char*)selectionBox -> getFolderName();
    printf("path %s\n", path);
    textf -> setString(path);
  }

public:
  FolderSelectionDialog(View* parent, const char* name, Args& args)
  :CustomDialog()
  {
    FolderSelectionDialog::create(parent, name, args);
  }

  FolderSelectionDialog()
  :CustomDialog()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    CustomDialog::create(parent, name, args);
  
    Form* workArea = getWorkArea();
    Args ar;
    ar.set(XmNtopAttachment,    XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM);
    ar.set(XmNleftAttachment,   XmATTACH_FORM);
    ar.set(XmNorientation, XmVERTICAL);
    panedw = new SashlessPanedWindow(workArea, "", ar);

    ar.reset();
    textf = new TextField(panedw, "", ar);
    textf -> setEditable(False);

    ar.reset();

    const char* rootFolder = "/";
    if (args.has(XmNrootFolderName)) {
      rootFolder = (const char*)args.get(XmNrootFolderName);
      if (rootFolder) {
        ar.set(XmNrootFolderName, rootFolder);
      }
    }
    selectionBox = new FolderSelectionBox(panedw, "", ar);
  
    selectionBox -> setSelectCallback(this,
            (Callback)&FolderSelectionDialog::selected, NULL);

    //Add OK, Cancel, Filter, Help buttons.

    static const char* buttons[] = { "OK", "Cancel", "Filter", "Help" };
    CompoundStringList csl(buttons, SizeOf(buttons) );
    addButton(csl);

    //FilterButton
    getNthButton(FILTER_BUTTON) -> setSensitive(False);

    //CancelButton
    getNthButton(CANCEL_BUTTON) -> addCallback(XmNactivateCallback, this,
                  (Callback)&PopupView::popdown, NULL);

    //HELPButton
    getNthButton(HELP_BUTTON) -> setSensitive(False);
  }

  ~FolderSelectionDialog()
  {
  }

  void select(Action& action)
  {
    const char* folderName = (const char*)action.getClientData();
    printf("select folderName %s\n", folderName);
  }

  FolderSelectionBox* getFolderSelectionBox()
  {
    return selectionBox;
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

  PushButton* getHelpButton()
  {
    return getNthButton(HELP_BUTTON);
  }

  void getFolderName(StringT<char>& name)
  {
    selectionBox -> getFolderName(name);
  }

  const char* getFolderName()
  {
    return (const char*)selectionBox -> getFolderName();
  }

  void getRootedFolderName(StringT<char>& name)
  {
    selectionBox -> getRootedFolderName(name);
  }

  StringT<char> getRootedFolderName()
  {
    return selectionBox -> getRootedFolderName();
  }

};
}


