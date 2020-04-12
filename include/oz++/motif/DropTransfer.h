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
 *      DropTransfer.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/View.h>
#include <oz++/motif/Transfer.h>
#include <Xm/DragDrop.h>

namespace OZ {

class DropTransfer :public CommonObject {
private:
  Widget  dropTransfer;
  View*  caller;
  Args*  args;
  Action*  transferAction;

  static void commonTransferProc(Widget widget, XtPointer clientData,
                   Atom* seltype, Atom* type, XtPointer value,
                        unsigned long *length, int format)
  {
    Action* action = (Action*)clientData;
    Transfer transfer(clientData, seltype, type,
                                    value, length, format);
    action -> call(transfer);
  }

public:
  DropTransfer(View* caller1, Args* args1)
  {
    args = new Args();
    args -> copy(args1);

    int num = (int)args->get(XmNnumDropTransfers);
    XmDropTransferEntryRec* entry = new XmDropTransferEntryRec[num];

    char*  ptr1 = (char*)entry;
    char*  ptr2 = (char*)args->get(XmNdropTransfers);
    memcpy(ptr1, ptr2, num*sizeof(XmDropTransferEntryRec));

    args -> put(XmNdropTransfers, (XtArgVal)entry);
    args -> set(XmNtransferProc,
        (XtArgVal)&DropTransfer::commonTransferProc);
    caller         = caller1;
    transferAction = NULL;
  }

  ~DropTransfer()
  {
    if(transferAction) delete transferAction;
    delete args;
  }

  void  start(Widget dragContext)
  {
    dropTransfer = XmDropTransferStart(dragContext,
        args->getArgList(),
        args->count());
  }

  void  cancel(Widget dragContext)
  {
    Args ar;
    ar.set(XmNtransferStatus, XmTRANSFER_FAILURE);
    ar.set(XmNtransferProc,
        (XtArgVal)&DropTransfer::commonTransferProc);
    XmDropTransferStart(dragContext,
      ar.getArgList(), ar.count());
  }

  void  registerTransferProc(CommonObject* callee,
        Callback callback, XtPointer data)
  {
    if(!transferAction) {
      transferAction = new Action(caller, callee, callback, data);
      XmDropTransferEntryRec* entry =
        (XmDropTransferEntryRec*)args->get(XmNdropTransfers);
      int num = (int)args->get(XmNnumDropTransfers);
      for(int i = 0; i<num; i++) {
        if(entry)
          entry[i].client_data = transferAction;
      }
    }
  }

};

}

