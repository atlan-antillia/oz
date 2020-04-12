/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ModuleFileName.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/CommonObject.h>
#include <oz++/Exception.h>
#include <oz++/StringT.h>
#include <oz++/File.h>
#include <unistd.h>
#include <limits.h>

namespace OZ {

class ModuleFileName :public CommonObject {

private:
  char path[PATH_MAX];

public:
  ModuleFileName()
  {
    memset(path,   0, SizeOf(path)); 
    ssize_t len = ::readlink("/proc/self/exe", path, SizeOf(path));
    if (len == -1 || len == SizeOf(path)) {
      len = 0;
    }
    path[len] = '\0';
    char* slash = strrchr(path, '/');
    if (slash ) {
      *slash = '\0';
    } 
  }
  
public:
  const char* getPath() const
  {
    return path;
  }

  StringT<char> getFullpath(const char* filename)
  {
    assert(filename);

    const char* base = getPath();
    char fullpath[PATH_MAX] = {0};
    if (filename[0] != '/') {
      sprintf(fullpath, "%s/%s", base, filename);
    } else {
      sprintf(fullpath, "%s", filename);
    }
    File file(fullpath);
    if (file.exists()) {
      return StringT<char>(fullpath);
    } else {
      throw IException("Not found %s", fullpath);
    }
  }

  StringT<char> getFullpath(const StringT<char>& filename)
  {
    return ModuleFileName::getFullpath((const char*)filename);
  }
};

}


