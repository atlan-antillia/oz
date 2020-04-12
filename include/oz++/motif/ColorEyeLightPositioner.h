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
 *  ColorEyeLightPositioner.h
 *
 *****************************************************************************/


#include <oz++/motif/BulletinBoard.h>
#include <oz++/motif/ColorPositioner.h>
#include <oz++/motif/EyePositioner.h>
#include <oz++/motif/LightPositioner.h>
#include <oz++/motif/Color.h>

namespace OZ {

const char* XmNeyePositionerMinimum = "eyePositionerMinimum";
const char* XmNeyePositionerMaximum = "eyePositionerMaximum";

const char* XmNlightPositionerMinimum = "lightPositionerMinimum";
const char* XmNlightPositionerMaximum = "lightPositionerMaximum";

class ColorEyeLightPositioner: public BulletinBoard {

private:
  SmartPtr<ColorPositioner> colorPositioner;
  SmartPtr<EyePositioner>   eyePositioner;
  SmartPtr<LightPositioner> lightPositioner;

public:
  ColorEyeLightPositioner(View* parent, const char* name, Args& args)
  :BulletinBoard(parent, name, args)
  {
    Args ar;
    ar.reset();
    ar.set(XmNx, 2);
    ar.set(XmNy, 2);
    colorPositioner = new ColorPositioner(this, "", ar);

    int eMin = -20;
    if (args.has(XmNeyePositionerMinimum)) {
      eMin = (int)args.get(XmNeyePositionerMinimum);
    }

    int eMax = 20;
    if (args.has(XmNeyePositionerMaximum)) {
      eMax = (int)args.get(XmNeyePositionerMaximum);
    }
    ar.reset();
    ar.set(XmNx, 2);
    ar.set(XmNy, 2+ 110);
    ar.set(XmNminimum, eMin);
    ar.set(XmNmaximum, eMax);
    eyePositioner = new EyePositioner(this, "", ar);
    
    int lMin = -20;
    if (args.has(XmNlightPositionerMinimum)) {
      lMin = (int)args.get(XmNlightPositionerMinimum);
    }

    int lMax = 20;
    if (args.has(XmNlightPositionerMaximum)) {
      lMax = (int)args.get(XmNlightPositionerMaximum);
    }

    ar.reset();
    ar.set(XmNx, 2);
    ar.set(XmNy, 2+ 110*2);
    ar.set(XmNminimum, lMin);
    ar.set(XmNmaximum, lMax);
    lightPositioner = new LightPositioner(this, "", ar);
  }
  
  ~ColorEyeLightPositioner() 
  {
  }

  void setExposeEventListener(View* view)
  {
    colorPositioner->setExposeEventListener(view);
    eyePositioner  ->setExposeEventListener(view);
    lightPositioner->setExposeEventListener(view);
  }

  void getColorPosition(int& r, int& g, int& b)
  {
     colorPositioner->getPosition(r, g, b);
  }

  void getColorPosition(float& r, float& g, float& b)
  {
     int x, y, z;
     colorPositioner->getPosition(x, y, z);
     r = (float)x/(float)255.0f;
     g = (float)y/(float)255.0f;
     b = (float)z/(float)255.0f;

  }

  void getEyePosition(int& x, int& y, int& z)
  {
     eyePositioner->getPosition(x, y, z);
  }

  void getEyePosition(float& x, float& y, float& z)
  {
     int ix, iy, iz;
     eyePositioner->getPosition(ix, iy, iz);
     x = (float)ix;
     y = (float)iy;
     z = (float)iz;
  }

  void getLightPosition(int& x, int& y, int& z)
  {
     lightPositioner->getPosition(x, y, z);
  }

  void getLightPosition(float& x, float& y, float& z)
  {
     int ix, iy, iz;
     lightPositioner->getPosition(ix, iy, iz);
     x = (float)ix;
     y = (float)iy;
     z = (float)iz;
  }

  void setColorPosition(int r, int g, int b)
  {
     colorPositioner->setPosition(r, g, b);
  }

  void setEyePosition(int x, int y, int z)
  {
     eyePositioner->setPosition(x, y, z);
  }

  void setLightPosition(int x, int y, int z)
  {
     lightPositioner->setPosition(x, y, z);
  }

  ColorPositioner* getColorPositioner()
  {
    return colorPositioner;
  }
 
  EyePositioner*   getEyePositioner()
  {
    return eyePositioner;
  }
  LightPositioner*   getLightPositioner()
  {
    return lightPositioner;
  }

  int getPositionerWidth()
  {
    return colorPositioner->getPositionerWidth();
  }
};

}


