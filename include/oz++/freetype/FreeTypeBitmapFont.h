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
 *  FreeTypeBitmapFont.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/motif/DC.h>
#include <oz++/freetype/FreeTypeFont.h>

namespace OZ {

class FreeTypeBitmapFont :public FreeTypeFont {
private:
  static const int BITS = 8;

public:
  FreeTypeBitmapFont(FreeTypeLibrary* library, const char* filePathname, 
                   FT_Long faceIndex = 0)
  :FreeTypeFont(library, filePathname, faceIndex )
  {
  }

  void drawString(DC* dc, int x, int y, const char* string, Pixel fg =0)
  {
    if (string) {
      for (int i = 0; i<strlen(string); i++) {
        const char c = string[i];
        loadGlyph(c, FT_LOAD_RENDER);
        FT_Bitmap bm = getBitmap();
        drawBitmap(dc, x, y, bm, fg);
        FT_Vector advance = getAdvance();             
        int xp = advance.x >>6;

        x += (xp*BITS);
      }
    } 
  }

  //This is a sample string drawing method with an FT_Bitmap  
  //on OZ::DC (X11 Graphics context).
  virtual void drawString(DC* dc, int x, int y, const wchar_t* string, Pixel fg =0)
  {
    if (string) {
      for (int i = 0; i< wcslen(string); i++) {
        const wchar_t c = string[i];
        loadGlyph(c, FT_LOAD_RENDER);
        FT_Bitmap bm = getBitmap();
        drawBitmap(dc, x, y, bm, fg);
        FT_Vector advance = getAdvance();
        int xp = advance.x >>6;

        x += (xp*BITS);
      }
    }
  }

  //This is a sample string drawing method with FT_Bitmap
  //on OZ::DC (X11 Graphics context).
  virtual void drawBitmap(DC* dc, int x, int y, FT_Bitmap bm,  Pixel fg)
  {
    dc -> setForeground(fg);
    for (int row = 0; row < bm.rows; row++) {
      for (int i = 0; i< BITS; i++) {
        int px = x + getBitmapLeft();
        for (int col = 0; col < bm.pitch; col++) {
          int n = bm.pitch * row + col;
          unsigned char c = bm.buffer[n];
          for (int bit = BITS-1; bit >= 0; bit--) {
            if (((c >> bit) & 1) == 0) {
              px++;
            } else {
              dc -> drawPoint(px++, y - getBitmapTop()*BITS);
            }
          }
        }
        y += 1;
      }
    }
  }

  void dumpBitmap(FT_Bitmap bm)
  {
    for (int row = 0; row < bm.rows; row++) {
      for (int i = 0; i<8; i++) {
        for (int col = 0; col < bm.pitch; col++) {
          int n = bm.pitch * row + col;
          unsigned char c = bm.buffer[n];
          for (int bit = 7; bit >= 0; bit--) {
            if (((c >> bit) & 1) == 0) {
              printf(" ");
            } else {
              printf("X"); 
            }
          }
        }
        printf("\n");
      }
    }
  }


};

}

