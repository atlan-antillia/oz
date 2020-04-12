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
 *      OpenGLApplication.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/StringT.h>
#include <oz++/motif/Application.h>
#include <oz++/opengl/OpenGLContext.h>

namespace OZ {
const char* XmNopenGLContext = "openGLContext";
  
class OpenGLApplication :public Application {

private:
  OpenGLContext*      glContext;
  int   majorVersion;
  int   minorVersion;

public:
  OpenGLApplication(const char* appclass1, int argc, char** argv)
  :Application(appclass1, argc, argv),
  glContext(NULL),
  majorVersion(2),
  minorVersion(1)
  {
    if (argc >= 3) {
      for (int i = 1; i< argc; i++) {
        if (StringT<char>::startsWith(argv[i], "major=")) {
          char* eq = strstr(argv[i], "=");
          int major = atoi(++eq);
          if (major > 1) {
            majorVersion = major;
          }
        }
        if (StringT<char>::startsWith(argv[i], "minor=")) {
          char* eq = strstr(argv[i], "=");
          int minor = atoi(++eq);
          if (minor >= 0) {
            minorVersion = minor; 
          }
        }
      }
    }
    printf("Major=%d, Minor=%d\n", majorVersion, minorVersion);
    glContext = new OpenGLContext(getDisplay(), majorVersion, minorVersion);
  }

public:
  OpenGLApplication(const char* appclass1, char** resources, 
      int argc, char** argv)
  :Application(appclass1, resources, argc, argv),
  glContext(NULL)
  {
    glContext = new OpenGLContext(getDisplay());
  }

  ~OpenGLApplication() 
  {
    if (glContext) {
      delete glContext;
      glContext = NULL;
    }
  }

  Visual* getVisual()
  {
    return glContext->getVisual();
  }

  int     getDepth()
  {
    return glContext->getDepth();
  }

  Colormap getColormap()
  {
    return glContext->getColormap();
  }

  OpenGLContext* getOpenGLContext() const
  {
    return glContext;
  }

  int getMajorVersion() 
  {
    return majorVersion;
  }

  int getMinorVersion()
  {
    return minorVersion;
  }
};

}

