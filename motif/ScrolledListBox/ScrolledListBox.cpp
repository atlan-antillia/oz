/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ScrolledListBox.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/CompoundString.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<ScrolledListBox>   sclist;

  void  browse(Action& action)
  {
    XmListCallbackStruct* cbs =
       (XmListCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->item);
    char* item;
    cs.get(&item);
    printf("item: %s\n", item);
    XtFree(item);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    static const char* items[] = {
                "white", "gray", "yellow", "green", "cyan",
                "blue",  "red",  "brown", "navy", "black" };
    CompoundStringList csl(items, XtNumber(items));

    Args ar;
    ar.set(XmNscrollBarDisplayPolicy, XmVARIABLE);
    ar.set(XmNlistSizePolicy,         XmCONSTANT);
    ar.set(XmNitems,    csl);
    ar.set(XmNvisibleItemCount,  4);
    ar.set(XmNitemCount, XtNumber(items));
    sclist = new ScrolledListBox(this, "list", ar);

    sclist -> getList()
           -> addCallback(XmNbrowseSelectionCallback, this,
                     (Callback)&MainView::browse, NULL);
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
    args.set(XmNgeometry, "400x300");

    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

