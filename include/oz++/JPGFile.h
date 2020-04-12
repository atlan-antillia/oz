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
 *  JPGFile.h
 *
 *****************************************************************************/

//2017/09/02 Modified to use fullpath in JPGFile constructor.

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/Exception.h>
#include <oz++/ModuleFileName.h>

#include <jpeglib.h>

namespace OZ {

class JPGFile :public CommonObject {
protected:
  typedef enum {
    READER = 0,
    WRITER = 1,
  } OPERATION;

private:
  FILE* fp;
  OPERATION operation;

private:
  JPGFile()
  {
  }

protected:
  JPGFile(OPERATION operation,const char* filename)
  :fp(NULL),
  operation(operation)
  {
    if (filename == NULL) {
      throw IException("Invalid argument");
    }
    //2017/09/02
    ModuleFileName module;
    char fullpath[PATH_MAX*2];
    if (filename[0] != SLASH) {
      sprintf(fullpath, "%s/%s", module.getPath(), filename);
    } else {
      sprintf(fullpath, "%s", filename);
    }
    static const char* mode[] =  { "rb", "wb" };

    fp = fopen(fullpath, mode[operation]);
    if (fp == NULL) {
      throw IException("Failed to open %s", filename);
    }
  }

  ~JPGFile()
  {
    close();
  }

  FILE* getFP()
  {
    return fp;
  }
	
  void close()
  {
    if (fp) {
     fclose(fp);
     fp = NULL;
    }
  }
	
};

}
 
