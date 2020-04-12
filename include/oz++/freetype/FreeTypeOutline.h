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
 *  FreeTypeOutline.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/motif/DC.h>
#include <oz++/freetype/FreeTypeLibrary.h>

/*
  typedef struct  FT_Outline_
  {
    short       n_contours;      // number of contours in glyph 
    short       n_points;        // number of points in the glyph

    FT_Vector*  points;          // the outline's points        
    char*       tags;            // the points flags           
    short*      contours;        // the contour end points    
    int         flags;           // outline masks            

  } FT_Outline;
*/

//typedef struct FT_FaceRec_*  FT_Face;

namespace OZ {

class FreeTypeOutline :public FreeTypeObject {

private:
  FreeTypeLibrary* library;

  FT_Outline outline

public:
  FreeTypeOutline( FreeTypeLibrary* library, 
                  FT_UInt      numPoints,
                  FT_Int       numContours)

  :library(library),
  outline(NULL)
  {
    if (library == NULL) {
      throw IException("Invalid argument");
    }

    if (FT_Outline_New(library->get(),
               numPoints,
               numContours,
               &outline) ) {
      throw IException("Failed FT_Outline_New");
    }
  }


  ~FreeTypeFont()
  {
    if (outline) {
      if (FT_Done_Outline(&outline)) {
        printf("Failed to FT_Done_Outline");
      }
    }
  }

  void copy(FT_Outline* target)
  {
    if (FT_Outline_Copy(&outline, target )) {
       throw IException("Faild to FT_Outline_Copy\n");
    }
  }

  void translate( FT_Pos xOffset,
                  FT_Pos yOffset )
  {
    if( FT_Outline_Translate( &outline,
                              xOffset,
                              yOffset ) ) {
      throw IException("FT_Outline_Translate failed");
    }
  }

  void transform( const FT_Matrix*   matrix )
  {
    if (FT_Outline_Transform( &outline, matrix ) ) {
      throw IException("Failed to FT_Outline_Transform");
    }
  }

  void embolden( FT_Pos       strength )
  {
    if (FT_Outline_Embolden( &outline, strength ) ) {
      throw IException("Failed to FT_Outline_Embolden");
    }
  }

  void emboldenXY( FT_Pos       xstrength,
                   FT_Pos       ystrength )
  {
    if (FT_Outline_EmboldenXY( &outline, xstrength, ystrength )) {
      throw IException("Failed to FT_Outline_EmboldexXY");
    }
  }

  void  FT_Outline_Reverse( )
  {
    FT_Outline_Reverse( &outline );
  }

  void check( )
  {
    if (FT_Outline_Check( &outline )) {
      throw IException("Failed to FT_Outline_Check");
    }
  }


  void   getCBox( FT_BBox *acbox )
  {
    FT_Outline_Get_CBox( &outline, acbox );
  }



  void getBBox( FT_BBox  *abbox )
  {
    if ( FT_Outline_Get_BBox( &outline, abbox )) {
      throw IException("Failed to FT_Outline_BBox");
    }
  }


  void getBitmap( const FT_Bitmap  *abitmap )
  {
    if ( FT_Outline_Get_Bitmap( library, &outline, abitmap )) {
      throw IException("Failed to FT_Outline_Get_Bitmap");
    }
  }

  void render( FT_Raster_Params*  params )
  {
    if (FT_Outline_Render( library, &outline, params )) {
      throw IException("Failed to FT_Outline_Render");
    }
  }

/*
typedef struct  FT_Outline_Funcs_
  {
    FT_Outline_MoveToFunc   move_to;
    FT_Outline_LineToFunc   line_to;
    FT_Outline_ConicToFunc  conic_to;
    FT_Outline_CubicToFunc  cubic_to;

    int                     shift;
    FT_Pos                  delta;

  } FT_Outline_Funcs;
 */
  void decompose( const FT_Outline_Funcs*  func_interface,
                        void*                    user )
  {
    if (FT_Outline_Decompose(&outline, func_interface, user )) {
      throw IException("Failed to FT_Outline_Decompose");
    }
  }

/*
typedef enum  FT_Orientation_
  {
    FT_ORIENTATION_TRUETYPE   = 0,
    FT_ORIENTATION_POSTSCRIPT = 1,
    FT_ORIENTATION_FILL_RIGHT = FT_ORIENTATION_TRUETYPE,
    FT_ORIENTATION_FILL_LEFT  = FT_ORIENTATION_POSTSCRIPT,
    FT_ORIENTATION_NONE

  } FT_Orientation;
 */
  Orientation FT_Outline_Get_Orientation( )
  {
    return FT_Outline_Get_Orientation( &outline );
  }
};

}

