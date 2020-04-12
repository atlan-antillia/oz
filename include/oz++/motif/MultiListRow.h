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
 *      MultiListRow.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/Exception.h>
#include <Xm/Xm.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>

namespace OZ {

class MultiListRow :public CommonObject {
private:
  int columns;
  int lineupLimit;
  XmMultiListRowInfo *rows;
  
public:
 MultiListRow(int columns, int lineupLimit)
 :columns(columns),
  lineupLimit(lineupLimit),
  rows(NULL)
 { 
    if (columns > 0 && lineupLimit > 0) {
      rows= (XmMultiListRowInfo *) new char[sizeof(XmMultiListRowInfo) * lineupLimit];
      
      for (int i = 0; i < lineupLimit; i++) {
        rows[i].values = (XmString *) new char[columns * sizeof(XmString)];
        for (int j = 0; j < columns; j++) {
          char item[256];
          sprintf(item, "(%d, %d)", i, j);
          XmString xms = XmStringCreateLocalized(item); 
          rows[i].values[j] = xms; 
        }
      }
    } else {
      throw IException("Invalid argument columns=%d lineupLimit=%d", columns, lineupLimit); 
    }
  }
  
public:
  ~MultiListRow()
  {
    for (int i = 0; i<lineupLimit; i++) {
      for (int j = 0; j < columns; j++) {
        XmStringFree(rows[i].values[j]);
      }
      delete rows[i].values;
    }
    delete [] rows;
    rows = NULL;
  }
  
public:
  XmMultiListRowInfo* get()
  {
    return rows;
  }
  
  XmString gerItem(int row, int col)
  {
    XmString item = NULL;
    if (row < lineupLimit && col <columns) {
       item = rows[row].values[col];
    } else {

    }
    return item;
  }

  XmMultiListRowInfo* getRow(int line, int& col)
  {
    XmMultiListRowInfo* row = NULL;
    col = 0;
    if (line < lineupLimit) {
      row = &rows[line];
      col = columns;
    }
    return row;
  }
};
 
}
