/******************************************************************************
 *
 * Copyright (c) 2015   TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  FTFontPreview.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/Pair.h>

#include <oz++/motif/PanedWindow.h>
#include <oz++/motif/LabeledFrame.h>

#include <oz++/motif/RowColumn.h>
#include <oz++/motif/ComboBox.h>
#include <oz++/motif/FontListView.h>
#include <oz++/motif/ToggleButton.h>
#include <fontconfig/fontconfig.h>
#include <oz++/xft/FTFont.h>
#include <oz++/xft/FTLabel.h>
#include <oz++/motif/XtSmartPtr.h>

namespace OZ {

class FTFontPreview :public PanedWindow {

private:
  CharString     fontName;
  int            fontSize;
  int            fontWeight;

  PanedWindow*      panedw;

  LabeledFrame*     fontNameListFrame;
  FontListView*     fontNameList; 

  RowColumn*        rowColumn;
  LabeledFrame*     sizeComboBoxFrame;
  LabeledFrame*     weightComboBoxFrame;
  ComboBox*         sizeComboBox;
  ComboBox*         weightComboBox;
  
  LabeledFrame*     preview;
  RowColumn*        previewRowColumn;
  ToggleButton*     slant;
  int               slantOnOff;
  FTFont*           font;
  FTLabel*          label;
  FTLabel*          alphabet;

private:
 void   sizeChanged(Action& action)
  {
    XmComboBoxCallbackStruct* cbs =
        (XmComboBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs -> item_or_text);
    XtSmartPtr<char> item = cs.unparse();
    printf("sizeChanged: item: %s\n", (const char*)item);
    fontSize = atoi(item);
    updateLabel();
  }

  void  nameChanged(Action& action)
  {
    XmListCallbackStruct* cbs =
       (XmListCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->item);
   
    char* item = cs.unparse();
    // The above item may contain two names separated by ',' delimiter.
    char* cm   = strrchr(item, ',');
    if (cm) {
       *cm = '\0';
    } 
    printf("nameChanged: item [%s]\n", item);
    fontName = item;
    updateLabel();
    XtFree(item);
  }

  void weightChanged(Action& action)
  {
    XmComboBoxCallbackStruct* cbs =
         (XmComboBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs -> item_or_text);
    XtSmartPtr<char> item = cs.unparse();
    printf("waitChanged: item=%s\n", (const char*)item);
     
    fontWeight = getWeight(item);
    printf("fontWeight: %d\n", fontWeight);
    updateLabel();
  }

  int getWeight(const char* name)
  {
    Pair<int, const char*> fontWeight[] = 
      {
      { FC_WEIGHT_THIN,   "THIN"},
      { FC_WEIGHT_LIGHT,  "LIGHT"},
      { FC_WEIGHT_BOOK,   "BOOK"},
      { FC_WEIGHT_REGULAR,"REGULAR"},
      { FC_WEIGHT_MEDIUM, "MEDIUM"},
      { FC_WEIGHT_BOLD,   "BOLD"},
      { FC_WEIGHT_BLACK,  "BLACK"},
      };
    int weight = FC_WEIGHT_REGULAR;
    for (int i = 0; i<SizeOf(fontWeight); i++) {
      if (strcmp(fontWeight[i].second, name) == 0) {
        weight = fontWeight[i].first;
        break;
      }
    }
    return weight;
  };

  void slantChanged(Action& action)
  {
  	XmToggleButtonCallbackStruct* cbs = 
  		(XmToggleButtonCallbackStruct*) action.getCallData();

  	printf("slan on/off %d\n", cbs->set);
  	slantOnOff = cbs -> set;
  	updateLabel();
  }
	
  void updateLabel()
  {
    if (font) {
      delete font;
    }
    if (label) {
      delete label;
    }
    Args ar;
    ar.reset();
    ar.set(XftNfamily, (const char*)fontName);
    ar.set(XftNweight, fontWeight);
    ar.set(XftNsize,   fontSize);
  	ar.set(XftNslant,  slantOnOff);
    font = new FTFont(getDisplay(), ar);

    char string[256];
    sprintf(string, "ABCDEFGHIJ...xyz 1234567890 %s", (const char*)fontName);
    ar.reset();
    ar.set(XftNfont, (XtArgVal)font);
    ar.set(XftNlabelString, string);
  	ar.set(XmNheight,  60);
    label = new FTLabel(previewRowColumn, "", ar);
  	
    Color color(this);
    label -> set(XmNbackground, color.whitePixel());
  }

public:
  FTFontPreview(View* parent, const char* name, Args& args)
  :PanedWindow(parent, name, args.set(XmNorientation, XmVERTICAL)
                                 .set(XmNsashWidth, 1)
                                 .set(XmNsashHeight, 1) ),
    panedw(NULL),
    fontNameListFrame(NULL),
    fontNameList(NULL),
    rowColumn(NULL),
    sizeComboBoxFrame(NULL),
    weightComboBoxFrame(NULL),
    sizeComboBox(NULL),
    weightComboBox(NULL),
	preview(NULL),
	previewRowColumn(NULL),
	slant(NULL),
	slantOnOff(0),
	font(NULL),
    label(NULL)
  {
    const char* family = "Arial";
    Args ar;

    ar.set(XmNorientation, XmHORIZONTAL);
    panedw = new PanedWindow(this, "paned", ar);

    ar.reset();
    fontNameListFrame =new LabeledFrame(panedw, "Family:", ar);

    ar.reset();
    ar.set(XmNvisibleItemCount,  20);
    ar.set(XmNwidth, 200);
    ar.set(XmNheight, 300);
    fontNameList = new FontListView(fontNameListFrame, "list", ar);
    fontNameList->listup(FontListView::NAME_ONLY);
    fontNameList -> getList()
                 -> addCallback(XmNbrowseSelectionCallback, this,
                     (Callback)&FTFontPreview::nameChanged, NULL);

    CompoundString cs(family);
    fontNameList -> getList()
                  ->selectItem(cs.get(), False);
    fontName = family;

    static const char* items[] = {"8", "10", "12", 
         "14", "16", "18", "20", "22", "24", "26", "28", "30", "32"};
    int defaultPosition = 2;
    fontSize = atoi(items[defaultPosition]);

    ar.reset();
    sizeComboBoxFrame = new LabeledFrame(panedw, "Size:", ar);

    CompoundStringList csl(items, XtNumber(items));
    ar.reset();
    ar.set(XmNwidth, 100);
    ar.set(XmNitems, csl);
    ar.set(XmNvisibleItemCount, 5);
    ar.set(XmNitemCount, XtNumber(items));
    ar.set(XmNselectedPosition, defaultPosition);
    sizeComboBox = new ComboBox(sizeComboBoxFrame, "", ar);
    sizeComboBox -> addCallback(XmNselectionCallback, this,
                         (Callback)&FTFontPreview::sizeChanged, NULL);
    
    ar.reset();
    weightComboBoxFrame = new LabeledFrame(panedw, "Weight:", ar);

    int defaultPos = 3;
    static const char* weight[] = { "THIN", "LIGHT", "BOOK", "REGULAR",
         "MEDIUM", "BOLD", "BLACK"};
    fontWeight = FC_WEIGHT_REGULAR;

    CompoundStringList wcsl(weight, XtNumber(weight));
    ar.reset();

    ar.set(XmNwidth, 100);
    ar.set(XmNitems, wcsl);
    ar.set(XmNvisibleItemCount, 5);
    ar.set(XmNitemCount, XtNumber(weight));
    ar.set(XmNselectedPosition, defaultPos);
    weightComboBox = new ComboBox(weightComboBoxFrame, "", ar);

    weightComboBox -> addCallback(XmNselectionCallback, this,
                         (Callback)&FTFontPreview::weightChanged, NULL);
    CompoundString prev("Preview");
    ar.reset();
    ar.set(XmNheight, 60);
    preview = new LabeledFrame(this, "Preview:", ar);
  	ar.reset();
  	previewRowColumn = new RowColumn(preview, "", ar);
  		
  	CompoundString csslant("Slant");
  	ar.reset();
  	ar.set(XmNlabelString, csslant);
  	slant  = new ToggleButton(previewRowColumn, "", ar);
  	slant -> addCallback(XmNvalueChangedCallback, this,
  		(Callback)&FTFontPreview::slantChanged, NULL);
  	
  	updateLabel();
  }

  ~FTFontPreview()
  {
    delete font;
    delete label;
    delete weightComboBox;
    delete sizeComboBox;
    delete fontNameList;
    delete rowColumn;

    delete fontNameListFrame;
    delete sizeComboBoxFrame;
    delete weightComboBoxFrame;
    delete panedw;
  }

public:
  const char* getFontName()
  {
    return (const char*)fontName;
  }
	
  int getFontSize()
  {
    return fontSize;
  }
	
  int getFontWeight()
  {
    return fontWeight;
  }
};
}

