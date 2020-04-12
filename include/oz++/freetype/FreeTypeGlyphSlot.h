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
 *  FreeTypeGlyphSlot.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/freetype/FreeTypeLibrary.h>

/*
  typedef struct  FT_GlyphSlotRec_
  {
    FT_Library        library;
    FT_Face           face;
    FT_GlyphSlot      next;
    FT_UInt           reserved;
    FT_Generic        generic;

    FT_Glyph_Metrics  metrics;
    FT_Fixed          linearHoriAdvance;
    FT_Fixed          linearVertAdvance;
    FT_Vector         advance;

    FT_Glyph_Format   format;

    FT_Bitmap         bitmap;
    FT_Int            bitmap_left;
    FT_Int            bitmap_top;

    FT_Outline        outline;

    FT_UInt           num_subglyphs;
    FT_SubGlyph       subglyphs;

    void*             control_data;
    long              control_len;

    FT_Pos            lsb_delta;
    FT_Pos            rsb_delta;

    void*             other;
    FT_Slot_Internal  internal;
  } FT_GlyphSlotRec;
*/

//typedef struct FT_GlyphSlotRec_*  FT_GlyphSlot;

namespace OZ {

class FreeTypeGlyphSlot :public FreeTypeObject {
private:
  FT_GlyphSlot slot;

public:
  FreeTypeGlyph(FT_GlyphSlot slot)
  :slot(slot)
  {
    if (slot == NULL) {
      throw IException("Invalid argument");
    }
  }

  ~FreeTypeGlyphSlot()
  {
    slot = NULL;
  }

  FreeTypeGlyph getNext()
  {
    return FreeTypeGlyph(slot ->next);
  }

  operator FT_GlyphSlot() const
  {
    return slot;
  }

  FT_Vector   getAdvance()
  {
    return slot -> advance; 
  }

  FT_Glyph_Format  getFormat()
  {
    return slot ->  format;
  }

  FT_Glyph_Metrics getGlyphMetrics()
  {
    return slot -> metrics; 
  }

  FT_SubGlyph getSubGlyph(FT_UInit& num_subglyphs)
  {
    num_subglyphs = slot -> num_subglyphs;
    return slot -> subglyphs; 
  }

  FT_Bitmap getBitmap()
  {

    return slot -> bitmap;
  }

  FT_int getBitmapLeft()
  {
    return slot ->  bitmap_left;
  }

  FT_int getBitmapTop()
  {
    return slot -> bitmap_top;
  }

  FT_Outline  getOutline()
  {
    return slot -> outline;
  }
};

}

