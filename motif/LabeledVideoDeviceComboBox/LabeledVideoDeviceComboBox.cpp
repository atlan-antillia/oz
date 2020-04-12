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
#include <oz++/motif/LabeledVideoDeviceComboBox.h>
#include <oz++/motif/CompoundStringList.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<RowColumn>         rowColumn;
  SmartPtr<LabeledVideoDeviceComboBox>   videoDeviceComboBox;

  void    selected(Action& action)
  {
    XmComboBoxCallbackStruct* cbs =
                        (XmComboBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs -> item_or_text);
    int position = videoDeviceComboBox->getSelectedPosition();
    char* item;
    cs.get(&item);
    printf("Selected position=%d item: %s\n", position, item);
    XtFree(item);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
     Args ar;
     ar.set(XmNorientation, XmVERTICAL);
     rowColumn = new RowColumn(this, "", ar);

     CompoundString cs("VideDevice");

     ar.reset();
     ar.set(XmNlabelString, cs);
     ar.set(XmNvisibleItemCount, 5);
     ar.set(XmNheight, 100);
     videoDeviceComboBox = new LabeledVideoDeviceComboBox(rowColumn, "list", ar);

     videoDeviceComboBox->addCallback(XmNselectionCallback, this, 
                  (Callback)&MainView::selected, NULL);


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

