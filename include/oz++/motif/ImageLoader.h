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
 *  ImageLoader.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/Exception.h>
#include <oz++/motif/View.h>
#include <oz++/motif/DC.h>
#include <oz++/PNGFileReader.h>
#include <oz++/JPGFileReader.h>

namespace OZ {

class ImageLoader {
private:
 static
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

public:
  //Convert cv::Mat to Xlib Pixmap.
  static 
  Pixmap convertToPixmap(View* view, cv::Mat& mat)
  {
    if (view && !mat.empty() ) {

      //1 Convert cv::Mat to XImage.
      XImage ximage;
      memset(&ximage, 0, sizeof(ximage));
      fromMatToXImage(mat, ximage);

      //2 Create Pixmap from XImage
      return fromXImageToPixmap(view, ximage);
    } else {
      throw IException("Invalid parameter.");
    }
  }

  //Convert a cv::Mat to an XImage.
  static
  void fromMatToXImage(cv::Mat& mat, XImage& ximage)
  {
    //Convert IplImage to XImage; the data field pointer of IplImage 
    //is merely copied to the data field to XImage.
    // 
    IplImage image = mat;

    //Check Iplimage depth field.
    //Is the depth value 8?
    dprintf("fromMatToXImage depth=%d channels=%d\n", 
	image.depth, image.nChannels);

    // 2017/08/20 
    cv::Mat cmat;// = mat.clone(); 
    if (image.depth != IPL_DEPTH_8U) {
      mat.convertTo(cmat, CV_8U);
      image = cmat;
    }
    assert(image.depth ==IPL_DEPTH_8U);
    //XImage depth 
    unsigned int depth = image.depth*image.nChannels;

    //The data field of XImage  points the imageData 
    //field of IplImage structure.
    //
    int byte_order = getByteOrder();

    ximage.width            = image.width;
    ximage.height           = image.height;
    ximage.xoffset          = 0;
    ximage.format           = ZPixmap;
    if (image.nChannels == 1) {
    ximage.format           = XYPixmap;
    }
    ximage.data             = image.imageData; 
                              //XImage data is a shallow copy of Iplimage data.
    ximage.byte_order       = byte_order;
    ximage.bitmap_unit      = 0;
    ximage.bitmap_bit_order = byte_order;
    ximage.bitmap_pad       = 8; 
    ximage.depth            = depth; //24;
    ximage.bytes_per_line   = image.widthStep;
    //ximage.bits_per_pixel   = 24;
    ximage.bits_per_pixel   = depth;
    
    //We don't use the following rgb-mask fields.
    //ximage.red_mask       = 0x0000;	// bits in z arrangement 
    //ximage.green_mask     = 0x0000;
    //ximage.blue_mask      = 0x0000;
    
    Status status = XInitImage(&ximage);
    if (status == 0) { 
      throw IException("Failed to XInitImage");
    }
   
  }

  //Create a Pixmap resource from an XImage.
  static
  Pixmap fromXImageToPixmap(View* view, XImage& ximage)
  {   
    DC dc(view);

    Dimension width  = ximage.width;
    Dimension height = ximage.height;
    unsigned int depth = ximage.depth;
    Display* display  = dc.getDisplay();
    Drawable drawable = dc.getDrawable();
    Pixmap pixmap = None; 
    GC       gc       = dc.getGC();
    Screen* screen = XDefaultScreenOfDisplay(display);
   
    if (ximage.format == ZPixmap) {
       pixmap = XCreatePixmap(display,
         RootWindowOfScreen(screen),
         width, height,
         depth);

      if (pixmap == BadAlloc || pixmap ==  BadDrawable ||
        pixmap ==  BadValue) {
        throw IException("Failed to XCreatePixmap");
      } else { 
        XPutImage(display, pixmap, gc, &ximage, 0, 0, 0, 0, width, height);
        return pixmap;
      }
    } else if (ximage.format ==XYPixmap) {
      Pixel bg = BlackPixelOfScreen(screen);
      Pixel fg = WhitePixelOfScreen(screen);

      pixmap = XCreatePixmapFromBitmapData(display, 
			DefaultRootWindow(display), 
			ximage.data, ximage.width, ximage.height, 
		fg, bg, ximage.depth) ;
      return pixmap;
    }
  }
};

}

