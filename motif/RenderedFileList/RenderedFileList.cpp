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
 *  RenditionList.cpp
 *
 *****************************************************************************/

//2015/02/22 

#include <oz++/DirectoryScanner.h>
#include <oz++/SmartPtrs.h>
#include <oz++/FileAttributes.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/CompoundStringList.h>
#include <oz++/motif/RenderTable.h>
#include <oz++/motif/RenditionTag.h>
#include <oz++/motif/Color.h>

namespace OZ {

static const XmStringTag TAG_DIR    = XmStringTag("dir");
static const XmStringTag TAG_SLINK  = XmStringTag("slink");
static const XmStringTag TAG_FILE   = XmStringTag("file");
static const XmStringTag TAG_EXE    = XmStringTag("exe");

class MainView :public ApplicationView {
private:
  SmartPtr<Form>            form;
  SmartPtr<Label>           label;
  SmartPtr<TextField>       textField;
  SmartPtr<ScrolledListBox> scrolledListBox;
  SmartPtr<RenderTable>     rtable;

  void   enter(Action& action)
  {
    char* dir = textField -> getString();
    try {
      //Does the directory of string exist?
      FileAttributes attr(dir);
      if (attr.isDirectory() ) {
        addDirectoriesAndFiles(dir);
      } else {
        printf("Not directory %s\n", dir);
      }
    } catch (Exception& ex) {
      caught(ex);
    }
    XtFree(dir);
  }

  void addDirectoriesAndFiles(const char* dir)
  {
    ListBox* listb= scrolledListBox -> getList();
    listb -> deleteAllItems();
    DirectoryScanner scanner(dir);
    DirEntryList dirList;

    int m = scanner.scanDir("*", dirList);
    if (m > 0) {
      SmartPtrs<char*> names = new char*[m]; 
      dirList.getNames((char**)names);
      CompoundStringList csl((char**)names, m, 
		(XmStringTag)NULL, XmCHARSET_TEXT, TAG_DIR); 

      listb -> addItemsUnselected(csl); 
    }

    int n = scanner.scanFile("*", dirList);
    if (n > 0) {
      char** names = new char*[n];
      dirList.getNames(names);
   
      CompoundStringList csl;
 
      for (int i = 0; i<n; i++) {
        char fullpath[PATH_MAX];
        if (__endsWith(dir, '/')) {
          sprintf(fullpath, "%s%s",  dir, names[i]);
        } else {
          sprintf(fullpath, "%s/%s", dir, names[i]);
        }
        try {
          FileAttributes attr(fullpath);
          XmStringTag tag = TAG_FILE;
          if (attr.isExecutable()) {
           tag = TAG_EXE;
          }
          csl.add(names[i], (XmStringTag)NULL, XmCHARSET_TEXT, tag);
        } catch (Exception& ex) {
          caught(ex);
        }
      }
      listb -> addItemsUnselected(csl); 
      
      SmartPtrs<char*> forgc = names;
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    form = new Form(this, "", ar);
    Color color(this);
  	Pixel black = color.blackPixel();
  	Pixel white = color.whitePixel();
  	Pixel blue  = color.allocNamedColor("blue");
  	Pixel red   = color.allocNamedColor("red");
  	Pixel green = color.allocNamedColor("darkgreen");
    
    rtable = new RenderTable();

    RenditionTag rtag[] = {
      {TAG_DIR,   "times-12", 12, blue},
//    {TAG_SLINK, "times-12", 12, red},
      {TAG_FILE,  "times-12", 12, black},
      {TAG_EXE,   "times-12", 12, green},
    };
    
    for (int i = 0; i< XtNumber(rtag); i++) {
      ar.reset();
  	  ar.set(XmNfontName, rtag[i].fontName);
  	  ar.set(XmNfontSize, rtag[i].fontSize);
      ar.set(XmNrenditionForeground, rtag[i].foreground);
      rtable->add(this, rtag[i].tag, ar); 
    }
	
    ar.reset();
    ar.set(XmNtopAttachment, XmATTACH_FORM);
    ar.set(XmNleftAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment, XmATTACH_FORM); 
    label = new Label(form, "Input a directory name with fullpath:", ar);
    const char* dir = "/";

    ar.reset();
    ar.set(XmNtopAttachment, XmATTACH_WIDGET);
    ar.set(XmNtopWidget, label); 
    ar.set(XmNleftAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment, XmATTACH_FORM); 
    textField = new TextField(form, "", ar);
    textField -> addCallback(XmNactivateCallback, this,
              (Callback)&MainView::enter, NULL);
 
    ar.reset();
    ar.set(XmNtopAttachment, XmATTACH_WIDGET);
    ar.set(XmNtopWidget, textField); 
    ar.set(XmNleftAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment, XmATTACH_FORM); 
    ar.set(XmNbottomAttachment, XmATTACH_FORM); 
    ar.set(XmNvisibleItemCount, 15);
    scrolledListBox = new ScrolledListBox(form, "", ar);
  	scrolledListBox -> getList() -> set(XmNbackground, white);
  	
    scrolledListBox -> getList() -> set(XmNrenderTable, rtable);

    addDirectoriesAndFiles(dir);

    textField -> setString((char*)dir);
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
    args.set(XmNwidth, 500);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    applet.noCleanup();

  } catch (Exception& ex) {
     caught(ex);
  }
  return 0;
}
