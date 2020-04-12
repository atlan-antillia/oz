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
 *      TableView.h
 *
 *****************************************************************************/

//2017/10/16 Modified to use create method.
//2019/06/10 Modified to be able to show or no-show columnHeader and rowHeader. 

#pragma once

#include <string>

#include <vector>
#include <iostream>
#include <fstream>

#include <oz++/CommonObject.h>
#include <oz++/SmartArray.h>
#include <oz++/StringTokenizerT.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/ColumnData.h>
#include <oz++/motif/ColumnHeader.h>
#include <oz++/motif/RowData.h>

#include <oz++/motif/RowHeader.h>

#include <oz++/motif/LabelCell.h>
#include <oz++/motif/PushButtonCell.h>

#include <oz++/motif/Color.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/TableModel.h>

namespace OZ {

#define XmNtableColumnHeight    "_tableColumnHeight"
#define XmNtableColumns         "_tableColumns"
#define XmNtableRows            "_tableRows"
#define XmNtableNumColumns      "_tableNumColumns"
#define XmNtableNumRows         "_tableNumRows"
#define XmNtableColumnData      "_tableColumnData"
#define XmNtablenNumColumnData  "_tableNumColumnData"

#define XmNtableRowWidth         "_tableRowWidth"
#define XmNtableRowData          "_tableRowData"
#define XmNtableNumRowData       "_tableNumRowData"
#define XmNtableShowRowHeader    "_tableShowRowHeader"
#define XmNtableShowColumnHeader "_tableShowColumnHeader"

class TableView :public BulletinBoard {

private:
  int            numColumns;
  int            numRows;
  int            columnHeight;
    
  int            rowWidth;
    
  Boolean showColumnHeader = FALSE;
  Boolean showRowHeader = FALSE;
  ColumnData* columnData = NULL;
  SmartArray<ColumnHeader*>         columnHeader;
  SmartArray<RowHeader*>            rowHeader;
 // SmartArray<SmartArray<Gadget*>* > rowColumns;
  SmartArray<SmartArray<Label*>* > rowColumns;
    
  static const int MARGIN                = 2;
  int          columnCount;      //The number of visible columns
  int          rowCount;         //The number of visible rows
  TableModel*  tableModel;

  int          rowTopPosition;
  int          columnLeftPosition;
  static const int DEFAULT_COLUMN_HEIGHT = 20; 

public:
  TableView(View* parent, const char* name, Args& args)
    :BulletinBoard(),
    numColumns(0),
    numRows(0),
    columnHeight(0),
    columnCount(0),
    rowCount(0),
    tableModel(NULL),
    rowTopPosition(0),
    columnLeftPosition(0)
  {
    TableView::create(parent, name, args);
  }

  TableView()
  :BulletinBoard(),
    numColumns(0),
    numRows(0),
    columnHeight(0),
    columnCount(0),
    rowCount(0),
    tableModel(NULL),
    rowTopPosition(0),
    columnLeftPosition(0)
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    BulletinBoard::create(parent, name, args.set(XmNmarginWidth, 0)
                                     .set(XmNmarginHeight, 0) );
    

    columnHeight = DEFAULT_COLUMN_HEIGHT;
    if (args.has (XmNtableColumnHeight)) {
      columnHeight = args.get(XmNtableColumnHeight);
      if (columnHeight <= 0) {
        columnHeight = DEFAULT_COLUMN_HEIGHT;
      }
    }

    if (args.has (XmNtableNumColumns)) {
      numColumns = args.get(XmNtableNumColumns);
    }
    if (args.has (XmNtableNumRows)) {
      numRows = args.get(XmNtableNumRows);
    }

    columnData = NULL;
    
    if (args.has(XmNtableColumnData)) {
        columnData = (ColumnData*)args.get(XmNtableColumnData);
    }
    
    RowData* rowData = NULL;
    
    if (args.has(XmNtableRowData)) {
        rowData = (RowData*)args.get(XmNtableRowData);
    }
    
    if (numColumns <= 0 || numRows <=0 || columnData == NULL) {
      return;   
    }
    
    rowWidth = 0;
    if (args.has(XmNtableRowWidth)) {
       rowWidth = (int)args.get(XmNtableRowWidth);
    }
    
    if (rowData) {
      int w = 0;
      for (int i = 0; i<numRows; i++) {
        if (rowData[i].width > w) {
          w = rowData[i].width;
        }
      }
      if (w > rowWidth) {
        rowWidth = w;
      }
    }
    //2019/06/06
    showRowHeader = FALSE;
    if (args.has(XmNtableShowRowHeader)) {
        showRowHeader= (Boolean)args.get(XmNtableShowRowHeader);
    }
    
    if (showRowHeader == FALSE) {
      rowWidth = 0;
    }
    //
 
   
    Args ar;
    
    int x = MARGIN + rowWidth;
    int y = MARGIN ;
    showColumnHeader = FALSE;
    if (args.has(XmNtableShowColumnHeader)) {
      showColumnHeader = args.get(XmNtableShowColumnHeader);
    }

    // Create column header
    for (int i = 0; i< numColumns; i++) {
      ar.reset();
      ar.set(XmNx, x);
      ar.set(XmNy, y);

      ar.set(XmNwidth, columnData[i].width + MARGIN); // - ColumnHeader::BORDER_WIDTH); 
     // ar.set(XmNheight, columnHeight+ MARGIN);  
      ar.set(XmNalignment, columnData[i].alignment);
      CompoundString cs(columnData[i].label);
      ar.set(XmNlabelString, cs);
      if (showColumnHeader) {
        ColumnHeader* ch = new ColumnHeader(this, "", ar);
        columnHeader[i] = ch ;//new ColumnHeader(this, "", ar);
        ch -> get(XmNheight, (XtArgVal)&columnHeight);
      }
 
      x = x + columnData[i].width + MARGIN;
    }

    //Create row header
    if (rowWidth > 0) {
      //y = MARGIN + columnHeight + MARGIN + ColumnHeader::BORDER_WIDTH;
      y = MARGIN;
      if (showColumnHeader) {
       y+= columnHeight;// + MARGIN;// + ColumnHeader::BORDER_WIDTH*2;
      } 
      for (int i = 0; i< numRows; i++) {
        ar.reset();
        ar.set(XmNx, 0);
        ar.set(XmNy, y);

        ar.set(XmNwidth, rowWidth + MARGIN - RowHeader::BORDER_WIDTH); 
        ar.set(XmNheight, columnHeight); //+ MARGIN);// -1);  
        char item[256];
        if (rowData) {
            memset(item, 0, sizeof(item));
            if (strlen(rowData[i].label) < sizeof(item)) {
              strcpy(item, rowData[i].label); 
            } else {
                strncpy(item, rowData[i].label, sizeof(item)-1);    
            }
            ar.set(XmNalignment, rowData[i].alignment);             
        } else {
          if (showRowHeader) {
            sprintf(item, "%d", i+1);
            ar.set(XmNalignment, XmALIGNMENT_END); 
          }
        }
        CompoundString cs(item); 
        ar.set(XmNlabelString, cs);
        rowHeader[i] = new RowHeader(this, "", ar);
        y = y + columnHeight;// + RowHeader::BORDER_WIDTH;//MARGIN;
      }
    }
    
    //Show sample rowData 
    x = MARGIN + rowWidth;
    y = MARGIN ;
    if (showColumnHeader) {
      y += columnHeight;
    }

    for(int j = 0; j< numRows; j++) {
      x = MARGIN + rowWidth;
      rowColumns[j] = new SmartArray<Label*>() ;

      for (int i = 0; i<numColumns; i++) {
        char item[256];
        sprintf(item, "Cell(%d, %d)", j+1, i+1);
        CompoundString cs(item);
        ar.reset();
        ar.set(XmNx, x);
        ar.set(XmNy, y);
        ar.set(XmNwidth, columnData[i].width + MARGIN + ColumnHeader::BORDER_WIDTH);
        ar.set(XmNheight, columnHeight);//+ MARGIN); //-1);// + ColumnHeader::BORDER_WIDTH );  
        ar.set(XmNalignment, columnData[i].alignment);
        ar.set(XmNlabelString, cs);
        LabelCell* label = new LabelCell(this, "", ar);

        rowColumns[j] ->put(i,  label);
        x = x+ columnData[i].width + MARGIN;
      } 
      y = y + columnHeight;// + ColumnHeader::BORDER_WIDTH; //MARGIN;
    }       
  }

  void addEventHandler(int row, int column, unsigned long eventMask, CommonObject* object,
        Handler handler, void* data)
  {
    if (row < numRows && column < numColumns && eventMask >0 && object != NULL  && handler !=NULL) {
        Label* label = rowColumns[row] ->get(column);
        if (label) {
          label -> set(XmNtraversalOn, TRUE);
          label -> addEventHandler(eventMask, object, handler, data);
        }
    }
  }
    
  void removeAllItems()
  {
    for(int j = 0; j< numRows; j++) {
      //One columns
      for (int i = 0; i<numColumns; i++) {
       CompoundString cs("");
        Label* label = rowColumns[j] ->get(i);
        
        if (label) {
          label -> set(XmNlabelString, cs);
        }
      }
    }
    rowCount = 0;
  }


  ~TableView() 
  {
  	columnHeader.clear();
  	rowHeader.clear();
  }

  void updateModel(TableModel* model)
  {
    if (tableModel != model) {
      delete tableModel;
    }
    tableModel = model;

    rowTopPosition = 0;
    
    updateView();
  }
    
  void updateView()
  {
    //to do
  }
    
  void setItem(int row, int column, const char* item)
  {
    if (row < numRows && column < numColumns) {
      if (item !=NULL) {
        CompoundString cs(item);
        Label* label = rowColumns[row] ->get(column);
        
        if (label) {
          label -> set(XmNlabelString, cs);
        }
      }
    } else {
        printf("Out of range row=%d column=%d\n", row, column); 
    }
  }

  void setItems(int row, const char** items, int num)
  {
    if (row < numRows) {
      if (rowCount <numRows && items !=NULL) {
       //One columns
        for (int i = 0; i<numColumns; i++) {
          if (num<=numColumns) {
            CompoundString cs(items[i]);
            Label* label = rowColumns[row] ->get(i);
        
            if (label) {
              label -> set(XmNlabelString, cs);
            }
          }
        }
      }
    }
  }

  Label* getGadget(int row, int column)
  {
    Label* rowColumn = NULL;
    if (row <rowCount && column <numColumns) {
        rowColumn = rowColumns[row] -> get(column); 
    }
    return rowColumn;
  }
    
  void addNewRow()
  {
     //printf("expanding rowCount=%d numRows=%d\n", rowCount, numRows);
     rowColumns[rowCount] = new SmartArray<Label*>() ;
     int x = MARGIN + rowWidth;
     int y = MARGIN;
     if (showColumnHeader) {
       y += columnHeight;
     }
     y +=  columnHeight *rowCount;

    for (int i = 0; i<numColumns; i++) {
        char item[256];
        sprintf(item, "%d", i+1);
        CompoundString cs(item);
        Args ar;
        ar.reset();
        ar.set(XmNx, x);
        ar.set(XmNy, y);
        ar.set(XmNwidth, columnData[i].width + MARGIN + ColumnHeader::BORDER_WIDTH);
        ar.set(XmNheight, columnHeight);//+ MARGIN); //-1);// + ColumnHeader::BORDER_WIDTH );
        ar.set(XmNalignment, columnData[i].alignment);
        ar.set(XmNlabelString, cs);
        LabelCell* label = new LabelCell(this, "", ar);

        rowColumns[rowCount] ->put(i,  label);
        x = x+ columnData[i].width + MARGIN;
    }
  }

  int addItems(const char** items, int num)
  {
    if (items == NULL || num > numColumns) {
        throw IException("Invalid argument");
    }
    if (rowCount >= numRows) {
      //printf("Expanding rowCount=%d numRows=%d\n", rowCount, numRows);
      addNewRow();
      numRows++;
    }
 
    if (rowCount <numRows) {
      for (int i = 0; i<numColumns; i++) {
        if (num<=numColumns) {
          CompoundString cs(items[i]);
          Label* cell = rowColumns[rowCount] ->get(i);
        
          if (cell) {
            cell -> set(XmNlabelString, cs);
          } else {
            printf("cell empty %d\n", i);
          }
        }
      }
      rowCount++;
    } else {
      throw IException("RowCount overflow %d", rowCount);  
    }
    return rowCount;
  }

  int addItems(std::vector<std::string>& items)
  {
    size_t num = items.size();

    if (num > numColumns) {
       throw IException("Invalid argument");
    }
    if (rowCount >= numRows) {
      //printf("Expanding rowCount=%d numRows=%d\n", rowCount, numRows);
      addNewRow();
      numRows++;
    }
 
    if (rowCount <numRows) {
      for (int i = 0; i<numColumns; i++) {
        if (num<=numColumns) {
          std::string item = items.at(i);
          CompoundString cs(item.c_str() );
          Label* cell = rowColumns[rowCount] ->get(i);
        
          if (cell) {
            cell -> set(XmNlabelString, cs);
          } else {
            printf("cell empty %d\n", i);
          }
        }
      }
      rowCount++;
    } else {
      throw IException("RowCount overflow %d", rowCount);  
    }
    return rowCount;
  }

  void parseCSVHeader(const char* line, std::vector<std::string>& items)
  {
    StringTokenizerT<char> tokenizer(line);
    while(tokenizer.hasMoreToken()) {
      char token[256] = { 0 };
      tokenizer.getToken(token, sizeof(token));
      items.push_back(std::string(token)); 
    }
  }

  //line is a set of space, comma, or tab separated strings.
  void addLine(const char* line)
  {
    std::vector<std::string> items;
    StringTokenizerT<char> tokenizer(line);
    while(tokenizer.hasMoreToken()) {
      char token[128] = { 0 };
      tokenizer.getToken(token, sizeof(token));
      items.push_back(std::string(token));
    }
    addItems(items);
  }

  void readCSVFile(const std::string& filename, bool ignore_header=true)
  {
    std::ifstream ifs(filename);
    if (!ifs) {
      throw IException("File not found %s", filename.c_str());
    }
    std::string line;
    bool first_line = true;

    while (!ifs.eof()) {
      std::getline(ifs, line);
      //Erase ¥n if line ends with it.
      if (line[line.size()-1] == '\n') {
        line.erase(line.size()-1);
      }
      //Erase ¥r if line ends with it.
      if (line[line.size()-1] == '\r') {
        line.erase(line.size()-1);
      }
      if (line.length() < 1) {
        continue;
      }
      if (first_line) {
        if (ignore_header) {
          std::vector<std::string> column_names;
          parseCSVHeader(line.c_str(), column_names);
        }
        first_line = false;
      } else {
        addLine(line.c_str());
      }
    }
  } 
};

}

