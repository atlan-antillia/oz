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
 *      LabelGadgetCell.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/LabelGadget.h>

namespace OZ {

class LabelGadgetCell :public LabelGadget {

public:
  LabelGadgetCell(View* parent, const char* name, Args& args)
  :LabelGadget()
  {
    LabelGadgetCell:create(parent, name, args);
  }

  LabelGadgetCell()
  :LabelGadget()
  {
  }

  LabelGadgetCell(View* parent, const char* name, Args& args)
  {
    LabelGadget::create(parent, name, 
                      args.set(XmNborderWidth, 1)
                          .set(XmNborder, parent->grayPixel() )
                          .set(XmNhighlightThickness, 1)  
                          .set(XmNshadowThickness, 0) 
                          .set(XmNtraversalOn, False)  
                          .set(XmNhighlightOnEnter, False)
                          .set(XmNrecomputeSize, False)
                          .set(XmNbackground, parent->whitePixel()) ); 
  }
};

}

