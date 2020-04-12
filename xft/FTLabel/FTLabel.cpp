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
 *  FTLabel.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/xft/FTFont.h>
#include <oz++/xft/FTLabel.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<FTFont>  font20;
  SmartPtr<FTFont>  font30;
  SmartPtr<FTFont>  font18;
 
  SmartPtr<RowColumn>     rowColumn;
  SmartPtr<FTLabel> label;
  SmartPtr<FTLabel> label1;
  SmartPtr<FTLabel> label2;

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Args ar;
    ar.reset();
    ar.set(XftNfamily, "Times-12");
    ar.set(XftNsize, 20.0);
    font20 = new FTFont(getDisplay(), ar);

    ar.reset();
    ar.set(XftNfamily, "Courier");
    ar.set(XftNsize, 30.0);
    ar.set(XftNslant, 0);    
    font30 = new FTFont(getDisplay(), ar);

    ar.reset();
    ar.set(XftNfamily, "VL ゴシック");
    ar.set(XftNsize, 18.0);
    ar.set(XftNslant, 1);    
    font18 = new FTFont(getDisplay(), ar);

    ar.reset();
    ar.set(XmNnumColumns, 1);
    ar.set(XmNorientation, XmVERTICAL);
    rowColumn = new RowColumn(this, "", ar);

    ar.reset();
    ar.set(XmNwidth, 100);
    ar.set(XmNheight, 40);
    ar.set(XmNalignment, XmALIGNMENT_BEGINNING);
    ar.set(XftNforeground, "red");
    ar.set(XftNfont, (XtArgVal)(FTFont*)font20);
    ar.set(XftNlabelString, 
          "Jillian Aversa - 'Atlantis Awakening' ( アトランティスの覚醒)"); 	
    label = new FTLabel(rowColumn, "", ar);

    ar.reset();
    ar.set(XmNwidth, 100);
    ar.set(XmNheight, 40);
    ar.set(XmNalignment, XmALIGNMENT_CENTER);
    ar.set(XftNforeground, "blue");
    ar.set(XftNfont, (XtArgVal)(FTFont*)font30);
//    ar.set(XftNlabelString, "\"We Also Walk Dogs\" by Robert Anson Heinlein");
    ar.set(XftNlabelString, "Мария сильна как никогда!");

    label1 = new FTLabel(rowColumn, "", ar);	
  	
    ar.reset();
    ar.set(XmNwidth, 100);
    ar.set(XmNheight, 40);
    ar.set(XmNalignment, XmALIGNMENT_END);
    ar.set(XftNforeground, "darkgreen");
    ar.set(XftNfont, (XtArgVal)(FTFont*)font18);
    ar.set(XftNlabelString, 
	"Xft is a simple library designed to interface the FreeType rasterizer ");
    label2 = new FTLabel(rowColumn, "", ar);
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
    args.set(XmNgeometry, "760x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

