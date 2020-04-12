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
 *      Image.h
 *
 *****************************************************************************/
//2015/05/14 Updated

#pragma once

#include <Xm/Xm.h>
#include <oz++/Exception.h>
#include <oz++/motif/Resource.h>

//Simple Image class to represend XImage structure 
/*
typedef struct _XImage {
	int width, height;		// size of image 
	int xoffset;			// number of pixels offset in X direction 
	int format;			// XYBitmap, XYPixmap, ZPixmap 
	char *data;			// pointer to image data 
	int byte_order;			// data byte order, LSBFirst, MSBFirst 
	int bitmap_unit;		// quant. of scanline 8, 16, 32 
	int bitmap_bit_order;		// LSBFirst, MSBFirst 
	int bitmap_pad;			// 8, 16, 32 either XY or ZPixmap 
	int depth;			// depth of image 
	int bytes_per_line;		// accelerator to next scanline 
	int bits_per_pixel;		// bits per pixel (ZPixmap) 
	unsigned long red_mask;		// bits in z arrangement 
	unsigned long green_mask;
	unsigned long blue_mask;
	XPointer obdata;		// hook for the object routines to hang on 
	struct funcs {			// image manipulation routines 
		struct _XImage *(*create_image)();
		int (*destroy_image)();
		unsigned long (*get_pixel)();
		int (*put_pixel)();
		struct _XImage *(*sub_image)();
		int (*add_pixel)();
	} f;
} XImage;
 */

namespace OZ {

class Image :public Resource {
private:
  Screen* screen;

public:
  Image(Display* display, unsigned int depth,
    int format, int offset, char* data, unsigned int width,
    unsigned int height, int pad, int bytes)
    :Resource(display)
  {
    screen  = XDefaultScreenOfDisplay(display);

    XImage* image = XCreateImage(display,
         DefaultVisualOfScreen(screen),
         depth, format, offset, data,
         width, height, pad, bytes);
    if (image) {
      Status status = initialize(image);
      if (status != 0) { 
        image -> byte_order = getByteOrder();
        set(image);
 
      } else {
       throw IException("Failed to XInitImage");
      }
    } else {
       throw IException("Failed to XCreateImage");
    }
  }

  Image(Display* display, Window window,
       int x, int y, unsigned int width, unsigned int height,
    unsigned long mask, int format)
    :Resource(display)
  {
    screen  = XDefaultScreenOfDisplay(display);
    set(XGetImage(display, window,
        x, y, width, height, mask, format));
  }

  ~Image()
  {
    XDestroyImage(get());
  }

  int getByteOrder () 
  {
    union {
      char c[sizeof(short)];
      short s;
    } order;

    order.s = 1;

    if ((1 == order.c[0])) {
      return LSBFirst;
    } else {
      return MSBFirst;
    }
  }

  Status initialize(XImage* ximage)
  {
     Status status = 0;
     if (ximage) {
       status = XInitImage(ximage);
     }
     return status;
  }

  XImage* get()
  {
    XImage* ximage = (XImage*)Resource::get();
    if (ximage) {
      return ximage;
    } else {
      throw IException("XImage is NULL.");
    }
  }

  /*
   See https://hamberg.no/erlend/posts/2011-01-06-read-current-x-background-to-jpeg.html
  */

  //Your have to delete the returned RGB data after its use.
  unsigned char* getRGBData()
  {
    XImage* image = get();
    int width  = image -> width;
    int height = image -> height;

    size_t bsize = sizeof(char) * 3 * width * height;

    unsigned char* rgb = new unsigned char[bsize];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned long pixel = XGetPixel(image, x, y);
            rgb[ y * width *3 + x * 3 + 0] = (char)(pixel >> 16);
            rgb[ y * width *3 + x * 3 + 1] = (char)((pixel & 0x00ff00)>>8);
            rgb[ y * width *3 + x * 3 + 2] = (char)(pixel & 0x0000ff);
        }
    }
    return rgb;
  }

  //Your have to delete the returned RGB data after its use.
  unsigned char* getBGRData()
  {
    XImage* image = get();
    int width  = image -> width;
    int height = image -> height;

    size_t bsize = sizeof(char) * 3 * width * height;

    unsigned char* rgb = new unsigned char[bsize];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned long pixel = XGetPixel(image, x, y);
            rgb[ y * width *3 + x * 3 + 0] = (char)(pixel & 0x0000ff);
            rgb[ y * width *3 + x * 3 + 1] = (char)((pixel & 0x00ff00)>>8);
            rgb[ y * width *3 + x * 3 + 2] = (char)(pixel >> 16);
        }
    }
    return rgb;
  }
   
  int  getDepth()
  {
    return get()->depth;
  }

  int  getWidth()
  {
    return get()->width;
  }

  int  getHeight()
  {
    return get()->height;
  }

  int getFormat()
  {
     return get()->format;      // XYBitmap, XYPixmap, ZPixmap
  }

  int getImageByteOrder()
  {
     return get()->byte_order;  // data byte order, LSBFirst, MSBFirst
  }

  int getBitmapUnit()
  {   
    return get()->bitmap_unit;   // quant. of scanline 8, 16, 32
  }

  int getBitmapBitOrder()
  {
    return get()->bitmap_bit_order;  // LSBFirst, MSBFirst
  }

  int getBitmapPad()
  {
    return get()->bitmap_pad;        // 8, 16, 32 either XY or ZPixmap
  }

  int getBytesPerLine()
  {
     return get()-> bytes_per_line;  // accelerator to next scanline
  }
  int getBitsPerPixel()
  {
     return get()->bits_per_pixel;    
  }

  char*  getData()
  {
    return get()->data;
  }

  unsigned long getPixel(int x, int y)
  {
    return XGetPixel(get(), x, y);
  }
  
  void putPixel(int x, int y, unsigned long pixel)
  {
    XPutPixel(get(), x, y, pixel);
  }
  
  XImage* subImage(int x, int y, unsigned int subimage_width, 
       unsigned int subimage_height)
  {
    return XSubImage(get(), x, y, subimage_width, subimage_height);
  }
  
  void  addPixel(long value)
  {
    XAddPixel(get(), value);
  }

};

}


