/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions, and the following disclaimer.
 * 
 * 2. The name of the author may not be used to endorse or promote products
 *  derived from this software without specific prior written permission.
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
 * TabList.h
 *
 *****************************************************************************/

#pragma once
#include <oz++/CommonObject.h>
#include <Xm/Tab.h
#include <Xm/TabList.h

namespace OZ {
	
class TabList :public CommonObject {
private:
  XmTabList tabList;
  
public:
  TabList()
  :tabList(NULL)
  {
  
  }
  
public:
  ~TabList()
  {
    if (tabList) {
      XmTabListFree (tabList) ;
    }
    tabList = NULL;
    
  }
public:
  void insert(Tab& tab, int pos = 0)
  {
    XmTabList oldList = tabList;
    tabList = XmTabListInsertTabs (oldbList, tabs, number, pos) ;
    XmTabListFree (oldList) ;
  }
  
public:
  void insert(XmTab* xmtabs, int number, int pos = 0)
  {
    XmTabList oldList = tabList;
    tabList = XmTabListInsertTabs (tabList, tabs, number, pos) ;
    XmTabListFree (oldList) ;
  }
   
public:
  XmTabList get()
  {
    return tabList;	
  }
};
	
}
