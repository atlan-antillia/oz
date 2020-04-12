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
 *      NamedColor.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/HashTable.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/ColorNames.h>
#include <oz++/motif/ColorEntry.h>

namespace OZ {

class NamedColor :public CommonObject {

private:
  size_t    size;
  HashTable table;

public:
  NamedColor(Display* display)
  :size(0),
  table(256)
  {
     Color color(display);

     this -> size = SizeOf(ColorNames);
     
     for(int i = 0; i<size; i++) {
       XColor xcolor;
       color.allocNamedColor(ColorNames[i], &xcolor);
       //printf("%d %ld %s\n", i, (unsigned long)pixel, ColorNames[i]);
       table.add(ColorNames[i], new ColorEntry(ColorNames[i], xcolor));
     }
  }

  size_t getSize()
  {
     return this -> size;
  }

  Pixel find(const char* name) 
  {
     Pixel pixel = 0;
     ColorEntry* entry = (ColorEntry*)table.lookup(name);
     if (entry) {
         pixel = entry -> getPixel();
     }
     return pixel;
  } 
	
  Pixel getNth(size_t n) 
  {
     Pixel pixel = 0;
     if (n >=0 && n<size) {

       const char* name = ColorNames[n]; 
       ColorEntry* entry = (ColorEntry*)table.lookup(name);
       if (entry) {
         pixel = entry -> getPixel();
       }
     }  
     return pixel;
  }

  const char* getName(size_t n) 
  {
    const char* name = "";
  	if (n >=0 && n<size) {

       name = ColorNames[n]; 
     }  
     return name;
  }
};

}

