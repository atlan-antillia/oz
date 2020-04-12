/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  LabeledComboBox.cpp
 *
 *****************************************************************************/



#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/LabeledComboBox.h>
#include <oz++/motif/CompoundStringList.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<RowColumn>         rowColumn;
  SmartPtr<LabeledComboBox>   comboBox1;
  SmartPtr<LabeledComboBox>   comboBox2;

  void    selected(Action& action)
  {
    XmComboBoxCallbackStruct* cbs =
                        (XmComboBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs -> item_or_text);
    char* item;
    cs.get(&item);
    printf("item: %s\n", item);
    XtFree(item);
  }

  void    selected2(Action& action)
  {
    XmComboBoxCallbackStruct* cbs =
                        (XmComboBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs -> item_or_text);
    char* item;
    cs.get(&item);
    printf("item: %s\n", item);
    XtFree(item);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
     Args ar;
     ar.set(XmNorientation, XmVERTICAL);
     rowColumn = new RowColumn(this, "", ar);

     static const char* items[] = {"Good morning", "Good afternoon", 
             "Good evening", "Good night", "Hello", "Goodbye",
             "おはよう", "こんにちは", "こんばんは" };
     CompoundStringList csl(items, XtNumber(items));
     CompoundString cs("Greeting");

     ar.reset();

     ar.set(XmNlabelString, cs);
     ar.set(XmNitems, csl);
     ar.set(XmNvisibleItemCount, 3);
     ar.set(XmNitemCount, XtNumber(items));
     ar.set(XmNheight, 30);
     comboBox1 = new LabeledComboBox(rowColumn, "list", ar);

     comboBox1 -> addCallback(XmNselectionCallback, this,
                  (Callback)&MainView::selected, NULL);

     static const char* items2[] = {"January", "February",
             "March", "April", "May", "June",
		"July", "August", "September", "October",
                "November", "December"};
     CompoundStringList csl2(items2, XtNumber(items2));
     CompoundString cs2("Month");

     ar.reset();
     ar.set(XmNlabelString, cs2);
     ar.set(XmNitems, csl2);
     ar.set(XmNvisibleItemCount, 5);
     ar.set(XmNitemCount, XtNumber(items2));
     ar.set(XmNheight, 30);
     comboBox2 = new LabeledComboBox(rowColumn, "list", ar);

     comboBox2 -> addCallback(XmNselectionCallback, this,
                  (Callback)&MainView::selected2, NULL);

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
    args.set(XmNgeometry, "400x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

