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
 *  IconBar.h
 *
 *****************************************************************************/


#include <oz++/motif/IconBox.h>
#include <oz++/motif/IconButton.h>
#include <oz++/motif/Color.h>
#include <oz++/motif/PixmapFile.h>
#include <oz++/SmartArray.h>

namespace OZ {

const char* XmNiconFileName = "iconFileName";
	
typedef struct {
  	  const char* name;
  	  int         menuId;
  	  const char* iconFile;
} IconBarItem;

class IconBar: IconBox {
private:
  SmartArray<IconButton*>  iconButtons;
  SmartArray<PixmapFile*>  pixmapFiles;

  int buttonCount;
  int fileCount;

private:
  
public:
  IconBar(View* parent, const char* name, Args& args)
  :IconBox(parent, name, args.set(XmNminimumHorizontalCells, 1)
                             .set(XmNminimumVerticalCells, 1)) 
  ,buttonCount(0)
  ,fileCount(0)
  {
  }

  IconBar()
  :IconBox()
  ,buttonCount(0)
  ,fileCount(0)
  {
  }
  
  void create(View* parent, const char* name, Args& args)
  {
    IconBox::create(parent, name, 
                         args.set(XmNminimumHorizontalCells, 1)
                             .set(XmNminimumVerticalCells, 1)) ;
  }

public:
  ~IconBar()
  {
    iconButtons.clear();
    pixmapFiles.clear();
  }


public:
  IconButton* addButton(Args& args, const char* name = NULL,
            CommonObject* object = NULL, Callback callback = NULL, XtPointer data=NULL)
  {
    args.set(XmNcellX, XmIconBoxAnyCell);
    args.set(XmNcellY, XmIconBoxAnyCell);
    args.set(XmNiconPlacement, XmIconTop);
    args.set(XmNalignment, XmALIGNMENT_CENTER);
    args.set(XmNrecomputeSize, False);

    if(args.has(XmNiconFileName)) {
      const char* fileName = (const char*)args.get(XmNiconFileName);
      if (fileName) {
          
         try {
           Pixel fg, bg;
           get(XmNforeground, (XtArgVal)&fg);
           get(XmNbackground, (XtArgVal)&bg);

           PixmapFile* pfile = new PixmapFile(getDisplay(), fileName, fg, bg);
           if (pfile) {
              args.set(XmNpixmap, pfile->get());
              pixmapFiles[fileCount++] = pfile;              
           }
           
         } catch (Exception& ex) {
         } 
      }
    }    
    IconButton* icb = new IconButton(this, "", args);
    iconButtons[buttonCount++] = icb; 
    if (name && object && callback) {
      icb -> addCallback(name, object, callback, data);
    } 

    set(XmNminimumHorizontalCells, buttonCount);

    return icb;
  }
};

}

