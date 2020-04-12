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
 *  NonblockingFileAccessWatcher.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/RenderTable.h>
#include <oz++/FileAccessWatcher.h>
#include <oz++/Logger.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  //Inner class starts.
  class MotifFileAccessEventMetaData :public FileAccessEventMetaData {
  private:
    static const int MAX_LISTITEM = 1000;
    ListBox* listBox; //Shallow copy

  public:
    MotifFileAccessEventMetaData(ListBox* list, int fd)
    :FileAccessEventMetaData(fd),
    listBox(list)
    {
    }

    virtual void display(const char* item)
    {
      int count = listBox -> getItemCount();
      if (count > MAX_LISTITEM) {
        listBox -> deletePos(count);
      }
      CompoundString cs(item);
      listBox -> addFirstUnselected(cs);
      listBox -> flush();
    }
  };

  class MotifFileAccessWatcher :public FileAccessWatcher {
  private:
    ListBox* listBox; //Shallow copy

  public:
    MotifFileAccessWatcher(const char* mount, ListBox* list)
    :FileAccessWatcher(mount),
     listBox(list)
    {
    }

    virtual void run()
    {
      int notifierFD = getNotifierFD();

      const nfds_t NFDS = 1;
      struct pollfd fds[NFDS];
      fds[0].fd     = notifierFD; 
      fds[0].events = POLLIN;

      while (looping) {
        sleepMilliSec(sleepmsec);
        int poll_num = poll(fds, NFDS, -1);
        if (poll_num == -1) {
          perror("poll");
          break;
        }

        if (poll_num > 0 && fds[0].revents & POLLIN) {
          try {
            MotifFileAccessEventMetaData metaData(listBox, notifierFD);
            metaData.parse(index);
            //index++;
          } catch (Exception& ex) {
            caught(ex);
          }
        }
      } //while
    }
  };
  //Inner class ends.

  SmartPtr<RenderTable>       rtable;
  SmartPtr<ScrolledListBox>   sclist;
  SmartPtr<MotifFileAccessWatcher> accessWatcher;

public:
  MainView(Application& applet, const char* name, Args& args, 
                const char* mount)
  :ApplicationView(applet, name, args)
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

    printf("Watch mount %s\n", mount);
    accessWatcher = new MotifFileAccessWatcher(mount, listBox);
    accessWatcher -> setSleepMilliSec(500);
    accessWatcher -> start();
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
  const char* mount = "/";

  if (argc == 2) {
    mount = argv[1];
  }
  else if (argc > 2) {
    printf("Usage: %s mout\n", argv[0]);
    return 1;
  }

  XInitThreads();
  try {

    const char*  appclass = "FileAccessWatcher";
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "600x400");

    MainView view(applet, argv[0], args, mount);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

