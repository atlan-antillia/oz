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
 *  ButtonBox.cpp
 *
 *****************************************************************************/

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/PanedWindow.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/ButtonBox.h>
#include <oz++/SmartArray.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<PanedWindow> panedw;

  SmartPtr<ButtonBox>  buttonBox;
  SmartArray<PushButton*> pushb;
  SmartPtr<ScrolledText> sctext;

  void open(Action& action)
  {
    printf("open\n");
  }

  void save(Action& action)
  {
    printf("save\n");
  }

  void clear(Action& action)
  {
    printf("clear\n");
    sctext -> getText() -> setString("");
  }

  void quit(Action& action)
  {
    printf("quit\n");
    confirm(action);
  }


public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
     static const char* items[] = {
               "Open", "Save", "Clear", "Quit"};

     static const Callback callbacks[] = {
        (Callback)&MainView::open,
        (Callback)&MainView::save,
        (Callback)&MainView::clear,
        (Callback)&MainView::quit,
     };

     Args ar;
     ar.set(XmNorientation, XmVERTICAL);
     panedw = new PanedWindow(this, "", ar);

     //ButtonBox doesn't support an alignment of elements contained in it. 
     ar.reset();
     ar.set(XmNorientation, XmHORIZONTAL);
     ar.set(XmNequalSize, TRUE);
     ar.set(XmNmarginHeight, 4);
     ar.set(XmNmarginWidth, 4);
     ar.set(XmNfillOption, XmFillNone); //XmFillAll|XmFillMajor);
     buttonBox = new ButtonBox(panedw, "buttonb", ar);

     for (int i = 0; i< XtNumber(items); i++) {
       CompoundString cs(items[i]);
       ar.reset();
       ar.set(XmNlabelString, cs);
       PushButton* button = new PushButton(buttonBox, "", ar);
       pushb[i] = button;
       button -> addCallback(XmNactivateCallback, this,
                callbacks[i], NULL);
    } 
    ar.reset();
    sctext = new ScrolledText(panedw, "", ar);
  }

  ~MainView()
  {
    pushb.clear(); //This line is must.
  }
};

}

//
int main(int argc, char** argv)
{
  const char*  appclass = argv[0];
  Application applet(appclass, argc, argv);

  Args args;
  args.set(XmNgeometry, "500x300");
  MainView view(applet, argv[0], args);
  view.realize();

  applet.run();
  return 0;
}

