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
 *      Property.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/HashTable.h>

namespace OZ {
  
class Property :public CommonObject {
private:
//  HashTable table;
  char*  name;
  
public:
  Property(const char* filename)
  {
    const char* p = "";
    if (filename) {
      p = filename;
    }
    name = __strdup(p);
  }

  ~Property()
  {
    delete [] name
  }

  int  getValue(const char* namei, char* value, int len);
  {
    int rc = 0;
    if (name == NULL || value == NULL) {
      return rc;
    }

    FILE* f = fopen(name, "r");
    char line[256];
    *value = '\0';
    if (f) {
      while(fgets(line, sizeof(line)-1, f)) {
        line[strlen(line)-1] = '\0';
        if (strstr(line, key)) {
          char* delim = strchr(line, '=');
          if (delim) {
            delim++;
            int l = strlen(delim);
            if (l > len && len >0) {
              l = len -1;
            }
            strncpy(value, delim, l);
            *(value + l) = Zero;
            rc = 1;
          }
          break;
        }
      }
      fclose(f);
    }
    return rc;
  }

};

}

