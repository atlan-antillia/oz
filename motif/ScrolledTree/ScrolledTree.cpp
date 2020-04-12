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
 *  ScrolledTree.cpp
 *
 *****************************************************************************/

#include <oz++/Locale.h>
#include <oz++/DirectoryScanner.h>
#include <oz++/LinkedListT.h>
#include <oz++/StringBufferT.h>
#include <oz++/SmartPtrs.h>

#include <oz++/motif/ApplicationView.h>
#include <oz++/motif/PanedWindow.h>
#include <oz++/motif/ScrolledTree.h>
#include <oz++/motif/ScrolledListBox.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/Color.h>

namespace OZ {

class MainView :public ApplicationView {
private:
  SmartPtr<PanedWindow>     panedWindow;
  SmartPtr<ScrolledTree>    scrolledTree;
  SmartPtr<ScrolledListBox> scrolledListBox;

  CharString toFullpath(LinkedListT<CompoundString>& list)
  {
    int m = list.getLength();
    StringBufferT<char> buffer;
    for (int i = 0; i<m; i++) {
      CompoundString* cs = list.getNth(i);
      char* name = cs->unparse();
      if (strcmp(name, "/") !=0) {
        buffer.append(name);
        buffer.append("/");
      } else {
        buffer.append(name);
      }
    }
    return CharString(buffer.getBuffer());
  }

  void   pushed(Action& action)
  {
    View* sender = (View*)action.getSender();
    LinkedListT<CompoundString> list; 
    int m = scrolledTree->getTree() -> getParentNodes(sender, list);  
    CharString fullpath = toFullpath(list);
  
    ListBox* listb= scrolledListBox -> getList();
    listb -> deleteAllItems();
    printf("%s\n", (const char*)fullpath);

    DirectoryScanner scanner((const char*)fullpath);
    DirEntryList dirList;

    int n = scanner.scanFile("*", dirList);
    printf("%d scanfiles \n", n);
    if (n > 0) {
      SmartPtrs<char*> names = new char*[n]; 
      dirList.getNames((char**)names);
      CompoundStringList csl(names, n); 

      listb -> addItemsUnselected(csl); 
    }
  }

  void buildTree(Tree* tree, View* root, const char* dir)
  {
     DirectoryScanner scanner(dir);
     DirEntryList dirList;
     int m = scanner.scanDir("*", dirList);
     if (m > 0) {
       for (int i = 0;  i<m; i++) {
         Args ar;
         ar.reset();
         const char* name = dirList.getName(i);
         View*  node = tree -> addPushButton(root, name, ar,
                 this, (Callback)&MainView::pushed);
         //If name of node is usr, add the subfolders of usr to the node.
         if (strcmp(name, "usr") == 0) {
           char user[256];
           sprintf(user, "%s/%s", dir, name);
           buildTree(tree, node, user); 
         }
       } 
     }
  }
 
public:
  MainView(Application& applet, const char* name, Args& args)
  :ApplicationView(applet, name, args)
  {
    Color color(this);
    
    Args ar;
    ar.reset();
    ar.set(XmNorientation, XmHORIZONTAL);
    panedWindow = new PanedWindow(this, "", ar);

    ar.reset();

    ar.set(XmNwidth, 260);
    scrolledTree = new ScrolledTree(panedWindow, "", ar);
    
    ar.reset();
    ar.set(XmNscrollingPolicy, XmAUTOMATIC);
    ar.set(XmNscrollBarDisplayPolicy, XmAS_NEEDED);
    ar.set(XmNwidth, 300);
    ar.set(XmNvisibleItemCount, 30);
    scrolledListBox = new ScrolledListBox(panedWindow, "", ar);
 
    ar.reset();
    Tree* tree =  scrolledTree -> getTree();
    tree -> set(XmNconnectStyle, XmTreeLadder);

    const char* dir = "/";

    ar.reset();
    ar.set(XmNbackground, color.allocNamedColor("red")); 
    View* root = tree -> addLabel(NULL, "/", ar);
    buildTree(tree, root, dir);
 
  }

  ~MainView()
  {
  }
};

}

//
int main(int argc, char** argv)
{
  try {
    Locale locale;
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);

    Args args;
    args.set(XmNgeometry, "600x300");
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}
