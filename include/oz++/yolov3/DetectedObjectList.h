/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  DetectedObjectList.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/TableView.h>
#include <oz++/motif/ScrolledTableView.h>

namespace OZ {

class DetectedObjectList :public ScrolledTableView {
private:
  int        numColumns = 0;
  TableView* tablev     = nullptr;

public:
  DetectedObjectList(View* parent, const char* name, Args& args)
  :ScrolledTableView()
  {
    static ColumnData columnData[]  = {
      {"id", 30, XmALIGNMENT_END}, 
      {"object", 70, XmALIGNMENT_BEGINNING},
      {"score", 40, XmALIGNMENT_END   },
      {"x", 40, XmALIGNMENT_END      },
      {"y", 40, XmALIGNMENT_END   },
      {"w", 40, XmALIGNMENT_END      },
      {"h", 40, XmALIGNMENT_END      },
	};

    numColumns = CountOf(columnData);

    Args ar;
    int w = args.get(XmNwidth);
    int h = args.get(XmNheight);

    ar.reset();

//  	ar.set(XmNtableColumnHeight, 20);
    ar.set(XmNwidth, w);
    ar.set(XmNheight, h);
    ar.set(XmNtableNumColumns, numColumns);
    ar.set(XmNtableColumnData, (XtArgVal)columnData);
    ar.set(XmNtableNumRows, 10);
    ar.set(XmNtableRowWidth, 60);
    ar.set(XmNtableShowColumnHeader,TRUE);
    ar.set(XmNtableShowRowHeader,FALSE);
    ar.set(XmNtopAttachment,    XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM);
    ar.set(XmNleftAttachment,   XmATTACH_FORM);

    create(parent, "listv", ar);
    tablev = getTableView();
    tablev -> removeAllItems();    	

  }

  void addItems(const char** items, size_t num)
  {
    if (items != nullptr && num == numColumns) {
      tablev->addItems(items, num);
    }
  } 
};

}
