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
 *      LabeledFrame.h
 *
 *****************************************************************************/

//2017/10/15 Updated to use create method.

#pragma once

#include <oz++/motif/Frame.h>
#include <oz++/motif/LabelGadget.h>

namespace OZ {
  
class LabeledFrame :public Frame {
private:
  SmartPtr<LabelGadget> label;

public:
  LabeledFrame(View* parent, const char* name, Args& args)
  :Frame()
  {
    LabeledFrame::create(parent, name, args);
  }

  LabeledFrame()
  :Frame()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    Frame::create(parent, name, args.set(XmNshadowThickness, 0));
    CompoundString cname(name);
   
    Args ar;
    ar.set(XmNframeChildType, XmFRAME_TITLE_CHILD);
    ar.set(XmNchildHorizontalAlignment, XmALIGNMENT_BEGINNING);
    ar.set(XmNchildHorizontalSpacing, 0); 
   
    if (args.has(XmNlabelString)) {
     ar.set(XmNlabelString, args.get(XmNlabelString));
    } else {
     ar.set(XmNlabelString, cname);
    }
   label = new LabelGadget(this, "", ar);
  }
 
  LabelGadget* getLabel()
  {
    return label;
  }
 
  ~LabeledFrame()
  {
  }
};

}

