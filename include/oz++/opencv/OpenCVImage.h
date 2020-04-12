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
 *  OpenCVImage.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/ModuleFileName.h>
#include <oz++/opencv/OpenCVObject.h>
#include <oz++/motif/DC.h>
#include <oz++/motif/Color.h>

using namespace cv;
using namespace std;

namespace OZ {
/*
  Based on cxtypes.h 
typedef struct _IplImage 
{ 
    int  nSize;            // sizeof(IplImage)  
    int  ID;               // version (=0)
    int  nChannels;        // Most of OpenCV functions support 1,2,3 or 4 channels  
    int  alphaChannel;     // ignored by OpenCV 
    int  depth;            // pixel depth in bits: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16S, 
                           // IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F are supported  
    char colorModel[4];    // ignored by OpenCV 
    char channelSeq[4];    // ditto 
    int  dataOrder;        // 0 - interleaved color channels, 
                           //          1 - separate color channels. 
                           //          cvCreateImage can only create interleaved images 
    int  origin;           //      0 - top-left origin, 
                           //          1 - bottom-left origin (Windows bitmaps style)
    int  align;            //       Alignment of image rows (4 or 8). 
                           //           OpenCV ignores it and uses widthStep instead 
    int  width;            //       image width in pixels 
    int  height;           //       image height in pixels 
    struct _IplROI *roi;       // image ROI. if NULL, the whole image is selected 
    struct _IplImage *maskROI; // must be NULL 
    void  *imageId;            // ditto 
    struct _IplTileInfo *tileInfo; // ditto 
    int  imageSize;            // image data size in bytes 
                               //          (==image->height*image->widthStep 
                               //          in case of interleaved data 
    char *imageData;           // pointer to aligned image data 
    int  widthStep;            //  size of aligned image row in bytes 
    int  BorderMode[4];        // ignored by OpenCV 
    int  BorderConst[4];       //  ditto 
    char *imageDataOrigin;     // pointer to very origin of image data 
                               //             (not necessarily aligned) - 
                               //             needed for correct deallocation 
} 
IplImage;  
  */

/*
typedef struct _XImage {
  int width, height;		// size of image 
  int xoffset;			// number of pixels offset in X direction 
  int format;			// XYBitmap, XYPixmap, ZPixmap 
  char *data;			// pointer to image data 
  int byte_order;		// data byte order, LSBFirst, MSBFirst 
  int bitmap_unit;		// quant. of scanline 8, 16, 32 
  int bitmap_bit_order;		// LSBFirst, MSBFirst 
  int bitmap_pad;		// 8, 16, 32 either XY or ZPixmap 
  int depth;			// depth of image 
  int bytes_per_line;		// accelerator to next scanline 
  int bits_per_pixel;		// bits per pixel (ZPixmap) 
  unsigned long red_mask;	// bits in z arrangement 
  unsigned long green_mask;
  unsigned long blue_mask;
  XPointer obdata;		// hook for the object routines to hang on 
  struct funcs {		// image manipulation routines 
    struct _XImage *(*create_image)();
    int (*destroy_image)();
    unsigned long (*get_pixel)();
    int (*put_pixel)();
    struct _XImage *(*sub_image)();
    int (*add_pixel)();
  } f;
} XImage;
 */

class OpenCVImage :public OpenCVObject {
private:
  cv::Mat image;
  cv::Mat resizedImage;

  XImage  ximage;

  static const int  MAX_IMAGE_WIDTH  = 640;
  static const int  MAX_IMAGE_HEIGHT = 480;
 
public:
  OpenCVImage(Display* display,const char* filename, int loadflag=1) 
  {
    memset(&ximage, 0, sizeof(ximage));
    ModuleFileName module;
    const StringT<char> fullpath = module.getFullpath(filename);
    image = cv::imread((const char*)fullpath, loadflag);
    if (image.empty()) {
      throw IException("Failed to load file %s", filename);
    } 
    int width  = image.rows;
    int height = image.cols;
    float ratio = 1.0f;
    //Width ratio
    float wr = (float)MAX_IMAGE_WIDTH/(float)width;
    //Height ratio
    float hr = (float)MAX_IMAGE_HEIGHT/(float)height;
    if (wr <1.0f || hr < 1.0f) {
      if (wr < hr) {
        ratio = wr;
      } else {
        ratio = hr;
      }
    }
    //ratio = 0.8f;    
    cv::resize(image, resizedImage, cv::Size(), ratio, ratio);

    IplImage iplimage = resizedImage;
    
    convertToXImage(display, &iplimage);  
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

  void convertToXImage(Display* display, const IplImage* image)
  {
    //Check Iplimage depth field.
    //Is the depth value 8?
    assert(image->depth ==IPL_DEPTH_8U);

    //XImage depth 
    unsigned int depth = image->depth*image->nChannels;

    //The data field of XImage  points the imageData 
    //field of IplImage structure.
    //
    int byte_order = getByteOrder();

    ximage.width            = image -> width;
    ximage.height           = image -> height;
    ximage.xoffset          = 0;
    ximage.format           = ZPixmap;
    ximage.data             = image->imageData; 
                              //XImage data is a shallow copy of Iplimage data.
    ximage.byte_order       = byte_order;
    ximage.bitmap_unit      = 0;     //24;
    ximage.bitmap_bit_order = byte_order;
    ximage.bitmap_pad       = 8;     //32;
    ximage.depth            = depth; //24;
    ximage.bytes_per_line   = image->widthStep;
    ximage.bits_per_pixel   = 24;
    /*
    ximage.red_mask       = 0xffff;	// bits in z arrangement 
    ximage.green_mask     = 0xff00;
    ximage.blue_mask      = 0x0000;
    */
    Status status = XInitImage(&ximage);
    if (status == 0) { 
      throw IException("Failed to XInitImage");
    }
  }

  void draw(DC& dc, int srcx, int srcy, int destx, int desty,
            int width, int height)
  {
    //printf("%d %d %d %d %d %d\n", srcx, srcy, destx, desty,
    //        width, height);
    if (ximage.data) {
      Display* disp = dc.getDisplay();
      GC       gc   = dc.getGC();
      Drawable drawable = dc.getDrawable();

      XPutImage(disp, drawable, gc,  &ximage, srcx, srcy, destx, desty, 
		width, height);
    }
  }
};

}

