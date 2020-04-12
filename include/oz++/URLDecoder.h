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
 *  URLDecoder.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/CharString.h>

/**
 */
namespace OZ {

class URLDecoder :public CommonObject {

public:
  /**
   */
  URLDecoder() {
  }

  /**
   */
  ~URLDecoder() {
  }

public:
  /**
   * @param data    [IN]  Null-terminated string to decode.
   * @param encoded  [OUT]  URL-decoded string.
   */
  bool decode(const char* data, CharString& decoded) {
    bool rc = false;
    char* dec = decode(data);
    if (dec) {
      decoded = dec;
      delete [] dec;
      rc = true;
    }
    return rc;
  }


public:
  CharString decode(const CharString bstring) {
    const char* dec = NULL;
    const char* string = (const char*)bstring;
    if (string) {
      dec = decode(string);
    }
    return CharString(dec, false);
  }

public:
  /**
   */
  char* decode(const char* data) {
    if (data == NULL) {
      return NULL;
    }

    int len = strlen(data);
    char* buffer = new char[len + 1];
    memset(buffer, 0, len+1);

    int n = 0;
    for (int i = 0; i < len; i++) {
      unsigned char c = data[i];
      if (c == '+') {
        buffer[n++] = ' ';
      } else if (c == '%') {
        int v1 = toInt(data[++i]);
        int v2 = toInt(data[++i]);
        char r = (char)(v1*16 + v2);
        buffer[n++] = r;
        } else {
        buffer[n++] = c;
      }
      }
  
    buffer[n] = ZERO;
  
    return buffer;
  }

private:

  /**
   */
  int toInt(char c) {
    c = toupper(c);

    int n = 0;
    if ((c >= '0') && (c <= '9')) {
      n = c - '0';
    } else if ((c >= 'A') && (c <= 'F')) {
      n = 10 + (c - 'A');
    } 
    return n;
  }


};

}



