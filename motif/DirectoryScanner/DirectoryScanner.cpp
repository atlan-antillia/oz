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
 *  DirectoryScanner.cpp
 *
 *****************************************************************************/

//2015/02/14 Updated

#include <oz++/DirectoryScanner.h>
#include <oz++/SmartPtrs.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/Label.h>
#include <oz++/FileAttributes.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<Form>            form;
  SmartPtr<Label>           label;
  SmartPtr<TextField>       textField;
  SmartPtr<ScrolledListBox> scrolledListBox;

  void   enter(Action& action)
  {
    char* string = textField -> getString();
    try {
      //Does the directory of string exist?
      FileAttributes attr(string);
      if (attr.isDirectory()) {
        addDirectories(scrolledListBox, string);
      } else {
        printf("Not directory %s\n", string);
      }
    } catch (Exception& ex) {
      caught(ex);
    }
    XtFree(string);
  }

  void addDirectories(ScrolledListBox* scrolledListBox, const char* dir)
  {
    ListBox* listb= scrolledListBox -> getList();
    listb -> deleteAllItems();
    listb -> listupDirectories(dir, "*");
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    form = new Form(this, "", ar);
    
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

    addDirectories(scrolledListBox, dir);
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
  } catch (Exception& ex) {
     caught(ex);
  }
  return 0;
}
