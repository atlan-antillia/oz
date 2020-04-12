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
 *  QueueT.cpp
 *
 *****************************************************************************/

#include <oz++/Locale.h>
#include <oz++/QueueT.h>
#include <oz++/CharString.h>
#include <oz++/WcharString.h>

//
int main(int argc, char* argv[])
{
  Locale locale;
  fwide(stdout, 1);

  try {
    QueueT<WcharString> queue;

    WcharString string1(L"1 OpenFlow: オープンフロー");
    WcharString string2(L"2 NFV: ネットワーク機能仮想化");

    WcharString string3(L"3 Hello world. こんにちは世界");
    WcharString string4(L"4 Goodbye world. さようなら世界");

    WcharString string5(L"5 Happy birthday. 誕生日おめでとう");
    WcharString string6(L"6 The end of eternity.  永遠の終わり");
  
    queue.add(&string1);
    queue.add(&string2);
    queue.add(&string3);
    queue.add(&string4);
    queue.add(&string5);
    queue.add(&string6);

    queue.display();

    
    WcharString* object = NULL; 
    
    while ( (object = queue.get()) !=NULL ) {
      object->display();
    }

  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}
