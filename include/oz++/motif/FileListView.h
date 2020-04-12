/******************************************************************************
 *
 * Copyright (c) 2017   TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  FileListView.h
 *
 *****************************************************************************/

#pragma once

//2017/05/10 Updated to use Pixelmap instead of PixmapFile.

#include <oz++/motif/ScrolledWindow.h>

//#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/FlatButton.h>
#include <oz++/motif/PushButton.h>

#include <oz++/motif/FlatButtonGadget.h>
#include <oz++/motif/Pixelmap.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/Color.h>
#include <oz++/DirectoryScanner.h>
#include <oz++/MonetaryFormatter.h>
#include <oz++/FileAttributes.h>
#include <oz++/LocalTime.h>
#include <oz++/SmartArray.h>
#include <oz++/SmartPtr.h>
#include <oz++/SmartPtrs.h>
#include <oz++/motif/StrDef.h>
#include <oz++/motif/xpm/file.xpm>

#include <oz++/motif/xpm/folder.xpm>

namespace OZ {
    
class FileListView :public ScrolledWindow {

private:
  static const int ITEM_HEIGHT = 26;
  static const int MARGIN      = 2;
  static const int NAME_WIDTH  = 380;
  //static const int SIZE_WIDTH  = 170;
  static const int CHANGEDTIME_WIDTH = 170;

private:
  SmartPtr<BulletinBoard>  bboard;
    
  SmartPtr<Pixelmap>   folderPixmap;
  SmartPtr<Pixelmap>   filePixmap;
  int                  nameWidth;
  int                  changedTimeWidth;
  int                  sizeWidth;
    
  SmartArray<PushButton*> files;
  
  SmartArray<Gadget*> ctimes;
  SmartArray<Gadget*> sizes;
    
  int x;
  int y;
  int index;
  int height;
    
  bool fileSizeUnitKB;
    
public:
  static const int DIR  = 1;
  static const int FILE = 2;
  
public:
  FileListView(View* parent, const char* name, Args& args)
  :ScrolledWindow(parent, name, args.set(XmNscrollingPolicy, XmAUTOMATIC)
                                      .set(XmNscrollBarDisplayPolicy, XmVARIABLE) ),
  folderPixmap(NULL),
  filePixmap(NULL)
  {
    nameWidth        = NAME_WIDTH;
    changedTimeWidth = CHANGEDTIME_WIDTH;
    sizeWidth        = SIZE_WIDTH;
    
    if (args.has(XmNnameWidth)) {
        int val = (int)args.get(XmNnameWidth);
        if (val > 0) {
          nameWidth = val;  
        }
    }

    if (args.has(XmNchangedTimeWidth)) {
        int val = (int)args.get(XmNchangedTimeWidth);
        if (val > 0) {
          changedTimeWidth = val;   
        }
    }
    
    if (args.has(XmNsizeWidth)) {
        int val = (int)args.get(XmNsizeWidth);
        if (val > 0) {
          sizeWidth = val;  
        }
    }
    
    fileSizeUnitKB = false;
    if (args.has(XmNsizeUnitKB)) {
        fileSizeUnitKB = (bool)args.get(XmNsizeUnitKB); 
    }
    
    Color color(this);
    Pixel white = color.whitePixel();

    x = MARGIN;
    y = MARGIN;
    index = 0;
    Args ar;
    ar.reset();
    ar.set(XmNmarginHeight, 0);
    ar.set(XmNmarginWidth,  0);
    bboard = new BulletinBoard(this, "", ar);
/*   
    Widget hsb = (Widget)get(XmNhorizontalScrollBar);
    Widget vsb = (Widget)get(XmNverticalScrollBar);
    setArea(hsb, vsb, bboard->getWidget());
*/
    Display* display = parent->getDisplay();
    Pixel fg, bg;
    bboard -> get(XmNforeground, (XtArgVal)&fg);
    bboard -> get(XmNbackground, (XtArgVal)&bg);
    try { 
      const char** folderPixmapData = folder_xpm;
      folderPixmap = new Pixelmap(display, bg, folderPixmapData);
      const char** filePixmapData = file_xpm;
      filePixmap = new Pixelmap(display, bg, filePixmapData);
    } catch (Exception& ex) {
      caught(ex);
    }
  }
    
public:
  ~FileListView()
  {
    files.clear();
    ctimes.clear();
    sizes.clear();
  }

public:
  void redraw(bool flag)
  {
    if (flag) {
       bboard  -> manage(); 
    } else {
       bboard -> unmanage();
    }
  }
    
public:
  void removeAllItems()
  {
    x = MARGIN;
    y = MARGIN;
    index     = 0;
    files.removeAllItems();     
    ctimes.removeAllItems();
    sizes.removeAllItems();
  }
    
public:
  void addDirectories(const char* dir, char** items, int num)
  {
    if (items && num >0) {
      addItems(dir, DIR, items, num, folderPixmap);
    }
  }
    
public:
  void addFiles(const char* dir, char** items, int num)
  {
    if (items && num > 0) {
      addItems(dir, FILE, items, num, filePixmap);
    }
  }
    
private:
  void addItems(const char* dir, int type, char** items, 
    int num, Pixelmap* pixelmap)
  {
    char path[PATH_MAX];
      
    for (int i = 0; i<num; i++) {
      sprintf(path, "%s/%s", dir, items[i]);
      try {
        FileAttributes attr(path);
        int fbwidth = nameWidth - MARGIN; 
        CompoundString label(items[i]);
        Args ar;
        ar.reset();
        ar.set(XmNx, x);
        ar.set(XmNy, y);
        ar.set(XmNrecomputeSize, FALSE);
        ar.set(XmNalignment, XmALIGNMENT_BEGINNING);
        ar.set(XmNlabelString, label);
        ar.set(XmNuserData, type);          
        unsigned int pw = 0;

        if (pixelmap) {
          ar.set(XmNlabelPixmap, pixelmap->get());
          ar.set(XmNlabelType, XmPIXMAP_AND_STRING);
          unsigned int ph = 0;
          pixelmap -> getGeometry(&pw, &ph);
          pw += MARGIN*2;  
        }
        fbwidth -= pw;
        
        ar.set(XmNwidth, fbwidth);
        ar.set(XmNhighlightThickness, 1);
        ar.set(XmNshadowThickness, 0);
        PushButton* fb = new PushButton(bboard, "folder", ar);
        //FlatButtonGadget* fb = new FlatButtonGadget(bboard, "folder", ar);
        files[index++] = fb;
        static Dimension th = 0;
        if (th == 0) {
          th = fb -> height();
        }
     
        LocalTime lt(attr.changedTime());
        CompoundString ctime(lt.toCharString());    
        ar.reset();
        ar.set(XmNx, x + nameWidth);
        ar.set(XmNy, y); 
        ar.set(XmNheight, th);
        
        ar.set(XmNlabelString, ctime);
        ar.set(XmNwidth, changedTimeWidth - MARGIN);
        ctimes[index++] = new LabelGadget(bboard, "", ar);
            
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
            
        CompoundString size(csize);
        ar.reset();
        ar.set(XmNx, x + nameWidth + changedTimeWidth);
        ar.set(XmNy, y);
        ar.set(XmNheight, th);
        ar.set(XmNalignment, XmALIGNMENT_END);
        ar.set(XmNlabelString, size);
        ar.set(XmNwidth, sizeWidth - MARGIN);
        sizes[index++] = new LabelGadget(bboard, "", ar);
        
        y += th; //height;
      } catch (Exception& ex) {
        caught(ex);
      }
    }  
  }

public:
  void listup(const char* dir, const char* pattern = "*")
  {
    redraw(false);
    removeAllItems();
    
    //const char* parent[] = {".."};
    //addDirectories("",  (char**)parent, 1);
        
    DirectoryScanner dirScanner(dir);
    DirEntryList dirList;
    int m = dirScanner.scanDir(pattern, dirList);
    if (m > 0) { 
      SmartPtrs<char*> dirNames = new char*[m];
      dirList.getNames((char**)dirNames);
      addDirectories(dir, dirNames, m);
    }  
    
    DirectoryScanner fileScanner(dir);
    DirEntryList fileList;
    int n = fileScanner.scanFile(pattern, fileList);
    if (n > 0) {
      SmartPtrs<char*> fileNames = new char*[n];
      fileList.getNames((char**)fileNames);
      addFiles(dir, fileNames, n);
    } 
    redraw(true);
  }
    
public:
  void addCallback(const char* name, CommonObject* object,
        Callback callback, void* data)
  {
    for (int i = 0; i<index; i++) {
      if (files[i]) {
        files[i] -> addCallback(name, object, callback, data);
      }
    }
  }
};

}
