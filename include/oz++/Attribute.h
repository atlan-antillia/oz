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
 *  Attribute.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/CharString.h>

namespace OZ {

class Attribute :public CommonObject {
private:
  CharString    _name;
  CommonObject* _value;

public:
  Attribute()
    :_name(""),
     _value(NULL)
  { 
  }

public:
  Attribute(const char* name, CommonObject* value)
    :_name(name),
     _value(value)
  { 
  }

  ~Attribute() 
  { 
    if (_value) {
      delete _value;
      _value = NULL;
    }
  }

  void set(const char* n, CommonObject* v)
  {
    if (n) {
      _name = n;
    }
    if (v) {
      _value = v;
    }
  }

  
  CommonObject*  value() 
  {
    return this->_value;
  }
  
  CharString getName()
  {
    return this->_name;
  }

  CharString getValue()
  {
    const char* v = this -> _value -> toString();
    //printf("Attribute %s=%s\n", (const char*)getName(), v);

    if (v == NULL) {
      v = "";
    }
    return CharString(v );
  }

  void value(CommonObject*  v) 
  {
    if (this-> _value != v) {
      delete this-> _value;
    }
    this -> _value = v;
  }

  void display()
  {
    if (_value) {
      printf("%s: %s\n", (const char*)getName(), (const char*)getValue());
    }
  }
};

}


