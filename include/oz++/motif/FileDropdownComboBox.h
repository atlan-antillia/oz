/******************************************************************************
 *
 * Copyright (c) 2017   TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  FileDropdownComboBox.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/motif/DropdownComboBox.h>
#include <oz++/Directory.h>

namespace OZ {

class FileDropdownComboBox : public DropdownComboBox {
public:
  FileDropdownComboBox(View* parent, const char* name, Args& args)
  :DropdownComboBox(parent, name, args.set(XmNvisibleItemCount, 10)
                                      .set(XmNselectedPosition, 1))
  {
  }

  FileDropdownComboBox()
  :DropdownComboBox()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    DropdownComboBox::create(parent, name, 
                                  args.set(XmNvisibleItemCount, 10)
                                      .set(XmNselectedPosition, 1));
  }

  void listupFiles(const char* dir, const char* pattern)
  {
    clear();
    Directory directory(dir);
    LinkedListT<CharString> list;

    directory.getFileList(CharString(pattern), list);
    list.sortDescending();
    ListEntryT<CharString>* e = list.getEntry();
    CharString firstItem;
    int count = 0;
    while(e) {
      CharString* string = (CharString*)e->getObject();
      const char* item = (const char*)(*string);
      printf("%d %s\n", count, item);
      if (count == 0) {
        firstItem = item;
      }
      add(count+1, item);
      count++;
      e = e->getNext();
    } 
    if (count>0) {
      select((const char*)firstItem);
    }
  }
};
}

