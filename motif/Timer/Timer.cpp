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
 *  Timer.cpp
 *
 *****************************************************************************/

#include <oz++/TimerThread.h>
#include <oz++/LocalDateTime.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/RenderTable.h>

namespace OZ {

class MainView :public ApplicationView {
private:
   SmartPtr<TimerThread> timer;
   SmartPtr<RenderTable> rtable;
   SmartPtr<Label>       label;
   CharString            ctime;

public:
  virtual void updateModel(CommonObject* object)
  {
     CharString* string = (CharString*)object; 
     ctime = (*string);
     updateView();
  }

  virtual void updateView()
  {
     CompoundString cs((const char*)ctime);
     label -> set(XmNlabelString, cs);
     flush();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    ar.set(XmNfontName, "Times");
    //ar.set(XmNfontName, "Courier");
    ar.set(XmNfontSize, 24);
    rtable = new RenderTable(this, "", ar);

    ar.reset();

    CompoundString cs(" ");
    ar.set(XmNrenderTable, (XtArgVal)rtable->get());

    ar.set(XmNlabelType, XmSTRING);
    ar.set(XmNlabelString, cs);
    label = new Label(this, "label", ar);

    //Create an instance of TimerThread class.
    timer = new TimerThread(1); //Interval 1 second.
    //Set timeout callback. 
    timer -> setTimeout(this, (Timeout)&MainView::updateModel);
    //Start it.
    timer -> start();
  }

  ~MainView()
  {
    timer -> stop();
    timer -> wait();
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
    args.set(XmNgeometry, "300x60");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

