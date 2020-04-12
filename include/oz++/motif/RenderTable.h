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
 *      RenderTable.h
 *
 *****************************************************************************/

//OpenMotif 2.1 and later
//2015/02/12 Updated
//2015/02/22 Updated

#pragma once

#include <oz++/CommonObject.h>
#include <Xm/Xm.h>
#include <oz++/motif/Rendition.h>

namespace OZ {

class RenderTable :public CommonObject {
private:
  XmRenderTable  renderTable;
 
public:
  RenderTable()
  :renderTable(NULL)
  {   
  }
 
  //2015/02/10
  RenderTable(IView* view, const char* name, Args& args, 
        XmMergeMode mergeMode = XmMERGE_NEW) // XmMERGE_REPLACE)
  :renderTable(NULL)
  {
   Rendition rendition(view, (XmStringTag)name, args);
   add(&rendition, mergeMode);
  }
 
  RenderTable(Rendition* rendition, XmMergeMode 
          mergeMode = XmMERGE_NEW) //XmMERGE_REPLACE)
  :renderTable(NULL)
  {
   if (rendition) {
     add(rendition, mergeMode);
   } else {
    throw IException("Invalid argument"); 
   }
  }

  ~RenderTable()
  {
   if (renderTable) {
     XmRenderTableFree(renderTable);
     renderTable = NULL;
   }
  }

  int getTags(XmStringTag** tags)
  {
    return XmRenderTableGetTags(renderTable, tags);
  }

  XmRenderTable copy(XmStringTag *tags, int tag_count)
  {
    return XmRenderTableCopy(renderTable,tags, tag_count);
  }

  XmRendition getRendition(XmStringTag tag)
  {
    return XmRenderTableGetRendition(renderTable, tag);
  }

  XmRendition* getRenditions(XmStringTag* tags, Cardinal tag_count)
  {
    return XmRenderTableGetRenditions(renderTable, tags, tag_count);
  }

  XmRenderTable removeRenditions(XmStringTag* tags, int tag_count)
  {
    return XmRenderTableRemoveRenditions(renderTable, tags, tag_count);
  }

  XmRenderTable get()
  {
   return renderTable;
  }

  //2015/02/22
  Boolean add(IView* view, XmStringTag name, Args& args, 
        XmMergeMode mergeMode=XmMERGE_REPLACE )//= XmMERGE_NEW)
  {
   Rendition rendition(view, name, args);
   return add(&rendition, mergeMode);
  }

  //Add only one XmRendition to XmRenditionTable
  Boolean add(Rendition* rendition, 
        XmMergeMode mergeMode = XmMERGE_REPLACE )
  {
   Boolean rc = False;
   if (rendition) {
     rc = add(rendition -> get(), mergeMode);
   }
   return rc;
  }

  //Add only one XmRedition to XmRenderTable
  Boolean add(XmRendition rendition, 
      XmMergeMode mergeMode = XmMERGE_NEW) //REPLACE )
  {
   Boolean rc = False;
   if (rendition) {
     rc = add(&rendition, 1, mergeMode);
   }
   return rc;
  }

 // Add an array of XmRedition of size count
  Boolean add(XmRendition* rendition, Cardinal count, 
      XmMergeMode mergeMode = XmMERGE_REPLACE) //2015/02/22 = XmMERGE_REPLACE )
  {
   Boolean rc = False;
   if (rendition) {
     XmRenderTable oldTable = renderTable;
     renderTable = XmRenderTableAddRenditions(oldTable, rendition, 
        count, mergeMode);
 
     /*if (oldTable) {
      //2015/02/22
    XmRenderTableFree(oldTable);
     }
     */
     if (renderTable == NULL) {
       throw IException("Failed to addRenditions");
     } else {
       rc = TRUE;
     }
   }
   return rc;
  }


  operator XmRenderTable()
  {
    return renderTable;
  }

};

}

