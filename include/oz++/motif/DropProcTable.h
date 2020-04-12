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
 *      DropProcTable.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/motif/Action.h>
#include <oz++/HashTable.h>

namespace OZ {
  
class DropProcTable :public CommonObject {
private:
  static HashTable* getHashTable()
  {
	static HashTable hashTable(513);
	return &hashTable;
  }
  
public:
  static Action* get(Widget widget)
  {
    static Action* action1 = NULL;
    static Widget  widget1 = NULL;

    if(widget == widget1 && action1 != NULL)
      return action1;

    widget1 = widget;
    Action* action = NULL;
    HashTable* hashTable = getHashTable();
    if(hashTable) {
      action1 = (Action*)hashTable->lookup((Key)widget);
    }
    return action1;
  }

  
  //static Action* get(Widget widget, int id);
  
  static void    put(Widget widget, Action* action)
  {
	HashTable* hashTable = getHashTable();
    if(hashTable) {
      hashTable -> add((Key)widget, action, 0);
	}
  }
  
  //static void    put(Widget widget, Action* action, int id);
};

}

