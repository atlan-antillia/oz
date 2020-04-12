/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ListView.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/ListView.h>
#include <oz++/motif/CompoundStringList.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<ListView>   listview;

  void    browse(Action& action)
  {
    XmListCallbackStruct* cbs =
       (XmListCallbackStruct*)action.getCallData();
    //item_position starts from 1.
    int position = cbs -> item_position;
    CompoundString cs(cbs -> item);
    char* item;
    cs.get(&item);
    printf("item: postion=%d  item=%s\n", position, item);
    XtFree(item);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    const char* columns[] = {"id", "object", "score", "x", "y", "w", "h"};
    CompoundStringList column_titles(columns, CountOf(columns));
    Args ar;
    ar.set(XmNfirstColumnPixmaps, True); 
    //args.set(XmNentryData, row_info); 
    ar.set(XmNnumRows, 0); 
    ar.set(XmNnumColumns,   CountOf(columns)); 
    ar.set(XmNcolumnTitles, column_titles);
    
    ar.set(XmNtitle, CompoundString("Lineup")); 
    //args.set(XmNsortFunctions, sort_funcs);        
    //ar.set(XmNselectionPolicy, XmSINGLE_SELECT);   
    ar.set(XmNshowSash, False);   
    
    //listview = new ListView(this, "listv", ar);
    //listbox -> addCallback(XmNbrowseSelectionCallback, this,
    //            (Callback)&MainView::browse, NULL);
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

