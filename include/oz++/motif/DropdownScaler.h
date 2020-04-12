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
 *      DropdownScaler.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/StrDef.h>
#include <oz++/motif/Primitive.h>
#include <oz++/motif/ComboBox.h>

namespace OZ {
  
class DropdownScaler :public DropdownComboBox {
public:
  DropdownScaler(View* parent, const char* name, Args& args)
  :DropdownComboBox(parent, name, args) 
  {
    // Consturctor
    static const char* items[] = {
     "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%","100%",
     "110%", "120%", "130%", "140%", "150%", "160%", "170%","180%", "190%", "200%"};

    CompoundStringList csl(items, CountOf(items));
    const char* defaultScale = (const char*)args.get(XmNdefaultScale);
    CompoundString selitem(defaultScale);
    Args ar;
    ar.set(XmNeditable, FALSE);
    ar.set(XmNitems, csl);
    ar.set(XmNvisibleItemCount, CountOf(items)/2);
    ar.set(XmNselectedItem, selitem);
    ar.set(XmNitemCount, CountOf(items));

    this->set(ar);
  }

  DropdownScaler()
  :DropdownComboBox()
  {
  }

  int getScale()
  {
    XmString xms;
    get(XmNselectedItem, (XtArgVal)&xms);
    CompoundString cs(xms);
    char* item = NULL;
    cs.get(&item);  
    printf("DropdownScaler %s\n", item);
    int value = atoi(item);
    XtFree(item);

    return value;
  }
};

}

