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
 *      SelectionDialog.h
 *
 *****************************************************************************/

//2017/10/16 Modified to use create method.

#pragma once

#include <oz++/motif/DialogView.h>
#include <oz++/motif/SelectionBox.h>

namespace OZ {

class SelectionDialog :public DialogView {
private:
  SmartPtr<SelectionBox> selectionbox;

public:
  SelectionDialog(View* parent, const char* name, Args& args)
  :DialogView()
  {
    SelectionDialog::create(parent, name, args);
  }

  SelectionDialog()
  :DialogView()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    DialogView::create(parent, name, args);
    Args ar; 
    selectionbox = new SelectionBox(this, "selectionbox",
            ar.setIfNotSet(XmNwidth, 400)
              .setIfNotSet(XmNheight,400));
  }

  ~SelectionDialog()
  {
  }

  void manage()
  {
    View::manage();
    if(selectionbox) {
      selectionbox -> manage();
    }  
  }

  SelectionBox* getSelectionBox()
  {
    return selectionbox;
  }
};

}


