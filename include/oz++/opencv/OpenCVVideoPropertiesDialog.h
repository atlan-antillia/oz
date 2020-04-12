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
 *  OpenCVVideoPropertiesDialog.h
 *
 *****************************************************************************/


#pragma

#include <oz++/motif/CustomDialog.h>
#include <oz++/motif/TableView.h>

namespace OZ {

class OpenCVVideoPropertiesDialog :public CustomDialog {
private:
  SmartPtr<TableView> table;
  int                 numColumns;
  int                 numRows;
public:
  OpenCVVideoPropertiesDialog(View* parent, const char* name, Args& args)
  :CustomDialog(parent, name, args.setIfNotSet(XmNwidth, 340)
                                  .setIfNotSet(XmNheight,460))
  {
    Color color(this);
    Pixel white = color.whitePixel();

    static ColumnData columnData[]  = {
      {"Value", 160, XmALIGNMENT_BEGINNING}, 
      };

    static RowData rowData[]  = {
      {"Pos msec",    120, XmALIGNMENT_END}, 
      {"Pos frames",    0, XmALIGNMENT_END}, 
      {"Pos avi ratio", 0, XmALIGNMENT_END}, 
      {"Frame width",   0, XmALIGNMENT_END}, 
      {"Frame height",  0, XmALIGNMENT_END}, 
      {"Fps",           0, XmALIGNMENT_END}, 
      {"FourCC",        0, XmALIGNMENT_END}, 
      {"Frame count",   0, XmALIGNMENT_END}, 
      {"Format",        0, XmALIGNMENT_END}, 
      {"Mode",          0, XmALIGNMENT_END}, 
      {"Brightness",    0, XmALIGNMENT_END}, 
      {"Contrast",      0, XmALIGNMENT_END}, 
      {"Saturation",    0, XmALIGNMENT_END}, 
      {"Hue",           0, XmALIGNMENT_END}, 
      {"Gain",          0, XmALIGNMENT_END}, 
      {"Exposue",       0, XmALIGNMENT_END}, 
	};
    Form* workArea = getWorkArea();

    numColumns = XtNumber(columnData);
    numRows    = XtNumber(rowData);

    Args ar;
    ar.reset();
    ar.set(XmNtopAttachment,    XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM);
    ar.set(XmNleftAttachment,   XmATTACH_FORM);

    ar.set(XmNtableColumnHeight, 18);
    ar.set(XmNtableNumColumns, numColumns);
    ar.set(XmNtableColumnData, (XtArgVal)columnData);
    ar.set(XmNtableNumRows, numRows);
    ar.set(XmNtableRowData, (XtArgVal)rowData);

    table = new TableView(workArea, "", ar);
    table -> removeAllItems();    	
    static const char* buttons[] = { "Close", };
    CompoundStringList csl(buttons, SizeOf(buttons) );
    addButton(csl);
    PushButton* closeb = getCloseButton();
    closeb->addCallback(XmNactivateCallback, this,
          (Callback)&OpenCVVideoPropertiesDialog::close, NULL);
  }
	
  ~OpenCVVideoPropertiesDialog() 
  {
  }

  void close(Action& action)
  {
    popdown();
  }

  PushButton* getCloseButton()
  {
    return getNthButton(0);
  }

  void set(double* values, int count)
  {
    if (values && count < numRows) {
      table -> removeAllItems();

      for (int i = 0; i< count; i++) {
        char item[100];
        sprintf(item, "%6.2f", values[i]);
        printf("set %s\n", item);
        table -> setItem(i, 0, item);
      }
    }
  }

};

}
