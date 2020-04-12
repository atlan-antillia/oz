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
 *  CustomFontDialog.cpp
 *
 *****************************************************************************/

#include <oz++/SmartPtr.h>
#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/PanedWindow.h>
#include <oz++/motif/RenderTable.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/CustomDialog.h>
#include <oz++/motif/FontListView.h>
#include <oz++/motif/ScrolledText.h>


namespace OZ {
// Simple FontDialog class derived from CustomDialog
	
class CustomFontDialog: public CustomDialog {

private:
  SmartPtr<FontListView> fontView;
  CharString   fontName;

  void browse(Action& action)
  {
    XmListCallbackStruct* cbs =
       (XmListCallbackStruct*)action.getCallData();
    CompoundString cs(cbs->item);
    char* item = cs.unparse();
    fontName = item;
   //   printf("Mainview::browse item: %s\n", item);
    XtFree(item);
  }

public:
  CustomFontDialog(View* parent, const char* name, Args& args)
  :CustomDialog(parent, name, args),
   fontName("")
  {
    Form* workArea = getWorkArea();
  	
    //Create an instance of FontView in the workArea. 
    Args ar;
    ar.set(XmNtopAttachment,    XmATTACH_FORM);
    ar.set(XmNbottomAttachment, XmATTACH_FORM);
    ar.set(XmNrightAttachment,  XmATTACH_FORM);
    ar.set(XmNleftAttachment,   XmATTACH_FORM);
    fontView = new FontListView(workArea, "", ar);
  	//Listup fontNames.
    fontView -> listup(FontListView::NAME_ONLY);
  	//Add a browse callback to the ListBox of FontView.
    fontView -> getList()
             -> addCallback(XmNbrowseSelectionCallback, this,
                  (Callback)&CustomFontDialog::browse, NULL);
  	
    //Add OK and Cancel buttons.
    static const char* buttons[] = { "OK", "Cancel", "Help" };
    CompoundStringList csl(buttons, SizeOf(buttons) );
    addButton(csl);
  	
    //CancelButton
    getNthButton(1) -> addCallback(XmNactivateCallback, this,
                  (Callback)&PopupView::popdown, NULL);
  } 

  ~CustomFontDialog()
  {
  }    

  PushButton* getOkButton()
  {
    return getNthButton(0);
  }

  const char* getFontName()
  {
    return (const char*)fontName;
  }
};

// Sample class to use FontDialog	
class MainView :public ApplicationView {
private:
  SmartPtr<PanedWindow>  panedw;
  SmartPtr<PushButton>   pushb;
  SmartPtr<TextField>    fontName;
  SmartPtr<ScrolledText> text;
  SmartPtr<RenderTable>  rtable;
  int                    fontSize;

  Boolean                firstTimePopup; 
  SmartPtr<CustomFontDialog> dialog;

  void   activate(Action& action)
  {
    XmPushButtonCallbackStruct* cbs =
        (XmPushButtonCallbackStruct*)action.getCallData();
    //dialog->popupSpringLoaded();
    if (firstTimePopup) {
      dialog->popupAt();
      firstTimePopup = FALSE;
    } else {
      dialog->popup();
    }
  }

  void ok(Action& action)
  {
    const char* name = dialog->getFontName();
    if (name != NULL && strlen(name)>0) {
      fontName -> set(XmNvalue, name);
      if (rtable) {
        rtable = NULL;
      }
      //SmartPtr<char> fname = __strdup(name);
      char* fname = __strdup(name);
      char* delim = (char*)strchr((const char*)fname, ',');
      if (delim) {
        *delim = '\0';
      }
      Args ar;
      ar.set(XmNfontSize, fontSize);
      ar.set(XmNfontName, (const char*)fname);
      rtable = new RenderTable(text, "", ar);
      text -> getText() -> set(XmNrenderTable, rtable);
      printf("FontName =[%s]\n", (const char*)fname);
      delete [] fname; 
    }
    dialog->popdown();
  }

public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args),
    fontSize(20),    //Font size
    firstTimePopup(TRUE)
  {
    Args ar;
    ar.set(XmNorientation, XmVERTICAL);
    panedw = new PanedWindow(this, "", ar);

    CompoundString cs("Showt FontDialog");

    ar.set(XmNlabelType, XmSTRING);
    ar.set(XmNlabelString, cs);
    pushb = new PushButton(panedw, "pushb", ar);

    pushb -> addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::activate, NULL);
    ar.reset();
    ar.set(XmNeditable, FALSE);
    fontName = new TextField(panedw, "fontName", ar);

    ar.reset();
    ar.set(XmNwidth,  500);
    ar.set(XmNheight, 400);
    text = new ScrolledText(panedw, "text", ar);

    ar.reset();
    ar.set(XmNwidth,  400);
    ar.set(XmNheight, 500);
    dialog = new CustomFontDialog(this, "FontDialog", ar);

    PushButton* ok = dialog->getOkButton();
    ok -> addCallback(XmNactivateCallback, this,
              (Callback)&MainView::ok, NULL);
  }

  ~MainView()
  {
  }
};
}

//
int main(int argc, char** argv)
{
  const char*  appclass = argv[0];
  Application applet(appclass, argc, argv);
  try {
    Args args;
    args.set(XmNgeometry, "400x500");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
     caught(ex);
  }
  return 0;
}
