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
 *  FTFileListView.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/xft/FTFileListView.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<FTFileListView> fileListView;
  
private:
  void press(Event& event)
  {
    int id = event.getClientIntData();
    XEvent* xev = event.getXEvent();
    FTMultiColumn* sender = (FTMultiColumn*)event.getSender();
    int fileType = 0;
    sender -> get(XmNuserData, (XtArgVal)&fileType);
    const char* type = "UNKNOWN";
    if (fileType == FTFileListView::DIR) {
      type = "DIR";
    }
    if (fileType == FTFileListView::FILE) {
      type = "FILE";
    }
  	
    Color color(this);	
    Pixel lgray = color.allocNamedColor("lightgray"); 
    Pixel white = color.allocNamedColor("white"); 
    if (event.isButtonPressed() || event.isEnterKeyPressed() ) {
      sender -> set(XmNbackground, lgray);
      printf("pressed fileType=%s row=%d item=[%s]\n", type, id, sender->getItem(0) );
    }
   
    if (event.isButtonReleased() || event.isEnterKeyReleased() ) {
      sender -> set(XmNbackground, white);
      printf("released row=%d\n", id);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    static FTColumnLayout columnLayout[] = {
      {300, XmALIGNMENT_BEGINNING, "navy"},      //name
      {180, XmALIGNMENT_BEGINNING,  "darkgreen"}, //changedTime
      {180, XmALIGNMENT_END,       "orange"},    //size 
    };
    Args ar;
    ar.reset();
    ar.set(XftNfamily, "Times-12");
    //ar.set(XftNfamily, "Arial");
    ar.set(XftNweight, FC_WEIGHT_THIN);
    ar.set(XftNsize, 12);
    ar.set(XmNfolderIconFile, "../../motif/xpm/folder.xpm");
    ar.set(XmNfileIconFile,   "../../motif/xpm/file.xpm");
	
    ar.set(XmNwidth,   700);
    ar.set(XmNcolumnLayout, (XtArgVal)columnLayout);
    ar.set(XmNnumColumns, XtNumber(columnLayout));
    fileListView = new FTFileListView(this, "", ar);
  	
    const char* dir = getenv("HOME");
    const char* pattern = "*";
    fileListView -> listupDirectories(dir, pattern);
    fileListView -> listupFiles(dir, pattern);
  	
    fileListView -> addEventHandler(ButtonPressMask|ButtonReleaseMask|
                KeyPressMask|KeyReleaseMask,
  		this, (Handler)&MainView::press, NULL);
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
    args.set(XmNgeometry, "760x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

