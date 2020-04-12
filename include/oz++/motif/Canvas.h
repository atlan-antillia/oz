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
 *      Canvas.h
 *
 *****************************************************************************/

//2017/10/10 Updated.

#pragma once
#include <oz++/CommonObject.h>
#include <oz++/motif/DrawnButton.h>

#include <oz++/motif/DC.h>
#include <oz++/motif/Color.h>

namespace OZ {

class Canvas :public DrawnButton {

public:
  virtual void circulateNotify(Event& event)
  {
    dprintf("Canvas::circulateNotify\n");
  }
  virtual void configureNofity(Event& event)
  {
    dprintf("Canvas::configureNotify\n");
  }

  virtual void destroyNofity(Event& event)
  {
    dprintf("Canvas::destroyNotify\n");
  }
	
  virtual void gravityNofity(Event& event)
  {
    dprintf("Canvas::gravityNotify\n");  	
  }

  virtual void mapNofity(Event& event)
  {
    dprintf("Canvas::mapNotify\n");
  }

  virtual void reparentNofity(Event& event)
  {
    dprintf("Canvas::reparentNotify\n");
  }

  virtual void unmapNofity(Event& event)
  {
    dprintf("Canvas::unmapNotify\n");
  }
	
  virtual void structureNotify(Event& event)
  {
    XEvent* xe = event.getXEvent();
    switch (xe -> type) {
    case CirculateNotify:
      circulateNotify(event);
      break;

    case ConfigureNotify:
      configureNofity(event);
      break;
 
    case DestroyNotify:
      destroyNofity(event);
      break;

   case GravityNotify:
      gravityNofity(event);
      break;

    case MapNotify:
      mapNofity(event); 
      break;
    	
    case ReparentNotify:
      reparentNofity(event); 
      break;

    case UnmapNotify:
      unmapNofity(event); 
      break;
    } 
  }

  virtual void buttonPress(Event& event)
  {
    dprintf("Canvas::buttonPress\n");
  }
	
  virtual void buttonRelease(Event& event)
  {
    dprintf("Canvas::buttonRelease\n");
  }
	
  virtual void buttonPressRelease(Event& event)
  {
    event.processTraversal();
  	
    XEvent* xe = event.getXEvent();
    switch (xe -> type) {
    case ButtonPress:
      buttonPress(event);
      break;

    case ButtonRelease:
      buttonRelease(event);
      break;
    }
  }

  virtual void keyPress(Event& event)
  {		
    dprintf("Canvas::keyPress\n");
  }
	
  virtual void keyRelease(Event& event)
  {
    dprintf("Canvas::keyRelease\n");
  }

  virtual void keyPressRelease(Event& event)
  {
    event.processTraversal();

    XEvent* xe = event.getXEvent();
    switch (xe -> type) {
    case KeyPress:
      keyPress(event);
      break;

    case KeyRelease:
      keyRelease(event);
      break;
    }
  }

  virtual void motionNotify(Event& event)
  {
    dprintf("Canvas::motionNotify\n");
  }
	
  virtual void pointerMotion(Event& event)
  {
    //event.processTraversal();

    XEvent* xe = event.getXEvent();
    if (xe -> type == MotionNotify) {
      motionNotify(event);
    }
  }

  virtual void  expose(Action& action)
  {
    dprintf("expose\n");
  }

public:
  Canvas(View* parent, const char* name, Args& args)
  :DrawnButton()
  {
    Canvas::create(parent, name, args);
  }
  
  Canvas()
  :DrawnButton()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    DrawnButton::create(parent, name, args.set(XmNshadowThickness, 0) );

    this -> addEventHandler(StructureNotifyMask, this,
          (Handler)&Canvas::structureNotify, NULL);

    this -> addEventHandler(KeyPressMask|KeyReleaseMask, this,
          (Handler)&Canvas::keyPressRelease, NULL);

    this -> addEventHandler(ButtonPressMask|ButtonReleaseMask, this,
          (Handler)&Canvas::buttonPressRelease, NULL);

    this -> addCallback(XmNexposeCallback, this,
                    (Callback)&Canvas::expose, NULL);
 
  }
  ~Canvas()
  {
  }

  
};

}

