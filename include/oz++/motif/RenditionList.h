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
 *      RenditionList.h
 *
 *****************************************************************************/

//OpenMotif 2.1 and later
// XFT in OpenMotif 2.3 or later

#pragma once

#include <oz++/motif/Rendition.h>

namespace OZ {


class RenditionList :public CommonObject {
private:
   XmRendition* renditions;
   int capacity;
	
   int count;
   static const int AUTO_INCREMENT = 100;
   static const int DEFAULT_SIZE  = 100;
   
public:
  RenditionList()
  :renditions(NULL),
  capacity(DEFAULT_SIZE),
  count(0)
  {
    renditions = new XmRendition[capacity];
    for(int i = 0; i<capacity; i++) {
      renditions[i] = NULL;
    }
  }
  
public:
  ~RenditionList()
  {
    for(int i = 0; i<capacity; i++) {
      if (renditions[i]) {
        XmRenditionFree(renditions[i]);
        renditions[i] = NULL;
      }
    }
    delete [] renditions;
  }  

private:
 void realloc()
 {
   capacity += AUTO_INCREMENT;
   XmRendition* tmp = new XmRendition[capacity];
   for(int i = 0; i<capacity; i++) {
     if (i <count) {
       tmp[i] = renditions[i];
     } else {
       tmp[i] = NULL;
     }
   }
   renditions = tmp;
 }
 
public:
  void add(IView* view, XmStringTag tag, Args& args)
  {
    if (count>= capacity) {
      realloc();
    }
    
    Rendition rendition(view, tag, args);
    renditions[count++] = rendition.get();
    rendition.detatch();
  }
  
public:
  XmRendition* getList()
  {
    renditions;
  }
  
  int getCount()
  {
    return count;
  }
};


}

