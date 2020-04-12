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
 *  Outline.cpp
 *
 *****************************************************************************/

#include <oz++/Locale.h>
#include <oz++/DirectoryScanner.h>

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/Outline.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/Color.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<Outline> outline;

  void   pushed(Action& action)
  {
    View* sender = (View*)action.getSender();
    XmString xms;
    sender -> get(XmNlabelString, &xms);
    CompoundString cs(xms); 
    char* name = cs.unparse();
    printf("Pushed node=%s\n", name);
    XtFree(name);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Color color(this);
    Args ar;
    ar.set(XmNbackground, color.allocNamedColor("white")); 
    outline = new Outline(this, "", ar);
    
    ar.reset();
    ar.set(XmNbackground, color.allocNamedColor("pink")); 
    View* root = outline -> addLabel(NULL, "CommonObject", ar);

    ar.reset();
    ar.set(XmNbackground, color.allocNamedColor("lightgreen")); 
    View* primitive = outline -> addLabel(root, "Primitive", ar);

    ar.reset();
    ar.set(XmNbackground, color.allocNamedColor("lightblue")); 
    View* composite = outline -> addLabel(root, "Composite", ar);

    ar.reset();
    View* label     = outline -> addPushButton(primitive, "Label", ar);

    ar.reset();
    View* combob    = outline -> addPushButton(primitive, "ComboBox", ar);

    ar.reset();
    View* pushb     = outline -> addPushButton(label, "PushButton", ar, 
                           this, (Callback)&MainView::pushed);

    ar.reset();
    View* scrollb   = outline -> addPushButton(primitive, "ScrollBar", ar,
                           this, (Callback)&MainView::pushed);
    ar.reset();
    View* text      = outline -> addPushButton(primitive, "Text", ar,
                           this, (Callback)&MainView::pushed);

    ar.reset();
    View* textf      = outline -> addPushButton(primitive, "TextField", ar,
                           this, (Callback)&MainView::pushed);

    ar.reset();
    View* drawnb      = outline -> addPushButton(primitive, "DrawnButton", ar,
                           this, (Callback)&MainView::pushed);

    ar.reset();
    View* manager   = outline -> addLabel(composite, "Manager", ar);

    ar.reset();
    View* mainw     = outline -> addLabel(manager, "MainWindow", ar);

    ar.reset();
    View* rowColumn = outline -> addLabel(manager, "RowColumn", ar);

    ar.reset();
    View* scrolledw = outline -> addLabel(manager, "ScrolledWindow", ar);

    ar.reset();
    View* hierarchy = outline -> addLabel(manager, "Hierarchy", ar);

    ar.reset();
    View* outlinev =  outline -> addLabel(hierarchy, "Tree", ar);
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
    Locale locale;
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "550x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}
