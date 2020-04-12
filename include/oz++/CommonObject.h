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
 *  CommonObject.h
 *
 *****************************************************************************/

#pragma once

#define __STDC_FORMAT_MACROS
#define True 1
#define False 0

#define TRUE 1
#define FALSE 0
#include <list>
#include <memory>
#include <vector>
#include <string>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <wchar.h>

#include <stdint.h>
#include <inttypes.h>
//2015/06/02
#include <linux/types.h>

#include <oz++/AuxDef.h>
#include <oz++/StrDef.h>
#include <oz++/Exception.h>
#include <oz++/SmartPtr.h>
#include <oz++/TypeConverter.h>

namespace OZ {

class CommonObject {
private:
  int id;
  size_t reference;

public:
  CommonObject() 
  {
    id = 0;
    reference = 0;
  }
  
public:
  virtual ~CommonObject() 
  {
  }

public:
  void incrementReference()
  {
    reference++;
  }  

  void decrementReference()
  {
    reference--;
  }  

public:
  virtual int compare(CommonObject* object)
  {
    return 0;
  }

  virtual const char* toString() 
  {
    return NULL;
  }

  virtual void display() 
  {
  }

  virtual void dump() 
  {
  }

  virtual const char* error() 
  {
    return strerror(errno);
  }

  void setId(int i)
  {
    this->id = i;
  }

  int getId()
  {
    return this->id;
  }
/*	
  virtual int operator<(CommonObject& object)
  {
  	return compare(&object);		
  }
	
  virtual int operator>(CommonObject& object)
  {
  	return -compare(&object);		
  }
*/
};
#ifdef DEBUG
#define dprintf printf
#else 
#define dprintf
#endif

}

using namespace OZ;

