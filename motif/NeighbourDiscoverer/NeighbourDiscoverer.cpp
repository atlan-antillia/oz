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
 *  NeighbourDiscoverer.cpp
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

#include <oz++/netlink/NeighbourDiscoveryAttribute.h>
#include <oz++/netlink/NetlinkMsgHeader.h>
#include <oz++/netlink/NeighbourDiscoveryMsg.h>
#include <oz++/netlink/NetlinkRequest.h>

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/TableView.h>


namespace OZ {

class NeighbourDiscoverer :public Thread {
private:
  View*                 view; //Shallow copy
  NetlinkRequest<ndmsg> request;
  SocketNetlink socketNetlink;

public:
  NeighbourDiscoverer(View* v, unsigned char family = AF_INET)
  :Thread(),
  view(v)
 {
    NetlinkMsgHeader header(&request.head);
    header.type(RTM_GETNEIGH);
    header.flags(NLM_F_REQUEST | NLM_F_DUMP);
    header.seq(100);
    header.pid(getpid()); 

    NeighbourDiscoveryMsg body(&request.body);
    body.family(family); 

    header.length( NLMSG_LENGTH(body.size()) );
  }
    
  int send()
  {
    return socketNetlink.send((const char*)&request, sizeof(request), 0);   
  }
    
  void recv()
  {
    int index = 0;
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
          replyHeader.display();
          NeighbourDiscoveryMsg discoveryMsg(replyHeader.data());

          NeighbourDiscoveryAttribute attr(replyHeader, discoveryMsg);
          do {
            attr.parse();
          } while (attr.next() );

          LinkedListT<Property> list;
          discoveryMsg.getPropertyList(list);

          //Call updateMmodel
          view -> updateModel(&list);
 
          index++;
        } while (replyHeader.next());

      } catch (Exception& ex) {
        caught(ex);   
      }
      fflush(stdout);  
    } //while
  }

  void run() 
  {
    printf("NeighbourDiscoverer::run start\n");
    try {
      send();
      recv();
    } catch (Exception& ex) {
      caught(ex);
    }
    //Delete this pointer
    delete this;
    printf("NeighbourDiscoverer::run end\n");
  }
};
    

class MainView :public ApplicationView {
private:
  SmartPtr<TableView>           table;
  int                           column;
  SmartArray<CharString*>       model;
  NeighbourDiscoverer*         discoverer;
  
private:
  virtual void updateModel(CommonObject* object)
  {
    if (object) {
      //Update model of one row data.
      LinkedListT<Property>* list = (LinkedListT<Property>*)object;
      model.removeAllItems();
      size_t size = list->getLength();
      for (size_t i = 0; i < size; i++) {
        Property* prop = list->getNth(i);
        model[i] = new CharString(prop -> value());
      }
      //Call updateView method
      updateView();
    }
  }

  virtual void updateView()
  {
    //Update one row in the table.
    int size = model.getSize();
    for (int i = 0; i<size; i++) {
      CharString* value = model[i];
      if (value) {
        printf("%d %s\n", i, (const char*)(*value));
        table -> setItem(i, column, (const char*)(*value));
      }
    }
    // Call flush to update items of the table immediately. 
    flush();

    column++;
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args),
  column(0)
  {
    ColumnData columnData[]  = {
      {"A", 180, XmALIGNMENT_BEGINNING}, 
      {"B", 180, XmALIGNMENT_BEGINNING}, 
      {"C", 180, XmALIGNMENT_BEGINNING}, 
      {"D", 180, XmALIGNMENT_BEGINNING}, 
      {"E", 180, XmALIGNMENT_BEGINNING}, 
      {"F", 180, XmALIGNMENT_BEGINNING}, 
      {"G", 180, XmALIGNMENT_BEGINNING}, 
    };

    RowData rowData[]  = {
      {"NDMSG family",  140, XmALIGNMENT_BEGINNING},
      {"NDMSG flags",     0, XmALIGNMENT_BEGINNING},
      {"NDMSG type",      0, XmALIGNMENT_BEGINNING},
      {"NDMSG state",     0, XmALIGNMENT_BEGINNING},
      {"NDMSG ifindex",   0, XmALIGNMENT_BEGINNING},

      {"NDA_DST",         0, XmALIGNMENT_BEGINNING},
      {"NDA_LLADDR",      0, XmALIGNMENT_BEGINNING},
      {"NDA_CACHEINFO",   0, XmALIGNMENT_BEGINNING},
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

    discoverer = new NeighbourDiscoverer(this);
    discoverer -> start();
  }
  
  ~MainView() 
  {
    discoverer = NULL;
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
    args.set(XmNgeometry, "860x240");
    MainView view(applet, argv[0], args);
    
    view.realize();
    applet.run();
    
  } catch (Exception& ex) {
    caught(ex); 
  }
  return 0;
}

