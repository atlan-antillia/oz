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
 *  ScrolledNotifierList.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/ScrolledNotifierList.h>
#include <oz++/motif/PixmapFile.h>

namespace OZ {

class MainView  :public ApplicationView {
private:
  SmartPtr<ScrolledNotifierList>  notifierList;
  SmartPtr<PixmapFile>            folderPixmap;
  SmartPtr<PixmapFile>            filePixmap;

  void activate(Event& event)
  {
    if (event.isEnterKey() ||  event.isButtonPressed() ) {
      event.processTraversal();

      View* sender = (View*)event.getSender();
      XmString xms;
      sender -> get(XmNlabelString, (XtArgVal)&xms);
      CompoundString cs(xms);
      char* label = cs.unparse();
      printf("acitvate %s\n", label);
      XtFree(label);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Color color(this);
    Pixel white = color.whitePixel();

    Pixel bg = white;
    Pixel fg = white;
    folderPixmap = new PixmapFile(getDisplay(), "../xpm/folder.xpm", fg, bg);
    filePixmap   = new PixmapFile(getDisplay(), "../xpm/file.xpm", fg, bg);

    Args ar;
    ar.set(XmNscrollingPolicy, XmAUTOMATIC);
    ar.set(XmNscrollBarDisplayPolicy, XmVARIABLE);
    notifierList = new ScrolledNotifierList(this, "", ar);

    NotifierList* list = notifierList -> getList();
    const char* home = getenv("HOME");
    const char* pattern = "*";
    list -> addDirectories(folderPixmap->get(), home, pattern, 
            ButtonPressMask|KeyPressMask, this,
            (Handler)&MainView::activate, NULL);
    list -> addFiles(filePixmap->get(), home, pattern, 
            ButtonPressMask|KeyPressMask, this,
            (Handler)&MainView::activate, NULL);
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

