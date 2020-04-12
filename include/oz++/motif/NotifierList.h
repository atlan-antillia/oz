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
 *      NotifierList.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/Notifier.h>
#include <oz++/DirectoryScanner.h>
#include <oz++/SmartArray.h>
#include <oz++/SmartPtrs.h>

namespace OZ {

class NotifierList :public BulletinBoard {

private:
  SmartArray<Notifier*> notifiers;
  int       index;
  Dimension labelHeight;
  int       margin;
    
public:
  NotifierList(View* parent, const char* name, Args& args)
  :BulletinBoard()
  {
    NotifierList::create(parent, name, args);
  }

  NotifierList()
  :BulletinBoard()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    BulletinBoard::create(parent, name, 
                        args.set(XmNmarginHeight, 0)
                            .set(XmNmarginWidth, 0)
                            .set(XmNbackground, parent->whitePixel()) );
  
    index       = 0;
    labelHeight = 0;
    margin      = 2;
  }

public:
  ~NotifierList()
  {
    notifiers.clear();
  }


public:
  void addItem(const char* name, Args& args, 
       unsigned long eventMask = 0,
       CommonObject* object = NULL,
       Handler handler = NULL, XtPointer data = NULL)
  {
    
     int x = margin;
     int y = index * labelHeight;
     args.set(XmNx, x);
     args.set(XmNy, y);
     Notifier* label = new Notifier(this, name, args);
     if (labelHeight == 0) {
       labelHeight = label -> height();
     }

     notifiers[index++] = label;
     if (eventMask && object && handler) {
       label -> addEventHandler(eventMask, object, handler, data);
     }
  }

public:
  void removeAllItems()
  {
     notifiers.clear();
     index = 0; 
  }

  Notifier* getNotifier(int i)
  {
    Notifier* label= NULL;
    if (i >= 0 && i<index) {
     label = notifiers[i];
    }
    return label;
  }
    
public:
  void addDirectories(Pixmap pixmap, const char* dir, 
        const char* pattern = "*",
       unsigned long eventMask = 0,
       CommonObject* object = NULL,
       Handler handler = NULL, XtPointer data = NULL)
  {
    if (dir) {
      DirectoryScanner scanner(dir);
      DirEntryList dirList;
      if (pattern == NULL) {
        pattern = "*";
      }
      int m = scanner.scanDir(pattern, dirList);
      if (m > 0) {
        SmartPtrs<char*> names = new char*[m];
        dirList.getNames((char**)names);
        addItems(pixmap, names, m, eventMask, object, handler, data);
      }
    } else {
      throw IException("Invalid argument");  
    }       
  }
    
public:
  void addFiles(Pixmap pixmap, const char* dir, 
       const char* pattern = "*",
       unsigned long eventMask = 0,
       CommonObject* object = NULL,
       Handler handler = NULL, XtPointer data = NULL)
  {
    if (dir) {
      DirectoryScanner scanner(dir);
      DirEntryList dirList;
      if (pattern == NULL) {
        pattern = "*";
      }
      int m = scanner.scanFile(pattern, dirList);
      if (m > 0) {
        SmartPtrs<char*> names = new char*[m];
        dirList.getNames((char**)names);
        addItems(pixmap, names, m, eventMask, object, handler, data);
      }
    } else {
      throw IException("Invalid argument");  
    }       
  }

public:
  void addItems(Pixmap pixmap, char** items, int m,
           unsigned long eventMask = 0,
           CommonObject* object = NULL,
           Handler handler = NULL, XtPointer data = NULL)

  {
    Dimension width = this -> width();
    if (items && m > 0) {
      for (int i = 0; i < m; i++) {
        CompoundString cs(items[i]);
        Args ar;
        ar.reset();
        ar.set(XmNlabelString, cs);
        ar.set(XmNwidth, width - margin*2);
        ar.set(XmNlabelType, XmPIXMAP_AND_STRING);
        ar.set(XmNlabelPixmap, pixmap);

        //ar.set(XmNbackground, white);
        ar.set(XmNtraversalOn, TRUE);
        ar.set(XmNshadowThickness, 0);
        ar.set(XmNhighlightThickness, 1);

        ar.set(XmNalignment, XmALIGNMENT_BEGINNING);
        addItem("", ar, eventMask, object, handler, data);
      }
    }
  }
};

}

