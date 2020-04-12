/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      ListView.h
 *
 *****************************************************************************/

//2015/09/07 Updated

#pragma once

#include <oz++/SmartPtrs.h>
#include <oz++/Exception.h>
#include <oz++/motif/Primitive.h>
#include <Xm/Ext18List.h>

#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>
//#include <oz++/motif/MultiListRow.h>


/*
 Usage:
  const char* columns[] = {"id", "object", "score", "x", "y", "w", "h"};
  CompoundStringList colum_titles(columns, CountOf(columns));
  Args args;
  args.set(XmNfirstColumnPixmaps, True); 
  //args.set(XmNentryData, row_info); 
  args.set(XmNnumRows, 0); 
  args.set(XmNnumColumns,   CountOf(columns)); 
  args.set(XmNcolumnTitles, column_titles); 
  
  args.set(XmNtitle, CompoundString("Lineup")); 
  //args.set(XmNsortFunctions, sort_funcs);        
  args.set(XmNselectionPolicy, XmSINGLE_SELECT);   
  args.set(XmNshowSash, False);   
  
  listv = new ListView(parent, "listv", args);
  

 */

namespace OZ {

class ListView :public Primitive {

public:
  ListView(View* parent, const char* name,  Args& args)
  :Primitive(parent, name, xmExt18ListWidgetClass, args)
  {
  }

  ListView()
  :Primitive()
  {
  }

  void create(View* parent, const char* name,  Args& args)
  {
    Primitive::create(parent, name, xmExt18ListWidgetClass, args);
  }
};

}


