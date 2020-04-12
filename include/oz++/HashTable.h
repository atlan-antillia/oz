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
 *  HashTable.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/CommonObject.h>
#include <oz++/HashEntry.h>

namespace OZ {

class HashTable  :public CommonObject {
private:
  int   size;
  HashEntry** table;
  bool    gc;//2015/02/15
	
private:
  Key   hash(const Key key) 
  { 
    return (key % size); 
  }


private:
  Key hash(const char* key)
  {
    long sum = 0;
    int len  = strlen(key);

    for(int i = 0; i<len; i++) {
      sum = sum + ((int)*key % size);
      key++;
    }
    return (Key)(sum % size);
  }

public:
  HashTable(int size1, bool doGC=false)
  {
    gc   = doGC; //2015/0215
    size = size1;
    table = new HashEntry*[size];

    for (int i =0 ; i<size; i++) {
      table[i] = NULL;
    }
  }

public:
  int add(Key key, CommonObject* object, int id)
  {
    int n = hash(key);

    HashEntry* ptr  = table[n];
    HashEntry* prev = ptr;
    int rc = TRUE;
    if(ptr == NULL) {
      table[n] = new HashEntry(key, object, id);
    } else {
      while(ptr) {
         prev = ptr;
         ptr  = ptr -> getNext();
         if(prev->getIntKey() == key &&
           prev->getSubKey() == id) {
           rc = False;
           break;
         }
       }
       if(rc == TRUE) {
         prev -> setNext(new HashEntry(key, object, id));
       }
     }
    return rc;
  }

public:
  int  add(const char* key, CommonObject* object)
  {
    int n = hash(key);

    HashEntry* ptr  = table[n];
    HashEntry* prev = ptr;
    int rc = TRUE;
    if(ptr == NULL) {
       table[n] = new HashEntry(key, object);
    } else {
      while(ptr) {
        prev = ptr;
        ptr  = ptr -> getNext();
        if(prev->getCharKey() == key) {
          rc = False;
          break;
        }
      }
      if(rc == TRUE) {
        prev -> setNext(new HashEntry(key, object));
      }
    }
    return rc;
  }

public:
  void enableGC()
  {
    gc = true;
  }

  void disableGC()
  {
    gc = false;
  }

public:
  CommonObject* lookup(Key key)
  {
    CommonObject* object = NULL;
    Key  n = hash(key);
    HashEntry* ptr = table[n];

    while (ptr) {
      if (ptr -> getIntKey() == key) {
        object = ptr -> getObject();
        break;
      }
      ptr = ptr -> getNext();
    }
    return object;
  }

public:
  CommonObject* lookup(Key key, int id)
  {
     CommonObject* object = NULL;
     int n = hash(key);
     HashEntry* ptr = table[n];
     while (ptr) {
        if(ptr -> getIntKey() == key &&
           ptr -> getSubKey() == id){
              object = ptr -> getObject();
              break;
         }
         ptr = ptr -> getNext();
     }
     return object;
  }

public:
  CommonObject* lookup(const char* key)
  {
    Key n = hash(key);
    HashEntry* ptr = table[n];
    CommonObject* object = NULL;

    while (ptr) {
      if(strcmp(ptr -> getCharKey(), key)==0) {
        object = ptr->getObject();
        break;
      }
      ptr = ptr -> getNext();
    }
    return object;
  }

public:
  void remove(HashEntry* prev, HashEntry* ptr, int n)
  {
    if(prev == ptr) {
      table[n] = ptr -> getNext();
    } else {
      prev -> setNext(ptr -> getNext());
    }
    delete ptr;
  }

public:
  int remove(Key key)
  {
    Key n = hash(key);
    HashEntry* ptr  = table[n];
    HashEntry* prev = ptr;

    while(ptr) {
      if(ptr -> getIntKey() == key) {
        remove(prev, ptr, n);
        return TRUE;
      }
      prev = ptr;
      ptr  = ptr -> getNext();
    }
    return False;
  }

public:
  int remove(const char* key)
  {
    Key n = hash(key);
    HashEntry* ptr  = table[n];
    HashEntry* prev = ptr;

    while (ptr) {
      if(strcmp(ptr -> getCharKey(), key)==0 ) {
        remove(prev, ptr, n);
        return TRUE;
      }
      prev = ptr;
      ptr  = ptr -> getNext();
    }
    return False;
  }

public:
  ~HashTable()
  {
    clear();
  }
	
  void clear()
  {
    if (table) {
      for(int i = 0; i < size; i++) {
        HashEntry* ptr  = table[i];
        HashEntry* prev = ptr;
        while(ptr) {
          prev = ptr;
          ptr  = ptr -> getNext();
          //2015/02/15
          if (gc) {
            CommonObject* object = prev->getObject();
            if (object) {
              //printf("HashTable::~HashTable gc\n");
              delete object;
              prev -> setObject(NULL);
            }
          }
          //
          delete prev;
        }
      }
      delete [] table;
    }

    table = NULL;
  }

public:
  //2014/12/20
  void display()
  {
    for(int i = 0; i < size; i++) {
      HashEntry* ptr  = table[i];
      while(ptr) {
      	CommonObject* object =  ptr -> getObject();
      	if (object) {
      	  object -> display();
      	}
        ptr  = ptr -> getNext();
      }
    }
  }

};

}



