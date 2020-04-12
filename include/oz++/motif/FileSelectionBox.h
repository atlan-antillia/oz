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
 *  FileSelectionBox.h
 *
 *****************************************************************************/

//2015/08/27
//2015/09/05
// Set XmNoutlineButtonPolicy resource to be XmOUTLINE_BUTTON_ABSENT 
//for Container.
//2014/09/06 Modified not to use setSelectCallback, and remove select method.
 
#pragma once

#include <oz++/File.h>
#include <oz++/FileAttributes.h>
#include <oz++/StringT.h>
#include <oz++/StringBufferT.h>
#include <oz++/DirectoryScanner.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/ScrolledWindow.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/Container.h>
#include <oz++/motif/IconGadget.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>
#include <oz++/motif/PixmapFile.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/FolderSelectionBox.h>
#include <oz++/motif/StrDef.h>

namespace OZ {

class FileSelectionBox :public ScrolledWindow {
public:
  enum {OK_BUTTON =0 , CANCEL_BUTTON, FILTER_BUTTON, HELP_BUTTON};

private:
  Boolean       includeFolder;

  SmartPtr<Container>  container;
  SmartPtr<PixmapFile> folderPixmap;
  SmartPtr<PixmapFile> filePixmap;

  StringT<char> folderName;

  StringT<char> fileName;

public:
  void  buildFileList(const char* dir)
  {
    folderName = dir;
    if (includeFolder) {
      addDirList(dir);
    }
    addFileList(dir);
  }

  const char* getFolderName()
  {
    return (const char*)folderName;
  }

  void addDirList(const char* dir)
  {
    DirectoryScanner scanner(dir);
    DirEntryList dirList;

    int m = scanner.scanDir("*", dirList);

    XmString details[3];
  
    for (int i = 0; i<m; i++) {
      const char* name = dirList.getName(i);
      char fullPath[PATH_MAX];
      sprintf(fullPath, "%s/%s", dir, name);
      FileAttributes attr(fullPath);
      char ssize[80];
      sprintf(ssize, "%ld", attr.getSizeKB());

      StringT<char> mtime;
      attr.getModifiedTime(mtime);

      //printf("name %s\n", name);
      CompoundString ncs(name);
      CompoundString scs(ssize);
      CompoundString tcs((const char*)mtime);

      details[0] = scs.get();
      details[1] = tcs.get();

      Args args;
      args.reset();
      args.set(XmNlabelString,  ncs.get() );
      args.set(XmNsmallIconPixmap, folderPixmap);
      args.set(XmNdetail, (XtArgVal)details);
      args.set(XmNdetailCount, 2);
      args.set(XmNshadowThickness, 0);
      args.set(XmNborderWidth,  0);
      args.set(XmNviewType, XmSMALL_ICON);

      IconGadget* child = new IconGadget(container, "", args);
      child -> setNodeType(IconGadget::FOLDER);

      container->add(child);
    }
  }

  void addFileList(const char* dir)
  {
    DirectoryScanner scanner(dir);
    DirEntryList fileList;

    int m = scanner.scanFile("*", fileList);
    //printf("DirList dir=%s m=%d\n", dir, m);

    XmString details[3];
   
    for (int i = 0; i<m; i++) {
      const char* name = fileList.getName(i);
      char fullPath[PATH_MAX];
      sprintf(fullPath, "%s/%s", dir, name);
      FileAttributes attr(fullPath);
      char ssize[80];
      sprintf(ssize, "%ld", attr.getSizeKB());

      StringT<char> mtime;
      attr.getModifiedTime(mtime);

      //printf("name %s\n", name);
      CompoundString ncs(name);
      CompoundString scs(ssize);
      CompoundString tcs((const char*)mtime);

      details[0] = scs.get();
      details[1] = tcs.get();
 
      Args args;
      args.reset();
      args.set(XmNlabelString,  ncs.get() );
      args.set(XmNsmallIconPixmap, filePixmap);
      args.set(XmNdetail, (XtArgVal)details);
      args.set(XmNdetailCount, 2);
      args.set(XmNshadowThickness, 0);
      args.set(XmNborderWidth,  0);
      args.set(XmNviewType, XmSMALL_ICON);
      IconGadget* child = new IconGadget(container, "", args);
      child -> setNodeType(IconGadget::FILE);
      container->add(child);
    }
  }

public:
  FileSelectionBox(View* parent, const char* name, Args& args)
  :ScrolledWindow(),
  includeFolder(False),
  folderName(""),
  fileName("")
  {
    FileSelectionBox::create(parent, name, args);
  }

  FileSelectionBox()
  :ScrolledWindow(),
  includeFolder(False),
  folderName(""),
  fileName("")
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    ScrolledWindow::create(parent, name, 
        args.set(XmNscrollingPolicy, XmAUTOMATIC)
        //.set(XmNrecomputeSize, TRUE)
          .set(XmNvisualPolicy, XmVARIABLE)
          .set(XmNscrollBarDisplayPolicy, XmAS_NEEDED)
          .set(XmNspacing, 0)
          .set(XmNborderWidth, 0) );
  
    Color color(this);
    Pixel selectColor = color.allocNamedColor("lightcyan");

    if (args.has(XmNincludeFolder)) {
      includeFolder = (Boolean)args.get(XmNincludeFolder);
      printf("Include Folder %d\n", includeFolder);
    }

    Args ar;

    static const char* columnHeaders[] = {
      "Name", "Size (KB)", "Modified Time" };
    int count = SizeOf(columnHeaders);
    CompoundStringList csl(columnHeaders, count);
    XmString* columns = csl.get();
    ar.set(XmNdetailColumnHeading, (XtArgVal)columns);
    ar.set(XmNdetailColumnHeadingCount, count);
    ar.set(XmNlayoutType, XmDETAIL);
    ar.set(XmNoutlineLineStyle, XmNO_LINE);
    ar.set(XmNselectColor, selectColor);
    ar.set(XmNoutlineButtonPolicy, XmOUTLINE_BUTTON_ABSENT); //2015/09/05

    container = new Container(this, "", ar);

    Pixel fg, bg;
    container -> get(XmNforeground, (XtArgVal)&fg);
    container -> get(XmNbackground, (XtArgVal)&bg);
    const char* folderPixmapFile = NULL;

    if (args.has(XmNfolderPixmapFile)) {
      folderPixmapFile = (const char*)args.get(XmNfolderPixmapFile);
      //  printf("folderPixmapFile %s\n", folderPixmapFile);
      if (folderPixmapFile) {
        try {
          folderPixmap = new PixmapFile(getDisplay(), folderPixmapFile,
                 fg, bg);
        } catch (Exception& ex) {
          ex.display();
        }
      }
    }
    const char* filePixmapFile = NULL;
    if (args.has(XmNfilePixmapFile)) {
      filePixmapFile = (const char*)args.get(XmNfilePixmapFile); 
      if (filePixmapFile) {
        try {
          filePixmap = new PixmapFile(getDisplay(), filePixmapFile,
                 fg, bg);
        } catch (Exception& ex) {
          ex.display();
        }
      }
    }
  }

  ~FileSelectionBox()
  {
  }

  void getFileName(StringT<char> name)
  {
    name = fileName;
  }

  const char* getFileName()
  {
    return (const char*)fileName;
  }

  //2015/09/06
  Container* getContainer()
  {
    return container;
  }

  //2015/09/06
  void clear()
  {
     container -> removeAllItems();
     container -> clearSelectedItems();
  }

  void update(const char* dir)
  {
    if (dir) {
      File file(dir);
      FileAttributes attr(dir);
      if(file.exists() && attr.isDirectory() ) {

        // This umap avoids excessive redrawings (flickering) of this window,
        // which will be caused by the following clear and build List operations.
        //unmap(); 

        clear();
        buildFileList(dir);
        container -> relayout();
        map(); 
      } else {
        throw IException("Not found directory(%s)", dir);
      }
    }
  }  
};
}


