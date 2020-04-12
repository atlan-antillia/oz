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
 *  IPAddressWatcher.cpp
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
#include <oz++/netlink/NetlinkReply.h>
#include <oz++/netlink/NetlinkMsgHeader.h>
#include <oz++/netlink/InterfaceAddrAttribute.h>
#include <oz++/netlink/InterfaceAddrMsg.h>


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/TableView.h>


namespace OZ {

class IPAddressWatcher: public Thread {

private:
  View*                    view; //Shallow copy
  int                      rtmGroups;
  SocketNetlink            socketNetlink;
  NetlinkReply<ifaddrmsg>  reply;
  bool                     looping;
	
public:
  IPAddressWatcher(View* v, int group = RTMGRP_IPV4_IFADDR) 
                           //|RTMGRP_IPV6_IFADDR)
  :view(v),
  rtmGroups(group)
  {
    looping = true;
  }
    
  int bind()
  {
    SockAddrNetlink  addrNetlink;
    addrNetlink.setFamily(AF_NETLINK);
    addrNetlink.setGroups(rtmGroups);
    return socketNetlink.bind(addrNetlink);
  }
    
  void recv()
  {
    while (looping) {
      bzero(&reply, sizeof(reply));

      int n = socketNetlink.recv((char*)&reply, sizeof(reply), 0);
      if ( n < 0 ){
        perror("recv");
        break;
      }
      try {         

        NetlinkMsgHeader netlinkMsgHeader(&reply.head);
        if (netlinkMsgHeader.isDone() || netlinkMsgHeader.isError()) {
          break;
        }

        do {
          LocalDateTime dateTime;
          CharString now = dateTime.nowToSeconds();
          InterfaceAddrMsg addrMsg(&reply.body);
      
          InterfaceAddrAttribute attribute(netlinkMsgHeader, addrMsg);
        
          do {
            attribute.parse();
          } while (attribute.next() );
        	
          LinkedListT<Property> list;
          list.add(new Property("DateTime", (const char*)now) );
          addrMsg.getPropertyList(list);
        	
          //Call updateModel        		
          view -> updateModel(&list);
        	
        } while (netlinkMsgHeader.next());
        
      } catch (Exception& ex) {
        caught(ex);   
      }
      fflush(stdout);       
    }
  }

  void stop()
  {
    looping = false;
    socketNetlink.close();
  }

  //Thread main procedure method
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
  SmartPtr<TableView>        table;
  SmartArray<CharString*>    model;
  SmartPtr<IPAddressWatcher> watcher;
  
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
    {"DateTime",          160, XmALIGNMENT_BEGINNING},
    {"IFADDR family",     0, XmALIGNMENT_BEGINNING},
    {"IFADDR prefixlen",  0, XmALIGNMENT_BEGINNING},
    {"IFADDR flags",      0, XmALIGNMENT_BEGINNING},
    {"IFADDR scope",      0, XmALIGNMENT_BEGINNING},
    {"IFADDR index",      0, XmALIGNMENT_BEGINNING},
    {"IFA_ADDRESS",       0, XmALIGNMENT_BEGINNING},
    {"IFA_LOCAL",         0, XmALIGNMENT_BEGINNING},
    {"IFA_LABEL",         0, XmALIGNMENT_BEGINNING},
    {"IFA_BROADCAST",     0, XmALIGNMENT_BEGINNING},
    {"IFA_ANYCAST",       0, XmALIGNMENT_BEGINNING},
    {"IFA_CACHEINFO",     0, XmALIGNMENT_BEGINNING},
    {"IFA_MULTICAST",     0, XmALIGNMENT_BEGINNING},
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

    watcher = new IPAddressWatcher(this);
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
int main(int argc, char** argv)
{
  XInitThreads(); //2015/06/10

  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);
    Args args;
    args.set(XmNgeometry, "700x300");
    MainView view(applet, argv[0], args);
    
    view.realize();
    applet.run();
    
  } catch (Exception& ex) {
    caught(ex); 
  }
  return 0;
}

