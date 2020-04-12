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
 *  Clipboard.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Clipboard.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/PushButton.h>

namespace OZ {

char* XtDate = (char*)"Date";

class MainView :public ApplicationView {
private:
  SmartPtr<RowColumn>      rowcol;
  SmartPtr<PushButton>     copyb;
  SmartPtr<PushButton>     retrieveb;
  SmartPtr<Clipboard>      clipb;

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args) 
  {
     Args ar;
     rowcol = new RowColumn(this, "rowcol", ar);

     ar.reset();
     copyb = new PushButton(rowcol, "copy", ar);
     copyb -> addCallback(XmNactivateCallback, this,
                          (Callback)&MainView::copy, NULL);
     ar.reset();
     retrieveb = new PushButton(rowcol, "retrieve", ar);
     retrieveb -> addCallback(XmNactivateCallback, this,
                          (Callback)&MainView::retrieve, NULL);
     clipb = NULL;
  }


  ~MainView() 
  {
  }

  void copy(Action& action) 
  {
    long id;
    long pid = 1;
    long did;
    if(clipb == NULL) {
      clipb = new Clipboard(this);
    }

    FILE* fp = popen("/bin/date", "r");
    if(fp == NULL)  {
      exit(action);
    }
    char line[256];
    fgets(line, 256, fp);
    pclose(fp);

    clipb -> startCopy(NULL, 0, copyb, NULL, &id);
    clipb -> registerFormat(XtDate, 8);
    clipb -> copy(id, XtDate, line, strlen(line), pid, &did);
    clipb -> endCopy(id);
  }


  void retrieve(Action& action) 
  {
    if(clipb == NULL) {
      clipb = new Clipboard(this);
    }

    char buff[100];
    unsigned long bytes;
    long pid;

    clipb -> retrieve(XtDate, buff, 100, &bytes, &pid);
    buff[bytes] = ZERO;
    printf("retrieve: %s\n", buff);
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
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

