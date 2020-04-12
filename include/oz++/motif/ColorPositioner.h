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
 *  ColorPositioner.h
 *
 *****************************************************************************/

//2017/10/16 Modified to use create method.

#pragma once

#include <oz++/motif/Positioner.h>

namespace OZ {


class ColorPositioner :public Positioner {

  const char** getNames()
  {
    static const char* names[] = {"R=", "G=", "B="};
    return (const char**)names;
  }

  const char** getColorNames()
  {
    static const char* names[] = {"Red", "Green", "Blue"};
    return (const char**)names;
  }


public:
  ColorPositioner(View* parent, const char* name, Args& args)
  :Positioner()
  {
    ColorPositioner::create(parent, name, args);
  }
  

  ColorPositioner()
  :Positioner()
  {
  }

  ~ColorPositioner() 
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Positioner::create(parent, name, args.set(XmNminimum, 0)
                                .set(XmNmaximum, 255)
                                .set(XmNlabelString,
                                  CompoundString("ColorPositioner") )
                                .set(XmNnames,
                                  (XtArgVal)getNames() )
                                .set(XmNcolorNames,
                                  (XtArgVal)getColorNames() ) );
  }

  void getBGRColor(float& fb, float& fg, float& fr)
  {
    int r, g, b;
    getPosition(r, g, b);
    fb = (float)b/255.0f;
    fg = (float)g/255.0f;
    fr = (float)r/255.0f;
  }

  void setBGRColor(float& fb, float& fg, float& fr)
  {
    int r, g, b;
    b = (int)b*255.0f;
    g = (int)g*255.0f;
    r = (int)r*255.0f;
    getPosition(r, g, b);
  }
};
}


