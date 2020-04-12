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
 *  NetlinkWatcher.cpp
 *
 *****************************************************************************/


#include <oz++/CommonObject.h>
#include <oz++/LocalDateTime.h>
#include <oz++/Exception.h>
#include <oz++/SmartArray.h>
#include <oz++/Thread.h>
#include <oz++/CharString.h>
#include <oz++/LinkedList.h>

#include <oz++/netlink/SocketNetlink.h>
#include <oz++/netlink/SockAddrNetlink.h>
#include <oz++/netlink/LinkLayerAttribute.h>
#include <oz++/netlink/NetlinkMsgHeader.h>
#include <oz++/netlink/LinkLayerMsg.h>

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/TableView.h>


namespace OZ {

class NetlinkWatcher: public Thread {

private:
  View*            view;
  SocketNetlink    socketNetlink;
  SockAddrNetlink  addr;
  bool             looping;

public:
  NetlinkWatcher(View* v)
  :Thread(),
   view(v)
  {
    looping = true;
  }
    
  ~NetlinkWatcher()
  {
     socketNetlink.close(); 
  }
    
  int bind()
  {
    addr.setFamily(AF_NETLINK);
    addr.setGroups(RTMGRP_LINK);
    return socketNetlink.bind(addr);
  }
    
  void recv()
  {
    while (looping) {
      char buffer[4096];
      bzero(buffer, sizeof(buffer));
      //Blocking recv from the socketNetlink.
      int n = socketNetlink.recv(buffer, sizeof(buffer), 0);
      if ( n < 0 ){
        perror("recv");
        break;
      }
      try {         
        NetlinkMsgHeader netlinkMsgHeader(buffer, n);
        if (netlinkMsgHeader.isDone() || netlinkMsgHeader.isError()) {
          break;
        }

        do {
          LocalDateTime dateTime;
          CharString now = dateTime.nowToSeconds();

          LinkLayerMsg linkLayerMsg(netlinkMsgHeader.data());
          LinkLayerAttribute attribute(netlinkMsgHeader, linkLayerMsg);
        
          do {
            attribute.parse();
          } while (attribute.next() );

          LinkedListT<Property> list;
          list.add(new Property("DateTime", (const char*)now));
          linkLayerMsg.getMinPropertyList(list);

          //Call updateModel
          view -> updateModel(&list);

        } while (netlinkMsgHeader.next());
        
      } catch (Exception& ex) {
        caught(ex);   
      }
    }
  }

  void stop()
  {
    looping = false;
    socketNetlink.close();
  }

  void run()
  {
    try {
      this -> bind();
      this -> recv();
    
    } catch (Exception& ex) {
      caught(ex);   
    }
  }
};
    

class MainView :public ApplicationView {
private:
  SmartPtr<TableView>       table;
  SmartArray<CharString*>   model;
  SmartPtr<NetlinkWatcher>  watcher;
  
private:
  virtual void updateModel(CommonObject* object)
  {
    if (object) {
      //Update model
      LinkedListT<Property>* list = (LinkedListT<Property>*)object;
      model.removeAllItems();
      size_t size = list->getLength();
      for (size_t i = 0; i < size; i++) {
        Property* prop = list->getNth(i);
        model[i] = new CharString(prop -> value());
      }
      //Call updateView
      updateView();
    }
  }

  virtual void updateView()
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
    // Call flush to update items of the table immediately. 
    flush();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    ColumnData columnData[]  = {
      {"Value", 500, XmALIGNMENT_BEGINNING}, 
    };

    RowData rowData[]  = {
    {"DateTime",      160, XmALIGNMENT_BEGINNING},
    {"IFINFO family",   0, XmALIGNMENT_BEGINNING},
    {"IFINFO type",     0, XmALIGNMENT_BEGINNING},
    {"IFINFO index",    0, XmALIGNMENT_BEGINNING},
    {"IFINFO flags",    0, XmALIGNMENT_BEGINNING},
    {"IFINFO change",   0, XmALIGNMENT_BEGINNING},
    {"IFLA ifname",     0, XmALIGNMENT_BEGINNING},
    {"IFLA address",    0, XmALIGNMENT_BEGINNING},
    {"IFLA broadcast",  0, XmALIGNMENT_BEGINNING},
    };

    int numColumns = XtNumber(columnData);
    int numRows    = XtNumber(rowData);

    Args ar;
    ar.set(XmNtableNumColumns, numColumns);
    ar.set(XmNtableColumnData, (XtArgVal)columnData);
    ar.set(XmNtableNumRows, numRows);
    ar.set(XmNtableRowData, (XtArgVal)rowData);
    table = new TableView(this, "", ar);
    table -> removeAllItems();

    watcher = new NetlinkWatcher(this);
    watcher -> start();
  }
  
  ~MainView() 
  {
    watcher -> cancel();
    watcher -> wait();
    //delete  watcher;

    model.clear();
  }
};
}

//
int main(int argc, char** argv){

  try {
    const char*  appclass = "NetlinkWatcher";
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

