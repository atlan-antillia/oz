/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      ListBox.h
 *
 *****************************************************************************/

//2015/02/07 Updated
//2017/10/15 Update to use create method.

#pragma once

#include <oz++/DirectoryScanner.h>
#include <oz++/DirEntryList.h>
#include <oz++/SmartPtrs.h>
#include <oz++/Exception.h>
#include <oz++/FileAttributes.h>
#include <oz++/LocalTime.h>
#include <oz++/motif/Primitive.h>
#include <Xm/List.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/CompoundStringList.h>

namespace OZ {

class ListBox :public Primitive {
public:
  static const int AT_FIRST = 1;
  static const int AT_LAST  = 0;

public:
  typedef enum {
    DESCENDING,
    ASCENDING,	
  } SORT_ORDER;
	

public:
  ListBox(View* parent, const char* name,  Args& args)
    :Primitive(parent, name, xmListWidgetClass, args)
  {
    // Consturctor
  }

  ListBox()
  :Primitive()
  {
  }

  void create(View* parent, const char* name,  Args& args)
  {
    Primitive::create(parent, name, xmListWidgetClass, args);
  }

  int getItemCount()
  {
    int size = 0;
    get(XmNitemCount, (XtArgVal)&size);
    return size;
  }

  void  addItem(XmString item, int pos)
  {
    XmListAddItem(getWidget(), item, pos);
  }

  void  addItemUnselected(XmString item, int pos)
  {
    XmListAddItemUnselected(getWidget(), item, pos);
  }

  void  addLastItemUnselected(CompoundString& item)
  {
    int size = 0;
    get(XmNitemCount, (XtArgVal)&size);
    XmListAddItemUnselected(getWidget(), item.get(), ++size);
  }

  void  addLastItemUnselected(XmString item)
  {
    int size = 0;
    get(XmNitemCount, (XtArgVal)&size);
    XmListAddItemUnselected(getWidget(), item, ++size);
  }

  void addItemUnselectedByAlphaSort(const char* string, SORT_ORDER op = DESCENDING)
  {
    if (string == NULL) {
      throw IException("Invalid argument");
    }
    XmString   xms;
    XmString*  xmslist;
    int  upper = 0;
    int  lower = 0;
    int  count = 0;
    get(XmNitemCount, (XtArgVal)&upper);
    get(XmNitems,  (XtArgVal)&xmslist);
    count = upper;
    upper--;

    while (upper >= lower) {
        int i = lower + (upper - lower) / 2;

        CompoundString item(xmslist[i]);

        char* text = item.unparse();
        if (text == NULL) {
          break;
        }
    	if (op == DESCENDING) {
          if (strcmp (text, string) > 0) {
            upper = i - 1; 
          }
          else {
            lower = i + 1; 
          }
    	} else if (op == ASCENDING) {
          if (strcmp (text, string) <= 0) {
            upper = i - 1; 
          }
          else {
            lower = i + 1; 
          }
    	}
        XtFree (text);
    }

    addItemUnselected(CompoundString(string),  lower+1);
  }


  //2015/02/03
  void  addItemUnselected(CompoundString& item, int pos = 0)
  {
    XmListAddItemUnselected(getWidget(), item, pos);
  }

  //2015/06/10
  void  addFirstUnselected(CompoundString& item)
  {
    XmListAddItemUnselected(getWidget(), item, AT_FIRST);
  }

  //2015/06/10
  void  addLastUnselected(CompoundString& item)
  {
    XmListAddItemUnselected(getWidget(), item, AT_LAST);
  }

  void  addItems(XmString* items, int num, int pos)
  {
    XmListAddItems(getWidget(), items, num, pos);
  }
  
  //2015/01/28
  void  addItems(CompoundStringList& csl, int pos = 0)
  {
    int num = csl.size();
    XmString* item = csl.get();
    XmListAddItems(getWidget(), item, num, pos);
  }

  void  addItemsUnselected(XmString* items, int num, int pos)
  {
    XmListAddItemsUnselected(getWidget(), items, num, pos);
  }
	
  //2015/01/28
  void  addItemsUnselected(CompoundStringList& csl, int pos = 0) 
  {
    int num = csl.size();
    XmString* item = csl.get();
    XmListAddItemsUnselected(getWidget(), item, num, pos);
  }

  //2015/06/10
  void  addFirstUnselected(CompoundStringList& csl)
  {
    addItemsUnselected(csl, AT_FIRST);
  }

  //2015/06/10
  void  addLastUnselected(CompoundStringList& csl)
  {
    addItemsUnselected(csl, AT_LAST);
  }

  //2015/01/28
  void  clear()
  {
    deleteAllItems();
  }
	
  void  deleteAllItems()
  {
    XmListDeleteAllItems(getWidget());
  }

  void  deleteItem(XmString item)
  {
    XmListDeleteItem(getWidget(), item);
  }

  void  deleteItems(XmString* items, int num)
  {
    XmListDeleteItems(getWidget(), items, num);
  }

  void  deleteItemsPos(int num, int pos)
  {
    XmListDeleteItemsPos(getWidget(), num, pos);
  }

  void  deletePos(int pos)
  {
    XmListDeletePos(getWidget(), pos);
  }

  void  deletePositions(int* pos, int num)
  {
    XmListDeletePositions(getWidget(), pos, num);
  }

  void  deselectAllItems()
  {
    XmListDeselectAllItems(getWidget());
  }

  void  deselectItem(XmString item)
  {
    XmListDeselectItem(getWidget(), item);
  }

  void  deselectPos(int pos)
  {
    XmListDeselectPos(getWidget(), pos);
  }

  int  getKbdItemPos()
  {
    return XmListGetKbdItemPos(getWidget());
  }

  Boolean  getMatchPos(XmString item, int** pos, int* num)
  {
    return XmListGetMatchPos(getWidget(), item, pos, num);
  }

  Boolean  getSelectedPos(int** pos, int* num)
  {
    return XmListGetSelectedPos(getWidget(), pos, num);
  }
  //2015/02/07
  Boolean  itemExists(CompoundString& item)
  {
    return XmListItemExists(getWidget(), item.get());
  }

  Boolean  itemExists(XmString item)
  {
    return XmListItemExists(getWidget(), item);
  }

  int  itemPos(XmString item)
  {
    return XmListItemPos(getWidget(), item);
  }

  Boolean  posSelected(int pos)
  {
    return XmListPosSelected(getWidget(), pos);
  }

  Boolean  posToBounds(int pos, Position* x, Position* y,
    Dimension* width, Dimension* height)
  {
    return XmListPosToBounds(getWidget(), pos, x, y,
      width, height);
  }

  void  replaceItems(XmString* oldItems, int num, XmString* newItems)
  {
    XmListReplaceItems(getWidget(), oldItems, num, newItems);
  }

  void  replaceItemsPos(XmString* newItems, int num, int pos)
  {
    XmListReplaceItemsPos(getWidget(), newItems, num, pos);
  }

  void  replaceItemsPosUnselected(XmString* newItems, int num, int pos)
  {
    XmListReplaceItemsPosUnselected(getWidget(), newItems, num, pos);
  }

  void  replaceItemsUnselected(XmString* oldItems, int num, XmString* newItems)
  {
    XmListReplaceItemsUnselected(getWidget(), oldItems, num, newItems);
  }

  void  replacePositions(int* pos, XmString* items, int num)
  {
    XmListReplacePositions(getWidget(), pos, items, num);
  }

  void  selectItem(XmString item, Boolean notify)
  {
    XmListSelectItem(getWidget(), item, notify);
  }

  void  selectPos(int pos, Boolean notify)
  {
    XmListSelectPos(getWidget(), pos, notify);
  }

  void  setAddMode(Boolean state)
  {
    XmListSetAddMode(getWidget(), state);
  }

  void  setBottomItem(XmString item)
  {
    XmListSetBottomItem(getWidget(), item);
  }

  void  setBottomPos(int pos)
  {
    XmListSetBottomPos(getWidget(), pos);
  }

  void  setHorizPos(int pos)
  {
    XmListSetHorizPos(getWidget(), pos);
  }

  void  setItem(XmString item)
  {
    XmListSetItem(getWidget(), item);
  }

  void  setKbdItemPos(int pos)
  {
    XmListSetKbdItemPos(getWidget(), pos);
  }

  void  setPos(int pos)
  {
    XmListSetPos(getWidget(), pos);
  }

  void  updateSelectedList()
  {
    XmListUpdateSelectedList(getWidget());
  }

  int  yToPos(Position y)
  {
    return XmListYToPos(getWidget(), y);
  }

  //2015/02/13
  void listupDirectories(const char* dir, const char* pattern="*")
  {
    if (dir) {
      DirectoryScanner scanner(dir);
      DirEntryList dirList;
      if (pattern == NULL) {
        pattern = "*";
      }
      int m = scanner.scanDir(pattern, dirList);
      if (m > 0) {
        SmartPtrs<char*> names = new char*[m];
        dirList.getNames((char**)names);
        CompoundStringList csl(names, m);

        addItemsUnselected(csl);
     }
    } else {
      throw IException("Invalid argument");	
    }
  }
	
  void listupDirectories(const char* dir, const char prefix, const char* pattern="*")
  {
  	//static const char prefix = '+';
  	if (dir) {
      DirectoryScanner scanner(dir);
      DirEntryList dirList;
  	  if (pattern == NULL) {
  		pattern = "*";
  	  }
      int m = scanner.scanDir(pattern, dirList);
      if (m > 0) {
        SmartPtrs<char*> names = new char*[m];
        dirList.getNames((char**)names);
      	
        CompoundStringList csl;
      	for (int i = 0; i < m; i++) {
          char item[PATH_MAX];
      	  sprintf(item, "%c %s", prefix, names[i]);
      	  csl.add(item);
      	}
        addItemsUnselected(csl);
     }
  	} else {
      throw IException("Invalid argument");	
  	}
  }

  //2015/02/13
/*
  void listupDirectoryAttributes(const char* dir, const char* pattern="*")
  {
  	if (dir) {
      DirectoryScanner scanner(dir);
      DirEntryList dirList;
  	  if (pattern == NULL) {
  		pattern = "*";
  	  }
      int m = scanner.scanDir(pattern, dirList);
      if (m > 0) {
        SmartPtrs<char*> names = new char*[m];
        dirList.getNames((char**)names);
      	char fullpath[PATH_MAX];
      	CompoundStringList csl;
      	for (int i = 0; i<m; i++) {
      	  sprintf(fullpath, "%s/%s", dir, names[i]);
      	  FileAttributes attr(fullpath);
      	  char item[PATH_MAX];
      	  LocalTime lt(attr.modifiedTime());
      		CharString slt = lt.toCharString();
      	  sprintf(item, "%s %ld %s - %s",  (const char*)attr.modeToString(), attr.getSize(), 
      	  	(const char*)slt, names[i]);
      		csl.add(item);
      	}
      	
        addItemsUnselected(csl);
     }
  	} else {
      throw IException("Invalid argument");	
  	}
  }
*/
	
  //2015/02/13
  void listupFiles(const char* dir, const char* pattern="*")
  {
    if (dir) {
      DirectoryScanner scanner(dir);
      DirEntryList dirList;
      if (pattern == NULL) {
  		pattern = "*";
      }
      int m = scanner.scanFile(pattern, dirList);
      if (m > 0) {
        SmartPtrs<char*> names = new char*[m];
        dirList.getNames((char**)names);
        CompoundStringList csl(names, m);

        addItemsUnselected(csl);
      }

    } else {
      throw IException("Invalid argument");	
    }
  }

  void listupFiles(const char* dir, const char prefix, const char* pattern="*")
  {
     //static const char prefix = '+';
     if (dir) {
       DirectoryScanner scanner(dir);
       DirEntryList dirList;
       if (pattern == NULL) {
        pattern = "*";
       }
       int m = scanner.scanFile(pattern, dirList);
       if (m > 0) {
        SmartPtrs<char*> names = new char*[m];
        dirList.getNames((char**)names);
      	
        CompoundStringList csl;
      	for (int i = 0; i < m; i++) {
          char item[PATH_MAX];
      	  sprintf(item, "%c %s", prefix, names[i]);
      	  csl.add(item);
      	}
        addItemsUnselected(csl);
      }
    } else {
      throw IException("Invalid argument");	
    }
  }

};
}

