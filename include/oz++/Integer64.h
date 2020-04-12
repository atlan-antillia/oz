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
 *	Integer64.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/CharString.h>

namespace OZ {

class Integer64 :public CommonObject {
private:
  int64_t     _value;
  CharString  _string;
public:
  Integer64(int64_t val) 
  :_value(val) 
  {
    _string = toCharString();
  }

  int64_t   getContents() const 
  { 
    return _value; 
  }

  void value(int64_t v)
  {
    this -> _value = v;
  } 

  int64_t  value() 
  {
     return this -> _value;
  } 

  operator int64_t() 
  { 
    return _value; 
  }

  void write()
  {
     printf("%" PRId64 "", _value);
  }

  //2015/03/24
  CharString toCharString()
  {
    char string[128];
    sprintf(string, "%" PRId64 "", _value);
    return CharString(string);
  }

  const char* toString()
  {
  	return (const char*)_string;
  }

  void display()
  {
     printf("%" PRId64 "\n", _value);
  }
};

}

