/******************************************************************************
 *
 * Copyright (c) 2015-2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  FontListView.h
 *
 *****************************************************************************/

//2017/10/15 Updated to use create method.

#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/CompoundString.h>

namespace OZ {
 
// Please add your own callback method in the following way
// FontListView* fontListView = new FontListView(...);
// ListBox* listb = fontListView ->getList();
// listb -> addCallback(XmNbrowseSelectionCallback, this,
//                  (Callback)&FontListView::browseCallback, NULL);

class FontListView :public ScrolledListBox {
public:
  typedef enum {
    ALL,
    WITHOUT_FILENAME,
    NAME_ONLY,
  } DISPLAY_MODE;


public:
  FontListView(View* parent, const char* name, Args& args)
  :ScrolledListBox()
  {
    FontListView::create(parent, name, args);
  }

  FontListView()
  :ScrolledListBox()
  {
  }

  void create(View* parent, const char* name, Args& args)
  {
    ScrolledListBox::create(parent, name,  
                        args.set(XmNscrollBarDisplayPolicy, XmVARIABLE)
                            .set(XmNlistSizePolicy,         XmAS_NEEDED) );
  }

public:
  void listup(DISPLAY_MODE mode =WITHOUT_FILENAME )
  {
    ListBox* listb = getList();

    const char* fname = "/tmp/oz_fontlist.txt";
    char command[PATH_MAX];
    sprintf(command, "fc-list >%s", fname);
    system(command);
    FILE* fp = fopen(fname, "r");
    int count = 0;
    if (fp) {
        char line[1024];
        while(fgets(line, sizeof(line), fp)) {
          int len = strlen(line);
          if (line[len-1]== '\n') {
            line[len-1] = '\0';
          }
          const char* name = line;

          if (mode == WITHOUT_FILENAME) {
            //line = fullpath: fontname:style...
            const char* colon = strchr(line, ':');
            if (colon) {
              name = ++colon;
             if (*name == ' ') {
              name++;
             }
            }
          }
          if (mode == NAME_ONLY) {
            const char* colon = strchr(line, ':');
            if (colon) {
              name = ++colon;
             if (*name == ' ') {
              name++;
             }
            
              char* ncolon = (char*)strstr(name, ":style");
              if (ncolon) {
               *ncolon = '\0';
              }
            }
          }

          if (listb -> itemExists(CompoundString(name)) == False) {
            listb -> addItemUnselectedByAlphaSort(name);
            //listb -> addItemUnselectedByAlphaSort(name, ListBox::ASCENDING);
          }
          count++;
        }
        fclose(fp);
     unlink(fname);
    } else {
      printf("Failed to open file %s\n", fname); 
    }
  }

};
}
