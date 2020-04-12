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
 *  LabeledFrame.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/SashlessPanedWindow.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/PanedWindow.h>
#include <oz++/motif/LabeledFrame.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/FileAttributes.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<SashlessPanedWindow>    vpanedWindow;
  SmartPtr<TextField>              textField;
  SmartPtr<PanedWindow>            panedWindow;
  SmartPtr<LabeledFrame>           dirFrame;
  SmartPtr<ScrolledListBox>        dirScrolledListBox; 
  SmartPtr<LabeledFrame>           fileFrame;
  SmartPtr<ScrolledListBox>        fileScrolledListBox; 

  void   enter(Action& action)
  {
    char* string = textField -> getString();
    printf("string [%s]\n", string);
    
    char dir[PATH_MAX];
    char pattern[PATH_MAX];
    memset(dir, '\0', sizeof(dir));

    strcpy(dir, string);
    strcpy(pattern, "*");
 
    char* slash = strrchr(string, '/');
    if (slash) {
      char* as = strchr(++slash, '*');
      if (as) {
        strcpy(pattern, slash);
        *slash = '\0';
        strcpy(dir, string);
      } else{
        strcpy(dir, string);
      }
    }
    printf("dir=[%s] pattern=[%s]\n", dir, pattern);
    try {
      FileAttributes attr(dir);
      if (attr.isDirectory()) {
        listup(dir, pattern);
      } else {
        printf("Not directory %s\n", dir);
      }
   
    } catch (Exception& ex) {
      caught(ex);
    }
    XtFree(string);
  }

  void listup(const char* dir, const char* pattern = "*") 
  {
     dirScrolledListBox -> clear();
     fileScrolledListBox -> clear();

     ListBox* dirListBox  = dirScrolledListBox -> getList();
     ListBox* fileListBox = fileScrolledListBox -> getList();

     dirListBox  -> listupDirectories(dir, pattern);
     fileListBox -> listupFiles(dir, pattern);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    const char* dir = getenv("HOME");

    ar.set(XmNorientation, XmVERTICAL);
    vpanedWindow = new SashlessPanedWindow(this, "", ar);
    ar.reset();
    textField = new TextField(vpanedWindow, "", ar);
    textField -> set(XmNvalue, dir);
    textField -> addCallback(XmNactivateCallback, this,
              (Callback)&MainView::enter, NULL);

    ar.reset();
    ar.set(XmNorientation, XmHORIZONTAL);
    panedWindow = new PanedWindow(vpanedWindow, "", ar);

    CompoundString cs("Directories:");
    ar.set(XmNlabelString, cs);
    dirFrame = new LabeledFrame(panedWindow, "dirframe", ar);

    ar.reset();
    dirScrolledListBox = new ScrolledListBox(dirFrame, "", ar);


    CompoundString fcs("Files:");
    ar.set(XmNlabelString, fcs);
    fileFrame = new LabeledFrame(panedWindow, "fileframe", ar);

    ar.reset();
    fileScrolledListBox = new ScrolledListBox(fileFrame, "", ar);

    listup(dir);
  }

  ~MainView()
  {
  }
};

}

//
int main(int argc, char** argv)
{
  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "400x400");

    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

