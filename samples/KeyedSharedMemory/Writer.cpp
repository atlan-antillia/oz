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
 *  Writer.cpp
 *
 *****************************************************************************/


#include <oz++/Thread.h>
#include <oz++/KeyedSharedMemory.h>
#include <oz++/KeyedSemaphore.h>

#include "Def.h"

namespace OZ {

class Writer :public CommonObject {
private:
  KeyedSharedMemory memory;

public:
  Writer()
  {
    size_t size = 1024;
    key_t shm_key = ftok(SHM_KEY_FILENAME, SHM_KEY_ID);

    memory.create(size, shm_key);
  }

public:
  ~Writer()
  {
  }


public:
  void write()
  {
    printf("Start write method \n");
    Table tables[10];
    for(int i = 0; i<SizeOf(tables); i++) {
      tables[i].value = i*100;
      sprintf(tables[i].name, "Someone like you %d.", i);
    }
    
    void* addr = memory.attach(NULL, 0);
    memcpy(addr, tables, sizeof(tables));

    printf("Written \n");
        for(int i = 0; i<SizeOf(tables); i++) {
            printf("tables[%d].value=%d\n", i, tables[i].value);
            printf("tables[%d].name =%s\n", i, tables[i].name);
      printf("\n");
        }

    printf("End write method \n");
  }
};

}


/**
 *
 */
int main(int argc, char* argv[])
{
  try {
    printf("Create an instance of Writer.\n");
    Writer writer;

    printf("Call write method.\n");
    writer.write();
  
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;    
}
