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
 *      MultiListBox.h
 *
 *****************************************************************************/

//2015/09/07 Updated

#pragma once

#include <oz++/DirectoryScanner.h>
#include <oz++/DirEntryList.h>
#include <oz++/SmartPtrs.h>
#include <oz++/Exception.h>
#include <oz++/motif/Primitive.h>
#include <Xm/List.h>

#include <Xm/MultiList.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>
#include <oz++/motif/MultiListRow.h>

/*
typedef struct _XmMultiListRowInfo {
     // Used by the XmIList widget.
    XmString * values;
    Pixmap pixmap;
    Boolean selected;

     // Provided for the convience of the application programmer.
    short *sort_id;
    XtPointer data;

     // Private to the XmIList widget (do not modify these).
    short pix_width;
    short pix_height;	
    short height;
    Boolean old_sel_state;
    short pix_depth;	
} XmMultiListRowInfo;
*/


/*
typedef struct _XmMultiListCallbackStruct {
    int reason;		
    XEvent *event;
    String string;
    int column;	
    XmMultiListRowInfo *row; 
    wchar_t *wc_string; 
} XmMultiListCallbackStruct;
*/

namespace OZ {

class MultiListBox :public Primitive {

public:
  MultiListBox(View* parent, const char* name,  Args& args)
  :Primitive(parent, name, xmMultiListWidgetClass, args)
  {
  }

  MultiListBox()
  :Primitive()
  {
  }

  void create(View* parent, const char* name,  Args& args)
  {
    Primitive::create(parent, name, xmMultiListWidgetClass, args);
  }

  XmMultiListRowInfo** getSelectedRows()
  {
    return XmMultiListGetSelectedRows(getWidget());
  }

  void unselectAllItems()
  {
     XmMultiListUnselectAllItems(getWidget());
  }

  void unselectItem(XmMultiListRowInfo* row_info)
  {
    XmMultiListUnselectItem(getWidget(), row_info);
  }

  void toggleRow(short row)
  {
    XmMultiListToggleRow(getWidget(), row);
  }

  void selectItems(XmString item, int column, Boolean notify)
  {
    XmMultiListSelectItems(getWidget(), item, column, notify);
  }

  void deselectItems(XmString item, int column)
  {
    XmMultiListDeselectItems(getWidget(), item, column);
  }

  void selectAllItems(Boolean notify)
  {
    XmMultiListSelectAllItems(getWidget(), notify);
  }
 
  void selectRow(int row, Boolean notify)
  {
    XmMultiListSelectRow(getWidget(), row, notify);
  }

  void deselectRow(int row)
  {
    XmMultiListDeselectRow(getWidget(), row);
  }

  int* getSelectedRowArray(int *num_rows)
  {
    return XmMultiListGetSelectedRowArray(getWidget(), num_rows);
  }

  void makeRowVisible(int row)
  {
    XmMultiListMakeRowVisible(getWidget(), row);
  }
};

}

