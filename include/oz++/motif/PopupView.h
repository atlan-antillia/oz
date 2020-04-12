/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      PopupView.h
 *
 *****************************************************************************/

//2015/02/11 Updated
//2017/05/29 Updated popup methods to be virtual.

#pragma once

#include <oz++/motif/VendorView.h>

namespace OZ {
  
class PopupView :public VendorView {
private:
  
  void hide(Action& action) 
  {
    unmanage(); 
  }
  
public:
  //2015/02/10 Modified
  PopupView(View* parent, const char* name,
     const  WidgetClass widgetClass, Args& args)
  :VendorView()
  {
    PopupView::create(parent, name, widgetClass, args);
  }
		
  PopupView()
  :VendorView()
  {
  }

  virtual void create(View* parent, const char* name,
     const  WidgetClass widgetClass, Args& args)
  {
    VendorView::create(parent, name, widgetClass,
                args.set(XmNdeleteResponse, XmDO_NOTHING)
                //    .set(XmNmappedWhenManaged, False)
                 );
  
    Atom protocol = XInternAtom (parent->getDisplay(),
                                "WM_DELETE_WINDOW", FALSE);

    addWMProtocolCallback(protocol, this, (Callback)&PopupView::hide, NULL);
  }

  Boolean isDialog() 
  {
    return TRUE;
  }

  virtual void  popupSpringLoaded() 
  {
    manage();
    XtPopupSpringLoaded(getWidget());
  }
  
  virtual void  popupSpringLoaded(int x, int y) 
  {
    manage();
    move(x, y);
    XtPopupSpringLoaded(getWidget());
  }

  virtual void  popup(XtGrabKind kind=XtGrabNonexclusive) 
  {
    manage();
    XtPopup(getWidget(), kind);
  }
	
  //2015/02/11
  virtual void  popupAt(XtGrabKind kind=XtGrabNonexclusive) 
  {
    int root_x, root_y;
    queryMousePointer(root_x, root_y);
    move(root_x, root_y);
    manage();
    XtPopup(getWidget(), kind);
  }


 //2017/06/15
  virtual void  popupAt(int x, int y, XtGrabKind kind=XtGrabNonexclusive) 
  {
    move(x, y);
    manage();
    XtPopup(getWidget(), kind);
  }

  void modalPopup()
  {
    popup(XtGrabNonexclusive);
  }		

  void modelessPopup()
  {
    popup(XtGrabNone );
  }
	
  void  popdown() 
  {
    XtPopdown(getWidget());
  }
};

}

