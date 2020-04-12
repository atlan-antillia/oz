/******************************************************************************
 *
 * Copyright (c) 2015-2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      FileIconBox.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/IconBox.h>

#include <oz++/motif/FlatIconButton.h>
#include <oz++/motif/PixmapFile.h>
#include <oz++/motif/Color.h>
#include <oz++/DirectoryScanner.h>
#include <oz++/SmartArray.h>
#include <oz++/SmartPtrs.h>

namespace OZ {
    
const char* XmNfolderIconFile = "folderIconFile";
const char* XmNfileIconFile   = "fileIconFile";

class FileIconBox :public IconBox {
public:
  typedef enum {
      DIR  = 1,
      FILE = 2,
  } FILE_TYPE;
    
private:
  SmartArray<FlatIconButton*> buttons;
  int index;
  SmartPtr<PixmapFile>   folderIcon;
  SmartPtr<PixmapFile>   fileIcon;

public:
  FileIconBox(View* parent, const char* name, Args& args)
  :IconBox()
  {
    FileIconBox::create(parent, name, args);
  }

  FileIconBox()
  :IconBox()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    IconBox::create(parent, name, 
               args.set(XmNbackground, parent->whitePixel()) );
  
    Display* display = parent->getDisplay();

    Color color(this);
    Pixel fg = color.whitePixel();
    Pixel bg = color.whitePixel();
    
    index = 0;
    if (args.has(XmNfolderIconFile)) {
      const char* fileName = (const char*)args.get(XmNfolderIconFile);
      folderIcon = new PixmapFile(display, fileName, fg, bg);
    }

    if (args.has(XmNfileIconFile)) {
      const char* fileName = (const char*)args.get(XmNfileIconFile);
      fileIcon = new PixmapFile(display, fileName, fg, bg);
    }
  }

public:
  ~FileIconBox()
  {
    buttons.clear();
  }

public:
  void addCallback(const char* name, CommonObject* object, Callback callback, XtPointer data)
  {
    for (int i = 0; i<index; i++) {
        if (buttons[i] != NULL) {   
          buttons[i] -> addCallback(name, object, callback, data);
        }
    }
  }
    
public:
  void addItems(char* items[], int num, int type, PixmapFile* pixmapFile)
  {
    Color color(this);
    Pixel white = color.whitePixel();
    
    for (int i = 0; i<num; i++) {
      CompoundString label(items[i]);
      Args ar;
      ar.reset();
      ar.set(XmNrecomputeSize, False);
      ar.set(XmNalignment, XmALIGNMENT_CENTER);
      ar.set(XmNlabelString, label);
      ar.set(XmNuserData, type);          
      ar.set(XmNcellX, XmIconBoxAnyCell);
      ar.set(XmNcellY, XmIconBoxAnyCell);
      ar.set(XmNiconPlacement, XmIconTop);
      ar.set(XmNbackground,   white);
      ar.set(XmNarmColor,     white);

      if (pixmapFile) {
        ar.set(XmNpixmap, pixmapFile);
      }
       
      //ar.set(XmNhighlightThickness, 1);
      ar.set(XmNshadowThickness, 0);
      FlatIconButton* fb = new FlatIconButton(this, "flaticonbutton", ar);
      buttons[index++] = fb; 
    }
  }
    
public:
  void removeAllItems()
  {
    buttons.removeAllItems();
    index = 0;
  }
    
public:
  void addDirectories(const char* dir, const char* pattern)
  {
    DirectoryScanner scanner(dir);
    DirEntryList dirList;
    int m = scanner.scanDir(pattern, dirList);
    
    if (m > 0) {
      SmartPtrs<char*> names = new char*[m];
      dirList.getNames((char**)names);
      addItems((char**)names, m, DIR, folderIcon);  
    }   
  }
    
public:
  void addFiles(const char* dir, const char* pattern)
  {
    DirectoryScanner scanner(dir);
    DirEntryList dirList;
    int m = scanner.scanFile(pattern, dirList);
    
    if (m > 0) {
      SmartPtrs<char*> names = new char*[m];
      dirList.getNames((char**)names);
      addItems((char**)names, m, FILE, fileIcon);   
    }
  }

};

}

