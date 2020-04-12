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
 *  Column.cpp
 *
 *****************************************************************************/

#include <oz++/Locale.h>

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Column.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/TextField.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  static const int     COUNT=5;
  SmartPtr<Column>     column;
  SmartPtr<Label>      label[COUNT];
  SmartPtr<PushButton> pushb[COUNT];
  SmartPtr<TextField>  textField[COUNT];

  void  createLabel(CompoundStringList& list)
  {
    Args ar;

    for(int i = 0; i<list.size(); i++) {
      ar.reset();
      ar.set(XmNentryLabelString, list.get(i));
      ar.set(XmNlabelString, list.get(i));

      label[i] = new Label(column, "", ar);
    }
  }

  void  createButton(CompoundStringList& list)
  {
    Args ar;
    
    for(int i = 0; i<list.size(); i++) {
      ar.reset();
      ar.set(XmNlabelString, list.get(i));
      pushb[i] = new PushButton(column, "", ar);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    const char* items[] = {"Name", "Mail", "Tel", "Age", "Country", };

    CompoundStringList csl(items, COUNT);
    Args ar;
    //ar.set(XmNorientation, XmHORIZONTAL);
    ar.set(XmNorientation, XmVERTICAL);

    column = new Column(this, "column", ar);
 
    for(int i = 0; i<csl.size(); i++) {
       Args ar;
       ar.reset();
       ar.set(XmNentryLabelString, csl.get(i));
       textField[i]=  new TextField(column, "", ar);
    }
  }

  ~MainView()
  {
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
    args.set(XmNgeometry, "600x400");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

