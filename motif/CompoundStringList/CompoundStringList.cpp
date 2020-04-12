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
 * WHETHER IN CONTRACT, STRICT , OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *  CompoundStringList.cpp
 *
 *****************************************************************************/


#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>
#include <oz++/motif/RenderTable.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/DC.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<RenderTable>  renderTable;
  CompoundStringList csl;

private:
  void    draw(Event& action) 
  {
    DC      dc(this);
    
    Color   color(this);
    dc.setRenderTable(*renderTable);
    //The following for loop will take a few seconds.
    int size = csl.size();
    for(int i = 0; i<size; i++) {
      Pixel p =   0xffff - i*0xffff/size <<24
                | 0xffff - i*0xffff/size<<16;
                //| 0xffff - i*0xffff/size<<8;
      //In case of renderTable. the follwing setForeground line will cause the next error.
      //X Error of failed request:  BadValue (integer parameter out of range for operation)
      //Major opcode of failed request:  91 (X_QueryColors)
      //dc.setForeground((Pixel)p);
      XmString xms = csl.get(i);
      CompoundString cs(xms);
      cs.draw(10, 10 + 20*i, &dc);
    }
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args) 
  {
    Args ar;
    ar.set(XmNfontName, "Times-a");
    ar.set(XmNfontSize, 16);
    renderTable = new RenderTable(this, "", ar);

    for (int i = 0; i<2048; i++) {
      char string[256];
      sprintf(string, "%d Motif Awakens. Motifの覚醒", i);
      csl.add(string);

    }
    addEventHandler(ExposureMask, this, 
        (Handler)&MainView::draw, NULL);
  }

public:
  ~MainView()
  {
  }
};
}

//
int main(int argc, char** argv) {
  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "500x500");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

