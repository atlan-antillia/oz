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
 *  LinkedListT.cpp
 *
 *****************************************************************************/


#include <oz++/Locale.h>
#include <oz++/LinkedListT.h>
#include <oz++/CharString.h>
#include <oz++/WcharString.h>


int main(int argc, char* argv[])
{
  Locale locale;
  fwide(stdout, 1);

  try {

    LinkedListT<CharString>  list;
    LinkedListT<WcharString> wlist;
    list.disableGC();
    wlist.disableGC();

    CharString string1("1 OpenFlow: オープンフロー");
    CharString string2("2 NFV: ネットワーク機能仮想化");

    CharString wstring1("3 Hello world. こんにちは世界");
    CharString wstring2("4 Goodbye world. さようなら世界");

    CharString wstring3("5 Happy birthday. 誕生日おめでとう");
    CharString wstring4("6 The end of eternity.  永遠の終わり");
  
    list.add(&string1);
    list.add(&string2);


    list.add(&wstring1);
    list.add(&wstring2);
    list.add(&wstring3);
    list.add(&wstring4);
    
    list.display();

  } catch (Exception& ex) {
    caught(ex);
  }

  return 0;
}
