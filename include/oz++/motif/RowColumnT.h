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
 *  RowColumnT.h 
 * 
 *****************************************************************************/ 
 
//2017/10/10 Mini tablelike layouter. 
 
#pragma once 
 
#include <oz++/motif/BulletinBoard.h> 
#include <oz++/motif/CompoundString.h> 
#include <oz++/motif/Label.h> 
#include <oz++/motif/PushButton.h> 
#include <oz++/motif/TextField.h> 
#include <oz++/motif/StrDef.h>
 
namespace OZ { 
 
template<class T, int ROW, int COLUMN> class RowColumnT :public BulletinBoard { 
private: 
  Label   label; 
  T       kernel[ROW][COLUMN]; 
  int     width; 
  int     height; 
  int     margin; 
   
public: 
  RowColumnT(View* parent, const char* name, Args& args) 
  :BulletinBoard(parent, name, 
        args.setIfNotSet(XmNwidth, 300) 
        .setIfNotSet(XmNheight, 300) ) 
  { 
    int topposition = 0; 
    CompoundString cs; 
    XmString xms = (XmString)args.get(XmNlabelString); 
    if (xms) { 
      cs.setCopy(xms); 
    } else { 
      if (name) { 
        cs.set(name); 
      } 
    } 
    { 
      Args ar; 
      ar.set(XmNx, 0); 
      ar.set(XmNy, 0); 
      ar.set(XmNlabelString, cs); 
      //ar.set(XmNheight, 28); 
      label.create(this, "", ar); 
      int h = label.height(); 
      topposition = h; 
    }
     
    width = 80; 
    if (args.has(XmNitemWidth)) { 
      width = (int)args.get(XmNitemWidth); 
      if (width < 20) { 
        width = 20; 
      } 
    } 
 
    height = 30; 
    if (args.get(XmNitemHeight)) { 
      height = (int)args.get(XmNitemHeight); 
      if (height < 20) { 
        height = 20; 
      } 
    } 
    margin = topposition + 2 + 10 ; 
 
    Args ar; 
    for (int i = 0; i < ROW; i++) { 
      for (int j = 0; j < COLUMN; j++) { 
        ar.reset(); 
        char value[128]; 
        sprintf(value, "%.4f", (float)0); 
        ar.set(XmNx, margin + width*i); 
        ar.set(XmNy, margin + height*j); 
        ar.set(XmNwidth, width - 2); 
        ar.set(XmNheight, height - 2); 
        //class T must have create method. 
        kernel[i][j].create(this, value, ar); 
      } 
    } 
  } 
   
  bool verifyIndex(int i, int j) 
  { 
    if (i>= 0 && i<ROW && j>= 0 && j<COLUMN) { 
      return true; 
    } else { 
      throw IException("Invalid parameter (%d, %d)", i, j); 
    } 
  } 
   
  void set(int i, int j, float val) 
  { 
    verifyIndex(i, j); 
     
    char value[128]; 
    sprintf(value, "%.4f", (float)val); 
    CompoundString cs(value);
    //Class T must have set method. 
    kernel[i][j].set(XmNlabelString, cs); 
  } 
   
  void set(int i, int j, int val) 
  { 
    verifyIndex(i, j); 
     
    char value[128]; 
    sprintf(value, "%d", val); 
    CompoundString cs(value);
    //Class T must have set method. 
    kernel[i][j].set(XmNlabelString, cs); 
  } 
 
  void set(int i, int j, const char* value) 
  { 
    verifyIndex(i, j); 
    //Class T must have set method. 
    CompoundString cs(value);
    kernel[i][j].set(XmNlabelString, cs); 
  } 
 
  T getItem(int i, int j) 
  { 
    verifyIndex(i, j); 
    return kernel[i][j]; 
  } 
   
  void disalbe() 
  { 
    for (int i = 0; i < ROW; i++) { 
      for (int j = 0; j < COLUMN; j++) { 
        kernel[i][j].disable(); 
      } 
    }    
  } 
  void enalbe() 
  { 
    for (int i = 0; i < ROW; i++) { 
      for (int j = 0; j < COLUMN; j++) { 
        kernel[i][j].enable(); 
      } 
    }    
  } 
}; 
} 
