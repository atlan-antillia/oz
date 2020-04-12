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
 *  FreeTypeFont.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/motif/DC.h>
#include <oz++/freetype/FreeTypeLibrary.h>

/*

  typedef struct  FT_FaceRec_
  {
    FT_Long           num_faces;
    FT_Long           face_index;

    FT_Long           face_flags;
    FT_Long           style_flags;

    FT_Long           num_glyphs;

    FT_String*        family_name;
    FT_String*        style_name;

    FT_Int            num_fixed_sizes;
    FT_Bitmap_Size*   available_sizes;

    FT_Int            num_charmaps;
    FT_CharMap*       charmaps;

    FT_Generic        generic;

    FT_BBox           bbox;

    FT_UShort         units_per_EM;
    FT_Short          ascender;
    FT_Short          descender;
    FT_Short          height;

    FT_Short          max_advance_width;
    FT_Short          max_advance_height;

    FT_Short          underline_position;
    FT_Short          underline_thickness;

    FT_GlyphSlot      glyph;
    FT_Size           size;
    FT_CharMap        charmap;

    //@private begin 
    FT_Driver         driver;
    FT_Memory         memory;
    FT_Stream         stream;
    FT_ListRec        sizes_list;

    FT_Generic        autohint;   
    void*             extensions; 
    FT_Face_Internal  internal;
    //private end 
  } FT_FaceRec;
*/

//typedef struct FT_FaceRec_*  FT_Face;

namespace OZ {

class FreeTypeFont :public FreeTypeObject {

private:
  FreeTypeLibrary* library;

  FT_Face face;

public:
  FreeTypeFont( FreeTypeLibrary* library, const char* filePathname, 
                   FT_Long faceIndex = 0)
  :library(library),
  face(NULL)
  {
    if (library == NULL || filePathname == NULL) {
      throw IException("Invalid argument");
    }

    if (FT_New_Face(library->get(),
               filePathname,
               faceIndex,
               &face) ) {
      throw IException("Failed FT_New_Face");
    }
  }

public:
  FreeTypeFont( FreeTypeLibrary*      library,
                const FT_Byte*  file_base,
                FT_Long         file_size,
                FT_Long         face_index )
  :library(library),
  face(NULL)
  { 
    if (library == NULL || file_base == NULL) {
      throw IException("Invalid argument");
    }
    if (FT_New_Memory_Face( library -> get(),
                      file_base,
                      file_size,
                      face_index,
                      &face )) {
      throw IException("Failed to FT_New_Memory_Face");
    }
  }

  /*
 typedef struct  FT_Open_Args_
  {
    FT_UInt         flags;
    const FT_Byte*  memory_base;
    FT_Long         memory_size;
    FT_String*      pathname;
    FT_Stream       stream;
    FT_Module       driver;
    FT_Int          num_params;
    FT_Parameter*   params;
  } FT_Open_Args;
   */

public:
  FreeTypeFont( FreeTypeLibrary* library, 
                const FT_Open_Args*  args,
                FT_Long              faceIndex)
  :library(library),
  face(NULL)
  {
    if (library == NULL || args == NULL) {
      throw IException("Invalid argument");
    }

    if (FT_Open_Face(library->get(),
                args,
                faceIndex,
                &face) )
    {
      throw IException("Failed FT_Open_Face");
    }
  }

  ~FreeTypeFont()
  {
    if (face) {
      if (FT_Done_Face(face)) {
        printf("Failed to FT_Done_Face");
      }
    }
  }

  void attachFile( const char*  filePathname )
  {
    if (FT_Attach_File(face, filePathname )) {
      throw IException("Faield to FT_Attach_File");
    }
  }

  void attachStream( FT_Open_Args*  parameters )
  {
    if (FT_Attach_Stream( face, parameters )) {
      throw IException("Failed to FT_Attach_Stream");
    }
  }

  void referenceFace( )
  {
    if (FT_Reference_Face( face )) {
      throw IException("FT_Reference_Face");
    }
  }

  void setCharSize( FT_F26Dot6  char_width,
                    FT_F26Dot6  char_height,
                    FT_UInt     horz_resolution,
                    FT_UInt     vert_resolution )
  {
    if (FT_Set_Char_Size( face,
                    char_width,
                    char_height,
                    horz_resolution,
                    vert_resolution )) {
      throw IException("FT_Set_Char_Size");
    }
  }

  void setPixelSizes( FT_UInt  pixel_width,
                      FT_UInt  pixel_height )

  {
    if (FT_Set_Pixel_Sizes( face,
                      pixel_width,
                      pixel_height )) {
      throw IException("Failed to FT_Set_Pixel_Sizes");
    }
  }

  void setPixelHeight( FT_UInt  pixel_height )
  {
    setPixelSizes( 0, pixel_height );
  }

  //Resize the scale of the active FT_Size object in a face.
  void requestSize( FT_Size_Request  req )
  {
    if ( FT_Request_Size(face, req )) {
      throw IException("Failed to FT_Request_Size");
    }
  }

  void selectSize( FT_Int strikeIndex )
  {
    if ( FT_Select_Size(face, strikeIndex )) {
      throw IException("Failed to FT_Select_Size");
    }
  }

  void setTransform( FT_Matrix*  matrix, FT_Vector*  delta )
  {
    FT_Set_Transform( face,
                   matrix,
                   delta );
  }

  FT_UInt getCharIndex(const char ch)
  {
    return  FT_Get_Char_Index(face, ch);
  }

  FT_UInt getCharIndex(const wchar_t ch)
  {
    return  FT_Get_Char_Index(face, ch);
  }

  //FT_LOAD_RENDER indicates that the glyph image must be immediately 
  //converted to an anti-aliased bitmap
  void loadGlyph( FT_UInt glyph_index, FT_Int32 load_flags=FT_LOAD_RENDER)
  {
    if (FT_Load_Glyph(face, glyph_index, load_flags )) {
      throw IException("Failed to FT_Load_Glyph");
    }
  }

  void loadGlyph(const char ch, FT_Int32 load_flags )
  { 
    FT_UInt index = getCharIndex(ch);
    if (FT_Load_Glyph(face, index, load_flags )) {
      throw IException("Failed to FT_Load_Glyph");
    }
  }

  void loadGlyph( const wchar_t ch, FT_Int32 load_flags )
  {
    FT_UInt index = getCharIndex(ch);
    if (FT_Load_Glyph(face, index, load_flags )) {
      throw IException("Failed to FT_Load_Glyph");
    }
  }

  void loadMonochromeGlyph( const char ch )
  {
    FT_UInt index = getCharIndex(ch);
    FT_Int32 load_flags=FT_LOAD_RENDER|FT_LOAD_MONOCHROME;
    if (FT_Load_Glyph(face, index, load_flags )) {
      throw IException("Failed to FT_Load_Glyph");
    }
  }

  void loadMonochromeGlyph( const wchar_t ch )
  {
    FT_UInt index = getCharIndex(ch);
    FT_Int32 load_flags=FT_LOAD_RENDER|FT_LOAD_MONOCHROME;
    if (FT_Load_Glyph(face, index, load_flags )) {
      throw IException("Failed to FT_Load_Glyph");
    }
  }

  void getCharIndex( FT_ULong  charcode )
  {
    if (FT_Get_Char_Index( face, charcode )) {
      throw IException("FT_Get_Char_Index");
    }
  }

  FT_ULong getFirstChar(FT_UInt  *agindex )
  {
     return FT_Get_First_Char( face, agindex );
  }

  FT_ULong getNextChar( FT_ULong  char_code, FT_UInt *agindex )
  {
    return FT_Get_Next_Char( face,  char_code, agindex );
  }

  FT_UInt getNameIndex( FT_String*  glyph_name )
  {
    return FT_Get_Name_Index( face, glyph_name );
  }

  FT_Long getFaceFlags()
  {
    return face ->  face_flags;
  }

  FT_Long  getStyleFlags()
  {
    return face -> style_flags;
  }

  FT_Long getNumGlypsh()
  {
    return face -> num_glyphs;

  }

  FT_String*  getFamilyName()
  {
    return face -> family_name;
  }
  
  FT_String*  getStyleName()
  {
     return face -> style_name;
  }

  void loadChar( FT_ULong char_code, FT_Int32 load_flags )
  {
    if (FT_Load_Char( face, char_code, load_flags )) {
      throw IException("Failed to FT_Load_Char");
    }
  }

  void setCharmap( FT_CharMap  charmap )
  {
    if (FT_Set_Charmap( face, charmap )) {
      throw IException("FT_Set_Charmap");
    }
  }

  FT_UShort getFSTypeFlags( )
  {
    return FT_Get_FSType_Flags( face );
  }

  void getKerning( FT_UInt     left_glyph,
                  FT_UInt     right_glyph,
                  FT_UInt     kern_mode,
                  FT_Vector  *akerning )
  {
    if (FT_Get_Kerning(face,
                  left_glyph,
                  right_glyph,
                  kern_mode,
                  akerning )) {
      throw IException("Failed to FT_Get_Kerning");
    }
  }

  void getTrackKerning( FT_Fixed   point_size,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    if (FT_Get_Track_Kerning( face,
                        point_size,
                        degree,
                        akerning )) {
      throw IException("Failed to FT_Get_Tracking");
    }
  }


  void getGlyphName( FT_UInt     glyph_index,
                     FT_Pointer  buffer,
                     FT_UInt     buffer_max )
  {
    if (FT_Get_Glyph_Name( face,
                       glyph_index,
                       buffer,
                       buffer_max )) {
      throw IException("Failed to FT_Get_Glyph_Name");
    }
  }

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

  FT_GlyphSlot getGlyphSlot()
  {
    return face -> glyph;
  }

  FT_Int  getBitmapLeft()
  {
    FT_GlyphSlot slot = getGlyphSlot();
    return slot -> bitmap_left;
  }

  FT_Int  getBitmapTop()
  {
    FT_GlyphSlot slot = getGlyphSlot();
    return slot -> bitmap_top;
  }

  FT_Vector   getAdvance()
  {
    FT_GlyphSlot slot = getGlyphSlot();
    return slot -> advance; 
  }

  FT_Glyph_Metrics getGlyphMetrics()
  {
    FT_GlyphSlot slot = getGlyphSlot();
    return slot -> metrics; 
  }

  FT_SubGlyph getSubGlyph(FT_UInt& num_subglyphs)
  {
    FT_GlyphSlot slot = getGlyphSlot();
    num_subglyphs = slot -> num_subglyphs;
    return slot -> subglyphs; 
  }

  FT_UShort  getUnitsPerEM()
  {
    return face -> units_per_EM;
  }

  FT_Short getAscender()
  {
    return face -> ascender;
  }

  FT_Short getDescender()
  {
    return face -> descender;
  }

  FT_Short getHeight()
  {
    return face -> height;
  }
 
  FT_Short getMaxAdvanceWidth()
  {
    return face -> max_advance_width;
  }
  
  FT_Short getMaxAdvanceHeight()
  {
    return face -> max_advance_height;
  }

  FT_Short  getUnderlinePosition()
  {
    return face -> underline_position;
  }

   
  FT_Short  getUnderlineThickness()
  {
    return face -> underline_thickness;
  }

/*
 typedef struct  FT_Bitmap_
  {
    unsigned int    rows;
    unsigned int    width;
    int             pitch;
    unsigned char*  buffer;
    unsigned short  num_grays;
    unsigned char   pixel_mode;
    unsigned char   palette_mode;
    void*           palette;
  } FT_Bitmap;
 */
  FT_Bitmap  getBitmap()
  {
    FT_GlyphSlot slot = getGlyphSlot();
    return slot -> bitmap; 
  }

  FT_Int  getBitmapRows()
  {
    FT_GlyphSlot slot = getGlyphSlot();
    return slot -> bitmap.rows; 
  }

  FT_Int  getBitmapWidth()
  {
    FT_GlyphSlot slot = getGlyphSlot();
    return slot -> bitmap.width; 
  }

  unsigned char*  getBitmapBuffer()
  {
    FT_GlyphSlot slot = getGlyphSlot();
    return slot -> bitmap.buffer; 
  }

  FT_CharMap* getCharMap(FT_Int& numCharmaps)
  {
    numCharmaps = face -> num_charmaps;
    return face -> charmaps;
  }
};

}

