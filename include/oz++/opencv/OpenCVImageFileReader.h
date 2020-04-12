/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  OpenCVImageFileReader.cpp
 *
 *****************************************************************************/

#pragma once

#include <oz++/File.h>
#include <oz++/ModuleFileName.h>
#include <oz++/opencv/OpenCVObject.h>

namespace OZ {

class OpenCVImageFileReader {
private:

  ModuleFileName module;

public:
  OpenCVImageFileReader()
  {
  }

  cv::Mat load(const char* filename, int loadingFlag = cv::IMREAD_COLOR)
  {
    const StringT<char> fullpath = module.getFullpath(filename);
    printf("fullpath %s\n", (const char*)fullpath);

    File file(fullpath);
    if (file.exists()) {
      cv::Mat image = cv::imread((const char*)fullpath, loadingFlag);
      if (!image.empty()) {
        return image;
      } else {
        throw OZ::IException("Failed to cv::imread %s", fullpath);
      }
    } else {
      throw OZ::IException("Not found %s", fullpath);
    }
  }
};

}

