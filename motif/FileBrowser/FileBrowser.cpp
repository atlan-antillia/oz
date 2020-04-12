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
 *  FileBrowser.cpp
 *
 *****************************************************************************/


#include <oz++/DirectoryScanner.h>
#include <oz++/SmartPtrs.h>
#include <oz++/SmartArray.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/SashlessPanedWindow.h>
#include <oz++/motif/PanedWindow.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/DropdownComboBox.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/FileAttributes.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<PanedWindow>     vpanedWindow;
  SmartPtr<PanedWindow>     panedWindow;
  SmartPtr<Form>            form;
  SmartPtr<TextField>       textField;

  SmartPtr<ScrolledListBox> dirScrolledListBox;
  CharString       currentDir;

  SmartPtr<ScrolledListBox> fileScrolledListBox;
  SmartPtr<TextField>       fileFullpath;
  SmartPtr<ScrolledText>    scrolledText;

  void   enter(Action& action)
  {
    char* string = textField -> getString();
    try { 
      updateListBoxes(string);

    } catch (Exception& ex) {
      caught(ex);
    }
    XtFree(string);
  }

  void   updateListBoxes(const char* string)
  {
    try {
      dirScrolledListBox -> clear();
      fileScrolledListBox -> clear();
      FileAttributes attr(string);
      if (attr.isDirectory()) {
        addDirectories(dirScrolledListBox, string);
        addFiles(fileScrolledListBox, string);
      } else {
        printf("Invalid %s\n", string);
      }
    } catch (Exception& ex) {
      caught(ex);
    }
  }

  void browseDir(Action& action)
  {
    XmListCallbackStruct* cbs =
       (XmListCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->item);
    char* item = cs.unparse();
    char* rname = item;
    char path[PATH_MAX];
    if ((item[0] == '+' || item[0] == '-' || item[0] == '.') && 
        (item[1] == ' ' || item[1] == '.') ) {
      rname = item + 2; 
    }    
    char* string = textField -> getString();
    if (item[0] == '.' && item[1] == '.') {
       char* slash = strrchr(string, '/');
       if (slash) {
         *slash = '\0'; 
         const char* dir = string;
         if (strlen(dir) == 0) {
            dir = "/"; 
         }
         currentDir = string;
         printf("browseDir update textField %s\n", dir);
         textField -> set(XmNvalue, dir);
         updateListBoxes(dir);
        XtFree(string);
        XtFree(item);
        return;
       }  
    }
    if (__endsWith(string, '/')) { 
      sprintf(path, "%s%s", string, rname);  
    } else {
      sprintf(path, "%s/%s", string, rname);  
    }
    currentDir = rname;

    printf("Mainview::browseDir item: %s path:%s\n", rname, path);
    addDirectories(fileScrolledListBox, path);
    addFiles(fileScrolledListBox, path);

    XtFree(string); 
    XtFree(item);
  } 

  void browseFile(Action& action)
  {
    XmListCallbackStruct* cbs =
       (XmListCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->item);
    char* item = cs.unparse();
    char* rname = item;
    char path[PATH_MAX];
    if ((item[0] == '+' || item[0] == ' ') && item[1] == ' ') {
      rname = item + 2;
    }   
    char* string = textField -> getString();
    if (item[0] == '+') {
      if (__endsWith(string, '/')) {
        sprintf(path, "%s%s/%s", string, (const char*)currentDir, rname); 
      } else {
        sprintf(path, "%s/%s/%s", string, (const char*)currentDir, rname); 
      }
      printf("Mainview::browseFile item: %s path:%s\n", rname, path);
      
      addDirectories(dirScrolledListBox, path);
      textField -> set(XmNvalue, path); 
      fileScrolledListBox -> getList() -> deleteAllItems(); 
      addFiles(fileScrolledListBox, path);
    } else {
     int* pos=NULL;
     int num = 0;
     dirScrolledListBox -> getList() 
                        -> getSelectedPos(&pos, &num);
     if (num > 0) { 
       XtFree((char*)pos);
       if (__endsWith(string, '/')) {
         sprintf(path, "%s%s/%s", string, (const char*)currentDir, rname); 
       } else {
         sprintf(path, "%s/%s/%s", string, (const char*)currentDir, rname); 
       }
     } else {
       if (__endsWith(string, '/')) {
         sprintf(path, "%s%s", string, rname); 
       } else {
         sprintf(path, "%s/%s", string, rname); 
       }
     }
      printf("load [%s]\n", path);
      scrolledText -> getText() -> load(path);
      fileFullpath -> set(XmNvalue, path);
    }

    XtFree(string);
    XtFree(item);
  }

  void addDirectories(ScrolledListBox* scrolledListBox, const char* dir)
  {
    ListBox* listb= scrolledListBox -> getList();
    listb -> deleteAllItems();
    if (scrolledListBox == dirScrolledListBox) {
      CompoundString cs("..");
      listb -> addItemUnselected(cs); 
    }
    try {
      listb -> listupDirectories(dir, '+');
    } catch (Exception& ex) {
      caught(ex);
    }
  }

  void addFiles(ScrolledListBox* scrolledListBox, const char* dir)
  {
    ListBox* listb= scrolledListBox -> getList();
    //listb -> deleteAllItems();

    try {
      listb -> listupFiles(dir, ' ');
    } catch (Exception& ex) {
      caught(ex);
    }
  }


public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args),
   currentDir("")
  {
    Args ar;
    ar.reset();
    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNseparatorOn, FALSE);
    ar.set(XmNspacing, 0);
    vpanedWindow = new SashlessPanedWindow(this, "panedw", ar);

    ar.reset();
    ar.set(XmNeditable, FALSE);
    textField = new TextField(vpanedWindow, "", ar);

    textField -> addCallback(XmNactivateCallback, this,
              (Callback)&MainView::enter, NULL);


    ar.set(XmNorientation, XmHORIZONTAL);
    panedWindow = new PanedWindow(vpanedWindow, "panedw", ar);

    const char* dir = "/";
    ar.reset();
    ar.set(XmNtopAttachment, XmATTACH_WIDGET);
    ar.set(XmNtopWidget, textField); 
    ar.set(XmNleftAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment, XmATTACH_FORM); 
    ar.set(XmNbottomAttachment, XmATTACH_FORM); 
    ar.set(XmNvisibleItemCount, 10);
    dirScrolledListBox = new ScrolledListBox(panedWindow, "", ar);
    ListBox* dirlistb = dirScrolledListBox -> getList(); 
    dirlistb -> set(XmNselectionPolicy, XmBROWSE_SELECT);
    dirlistb -> addCallback(XmNbrowseSelectionCallback, this,
                       (Callback)&MainView::browseDir, NULL);
                     
    textField -> setString((char*)dir);

    ar.reset();
    fileScrolledListBox = new ScrolledListBox(panedWindow, "", ar);
    ListBox* filelistb = fileScrolledListBox -> getList();
    filelistb -> set(XmNselectionPolicy, XmBROWSE_SELECT); 
    filelistb -> addCallback(XmNbrowseSelectionCallback, this,

                       (Callback)&MainView::browseFile, NULL);
                     
   updateListBoxes(dir);

   ar.reset();
   ar.set(XmNeditable, FALSE);
   fileFullpath = new TextField(vpanedWindow, "", ar);

   ar.reset();
   scrolledText = new ScrolledText(vpanedWindow, "", ar);
  }

  ~MainView()
  {
  }
};
}

//
int main(int argc, char** argv)
{
  const char*  appclass = argv[0];
  Application applet(appclass, argc, argv);
  try {
    Args args;
    args.set(XmNgeometry, "600x600");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
     caught(ex);
  }
  return 0;
}
