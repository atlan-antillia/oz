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
 *      Clipboard.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/View.h>
#include <Xm/CutPaste.h>

namespace OZ {
  
class Clipboard :public CommonObject {
private:
  Display* display;
  Window   window;
  
public:
  Clipboard(View* view)
  {
    display = view -> getDisplay();
    window  = view -> getWindow();
  }

  void cancelCopy(long itemId)
  {
    XmClipboardCancelCopy(display, window, itemId);
  }


  int  copy(long itemId, char* format, XtPointer buffer, 
    unsigned long length, long privateId, long* dataId)
  {
    int rc;
    while ((rc = XmClipboardCopy(display, window, itemId, format, buffer,
      length, privateId, dataId)) == ClipboardLocked);
    return rc;
  }


  int  copyByName(long dataId, XtPointer buffer, 
    unsigned long length, long privateId)
 {
   int rc;
   while ((rc = XmClipboardCopyByName(display, window, dataId, buffer, 
      length, privateId)) == ClipboardLocked);
    return rc;
  }

  int  endCopy(long itemId)
  {
    int rc;
    while ((rc = XmClipboardEndCopy(display, window, itemId)) 
      == ClipboardLocked);
    return rc;
  }

  int  endRetrieve()
  {
    int rc;
    while ((rc = XmClipboardEndRetrieve(display, window))
      == ClipboardLocked);
    return rc;
  }

  int  inquireCount(int* count, unsigned long* maxLength)
  {
    int rc;
    while ((rc = XmClipboardInquireCount(display, window, 
      count, maxLength)) == ClipboardLocked);
    return rc;
  }


  int  inquireFormat(int indx, char* buffer, 
    unsigned long len, unsigned long* realLen)
  {
    int rc;
    while ((rc = XmClipboardInquireFormat(display, window, indx, buffer, 
      len, realLen)) == ClipboardLocked);
    return rc;
  }

  int  inquireLength(char* format, unsigned long* len)
  {
    int rc;
    while ((rc = XmClipboardInquireLength(display, window, 
      format, len)) == ClipboardLocked);
    return rc;
  }

  int  inquirePendingItems(char* format, 
    XmClipboardPendingList* list, unsigned long* count)
  {
    int rc;
    while ((rc = XmClipboardInquirePendingItems(
                       display, window, format, list, count))
      == ClipboardLocked);
    return rc;
  } 


  int  lock()
  {
    return XmClipboardLock(display, window);
  } 

  int  retrieve(char* format, XtPointer buffer, unsigned long length,
    unsigned long *outlength, long* privateId)
  {
    int rc;
    while ((rc = XmClipboardRetrieve(display, window, 
      format, buffer, length, outlength, privateId))
       == ClipboardLocked);
    return rc;
  }


  int  registerFormat(char* format, int length)
  {
    int rc;
    while ((rc = XmClipboardRegisterFormat(display, format,
                       length)) == ClipboardLocked);
    return rc;
  } 


  int  startCopy(XmString label, Time time, View* view, 
    XmCutPasteProc proc, long* itemId)
  {
    int rc;
    while ((rc = XmClipboardStartCopy(display, window, label, time,
      view->getWidget(), proc, itemId)) 
      == ClipboardLocked);
    return rc;
  }

  int  startRetrieve(Time time)
  {
    int rc;
    while ((rc = XmClipboardStartRetrieve(display, window,
                       time)) == ClipboardLocked); 
    return rc; 
  }

  int  unlock(int allLevels)
  {
    int rc;
    while ((rc = XmClipboardUnlock(display, window,
      allLevels)) == ClipboardLocked);
    return rc;
  }

  int  withdrawFormat(int dataId)
  {
    int rc;
    while ((rc = XmClipboardWithdrawFormat(display, window,
      dataId)) == ClipboardLocked);
    return rc;
  }

};

}

