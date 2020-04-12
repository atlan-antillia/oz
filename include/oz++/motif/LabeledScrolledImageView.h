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
 *      LabeledScrolledImageView.h
 *
 *****************************************************************************/

//2017/10/15 Modified to use create method.

#pragma once

#include <oz++/motif/LabeledFrame.h>
#include <oz++/motif/ScrolledImageView.h>

namespace OZ {
  
class LabeledScrolledImageView :public LabeledFrame {
private:
  SmartPtr<ScrolledImageView> imageView;
 
  virtual void mapNotify(Event& event)
  {
    //Call virtual mapped method.
    mapped();
  }

  virtual void mapped()
  {
  }

  virtual void structureNotify(Event& event)
  {
    XEvent* xev = event.getXEvent();
    if (xev->type == MapNotify) {
      mapNotify(event);
    }
  }

public:
  LabeledScrolledImageView(View* parent, const char* name, Args& args)
  :LabeledFrame()
  {
    LabeledScrolledImageView::create(parent, name, args); 
  }

  LabeledScrolledImageView()
  :LabeledFrame()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    LabeledFrame::create(parent, name, args); 
   
    Args ar;
    imageView = new ScrolledImageView(this, "", ar);
    addEventHandler(StructureNotifyMask, this,
       (Handler)&LabeledScrolledImageView::structureNotify, NULL);

  }
	
  ScrolledImageView* getImageView()
  {
     return imageView;
  }
	
  ~LabeledScrolledImageView()
  {
  }
/*
  void addCallback(const char* name, CommonObject* object,
	Callback callback, void* data)
  {
     imageView->addCallback(name, object, callback, data);
  }
*/
};

}

