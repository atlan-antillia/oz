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
 *  FolderSelectionBox.h
 *
 *****************************************************************************/

//2015/08/27
//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/StringT.h>
#include <oz++/StringBufferT.h>
#include <oz++/DirectoryScanner.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/ScrolledWindow.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/Container.h>
#include <oz++/motif/IconGadget.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/Pixelmap.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/StrDef.h>

//2017/05/10
#include <oz++/motif/xpm/folder.xpm>
//It contains static const char* folder_xpm[];

namespace OZ {

class FolderSelectionBox :public ScrolledWindow {
public:
  enum {OK_BUTTON =0 , CANCEL_BUTTON, FILTER_BUTTON, HELP_BUTTON};

private:
  SmartPtr<Container>    container;
  SmartPtr<IconGadget>   root;
  SmartPtr<Pixelmap>     folderPixmap;

  StringT<char>          folderName;

  StringT<char>          rootFolder;

  SmartPtr<Action>       selectAction;

  void selected(Action& action)
  {
    XmContainerSelectCallbackStruct* cbs = 
	(XmContainerSelectCallbackStruct*)action.getCallData();
    Container* container = (Container*)action.getClientData();

    int itemCount = cbs -> selected_item_count;
 
    for (int i = 0; i < itemCount; i++) {
      Widget w =  cbs->selected_items[i];
      IconGadget* selected = (IconGadget*)container->lookup(w);
      if (selected) {
        StringBufferT<char> path;
        selected -> getHierarchy(path);
        const char* folder = (const char*)path;
        if (StringT<char>::startsWith(folder, "//")) {
          folder++;
        }
        printf ("selected folder: %s\n", folder);
        folderName = folder;
        if (selected -> hasChildren() == false) {
          buildDirTree(container, selected, folder);
        }
        if (selectAction) {
          printf("itemCount %d\n", i);
          selectAction -> setCallData((XtPointer)selected);
          selectAction -> setClientData((XtPointer)folder);
          selectAction -> call(*selectAction);
        }
      } //selected
    } //for loop
  }

  void  buildDirTree(Container* container, IconGadget* node, const char* dir)
  {
    char path[PATH_MAX];

    if (StringT<char>::startsWith(dir, "/") == false) {
      sprintf(path, "/%s", dir);
    } else {
      sprintf(path, "%s", dir);
    }

    printf("buildDirTree %s\n", path);
    
    DirectoryScanner scanner(path);
    DirEntryList dirList;

    int m = scanner.scanDir("*", dirList);
    printf("DirList dir=%s count=%d\n", dir, m);

    for (int i = 0; i<m; i++) {
      const char* name = dirList.getName(i);
      printf("name %s\n", name);
      Args args;
      args.reset();
      args.set(XmNentryParent,     0);
      args.set(XmNoutlineState,    XmEXPANDED); 
      args.set(XmNsmallIconPixmap, folderPixmap);
      args.set(XmNviewType,        XmSMALL_ICON);
      args.set(XmNshadowThickness, 0);
      args.set(XmNborderWidth,     0);
 
      IconGadget* child = NULL;
      if (node) {
        child = node -> add(name, args);
      } else {
        child = new IconGadget(container, name, args); 
      }
      container->add(child);
    }
  }

public:
  FolderSelectionBox(View* parent, const char* name, Args& args)
  :ScrolledWindow()
  {
    FolderSelectionBox::create(parent, name, args);
  }

  FolderSelectionBox()
  :ScrolledWindow()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    ScrolledWindow::create(parent, name, 
        args.set(XmNscrollingPolicy, XmAUTOMATIC)
          .set(XmNadjustLast, TRUE)
          .set(XmNvisualPolicy, XmVARIABLE)
          .set(XmNscrollBarDisplayPolicy, XmAS_NEEDED)
          .set(XmNspacing, 0)
          .set(XmNborderWidth, 0) );

    folderName = "";
  
    Args ar;
    ar.reset();

    Color color(this);
    //Pixel selectColor = color.allocNamedColor("lightblue");
    Pixel selectColor = color.allocNamedColor("lightcyan");

    ar.reset();
    ar.set(XmNlayoutType, XmOUTLINE);
    ar.set(XmNoutlineLineStyle, XmNO_LINE); 
    ar.set(XmNselectionPolicy, XmBROWSE_SELECT);
    ar.set(XmNprimaryOwnership, XmOWN_MULTIPLE);
    //ar.set(XmNprimaryOwnership, XmOWN_NEVER); //XmOWN_MULTIPLE);
    ar.set(XmNselectColor, selectColor);

    container = new Container(this, "folderSelectionContainer", ar);    
/*
    Widget hsb = (Widget)get(XmNhorizontalScrollBar);
    Widget vsb = (Widget)get(XmNverticalScrollBar);
    setArea(hsb, vsb, container->getWidget());  
*/
    try {
      Pixel bg;
      container -> get(XmNbackground, (XtArgVal)&bg);
      //We use folder_xpm variable included by xpm/folder.xpm
      folderPixmap = new Pixelmap(getDisplay(), bg, folder_xpm);
    } catch (Exception& ex) {
      ex.display();
    }

    static const char* rootName = "/";
    //const char* 
    rootFolder = rootName;
    if (args.has(XmNrootFolderName)) {
      rootFolder = (const char*)args.get(XmNrootFolderName); 
      if(StringT<char>::startsWith(rootFolder, rootName) == false) {
        rootFolder = rootName; 
      } else {
        //rootFolder = rootFolder;
      }
    }

    ar.reset(); 
    ar.set(XmNentryParent, 0);
    ar.set(XmNoutlineState, XmEXPANDED);
    if (folderPixmap) {
      ar.set(XmNsmallIconPixmap, folderPixmap);
    }
    ar.set(XmNviewType, XmSMALL_ICON);
    ar.set(XmNshadowThickness, 0);
    ar.set(XmNborderWidth,  0);
    if (strcmp((const char*)rootFolder, "/") ) {
      //If rootFolder is not "/", then we create IconGadget
      root = new IconGadget(container, rootFolder, ar);
      container -> add(root);
    } else {
      //else , we don't create IconGadage to "/",
      //which means root IconGadget is NULL.

      root = NULL;
    }
  
    buildDirTree(container, root, rootFolder);

    container -> addCallback(XmNselectionCallback, this,
		(Callback)&FolderSelectionBox::selected, container);
  }

  ~FolderSelectionBox()
  {
  }

  void getFolderName(StringT<char>& name)
  {
    name = folderName;
  }

  //To retrieve a folder name which starts with "/".
  void getRootedFolderName(StringT<char>& name)
  {
    name = "/";
    name +=  folderName;
  }

  const char* getFolderName()
  {
    return (const char*)folderName;
  }

  //To retrieve a folder name which starts with "/".
  StringT<char> getRootedFolderName()
  {
     return (StringT<char>("/") + folderName);  
  }

  void setSelectCallback(CommonObject* object,
         Callback callback, XtPointer data)
  {
    if (selectAction) {
      selectAction = NULL;
    }
    if (selectAction == NULL) {
      selectAction = new Action(this, object, callback, data, "");
    }
  }
};
}


