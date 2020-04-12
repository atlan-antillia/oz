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
 *  InodeWatcher.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/RenderTable.h>
#include <oz++/NonblockingInodeWatcher.h>
#include <oz++/Logger.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  //Inner class start
  class MotifInodeWatcher :public NonblockingInodeWatcher {
  private:
    static const int MAX_LISTITEM = 1000;

    ListBox* listBox; //Shallow copy.

  public:
    //Create a nonblocking InodeWatcher.
    MotifInodeWatcher(ListBox* list)
    :NonblockingInodeWatcher(),
     listBox(list)
    {
    }

    virtual void display(struct inotify_event* event)
    {
      int count = listBox -> getItemCount();
      if (count > MAX_LISTITEM) {
        listBox -> deletePos(count);
        //printf("ListBox::deletePos %d\n", count);
      }

      const char* name = "";
      if  (event->len >0) {
         name = event->name;
      }
      LocalDateTime ldt;
      CharString now = ldt.nowToSeconds();

      const char* path = "";
      CharString* folder = lookupFolderName(event->wd);
      if (folder) {
        path = (const char*)(*folder);
      }
      const char* mask = maskToString(event->mask);
      char item[PATH_MAX];
      sprintf(item, "%s %s %s/%s", (const char*)now, mask, path, name);
      CompoundString cs(item);
      listBox -> addFirstUnselected(cs);
      listBox -> flush();
    }
  };
  //Inner class end

  SmartPtr<RenderTable>       rtable;
  SmartPtr<ScrolledListBox>   sclist;
  SmartPtr<MotifInodeWatcher> inodeWatcher;

public:
  MainView(Application& applet, const char* name, Args& args, 
                int watchCount, char** watchPath)
  :ApplicationView(applet, name, args),
   inodeWatcher(NULL)
  {
    Args ar;
    ar.set(XmNscrollBarDisplayPolicy, XmVARIABLE);
    ar.set(XmNlistSizePolicy,         XmCONSTANT);
    ar.set(XmNvisibleItemCount,  30);
    sclist = new ScrolledListBox(this, "list", ar);
    ListBox* listBox = sclist -> getList();

    // Create an instance of RenderTable based on a Rendition object.
    ar.reset();
    ar.set(XmNfontSize, 12);
    ar.set(XmNfontName, "Courier");
    rtable = new RenderTable(listBox, "", ar);

    // Set the renderTable to the ListBox instance.
    listBox -> set(XmNrenderTable, rtable);

    inodeWatcher = new MotifInodeWatcher(listBox);
    inodeWatcher -> setSleepMilliSec(500);
    for (int i = 0; i< watchCount; i++) {
      try {
        printf("Adding a watchpath %s\n", watchPath[i]);
        inodeWatcher -> addWatch(watchPath[i]);
      } catch (Exception& ex) {
        caught(ex);
      }
    }
    inodeWatcher -> start();
  }

  ~MainView()
  {
  }
};

}

// You can specify pathnames to watch on the command line, as shown below:
// InodeWatcher /etc /tmp

int main(int argc, char** argv)
{
  sigignore(SIGPIPE);

  if (argc == 1) {
    printf("Usage: %s watchpath1 watchpath2,...\n", argv[0]);
    return 1;
  }

  XInitThreads();
  try {

    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "600x400");

    MainView view(applet, argv[0], args, argc-1, &argv[1]);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

