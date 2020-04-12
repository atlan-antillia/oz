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
 *  ScreenCapture.h
 *
 *****************************************************************************/

/*
To avoid a collision of NT32 typedef definition in Xmd.h and jmorecfg.h,
in  /usr/include/jmorecfg.h, we modified the definition of INT32

in the following way:

//typdef long INT32;
#ifndef INT32
typedef int INT32;

 */

#include <oz++/motif/Application.h>
#include <oz++/motif/Image.h>
#include <oz++/JPGFileWriter.h>

namespace OZ {

class ScreenCapture :public CommonObject {
private:
  Display* display;
  Image* image; 
  int x;
  int y;
  unsigned int width;
  unsigned int height;

public:
  ScreenCapture(Application& applet)
   :display(applet.getDisplay()),
   image(NULL),
   x(0),
   y(0),
   width(0),
   height(0)
  {
  }

  ~ScreenCapture()
  {
    clear(); 
  }

  void clear()
  {
    if (image) {
      delete image;
      image = NULL;
    }
  }

  bool capture(int x, int y, unsigned int width, unsigned int height)
  {
    bool rc = true;
    this -> x = x;
    this -> y = y;
    this -> width =  width;
    this -> height = height;

    clear(); 

    //Get a default root window of the display.
    Window window = XDefaultRootWindow(display);

    //Create an XImange of a subregion of the default root window above.
    image = new Image(display, window,
          x, y, width, height, AllPlanes, ZPixmap);
    if (image == NULL) {
      throw IException("Failed to create an image");
    }
    return rc;
  }
  
  Image* getImage()
  {
    return image;
  }

  int getX()
  {
    return x;
  }

  int getY()
  {
    return y;
  }

  unsigned int getWidth()
  {
    return width;
  }

  unsigned int getHeight()
  {
    return height;
  }

  void getGeometry(int& x, int& y, unsigned int& width, unsigned int& height)
  {
    x      = this -> x;
    y      = this -> y;
    width  = this -> width;
    height = this -> height;
  }

  void saveAsJPG(const char* filename, int quality)
  {
    try {
      JPGFileWriter writer(filename);

      unsigned char* data = (unsigned char*)image -> getRGBData();
      int depth = image -> getDepth();
      printf("filename=%s x=%d y=%d width=%d height=%d depth=%d\n", 
             filename,
             x, y, width, height, depth);
      //2015/08/26
      int channels = depth/8; 
      bool vertFlip = false;
      writer.write(width, height, depth, channels, data, quality, vertFlip);
      delete [] data;

    } catch (Exception& ex) {
      ex.display();
    }
  }
};
}

