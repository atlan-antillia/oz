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
 *      Rendition.h
 *
 *****************************************************************************/

//OpenMotif 2.1 and later
// XFT in OpenMotif 2.3 or later

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/CharString.h>
#include <oz++/Exception.h>
#include <oz++/motif/Args.h>

#include <oz++/motif/IView.h>

#include <Xm/Xm.h>

namespace OZ {

class Rendition :public CommonObject {
public:
  Widget     widget;
  //CharString fontListTag;
 
  XmRendition     rendition;
 
public:
  Rendition(IView* view, XmStringTag tag, Args& args)
  :widget(NULL),
  rendition(NULL)
  {
   
   if (view == NULL) {
    throw IException("Invalid parameter");
   }
   widget = view ->getWidget();
   
   //Setting default values.
   if (tag == NULL) {
     tag = XmFONTLIST_DEFAULT_TAG;
   }
   else if (strcmp((const char*)tag, "") == 0) {
     tag = XmFONTLIST_DEFAULT_TAG;  
   }
   if (args.has(XmNfontType) == False) {
    args.set(XmNfontType, XmFONT_IS_XFT);                 
    }
/*   
   if (args.has(XmNfontType) == False) {
     args.set(XmNfontType,  XmFONT_IS_XFT);
   }
 */  
   if (args.has(XmNfontName) == False) {
     args.set(XmNfontName, "Times-12");     //Default font name
   }
 
   if (args.has(XmNfontStyle) == False) {
     args.set(XmNfontStyle, "regular"); 
     //; //"regular", "bold", "italic" Default font style, bold
   }

   if (args.has(XmNfontSize) == False) {
     args.set(XmNfontSize, 12);   //Default size
   }
   
   if (args.has(XmNloadModel) == False) {
     args.set(XmNloadModel, XmLOAD_IMMEDIATE);//XmLOAD_DEFERRED); 
   }
   
   int count = args.count();
   ArgList argList = args.getArgList();
   
    rendition = XmRenditionCreate(widget, tag, argList, count);
   if (rendition == NULL) {
    throw IException("Failed to create XmRenditionCreate"); 
   }
  }
 
  ~Rendition()
  {
   if (rendition) {
     XmRenditionFree(rendition);
     rendition = NULL;
   }
  }
 
  XmRendition get()
  {
    return rendition;
  }
 
  //2015/02/21
  void detatch()
  {
    rendition = NULL;
  }
 
  void get(Args& args)
  {
    int count = args.count();
    ArgList argList = args.getArgList();
    XmRenditionRetrieve (rendition,
                         argList,
                         count);
  }
 
  void update(Args& args)
  {
    int count = args.count();
    ArgList argList = args.getArgList();
    XmRenditionUpdate (rendition,
                         argList,
                         count);
  }
};

}

