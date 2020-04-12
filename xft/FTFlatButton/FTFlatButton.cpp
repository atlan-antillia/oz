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
 *  FTFlatButton.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/xft/FTFlatButton.h>
#include <oz++/xft/FTFont.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<FTFont>       font22;
  SmartPtr<FTFont>       font28;
  SmartPtr<FTFont>       font32;

  SmartPtr<RowColumn>    rowColumn;
  SmartPtr<FTFlatButton> button;
  SmartPtr<FTFlatButton> button1;
  SmartPtr<FTFlatButton> button2;

  void press(Action& action)
  {
     printf("Button pressed \n");
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    ar.set(XmNnumColumns, 1);
    ar.set(XmNorientation, XmVERTICAL);
    rowColumn = new RowColumn(this, "", ar);

    ar.reset();
    ar.set(XftNfamily, "Times-12");
    ar.set(XftNsize, 22.0);
    //ar.set(XftNweight, FC_WEIGHT_BOLD );
    ar.set(XftNweight, FC_WEIGHT_THIN);
    font22 = new FTFont(getDisplay(),  ar);

    ar.reset();
    ar.set(XftNfamily, "Times");
    ar.set(XftNsize, 28.0);
    ar.set(XftNslant, 1);    
    ar.set(XftNweight, FC_WEIGHT_BOLD );
    font28 = new FTFont(getDisplay(),  ar);

    ar.reset();
    ar.set(XftNfamily, "VL ゴシック");
    ar.set(XftNsize, 34.0);
    ar.set(XftNweight, FC_WEIGHT_LIGHT);
    font32 = new FTFont(getDisplay(),  ar);

    ar.reset();
    ar.set(XmNwidth, 100);
    ar.set(XmNheight, 60);
    ar.set(XftNfont, (XtArgVal)(FTFont*)font22);
    ar.set(XftNforeground, "red");
    ar.set(XftNlabelString, "フリータイプの覚醒, The FreeType Awakens");
  	
    button = new FTFlatButton(rowColumn, "", ar);
    button -> addCallback(XmNactivateCallback, this,
             (Callback)&MainView::press, NULL);

    ar.reset();
    ar.set(XmNwidth, 100);
    ar.set(XmNheight, 60);
    ar.set(XftNfont, (XtArgVal)(FTFont*)font28);
    ar.set(XftNforeground, "blue");
    ar.set(XftNlabelString, "Is Motif a sleeping beauty?");
    button1 = new FTFlatButton(rowColumn, "", ar);

    ar.reset();
    ar.set(XmNwidth, 100);
    ar.set(XmNheight, 60);
    ar.set(XftNfont, (XtArgVal)(FTFont*)font32);
    ar.set(XftNforeground, "darkgreen");
    ar.set(XftNlabelString, "Vanity Fair 虚栄の市");
    button2 = new FTFlatButton(rowColumn, "", ar);
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
    args.set(XmNgeometry, "700x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

