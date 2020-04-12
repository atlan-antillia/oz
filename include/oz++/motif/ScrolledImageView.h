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
 *  ScrolledImageView.h
 *
 *****************************************************************************/

//2017/05/10
//2017/10/16 Modified to use create method.

#pragma once
//
#include <oz++/motif/CustomScrolledView.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/DC.h>
#include <oz++/RECT.h>

namespace OZ {

class ScrolledImageView :public CustomScrolledView {
private:
  Pixmap pixmap;

  //Pixmap geometry
  uint    width;  //Pixmap Width
  uint    height; //Pixmap Height
  uint    borderw;
  uint    depth;

  void  getImageGeometry(Pixmap pixmap, uint& width, uint& height, 
          uint& borderw, uint& depth) 
  {
     Window root;
     int    x, y;
     if (pixmap != None) {
       XGetGeometry(getDisplay(), pixmap, &root,
                                &x, &y, &width, &height, 
                                &borderw, &depth);
    }
  }
 
public:
  ScrolledImageView(View* parent, const char* name, Args& args)
  :CustomScrolledView(parent, name, args),
  pixmap(None),
  width(0),
  height(0),
  borderw(0),
  depth(0)
  {
  }

  ScrolledImageView()
  :CustomScrolledView(),
  pixmap(None),
  width(0),
  height(0),
  borderw(0),
  depth(0)
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    CustomScrolledView::create(parent, name, args);
  }

  ~ScrolledImageView()
  {
    clear();
  }

  void clear()
  {
    if (pixmap != None) {
      XFreePixmap(getDisplay(), pixmap);
      pixmap = None;
    }
  }


  void  getImageGeometry(uint& w, uint& h, 
          uint& bw, uint& d) 
  {
    w  = width;
    h  = height;
    bw = borderw;
    d  = depth;  
  }

  void showImageGeometry()
  {
    printf("width=%d  height=%d  borderw=%d  depth=%d\n",
         width, height, borderw, depth);
  }
  //virtual function
  virtual void createPixmap() 
  {
  }

  void setPixmap(Pixmap pixmap, bool keepScrollBarPosition=false)
  {
    clear();
    this->pixmap = pixmap;
    getGeometry(pixmap, width, height, borderw, depth);
    //getImageGeometry(pixmap, width, height, borderw, depth);
    //The values returned by width() and height() don't return
    // real window size until this view is mapped.
    BulletinBoard* bb = getWorkArea();
    int w = bb->width();  
    int h = bb->height();
    dprintf("setPixmap pixmap(w=%d, h=%d), view(w=%d, h=%d)\n",
      width, height, w, h);
    setScrollBarMaximum((Dimension)width, (Dimension)height);
    if (keepScrollBarPosition == false) {
      setScrollBarPosition(0, 0);
    }

    //printf("bboard w =%d image width=%d\n", w, width);
    if (w > width)  {
       w = width;
    }
    setHorizScrollBarSliderSize(w);

    if (h > height) {
       h = height;
    }
    setVertScrollBarSliderSize(h);
    

    if (keepScrollBarPosition == false) {
      setScrollBarPosition(0, 0);     
    }
    sendExposeEvent();
  }

  Pixmap getPixmap()
  {
    return pixmap;
  }

  void  getGeometry(Pixmap pixmap, uint& width, uint& height, 
          uint& borderw, uint& depth) 
  {
    Window root;
    int    x, y;
    XGetGeometry(getDisplay(), pixmap, &root,
                                &x, &y, &width, &height, 
    &borderw, &depth);
  }
 
  virtual void display()
  {
    Position hpos = 0;
    Position vpos = 0;
    getScrollBarPosition(hpos, vpos);

    Pixmap pixmap = getPixmap();

    if (pixmap == None) {
      createPixmap();
    }
    pixmap = getPixmap();

    if (pixmap != None) {
      //1 Copy the source pixmap to the destination workArea of this scrolled window. 
      BulletinBoard* dest = getWorkArea();

      //2 Get the size of the workArea;
      uint w = dest->width();

      uint h = dest->height(); 

      //3 Get a destination device context of the destinatin bulletinBoard.
      DC destDC(dest);

      //4 Copy the source pixmap to the destination bulletin board through destDC.
      destDC.copyArea(pixmap, 0, 0, width, height, -hpos, -vpos); 

      //5 You have to clear a garbage area in the following way.
      if (w > width) {
        RECT rr= {width, 0, w, height};
        if (h>height) {
          rr.bottom = h;
        }
        destDC.fillRectangle(rr.left, rr.top, rr.right-rr.left, rr.bottom - rr.top);
      }
      if (h > height) {
        RECT rr = {0, height, width, h};
        if (w>width) {
          rr.right = w;
        }
        destDC.fillRectangle(rr.left, rr.top, rr.right-rr.left, rr.bottom - rr.top);
      }
    } else {
      //pixmap is None
    }
  }
};
}


