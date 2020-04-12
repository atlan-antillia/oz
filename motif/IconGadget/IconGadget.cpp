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
 *  IconGadget.cpp
 *
 *****************************************************************************/

#include <oz++/DirectoryScanner.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/MainWindow.h>
#include <oz++/motif/ScrolledWindow.h>
#include <oz++/motif/Container.h>
#include <oz++/motif/IconGadget.h>
#include <oz++/motif/CompoundString.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<MainWindow>     mainw;
  SmartPtr<ScrolledWindow> scrolledWindow;
  SmartPtr<Container>      container;
  SmartPtr<IconGadget>     root;

  void selected(Action* action)
  {
    XmContainerSelectCallbackStruct* cbs = 
	(XmContainerSelectCallbackStruct*)action ->getCallData();

    for (int i = 0; i < cbs->selected_item_count; i++) {
       View sel(cbs->selected_items[i]);
       XmString xms;
       sel.get(XmNlabelString, &xms); 
       CompoundString cs(xms);
       char* name = cs.unparse();
       printf ("Selected: %s\n", name);
       XtFree(name);
    }
  }

  void  buildRoot(View* container, IconGadget* node, const char* dir)
  {
     DirectoryScanner scanner(dir);
     DirEntryList dirList;

    int m = scanner.scanDir("*", dirList);
    printf("DirList m=%d\n", m);

    for (int i = 0; i<m; i++) {
      const char* name = dirList.getName(i);
      printf("name %s\n", name);
      Args args;
      args.reset();
      //args.set(XmNlayoutType     , XmDETAIL);
	  //args.set(XmNoutlineState   , XmEXPANDED 
      Gadget* child = node -> add(name, args);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    mainw = new MainWindow(this, "", ar);

    ar.reset();
    ar.set(XmNscrollingPolicy, XmAUTOMATIC);

    scrolledWindow = new ScrolledWindow(mainw, "", ar);

    ar.reset();
    ar.set(XmNlayoutType, XmDETAIL);
    container = new Container(scrolledWindow, "", ar);    

    ar.reset(); 
    ar.set(XmNentryViewType, XmSMALL_ICON);
    ar.set(XmNselectionPolicy, XmSINGLE_SELECT);     
    ar.set(XmNentryParent, 0);
    ar.set(XmNoutlineState, XmEXPANDED);
    ar.set(XmNlayoutType, XmDETAIL);

    root = new IconGadget(container, "root", ar);
    buildRoot(container, root, "/" );

    container -> addCallback(XmNselectionCallback, this,
			(Callback)&MainView::selected, NULL);
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
    args.set(XmNgeometry, "600x500");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

