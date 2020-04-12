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
 *  RenderTable.cpp
 *
 *****************************************************************************/

//2015/01/26 Modified to use Xft true type.
//2015/02/11 Updated
 
#include <oz++/DirectoryScanner.h>

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/RenderTable.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<RowColumn>        rowColumn;
  SmartPtr<ScrolledListBox>  scrolledListBox;
  SmartPtr<PushButton>       pushb;
  SmartPtr<RenderTable>      rtable;

  void   activate(Action& action)
  {
    XmPushButtonCallbackStruct* cbs =
        (XmPushButtonCallbackStruct*)action.getCallData();
    if(cbs->click_count == 2) {
      exit(action);
    }
  }

  void addRootDirDirs(ListBox* listb)
  {
    DirectoryScanner scanner("/");
    DirEntryList dirList;

    int m = scanner.scanDir("*", dirList);

    for (int i = 0; i<m; i++) {
      const char* name = dirList.getName(i);
      CompoundString cs(name);
      listb -> addItem(cs.get(), i);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNnumColumns, 1);
    rowColumn = new RowColumn(this, "", ar);

    ar.reset();
    ar.set(XmNvisibleItemCount, 10);
    scrolledListBox = new ScrolledListBox(rowColumn, "", ar);
    ListBox* listBox = scrolledListBox -> getList();
    addRootDirDirs(listBox);

    ar.reset();
    //1. Create an ordinay instance of PushButton
    CompoundString cs("Please click twice this button to exit\n"
                      "終了するには、2回押してください");

    ar.set(XmNlabelType, XmSTRING);
    ar.set(XmNlabelString, cs);
    pushb = new PushButton(rowColumn, "pushb", ar);

    pushb -> addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::activate, NULL);

    //2. Create an instance of RenderTable based on a Rendition object.
    ar.reset();
    ar.set(XmNfontSize, 20);
    ar.set(XmNfontName, "Times-12");
    rtable = new RenderTable(pushb, "", ar); 

    //3. Set the renderTable to the PushBitton instance.
    pushb -> set(XmNrenderTable, rtable);
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
    MainView view(applet, argv[0], args);
    view.realize();

     applet.run();
  } catch (Exception& ex) {
     caught(ex);
  }
  return 0;
}
