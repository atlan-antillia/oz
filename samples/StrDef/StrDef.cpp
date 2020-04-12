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
 *  StrDef.cpp
 *
 *****************************************************************************/


#include <oz++/StrDef.h>
#include <oz++/CharString.h>
#include <oz++/WcharString.h>
#include <oz++/Locale.h>

int main(int argc, char* argv[])
{
  try {
    Locale locale;
    fwide(stdout,1);

    CharString string("Hello world");
    char buff[128];

    __strcpy(buff, (const char*)string);

    WcharString helloworld = L"こんにちは、世界のみなさん！";
    //wchar_t* helloworld = L"こんにちは、世界のみなさん！";
    wchar_t wbuff[128];
    __strcpy(wbuff, (const wchar_t*)helloworld);

    wprintf(L"buff=%s\n", buff);
    wprintf(L"wbuff=%ls\n", wbuff);
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

