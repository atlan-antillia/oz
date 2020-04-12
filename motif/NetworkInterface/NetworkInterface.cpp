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
 *  NetworkInterface.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/SashlessPanedWindow.h>
#include <oz++/motif/ListBox.h>
#include <oz++/motif/TableView.h>
#include <oz++/NetworkInterface.h>
#include <oz++/DirectoryScanner.h>
#include <oz++/NetworkInterface.h>
#include <oz++/SmartArray.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<SashlessPanedWindow> panedw;
  SmartPtr<ListBox>             listBox;
  SmartPtr<TableView>           table;
  SmartArray<CharString*>       model;

  void listupInterfaceNames(ListBox* listb)
  {
    try {
      const char* dir = "/sys/class/net";

      DirectoryScanner scanner(dir);
      DirEntryList dirList;
      int count = scanner.scanDir("*", dirList);

      for(int i = 0; i<count; i++) {
        const char* ifname = dirList.getName(i);
        printf("IfName: %s\n", ifname);
        CompoundString cs(ifname);
        listb -> addLastItemUnselected(cs);
      }
    } catch (Exception& ex) {
      caught(ex);
    }
  }

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

    //Call updateModel
    updateModel(item);

    XtFree(item);
  }
  
  void  updateModel(const char* ifname)
  {
    model.removeAllItems();

  	try { 
      NetworkInterface interface(ifname);
      int n = 0;
      Bytes bytes = interface.hardwareAddr(); 
      int mtu = interface.mtu();
      char value[20];
      sprintf(value, "%d", mtu);

      model[n++] = new CharString(interface.ifAddrString());
      model[n++] = new CharString(interface.dstAddrString());
      model[n++] = new CharString(interface.broadAddrString());
      model[n++] = new CharString(bytes.toHexString());
      model[n++] = new CharString(interface.flags());
      model[n++] = new CharString(value);
 
  	} catch (Exception& ex) {
  		caught(ex);	
  	}

    //Call updateView
    updateView();
  }

  void updateView()
  {
    table -> removeAllItems();

    int size = model.getSize();
    for (int i = 0; i<size; i++) {
      CharString* value = model[i];
      if (value) {
        printf("%d %s\n", i, (const char*)(*value));
        table -> setItem(i, 0, (const char*)(*value));
      }
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    ColumnData columnData[]  = {
      {"Value", 440, XmALIGNMENT_BEGINNING}, 
    };

    RowData rowData[]  = {
      {"InterfaceAddress", 160, XmALIGNMENT_END}, 
      {"Destinationddress",  0, XmALIGNMENT_END}, 
      {"BroadcastAddress",   0, XmALIGNMENT_END}, 
      {"HardwareAddress",    0, XmALIGNMENT_END}, 
      {"Flags",              0, XmALIGNMENT_END}, 
      {"Mtu",                0, XmALIGNMENT_END}, 
    };

    Args ar;
    ar.set(XmNorientation, XmHORIZONTAL);
    panedw = new SashlessPanedWindow(this, "", ar);

    ar.reset();
    listBox = new ListBox(panedw, "", ar);

    listupInterfaceNames(listBox);
    
    listBox -> addCallback(XmNbrowseSelectionCallback, this,
         (Callback)&MainView::browse, NULL);

    int numColumns = XtNumber(columnData);
    int numRows    = XtNumber(rowData);

    ar.reset();
    ar.set(XmNtableNumColumns, numColumns);
    ar.set(XmNtableColumnData, (XtArgVal)columnData);
    ar.set(XmNtableNumRows, numRows);
    ar.set(XmNtableRowData, (XtArgVal)rowData);
    table = new TableView(panedw, "", ar);
    table -> removeAllItems();
  }
  
  ~MainView() 
  {
    model.clear();
  }
};
}

//
int main(int argc, char** argv){

  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);
    Args args;
    args.set(XmNgeometry, "700x240");
    MainView view(applet, argv[0], args);
    
    view.realize();
    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

