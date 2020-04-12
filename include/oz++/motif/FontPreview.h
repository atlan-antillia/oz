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
 *  FontPreView.h
 *
 *****************************************************************************/

#pragma once

#include <oz++/Pair.h>

#include <oz++/motif/PanedWindow.h>
#include <oz++/motif/LabeledFrame.h>

#include <oz++/motif/RowColumn.h>
#include <oz++/motif/ComboBox.h>
#include <oz++/motif/FontListView.h>
#include <oz++/motif/ScrolledText.h>
#include <oz++/motif/RenderTable.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/Label.h>

#include <oz++/motif/XtSmartPtr.h>

namespace OZ {

class FontPreview :public PanedWindow {

private:
  CharString     fontName;
  int            fontSize;
  
  SmartPtr<PanedWindow>      panedw;

  SmartPtr<LabeledFrame>     fontNameListFrame;
  SmartPtr<FontListView>     fontNameList; 

  SmartPtr<RowColumn>        rowColumn;
  SmartPtr<LabeledFrame>     sizeComboBoxFrame;
  SmartPtr<ComboBox>         sizeComboBox;

  SmartPtr<LabeledFrame>     styleComboBoxFrame;
  SmartPtr<ComboBox>         styleComboBox;
  CharString                 style;
 
  SmartPtr<LabeledFrame>     strikethruComboBoxFrame;
  SmartPtr<ComboBox>         strikethruComboBox;
  int                        strikethruType;
 
  SmartPtr<LabeledFrame>     underlineComboBoxFrame;
  SmartPtr<ComboBox>         underlineComboBox;
  int                        underlineType;
 
  SmartPtr<LabeledFrame>     previewFrame;
  SmartPtr<RowColumn>        previewRowColumn;
  SmartPtr<Label>            previewLabel;
  SmartPtr<RenderTable>      rtable;
  SmartPtr<ScrolledText>     scrolledText;
 
private:
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
    updateText();
    XtFree(item);
  }

private:
 void   sizeChanged(Action& action)
  {
    XmComboBoxCallbackStruct* cbs =
        (XmComboBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs -> item_or_text);
    XtSmartPtr<char> item = cs.unparse();
    printf("sizeChanged: item: %s\n", (const char*)item);
    fontSize = atoi(item);
    updateText();
  }
 
private:
  void  styleChanged(Action& action)
  {
    XmListCallbackStruct* cbs =
       (XmListCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->item);
   
    char* item = cs.unparse();
    // The above item may contain two names separated by ',' delimiter.
    printf("styleChanged: item [%s]\n", item);
    style = item;
    updateText();
    XtFree(item);
  }

 
private:
 void   strikethruChanged(Action& action)
  {
    XmComboBoxCallbackStruct* cbs =
        (XmComboBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs -> item_or_text);
    XtSmartPtr<char> item = cs.unparse();
    printf("strikethruChanged: item: %s\n", (const char*)item);
    strikethruType = getLineType(item);
    updateText();
  }
 
private:
 void   underlineChanged(Action& action)
  {
    XmComboBoxCallbackStruct* cbs =
        (XmComboBoxCallbackStruct*)action.getCallData();
    CompoundString cs(cbs -> item_or_text);
    XtSmartPtr<char> item = cs.unparse();
    printf("underlineChanged: item: %s\n", (const char*)item);
    underlineType = getLineType(item);
    updateText();
  }
 
  int getLineType(const char* name)
  {
    Pair<int, const char*> pairs[] = {
     {XmNO_LINE,            "NO_LINE" }, 
     {XmSINGLE_LINE,        "SINGLE_LINE"}, 
     {XmDOUBLE_LINE,        "DOUBLE_LINE"}, 
     {XmSINGLE_DASHED_LINE, "SINGLE_DASHED_LINE"}, 
     {XmDOUBLE_DASHED_LINE, "DOUBLE_DASHED_LINE"},
     };
   int type = XmNO_LINE;
   for (int i = 0; i< SizeOf(pairs); i++) {
    if (strcmp(name, pairs[i].second) == 0) {
      type = pairs[i].first;
      break;
    }
   }
   return type;
  }
 
  void updateText()
  {
    Color color(this);
   
    Pixel red = color.allocNamedColor("red");
    Args ar;
    ar.reset();
    ar.set(XmNfontType, XmFONT_IS_XFT);
    ar.set(XmNfontName, (const char*)fontName);
    ar.set(XmNfontStyle, (const char*)style);
    ar.set(XmNfontSize,   fontSize);
    ar.set(XmNstrikethruType, strikethruType);
    ar.set(XmNunderlineType,  underlineType);
    ar.set(XmNrenditionForeground, red);
    rtable = new RenderTable(scrolledText, "", ar);
   
    CompoundString lcs((const char*)fontName);
   
    previewLabel -> set(XmNlabelString, lcs);
    previewLabel -> set(XmNrenderTable, rtable);
   
    Text* text = scrolledText -> getText();
    text -> set(XmNrenderTable, rtable);
   
    char string[256];
    sprintf(string, "1234567890ABCDEFGH...xyz");
  
    text -> setString(string);
  }

public:
  FontPreview(View* parent, const char* name, Args& args)
  :PanedWindow()
  {
    FontPreview::create(parent, name, args);
  }

  FontPreview()
  :PanedWindow()
  {
  }


  void create(View* parent, const char* name, Args& args)
  {
    PanedWindow::create(parent, name, args.set(XmNorientation, XmVERTICAL)
                                 .set(XmNsashWidth, 1)
                                 .set(XmNsashHeight, 1) );
    underlineType = XmSINGLE_LINE; 
 
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
                     (Callback)&FontPreview::nameChanged, NULL);

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
    ar.set(XmNwidth, 80);
    ar.set(XmNitems, csl);
    ar.set(XmNvisibleItemCount, 5);
    ar.set(XmNitemCount, XtNumber(items));
    ar.set(XmNselectedPosition, defaultPosition);
    sizeComboBox = new ComboBox(sizeComboBoxFrame, "", ar);

    sizeComboBox -> addCallback(XmNselectionCallback, this,
                         (Callback)&FontPreview::sizeChanged, NULL);
    //--
    static const char* styles[] = {"regular", "bold", "italic", "bold italic"};
   
    ar.reset();
    styleComboBoxFrame = new LabeledFrame(panedw, "Style:", ar);
    style = "Regular";
   
    CompoundStringList styleCsl(styles, XtNumber(styles));
    ar.reset();
    ar.set(XmNwidth, 120);
    ar.set(XmNitems, styleCsl);
    ar.set(XmNvisibleItemCount, 5);
    ar.set(XmNitemCount, XtNumber(styles));
    ar.set(XmNselectedPosition, 0);
    styleComboBox = new ComboBox(styleComboBoxFrame, "", ar);
    styleComboBox -> addCallback(XmNselectionCallback, this,
                         (Callback)&FontPreview::styleChanged, NULL);
//
   
    static const char* sitems[] = {
            "NO_LINE", "SINGLE_LINE", "DOUBLE_LINE", 
            "SINGLE_DASHED_LINE", "DOUBLE_DASHED_LINE"};
 
    ar.reset();
    strikethruComboBoxFrame = new LabeledFrame(panedw, "StrikeThru:", ar);
    CompoundStringList scsl(sitems, XtNumber(sitems));
    ar.reset();
    ar.set(XmNwidth, 200);
    ar.set(XmNitems, scsl);
    ar.set(XmNvisibleItemCount, 5);
    ar.set(XmNitemCount, XtNumber(sitems));
    ar.set(XmNselectedPosition, 1);
    strikethruComboBox = new ComboBox(strikethruComboBoxFrame, "", ar);
    strikethruComboBox -> addCallback(XmNselectionCallback, this,
                         (Callback)&FontPreview::strikethruChanged, NULL);

    ar.reset();
    underlineComboBoxFrame = new LabeledFrame(panedw, "Underline:", ar);
 

    CompoundStringList ucsl(sitems, XtNumber(sitems));
    ar.reset();
    ar.set(XmNwidth, 200);
    ar.set(XmNitems, ucsl);
    ar.set(XmNvisibleItemCount, 5);
    ar.set(XmNitemCount, XtNumber(sitems));
    ar.set(XmNselectedPosition, 1);
    underlineComboBox = new ComboBox(underlineComboBoxFrame, "", ar);
    underlineComboBox -> addCallback(XmNselectionCallback, this,
                         (Callback)&FontPreview::underlineChanged, NULL);


    CompoundString prev("Preview");
    ar.reset();
    ar.set(XmNheight, 120);
    previewFrame = new LabeledFrame(this, "Preview:", ar);
   
    ar.reset();
    ar.set(XmNorientation, XmVERTICAL);
    ar.set(XmNnumRows, 2);
    previewRowColumn = new RowColumn(previewFrame, "", ar);
   
    CompoundString slabel("family");
    ar.reset();
    ar.set(XmNlabelString, slabel);
   
    previewLabel = new Label(previewRowColumn, "", ar);
   
    ar.reset();
    ar.set(XmNheight, 80);
    scrolledText = new ScrolledText(previewRowColumn, "", ar);
    Color color(this);
    scrolledText -> getText() -> set(XmNbackground, color.whitePixel());
   
    updateText();
  }

  ~FontPreview()
  {
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
};
}

