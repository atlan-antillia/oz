/******************************************************************************
 *
 * Copyright (c) 2016-2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  Positioner.h
 *
 *****************************************************************************/

//2017/10/10 Updated.
//2017/11/27 Updated.

#pragma once

#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/ScrollBar.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/Color.h>

namespace OZ {

static const char* XmNnames = "names";
static const char* XmNcolorNames = "colorNames";
//static const char* XmNpositionerChanged = "positionerChanged";
static const char* XmNpositionChangedCallback = "positionChangedCallback";

class Positioner :public BulletinBoard {
private:
  static const int THREE = 3;

private:
  SmartPtr<Label>      title;

  SmartPtr<ScrollBar>  scrollbar[THREE];
  SmartPtr<Label>      label[THREE];

  View*          exposeEventTarget;

  virtual const char** getNames()
  {
    static const char* names[] = {"X=", "Y=", "Z="};
    return (const char**)names;
  }

public:
  void  valueChanged(Action& action)
  {
    setLabels();
    //display();
    if (exposeEventTarget) {
      exposeEventTarget->sendExposeEvent();
    }
  }

public:
  void  setLabels()
  {
     const char** names = getNames();
     
     for (int i = 0; i<THREE; i++) {
       int val = 0;
       scrollbar[i]->get(XmNvalue, (XtArgVal)&val);

       char name[128];
       sprintf(name, "%s%d", names[i], val);

       CompoundString cs(name);
       label[i]->set(XmNlabelString, cs);
     }
  }

public:
  static const int SCROLLBAR_WIDTH = 200;
  static const int SCROLLBAR_HEIGHT= 20;
  static const int SLIDER_SIZE     = 20;

  static const int POSITIONER_WIDTH = 260;
  static const int POSITIONER_HEIGHT = 100;

public:
  static int getPositionerWidth()
  {
    return POSITIONER_WIDTH;
  }

  static int getPositionerHeight()
  {
    return POSITIONER_HEIGHT;
  }

public:
  Positioner(View* parent, const char* name, Args& args)
  :BulletinBoard(), 
   exposeEventTarget(NULL)
  {
    Positioner::create(parent, name, args);
  }

  Positioner()
  :BulletinBoard(),
   exposeEventTarget(NULL)
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    BulletinBoard::create(parent, name, 
                               args.set(XmNmarginHeight, 2)
                                   .set(XmNmarginWidth, 2)
                                   .set(XmNwidth, POSITIONER_WIDTH) 
                                   .set(XmNheight, POSITIONER_HEIGHT) );
    Color color(this);
    Args ar;
    const char** names = getNames();
    if (args.get(XmNnames)) {
      names = (const char**)args.get(XmNnames);
    }

    int minimum = 0;
    if (args.has(XmNminimum)) {
      minimum = args.get(XmNminimum);
    }
    int maximum = 128;
    if (args.has(XmNmaximum)) {
      maximum = args.get(XmNmaximum);
    }

    const char** cnames = NULL;
    if (args.has(XmNcolorNames)) {
      cnames = (const char**)args.get(XmNcolorNames);
    }
    
    int y = 0;
    ar.set(XmNx, 2);
    ar.set(XmNy, 2);
    CompoundString cs("Positioner");
    if (args.has(XmNlabelString)) {
      ar.set(XmNlabelString, (XtArgVal)args.get(XmNlabelString));
    } else {
      ar.set(XmNlabelString, cs);
    }
    title = new Label(this, "", ar);
    int h = 0;
    title->get(XmNheight, h);
    y = h + 2; //24;

    for (int i = 0; i<3; i++) {
      ar.reset();
      ar.set(XmNminimum,  minimum);
      ar.set(XmNx, 0);
      ar.set(XmNy, y + (SCROLLBAR_HEIGHT+4)*i);
      ar.set(XmNwidth, SCROLLBAR_WIDTH);
      ar.set(XmNheight, SCROLLBAR_HEIGHT);
      ar.set(XmNorientation, XmHORIZONTAL);
      ar.set(XmNsliderSize,  SLIDER_SIZE);
      if (cnames) {
        ar.set(XmNtroughColor, color.colorPixel(cnames[i]));
      }
      scrollbar[i] = new ScrollBar(this, "", ar);

      int size = 0;
      scrollbar[i]->getSize(size);
      scrollbar[i]->set(XmNmaximum, maximum + size);
      scrollbar[i] -> addCallback(XmNdragCallback, this,
                       (Callback)&Positioner::valueChanged, NULL);
      scrollbar[i] -> addCallback(XmNvalueChangedCallback, this,
                       (Callback)&Positioner::valueChanged, NULL);
 
      CompoundString css(names[i]);
      ar.reset();
      ar.set(XmNx, SCROLLBAR_WIDTH+4);
      ar.set(XmNy, y -4 + (SCROLLBAR_HEIGHT+4)*i);
      ar.set(XmNheight, SCROLLBAR_HEIGHT);
      ar.set(XmNlabelString, css);
      label[i] = new Label(this, "", ar);
    }
  }
  
  ~Positioner() 
  {
  }

  void setExposeEventListener(View* view)
  {
    exposeEventTarget = view;
  }

  void getPosition(int& x, int& y, int& z)
  {
    int i = 0;
    scrollbar[i++]->get(XmNvalue, (XtArgVal)&x);
    scrollbar[i++]->get(XmNvalue, (XtArgVal)&y);
    scrollbar[i  ]->get(XmNvalue, (XtArgVal)&z);
  }

  void setPosition(int x, int y, int z)
  {
    int val[] = {x, y, z};
    for (int i = 0; i<THREE; i++) {
      scrollbar[i]->set(XmNvalue, (XtArgVal)val[i]);
    }
    setLabels();
  }

  void setSliderSize(int x, int y, int z)
  {
    int val[] = {x, y, z};
    int i = 0;
    for (int i = 0; i<THREE; i++) {
      scrollbar[i]->set(XmNsliderSize, (XtArgVal)val[i]);
    }
  }

  //2017/11/27 Modified
  //void addCallback(const char* name, Callback callback, 
  //              CommonObject* object, void *data)
  void addCallback(const char* name, CommonObject* object,
               Callback callback, void *data)
  {
    //2017/11/27
    //if (name == XmNpositionerChanged) {
    if (name == XmNpositionChangedCallback) {
      for (int i = 0; i<THREE; i++) {
        scrollbar[i] -> addCallback(XmNdragCallback, object,
                       callback, data);
        scrollbar[i] -> addCallback(XmNvalueChangedCallback, object,
                       callback, data);
      }
    }
   }

  void display()
  {
     int x, y, z;
     int i = 0;
     scrollbar[i++]->get(XmNvalue, (XtArgVal)&x);
     scrollbar[i++]->get(XmNvalue, (XtArgVal)&y);
     scrollbar[i  ]->get(XmNvalue, (XtArgVal)&z);
     printf("Position(%d, %d, %d)\n", x, y, z);
  }
};
}


