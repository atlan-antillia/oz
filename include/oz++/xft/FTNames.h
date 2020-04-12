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
 *      FTNames.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/CommonObject.h>
#include <oz++/Pair.h>

#include <X11/Xft/Xft.h>
#include <fontconfig/fontconfig.h>

namespace OZ {

//typedef 
enum XFCTYPE{
  XFCTYPE_VOID,
  XFCTYPE_STRING,
  XFCTYPE_INTEGER,
  XFCTYPE_DOUBLE,
  XFCTYPE_BOOL,
  XFCTYPE_FT_FACE,
  XFCTYPE_CHARSET,
  XFCTYPE_MATRIX,
	
  XFCTYPE_NONE,
};// XFCTYPE;


const char* XftNlabelString    = "xftLabelstring";
const char* XftNforeground     = "xftForeground";
const char* XftNfont           = "xftFont";
	
const char* XftNamePrefix      = "xft_";
	
const char* XftNfamily         = "xft_family"; 
const char* XftNstyle          = "xft_style";
const char* XftNslant          = "xft_slant";
const char* XftNweight         = "xft_weight";
const char* XftNsize           = "xft_size";
const char* XftNaspect         = "xft_aspect";
const char* XftNpixelSize      = "xft_pixelsize";
const char* XftNspacing        = "xft_spacing";
const char* XftNfoundry        = "xft_foundry";
const char* XftNantialias      = "xft_antialias";
const char* XftNhinting        = "xft_hinting";
const char* XftNhintstyle      = "xft_hintstyle";
const char* XftNverticallayout = "xft_verticallayout";
const char* XftNautohint       = "xft_autohint";
const char* XftNglobaladvance  = "xft_globaladvance";
const char* XftNwidth          = "xft_width";
const char* XftNfile           = "xft_file";
const char* XftNindex          = "xft_index";
const char* XftNftface         = "xft_ftface";
const char* XftNrasterizer     = "xft_rasterizer";
const char* XftNoutline        = "xft_outline";
const char* XftNscalable       = "xft_scalable";
const char* XftNscale          = "xft_scale";
const char* XftNdpi            = "xft_dpi";
const char* XftNrgba           = "xft_rgba";
const char* XftNminspace       = "xft_minspace";
//const char* XftNsource         = "xft_source";
const char* XftNcharset        = "xft_charset";
const char* XftNlang           = "xft_lang";
const char* XftNfontversion    = "xft_fontversion";
const char* XftNfullname       = "xft_fullname";
const char* XftNfamilylang     = "xft_familylang";
const char* XftNstylelang      = "xft_stylelang";
const char* XftNfullnamelang   = "xft_fullnamelang";
const char* XftNcapability     = "xft_capability";
const char* XftNfontformat     = "xft_fontformat";
const char* XftNembolden       = "xft_embolden";
const char* XftNembeddedbitmap = "xft_embeddedbitmap";
const char* XftNdecorative     = "xft_decorative";
const char* XftNlcdfilter      = "xft_lcdfilter";
const char* XftNcharwidth      = "xft_charwidth";
const char* XftNcharheight     = "xft_charheight";
const char* XftNmatrix         = "xft_matrix";

//
class FTNames {
private:
  //Inner class
  class NameTypePairs {
  private:
    Pair<const char*, XFCTYPE>* pairs;
    size_t    size;
 
  public:
    NameTypePairs()
    :pairs(NULL),
    size(0)
    {
    }
    
    void put(Pair<const char*, XFCTYPE>* pairs, size_t size)
    {
  	  if (pairs == NULL || size <=0) {
  		throw IException("Invalid argument");
  	  }
  	  this -> pairs = pairs;
      this -> size  = size;

    }
	
    XFCTYPE getType(const char* name) 
    {
  	  XFCTYPE type = XFCTYPE_NONE;
  	  if (pairs !=NULL  && size > 0) {
  	    for (size_t i = 0; i<size; i++) {
  		  if (strcmp(name, pairs[i].first) == 0) {
  			type = pairs[i].second;
  			break;
  		  }
  	    }
  	  }
  	  return type;
    }
  };

private:
  NameTypePairs nameTypePairs;
  
//Constructor
public:
  FTNames()
  {
    static  Pair<const char*, enum XFCTYPE> fcNameTypes[] ={
	{ XftNfamily,          XFCTYPE_STRING},		// "family"		 	String 
	{ XftNstyle,           XFCTYPE_STRING},		// "style"			String 
	{ XftNslant,	       XFCTYPE_INTEGER}, 	// "slant"		 	Int 
	{ XftNweight,          XFCTYPE_INTEGER},	// "weight"		 	Int 
	{ XftNsize,            XFCTYPE_DOUBLE},		// "size"		 	Double 
	{ XftNaspect,	       XFCTYPE_DOUBLE},    	// "aspect"		 	Double 
	{ XftNpixelSize,       XFCTYPE_DOUBLE},	  	// "pixelsize"		Double 
	{ XftNspacing,         XFCTYPE_INTEGER},	// "spacing"		Int 
	{ XftNfoundry,         XFCTYPE_STRING}, 	// "foundry"		String 
	{ XftNantialias,       XFCTYPE_BOOL},	    // "antialias"		Bool (depends) 
	{ XftNhinting,         XFCTYPE_BOOL},   	// "hinting"		Bool (true) 
	{ XftNhintstyle,       XFCTYPE_INTEGER},	// "hintstyle"		Int 
	{ XftNverticallayout,  XFCTYPE_BOOL}, 		// "verticallayout"	Bool (false) 
	{ XftNautohint,        XFCTYPE_BOOL},	    // "autohint"		Bool (false) 
	{ XftNglobaladvance,   XFCTYPE_BOOL},		// "globaladvance"	Bool (true) 
	{ XftNwidth,           XFCTYPE_INTEGER}, 	// "width"		 	Int 
	{ XftNfile,            XFCTYPE_STRING},  	//	"file"		 	String 
	{ XftNindex,           XFCTYPE_INTEGER},	// "index"			Int 
	{ XftNftface,          XFCTYPE_FT_FACE},  	// "ftface"		 	FT_Face 
	{ XftNrasterizer,      XFCTYPE_STRING},	    // "rasterizer"	 	String 
	{ XftNoutline,         XFCTYPE_BOOL}, 		// "outline"		Bool 
	{ XftNscalable,        XFCTYPE_BOOL},	 	// "scalable"		Bool 
	{ XftNscale,           XFCTYPE_DOUBLE}, 	// "scale"		 	double 
	{ XftNdpi,             XFCTYPE_DOUBLE},  	// "dpi"		 	double 
	{ XftNrgba,            XFCTYPE_INTEGER},	// "rgba"		 	Int 
	{ XftNminspace,        XFCTYPE_BOOL},		// "minspace"		Bool use minimum line spacing 
//	{ XftNsource,          XFCTYPE_STRING}, 	// "source"		 	String (deprecated) 
	{ XftNcharset,         XFCTYPE_CHARSET}, 	// "charset"		CharSet 
	{ XftNlang,            XFCTYPE_STRING},		// "lang"		 	String RFC 3066 langs 
	{ XftNfontversion,	   XFCTYPE_INTEGER},  	// "fontversion"	Int from 'head' table 
	{ XftNfullname,        XFCTYPE_STRING},  	// "fullname"		String 
	{ XftNfamilylang,      XFCTYPE_STRING},	 	// "familylang"	 	String RFC 3066 langs 
	{ XftNstylelang,       XFCTYPE_STRING}, 	// "stylelang"		String RFC 3066 langs 
	{ XftNfullnamelang,    XFCTYPE_STRING}, 	// "fullnamelang"	String RFC 3066 langs 
	{ XftNcapability,      XFCTYPE_STRING},  	// "capability"	 	String 
	{ XftNfontformat,      XFCTYPE_STRING},		// "fontformat"	 	String 
	{ XftNembolden,        XFCTYPE_BOOL},  		// "embolden"		Bool - true if emboldening needed
	{ XftNembeddedbitmap,  XFCTYPE_BOOL},  		// "embeddedbitmap"	Bool - true to enable embedded bitmaps 
	{ XftNdecorative,      XFCTYPE_BOOL}, 		// "decorative"	 	Bool - true if style is a decorative variant 
	{ XftNlcdfilter,       XFCTYPE_INTEGER},	// "lcdfilter"		Int 

	{ XftNcharwidth,       XFCTYPE_INTEGER},	// "charwidth"	 	Int 
	{ XftNcharheight,      XFCTYPE_INTEGER},	// "charheight" 	Int 
	{ XftNmatrix,          XFCTYPE_MATRIX},     // "matrix"     	FcMatrix 
	};

    nameTypePairs.put(fcNameTypes, SizeOf(fcNameTypes));
    
  }
public:
  XFCTYPE getType(const char*  fcName)
  {
    return nameTypePairs.getType(fcName);
  } 	
};

}

