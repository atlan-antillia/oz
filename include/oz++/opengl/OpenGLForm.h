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
 *  OpenGLForm.h
 *
 *****************************************************************************/

#pragma once


#include <oz++/motif/Form.h>
#include <oz++/opengl/OpenGLMainView.h>
#include <oz++/opengl/OpenGLApplication.h>
#include <oz++/opengl/OpenGLVisualBuffer.h>

namespace OZ {

class OpenGLForm :public Form {

private:
  OpenGLContext* context;
  Colormap     colormap;
  Visual*      visual;
  int          depth;
  int          bufferType;

public:
  OpenGLForm(OpenGLMainView* mainv, const char* name, Args& args)
  :Form(mainv->getMainWindow(), name, args)
  ,context(NULL)
  {
    this -> bufferType = mainv->getBufferType();//OpenGL_DOUBLE_BUFFER;  
    this -> depth      = mainv->getDepth();
    this -> visual     = mainv->getVisual();
    this -> colormap   = mainv->getColormap();

    if (args.has(XmNopenGLBufferType)) {
    	bufferType = (int)args.get(XmNopenGLBufferType);
    }

    this->context = mainv ->getOpenGLContext();

  }
  
  ~OpenGLForm()
  {
  }

  OpenGLContext* getOpenGLContext() const 
  {
    return context;
  }    

  int getBufferType()
  {
    return bufferType;	
  }

  int     getDepth()
  {
    return depth;
  }
  
  Visual* getVisual()
  {
    return visual;
  }

  Colormap getColormap()
  {
    return colormap;
  }
};

}

