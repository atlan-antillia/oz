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
 *  SashlessPanedWindow.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/FileDialog.h>
#include <oz++/motif/SashlessPanedWindow.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/PushButton.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<SashlessPanedWindow> panedw;
  SmartPtr<RowColumn>           rowColumn;
  SmartPtr<PushButton>          open;
  SmartPtr<PushButton>          save;
  SmartPtr<PushButton>          quit;
  CharString                    filepath;
  SmartPtr<TextField>           textf;
  SmartPtr<ScrolledText>        sctext;
  SmartPtr<FileDialog>          fileDialog;
	
  void    openFile(Action& action)
  {
    XmFileSelectionBoxCallbackStruct* cbs =
           (XmFileSelectionBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->value);
  	char* filename = cs.unparse();
    printf("filename: %s\n", filename);
  	textf -> set(XmNvalue, filename);
  	sctext -> getText() ->load(filename);
    XtFree(filename);
  	fileDialog->unmanage();
  }
	
  void    cancelFile(Action& action)
  {
    XmFileSelectionBoxCallbackStruct* cbs =
           (XmFileSelectionBoxCallbackStruct*)action.getCallData();
    fileDialog->unmanage();
  }


  void    openFileDialog(Action& action)
  {
    fileDialog -> manage();
  }
	
  void    saveFile(Action& action)
  {
    char* filename = textf -> getString();
    if (sctext-> getText() -> save(filename)) {
       printf("Save a file %s\n", filename);
    }
    filepath = filename;
    XtFree(filename);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    ar.set(XmNorientation, XmVERTICAL);
    panedw = new SashlessPanedWindow(this, "panedw", ar);
 
    ar.reset();
    ar.set(XmNorientation, XmHORIZONTAL);
    rowColumn = new RowColumn(panedw, "", ar);

    CompoundString csopen("Open");
    ar.reset();
    //ar.set(XmNheight, 30);
    ar.set(XmNlabelString, csopen);
    open = new PushButton(rowColumn, "", ar); 
    open -> addCallback(XmNactivateCallback, this,
         (Callback)&MainView::openFileDialog, NULL);

    CompoundString cssave("Save");
    ar.reset();
    //ar.set(XmNheight, 30);
    ar.set(XmNlabelString, cssave);
    save = new PushButton(rowColumn, "", ar); 
    save -> addCallback(XmNactivateCallback, this,
         (Callback)&MainView::saveFile, NULL);
 
    CompoundString csquit("Quit");
    ar.reset();
    //ar.set(XmNheight, 30);
    ar.set(XmNlabelString, csquit);
    quit = new PushButton(rowColumn, "", ar); 
    quit -> addCallback(XmNactivateCallback, this,
         (Callback)&MainView::exit, NULL);

    ar.reset();
    textf = new TextField(panedw, "textf", ar);
    //textf -> addCallback(XmNactivateCallback, this, 
    //             (Callback)&MainView::entered, NULL);

    ar.reset();
    ar.set(XmNeditMode, XmMULTI_LINE_EDIT);
 
  	ar.set(XmNheight, 400);
    sctext = new ScrolledText(panedw, "text", ar);

    ar.reset();
    fileDialog = new FileDialog(this, "filed", ar);
    fileDialog -> getFileBox()
               -> addCallback(XmNokCallback, this,
                      (Callback)&MainView::openFile, NULL);  	
    fileDialog -> getFileBox()
               -> addCallback(XmNcancelCallback, this,
                      (Callback)&MainView::cancelFile, NULL);  	

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
    args.set(XmNwidth, 500);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

