/******************************************************************************
 *
 * Copyright (c) 2014  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  QPEncoder.cpp
 *
 *****************************************************************************/


#include <oz++/QPEncoder.h>
#include <oz++/Bytes.h>

/**
 *
 */
int main(int argc, char** argv)
{
  unsigned char input[128];
  for (size_t i = 0; i<sizeof(input); i++) {
    input[i] = (i+1)*(i+2)*(i+3) % 128;  
  }  
  printf("Original bytes len=%zd\n", sizeof(input));

  Bytes::dump(input, sizeof(input));

  // 1 QPEncoder
  Bytes byteArray(input, sizeof(input));
  CharString encString = "";

  QPEncoder encoder;
  encoder.encode(byteArray, encString);

  printf("Encoded=[%s]\n", (const char*)encString);
  
  // 2 QPEncoder
  char* enc = NULL;
  encoder.encode(input, sizeof(input), &enc);
  printf("Encoded2=[%s]\n", (const char*)enc);

  delete [] enc;
}
