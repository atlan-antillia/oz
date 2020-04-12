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
 *  ScrolledFileIconBox.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/ScrolledFileIconBox.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/PixmapFile.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<ScrolledFileIconBox>        scrolledFileIconBox;
 
  void   activate(Action& action)
  {
    XmIconButtonCallbackInfo* cbs =
        (XmIconButtonCallbackInfo*)action.getCallData();
   
    View* sender = (View*)action.getSender();
    XmString xms;
    sender -> get(XmNlabelString, (XtArgVal)&xms);
    int fileType;
    sender -> get(XmNuserData, (XtArgVal)&fileType);
    CompoundString cs(xms);
    char* name = cs.unparse();
    printf("activated %s fileType=%d\n", name, fileType);
    XtFree(name);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Color color(this);
 
    Args ar;
    ar.set(XmNminimumHorizontalCells, 6);
    ar.set(XmNfolderIconFile, "../xpm/folder48.xpm");
    ar.set(XmNfileIconFile,   "../xpm/file48.xpm");
    scrolledFileIconBox = new ScrolledFileIconBox(this, "", ar);
  
    FileIconBox* iconBox = scrolledFileIconBox -> getIconBox();
    Pixel fg = color.whitePixel();
    iconBox -> set(XmNbackground, fg);

    const char* dir = getenv("HOME");
    const char* pattern = "*";

    iconBox -> addDirectories(dir, pattern);
    iconBox -> addFiles(dir, pattern);
    iconBox -> addCallback(XmNactivateCallback, this,
          (Callback)&MainView::activate, NULL);
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
    args.set(XmNgeometry, "600x500");
    MainView view(applet, argv[0], args);
    view.realize();

     applet.run();
  } catch (Exception& ex) {
     caught(ex);
  }
  return 0;
}
