/******************************************************************************
 *
 * Copyright (c) 2015-2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      LabelCell.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/Label.h>

namespace OZ {

class LabelCell :public Label {

public:
  LabelCell(View* parent, const char* name, Args& args)
  :Label()
  {
    LabelCell::create(parent, name, args);
  }

  LabelCell()
  :Label()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Label::create(parent, name,
                args.set(XmNborderWidth, 1)
                    .set(XmNborder,parent->grayPixel() )
                    .set(XmNhighlightThickness, 1) 
                    .set(XmNshadowThickness, 0)
                     //.set(XmNtraversalOn, False)
                    .set(XmNhighlightOnEnter, FALSE)
                    .set(XmNrecomputeSize, FALSE)
                    .set(XmNbackground, parent->whitePixel()) );
  }  
};

}

