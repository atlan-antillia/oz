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
 *  RoutingTableWatcher.cpp
 *
 *****************************************************************************/


#include <oz++/CommonObject.h>
#include <oz++/LocalDateTime.h>
#include <oz++/Exception.h>
#include <oz++/SmartArray.h>
#include <oz++/Thread.h>
#include <oz++/CharString.h>
#include <oz++/LinkedList.h>
#include <oz++/Property.h>

#include <oz++/netlink/SocketNetlink.h>
#include <oz++/netlink/SockAddrNetlink.h>
#include <oz++/netlink/RoutingTableAttribute.h>
#include <oz++/netlink/NetlinkMsgHeader.h>
#include <oz++/netlink/RoutingTableMsg.h>
#include <oz++/netlink/NetlinkReply.h>


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/TableView.h>


namespace OZ {

class RoutingTableWatcher :public Thread {
private:
  View*           view; //Shallow copy
  SockAddrNetlink addr;
  SocketNetlink   socketNetlink;

public:
  RoutingTableWatcher(View* v)
  :Thread(),
  view(v)
  {
  }

  int bind()
  {
    addr.setFamily(AF_NETLINK);
    addr.setGroups(RTMGRP_IPV4_ROUTE);
    return socketNetlink.bind(addr);
  }
	
  void recv()
  {
    while (true) {
      char buffer[4096];
      bzero(buffer, sizeof(buffer));
      int n = socketNetlink.recv(buffer, sizeof(buffer), 0);
      if (n < 0){
        break;;
      }
        
      NetlinkMsgHeader replyHeader(buffer, n);
      if (replyHeader.isDone() || replyHeader.isError() ) {
        break;
      }

      try {
        do {
          RoutingTableMsg routingTableMsg(replyHeader.data());
          LocalDateTime dateTime;
          CharString now = dateTime.nowToSeconds();
        	
          RoutingTableAttribute attr(replyHeader, routingTableMsg);
          do {
            attr.parse();
          } while (attr.next() );

         	
          LinkedListT<Property> list;
          list.add(new Property("DateTime", (const char*)now) );
          routingTableMsg.getPropertyList(list);
        	
          //Call updateModel        		
          view -> updateModel(&list);
        	
        } while (replyHeader.next());

      } catch (Exception& ex) {
        caught(ex);   
      }
    } //while
  }
	
public:
  //Thread main proc method
  void run()
  {
    try {
      bind();
      recv();
    } catch (Exception& ex) {
      caught(ex);
    }
  }	
};


class MainView :public ApplicationView {
private:
  SmartPtr<TableView>           table;
  SmartArray<CharString*>       model;
  SmartPtr<RoutingTableWatcher> watcher;
  
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
  :ApplicationView(applet, name, args),
  table(NULL)
  {
    ColumnData columnData[]  = {
      {"Value", 500, XmALIGNMENT_BEGINNING}, 
    };

    RowData rowData[]  = {
    {"DateTime",      160, XmALIGNMENT_BEGINNING},
    {"RTMSG family",    0, XmALIGNMENT_BEGINNING},
    {"RTMSG dst_len",   0, XmALIGNMENT_BEGINNING},
    {"RTMSG src_len",   0, XmALIGNMENT_BEGINNING},
    {"RTMSG tos",       0, XmALIGNMENT_BEGINNING},
    {"RTMSG table",     0, XmALIGNMENT_BEGINNING},
    {"RTMSG protocol",  0, XmALIGNMENT_BEGINNING},
    {"RTMSG scope",     0, XmALIGNMENT_BEGINNING},
    {"RTMSG type",      0, XmALIGNMENT_BEGINNING},
    {"RTMSG flags",     0, XmALIGNMENT_BEGINNING},

    {"RTA_DST",         0, XmALIGNMENT_BEGINNING},
    {"RTA_SRC",         0, XmALIGNMENT_BEGINNING},
    {"RTA_IIF",         0, XmALIGNMENT_BEGINNING},
    {"RTA_OIF",         0, XmALIGNMENT_BEGINNING},
    {"RTA_GATEWAY",     0, XmALIGNMENT_BEGINNING},

    {"RTA_PRIORITY",    0, XmALIGNMENT_BEGINNING},
    {"RTA_PREFSRC",     0, XmALIGNMENT_BEGINNING},
    {"RTA_METRICS",     0, XmALIGNMENT_BEGINNING},
    {"RTA_MULTIPATH",   0, XmALIGNMENT_BEGINNING},
    {"RTA_FLOW",        0, XmALIGNMENT_BEGINNING},
    {"RTA_CACHEINFO",   0, XmALIGNMENT_BEGINNING},
    {"RTA_TABLE",       0, XmALIGNMENT_BEGINNING},
    {"RTA_MARK",        0, XmALIGNMENT_BEGINNING},
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

    watcher = new RoutingTableWatcher(this);
    watcher -> start();
  }
  
  ~MainView() 
  {
    watcher -> cancel();
    watcher -> wait();

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
    args.set(XmNgeometry, "700x500");
    MainView view(applet, argv[0], args);
    
    view.realize();
    applet.run();
    
  } catch (Exception& ex) {
    caught(ex); 
  }
  return 0;
}

