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
 *  LabeledColorListView.cpp
 *
 *****************************************************************************/



#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/Form.h>
#include <oz++/motif/LabeledColorListView.h>
#include <oz++/motif/CompoundStringList.h>

namespace OZ {
typedef unsigned char BYTE;
typedef unsigned short WORD;

#define GetBValue(rgb) ((BYTE)(rgb))
#define GetGValue(rgb) ((BYTE)(((WORD)(rgb)) >> 8))
#define GetRValue(rgb) ((BYTE)((rgb)>>16))

class MainView :public ApplicationView {
private:
  SmartPtr<BulletinBoard>          bboard;
  SmartPtr<Form>                   form;
  SmartPtr<LabeledColorListView>   colorListView;

  void    selected(Event& event)
  {
    Pixel pixel = (Pixel)event.getClientData();
    printf("selected pixel=0x%lx\n", pixel);
    unsigned char r = GetRValue(pixel);
    unsigned char g = GetGValue(pixel);
    unsigned char b = GetBValue(pixel);

    printf("rgb(%x, %x, %x)\n", r, g, b);
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
     Args ar;
     ar.set(XmNorientation, XmVERTICAL);
     bboard = new BulletinBoard(this, "", ar);
     ar.reset();
     form = new Form(bboard, "", ar);

     ar.reset();
    ar.set(XmNtopAttachment, XmATTACH_FORM);
    ar.set(XmNleftAttachment, XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNhighlightThickness, 2); //2015/02/08
 
     colorListView = new LabeledColorListView(form, "list", ar);

     colorListView -> addEventHandler(LabeledColorListView::EVENT_MASK, this,
                  (Handler)&MainView::selected, NULL);

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

