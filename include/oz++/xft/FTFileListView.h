/******************************************************************************
 *
 * Copyright (c) 2015   TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  FTFileListView.h
 *
 *****************************************************************************/


#include <oz++/motif/ScrolledWindow.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/PixmapFile.h>
#include <oz++/xft/FTFont.h>
#include <oz++/xft/FTColumnLayout.h>
#include <oz++/xft/FTMultiColumn.h>
#include <oz++/SmartArray.h>
#include <oz++/FileAttributes.h>
#include <oz++/DirectoryScanner.h>
#include <oz++/MonetaryFormatter.h>
#include <oz++/FileAttributes.h>
#include <oz++/LocalTime.h>
#include <oz++/SmartPtrs.h>

namespace OZ {
const char* XmNsizeUnitKB       = "sizeUnitKB";
const char* XmNfolderIconFile   = "folderIconFile";
const char* XmNfileIconFile     = "fileIconFile";
  
class FTFileListView: public ScrolledWindow {
public:
  typedef enum {
   DIR  = 1,
   FILE = 2,
  } FILE_TYPE;

private:  
  FTFont*  font;
  PixmapFile* folderIcon;
  PixmapFile* fileIcon;
  
  BulletinBoard* bboard;
  FTColumnLayout* columnLayout;
  int numColumns;

  SmartArray<FTMultiColumn*> columns;
  int index;
  int columnHeight;
  int columnWidth;
  
  bool fileSizeUnitKB;
private:
  static const int MARGIN = 2;
  
public:
  FTFileListView(View* parent, const char* name, Args& args)
  :ScrolledWindow(parent, name, args.set(XmNscrollingPolicy, XmAUTOMATIC)
                                      .set(XmNscrollBarDisplayPolicy, XmVARIABLE) ),
  font(NULL),
  folderIcon(NULL),
  fileIcon(NULL),
  
  bboard(NULL),
  columnLayout(NULL),
  numColumns(0),
  index(0),
  columnHeight(0),
  columnWidth(0),
  fileSizeUnitKB(true)
  {
    Color color(this);
    Pixel white = color.whitePixel();
    int fontsize = 12;
    unsigned int iconw = 0;
    unsigned int iconh = 0;
    
    if (args.has(XmNfolderIconFile)) {
      const char* filename = (const char*)args.get(XmNfolderIconFile);
      folderIcon = new PixmapFile(getDisplay(), filename, white, white);
      if (folderIcon) {
        folderIcon -> getGeometry(&iconw, &iconh);  
      }
    }
    if (args.has(XmNfileIconFile)) {
      const char* filename = (const char*)args.get(XmNfileIconFile);
      fileIcon = new PixmapFile(getDisplay(), filename, white, white);
    }
    
    Args ar;
    ar.reset();
    if (args.has(XftNfamily)) {
      ar.set(XftNfamily, (const char*)args.get(XftNfamily));
    } else {
      ar.set(XftNfamily, "Times-12");  //Default
    }
    if (args.has(XftNweight)) {
      ar.set(XftNweight, (int)args.get(XftNweight));  
    } else {
      ar.set(XftNweight, FC_WEIGHT_THIN);//;
    }
    if (args.has(XftNsize)) {
      fontsize = (int)args.get(XftNsize); //
      ar.set(XftNsize, fontsize);
    } else {
      ar.set(XftNsize, fontsize);  //Default
    }
    font = new FTFont(getDisplay(), ar);
  
    if (iconh > fontsize) {
      columnHeight = iconh + MARGIN;
    } else {
      columnHeight = fontsize *2; //
    }
    
    ar.reset();
    ar.set(XmNmarginWidth, 0);
  ar.set(XmNmarginHeight, 0);
  ar.set(XmNbackground, white);
    bboard = new BulletinBoard(this, "", ar);
  if (args.has(XmNcolumnLayout) && args.has(XmNnumColumns) ) {
    columnLayout = (FTColumnLayout*)args.get(XmNcolumnLayout);
    numColumns   = (int)args.get(XmNnumColumns);
  } else {
    throw IException("XmNcolumnLayout or XmNnumColumns is not sepecified");  
  }
  
    columnWidth = MARGIN*2;
  for (int i = 0; i<numColumns; i++) {
    columnWidth += columnLayout[i].width;
    }
    

  } 
  
  ~FTFileListView()
  {
    columns.clear();
    delete font;
    delete folderIcon;
    delete fileIcon;
  delete bboard;
    
  }
  
  void removeAllItems()
  {
     columns.removeAllItems();
     index = 0;
  }

  void addItems(const char* dir, int m, char** names, int type, PixmapFile* icon)
  {
    char fullpath[PATH_MAX];
     
    for (int i = 0; i<m; i++) {
      try {
       sprintf(fullpath, "%s/%s", dir, names[i]);
       FileAttributes attr(fullpath);

       Args ar;
        ar.reset();
       ar.set(XmNx, MARGIN);
       ar.set(XmNy, (columnHeight+ MARGIN*2) * index + MARGIN);
        ar.set(XmNrecomputeSize, False);
        ar.set(XmNcolumnLayout, (XtArgVal)columnLayout);
        ar.set(XmNnumColumns, numColumns);
       ar.set(XmNwidth, columnWidth);
        ar.set(XftNfont, (XtArgVal)font);
        //ar.set(XmNhighlightOnEnter, True);
        ar.set(XmNuserData, type);
        CompoundString cs("");
        ar.set(XmNlabelString, cs);
        ar.set(XmNlabelType, XmPIXMAP_AND_STRING);
        if (icon) {
          ar.set(XmNlabelType, XmPIXMAP_AND_STRING);
          ar.set(XmNlabelPixmap, (XtArgVal)icon->get());
        }
        
        FTMultiColumn* col = new FTMultiColumn(bboard, "", ar);
        
       LocalTime lt(attr.changedTime());
        CharString ctime = lt.toCharString();
         
        char csize[128];
        MonetaryFormatter mformatter;  
        if (fileSizeUnitKB) {
          unsigned long fs = attr.getSize();
          unsigned long kb = fs/1024L;
          if ((fs % 1024L)) {
            kb++;
          }
          CharString value = mformatter.toString(kb);
          sprintf(csize, "%s KB", (const char*)value);
        } else {
          CharString value = mformatter.toString((long)attr.getSize());
          sprintf(csize, "%s B", (const char*)value);
        }

       const char* items[(const int)numColumns];
       int n = 0;
       items[n++] = names[i];
         items[n++] = (const char*)ctime;
       items[n++] = csize;
       col -> setItems(items, n);
        
        
        columns[index++] = col;
      } catch (Exception& ex) {
         ex.display();  
      }
    }
  }
  
  void addEventHandler(unsigned long eventMask, CommonObject* object, Handler handler, XtPointer unused)
  {
    for (int i = 0; i<index; i++) {
      FTMultiColumn* col = columns[i];
      if (col) {
      //  col -> resize();
        col -> addEventHandler(eventMask, object,
          handler, to_voidptr(i));
      }
    }
  }
  
  void  listupDirectories(const char* dir, const char* pattern)
  {
    DirectoryScanner scanner(dir);
    DirEntryList dirList;
    int m = scanner.scanDir(pattern, dirList);
    if (m > 0) {
      SmartPtrs<char*> names = new char*[m];
      dirList.getNames((char**)names);
      
      addItems(dir, m, names, DIR, folderIcon); 
    }
  }

  void  listupFiles(const char* dir, const char* pattern)
  {
    DirectoryScanner scanner(dir);
    DirEntryList dirList;
    int m = scanner.scanFile(pattern, dirList);
    if (m > 0) {
      SmartPtrs<char*> names = new char*[m];
      dirList.getNames((char**)names);
      addItems(dir, m, names, FILE, fileIcon);
      
    }
  }
};

}

