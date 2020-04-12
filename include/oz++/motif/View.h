/******************************************************************************
 *
 * Copyright (c) 2014-2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      View.h
 *
 *****************************************************************************/

// 2015/01/18 Updated to add grabPointer, ungrabPointer methods.
// 2015/01/26 Added set method which takes a pointer to RenderTable class.
// 2015/02/11 Added mulitiple queryMousePointer methods.
// 2015/02/20 Added some methods to get geometry properities.
// 2015/08/10 Added methods corresponding to Xme* APIs in <Xm/TransferP.h>.
// 2016/03/27 Added sendExposeEvent method.
// 2016/09/12 Added getParent method.

#pragma once

#include <oz++/motif/IView.h>

#include <oz++/motif/Args.h>
#include <oz++/motif/Action.h>
#include <oz++/motif/Event.h>
#include <oz++/motif/Transducer.h>
#include <oz++/motif/CompoundString.h>
#include <oz++/motif/PlainString.h>
#include <oz++/motif/CompoundStringList.h>
#include <oz++/motif/Application.h>

#include <oz++/LinkedList.h>

#include <oz++/motif/RenderTable.h>
//2015/08/15
#include <Xm/TransferP.h>

namespace OZ {
  
class View :public IView {
  
private:
  Boolean destructive;
  Widget  widget;
  LinkedList callbackList;
  LinkedList handlerList;

  static void commonHandler(Widget widget, XtPointer client,
                                XEvent* xevent, Boolean* flag)
  {
    Event* event = (Event*)client;
    if (event) {
      event -> setXEvent(xevent);
      event -> setOptionData((void*)flag);
      event -> call(*event);
    }
  }

  static void commonCallback(Widget widget, XtPointer client, 
        XtPointer callData)
  {
    Action* action = (Action*)client;
    if (action) {
      action -> setCallData(callData);
      action -> call(*action);
    }
  }
 
protected:
  XtEventHandler getCommonHandler() 
  {
    return &View::commonHandler;
  }
  

  XtCallbackProc getCommonCallback() 
  {
    return &View::commonCallback;
  }
  
  View() 
  { 
    destructive = TRUE;
    widget = NULL; 
  } 

public:
  //2015/01/22
  View(Widget w)
  {
    widget = w;
    nondestructive();
  }
  
public:
  ~View() 
  {
    if(widget && destructive) {
      XtDestroyWidget(widget);
    }
  }
  
  //2015/01/18
  void nondestructive()
  {
    destructive = False;
  }
  
  virtual void addCallback(const char* name, CommonObject* object, 
      Callback callback, XtPointer data)
  {
    Action* action = new Action(this, object, callback, data, name);
    XtAddCallback(getWidget(), name,
                         &View::commonCallback, action);
    // 2014.10.17
    callbackList.add(action);
  }

  virtual void addEventHandler(EventMask mask, 
      CommonObject* object, Handler handler, XtPointer data)
  {
    Event* event = new Event(this, object, handler, data);
    XtAddEventHandler(getWidget(), mask, False,
      &View::commonHandler, event); 
    handlerList.add(event);
  }

  virtual void addEventHandler(EventMask mask, Boolean nonmaskable,
      CommonObject* object, Handler handler, XtPointer data)
  {
    Event* event = new Event(this, object, handler, data);
    XtAddEventHandler(getWidget(), mask, nonmaskable,
      &View::commonHandler, event); 
    handlerList.add(event);
  }

  virtual void addRawEventHandler(EventMask mask, Boolean nonmaskable,
      CommonObject* object, Handler handler, XtPointer data)
  {
    Event* event = new Event(this, object, handler, data);
    XtAddEventHandler(getWidget(), mask, nonmaskable,
                        &View::commonHandler, event);
    handlerList.add(event);
  }

  void  augmentTranslation(Application& applet, Transducer* transd,
                        CommonObject* object, Callback callback, XtPointer data)
  {
    char translation[256];
    sprintf(translation, "%s: %s(%d,%s)",
                        transd->getEvents(), transd->getProc(),
                        transd->getId(), transd->getArgs());

    Action* action = NULL;

    applet.getAppContext()
        ->addAction(transd->getProc());

    action = new Action(this, object, callback, data);
    ActionProcTable::put(widget, action, transd->getId());
    XtAugmentTranslations(widget,
                        XtParseTranslationTable(translation));
  }
  
  //2015/01/28 Modified to be virtual function
  virtual void  clear() 
  {
    XClearWindow(XtDisplay(widget), XtWindow(widget));
  }

  void  configure(Position x, Position y,
      Dimension width, Dimension height, Dimension border) 
  {
    XtConfigureWidget(widget, x, y, width, height, border);
  }

  //2015/02/20
  Position x()
  {
    Position value = 0;
    get(XmNx, (XtArgVal)&value);
    return value;
  }

  Position y()
  {
    Position value = 0;
    get(XmNy, (XtArgVal)&value);
    return value;
  }
  
  Dimension width()
  {
    Dimension value = 0;
    get(XmNwidth, (XtArgVal)&value);
    return value;
  }

  Dimension height()
  {
    Dimension value = 0;
    get(XmNheight, (XtArgVal)&value);
    return value;
  }

  void  get(Args* args) 
  { 
    XtGetValues(widget, args->getArgList(), args->count());
  }

  void get(const char* name, XmString* xms) 
  {
    Args ar;
    ar.set(name, (XtArgVal)xms);
    XtGetValues(getWidget(), ar.getArgList(), 1);
  }

  void get(const char* name, CompoundString* cs) 
  {
    if (cs) {
      XmString xms;
      Args ar;
      ar.set(name, (XtArgVal)&xms);
      XtGetValues(getWidget(), ar.getArgList(), 1);
      cs -> set(xms);
    }  
  }

  void get(const char* name, CompoundString& cs) 
  {
    XmString xms;
    Args ar;
    ar.set(name, (XtArgVal)&xms);
    XtGetValues(getWidget(), ar.getArgList(), 1);
    cs.set(xms);
  }

  
  //2015/01/23 Modified to be virtual
  virtual void get(const char* name, XtArgVal value) 
  {
    Args ar;
    ar.set(name, value);
    XtGetValues(getWidget(), ar.getArgList(), 1);
  }

  virtual void get(const char* name, Dimension& value) 
  {
    Args ar;
    ar.set(name, (XtArgVal)&value);
    XtGetValues(getWidget(), ar.getArgList(), 1);
  }
  
  virtual void get(const char* name, Position& value) 
  {
    Args ar;
    ar.set(name, (XtArgVal)&value);
    XtGetValues(getWidget(), ar.getArgList(), 1);
  }

  virtual void get(const char* name, int& value) 
  {
    Args ar;
    ar.set(name, (XtArgVal)&value);
    XtGetValues(getWidget(), ar.getArgList(), 1);
  }

  //2017/05/10
  XtArgVal get(const char* name)
  {
    XtArgVal value;
    Args ar;
    ar.set(name, (XtArgVal)&value);
    XtGetValues(getWidget(), ar.getArgList(), 1);
    return value;
  }

  Display* getDisplay() const 
  {
    return XtDisplay(widget);
  }
  
  Widget   getWidget() const 
  {
    return widget;
  }

  Window   getWindow() const 
  {
    return XtWindow(widget);
  }
  
  virtual Boolean isDialog() const 
  {
    return False;
  }
  
  Boolean  isManaged() const 
  {
    return XtIsManaged(widget);
  }
  
  Boolean  isSensitive() const 
  {
    return XtIsSensitive(widget);
  }
  
  virtual void manage() 
  {
    XtManageChild(widget);
  }
  
  void  map() 
  {
    XtMapWidget(widget);
  }
  
  void  move(Position x, Position y) 
  {
    XtMoveWidget(widget, x, y);
  }
  
  
  void overrideTranslation(Application& applet, Transducer* transd,
    CommonObject* object, Callback callback, XtPointer data)
  {
  char translation[256];
        //sprintf(translation, "%s: (%d,%s)",
        sprintf(translation, "#override %s: %s()",
        //sprintf(translation, "%s: %s()",
                        transd->getEvents(), 
                        transd->getProc());
                        //"commonActionProc"); 
                        //transd->getId(), transd->getArgs());

  Action* action = NULL;
  applet.getAppContext()
        ->addAction(transd->getProc());

  action = new Action(this, object, callback, data);
  ActionProcTable::put(widget, action, transd->getId());
        XtTranslations parsed = XtParseTranslationTable(translation); 
        printf("Translation %s\n", translation);
        if (parsed) {
    XtOverrideTranslations(widget, parsed);
          set(XmNtranslations, (XtArgVal)parsed);
        } else {
          printf("Failed to XtParseTranslationTable :%s", translation);
        }
  }

  void  realize() 
  {
    XtRealizeWidget(widget);
  }


  virtual void resize(Position x, Position y, Dimension width, Dimension height, Dimension border)
  {
    XtMoveWidget(widget, x, y);
    XtResizeWidget(widget, width, height, border);  
  }

  virtual void reshape(int x, int y, int width, int height)
  {
    XtMoveWidget(widget, x, y);
    XtResizeWidget(widget, width, height, 0);
  }

  virtual void  resize(Dimension width, Dimension height) 
  {
    XtResizeWidget(widget, width, height, 0);
  }

  void  resize(Dimension width, Dimension height, Dimension border) 
  {
    XtResizeWidget(widget, width, height, border);
  }
  
  void  resize() 
  {
    XtResizeWindow(widget);
  }
  
  void moveResize(int x, int y, unsigned int width, unsigned int height)
  {
    XMoveResizeWindow(getDisplay(), getWindow(), x, y,
      width, height);
  
  }
  
  void  removeCallback(const char* name, Action* action)
  {
    XtRemoveCallback(widget, (char*)name,
                                (XtCallbackProc)&View::commonCallback,
                                (XtPointer*)action);
    callbackList.remove(action);
  }

  void  removeAllCallbacks(const char* name)
  {
    XtRemoveAllCallbacks(widget, name);
    // To do: 
    //remove actions corresponding to the name from callbackList.
  }
  
  void  removeEventHandler(EventMask mask, Event* event)
  {
    XtRemoveEventHandler(widget, mask, False,
                                    (XtEventHandler)&View::commonHandler,
                                    (XtPointer)event);
  }


  void  removeEventHandler(EventMask mask, Boolean nonmaskable,
        Event* event)
  {
    XtRemoveEventHandler(widget, mask, nonmaskable,
                                    (XtEventHandler)&View::commonHandler,
                                    (XtPointer)event);
  }

  void   set(Args* args) 
  {
    XtSetValues(widget, args->getArgList(), args->count());
  }

  //2015/01/26
  void   set(Args& args) 
  {
    XtSetValues(widget, args.getArgList(), args.count());
  }

  virtual void set(const char* name, XtArgVal value) 
  {
    Args  ar;
    ar.set(name, value);
    XtSetValues(getWidget(), ar.getArgList(), 1);
  }

  void set(const char* name, int value) 
  {
    Args  ar;
    ar.set(name, value);
    XtSetValues(getWidget(), ar.getArgList(), 1);
  }
  
  //2015/01/26
  void set(const char* name, Pixel value) 
  {
    Args  ar;
    ar.set(name, (XtArgVal)value);
    XtSetValues(getWidget(), ar.getArgList(), 1);
  }

  void set(const char* name, double value) 
  {
    Args  ar;
    ar.set(name, value);
    XtSetValues(getWidget(), ar.getArgList(), 1);
  }

  void set(const char* name, Widget w) 
  {
    Args  ar;
    ar.set(name, w);
    XtSetValues(getWidget(), ar.getArgList(), 1);
  }

  void set(const char* name, const char* string)
  {
    Args  ar;
    ar.set(name, string);
    XtSetValues(getWidget(), ar.getArgList(), 1);
  }

  void set(const char* name, CompoundString& cstring)
  {
    Args  ar;
    ar.set(name, cstring);
    XtSetValues(getWidget(), ar.getArgList(), 1);
  }

  void set(const char* name, View* view)
  {
    Args  ar;
    ar.set(name, view);
    XtSetValues(getWidget(), ar.getArgList(), 1);
  }

  //2015/01/26
  void set(const char* name, RenderTable* renderTable) 
  {
    if (name && renderTable) {
      Args  ar;
      ar.set(name, (XtArgVal)renderTable->get());
      XtSetValues(getWidget(), ar.getArgList(), 1);
    }
  }

  void  setMappedWhenManaged(Boolean mapped) 
  {
    XtSetMappedWhenManaged(widget, mapped);
  }
  
  void  setSensitive(Boolean sensitive) 
  {
    XtSetSensitive(widget, sensitive);
  }
  
  void  enable()
  {
    setSensitive(TRUE);
  }

  void  disable()
  {
    setSensitive(False);
  }

  void   setWidget(Widget widget1) 
  {
    widget = widget1;
  }

  void  translateCoords(Position x, Position y,
        Position* rx, Position* ry) 
  {
    XtTranslateCoords(widget, x, y, rx, ry);
  }

  void  ungrabKey(KeyCode keycode, Modifiers modifiers) 
  {
    XtUngrabKey(widget, keycode, modifiers);
  }

  void  ungrabKeyboard(Time time) 
  {
    XtUngrabKeyboard(widget, time);
  }
  
  void  ungrabPointer(Time time) 
  {
    XtUngrabPointer(widget, time);
  }
  
  void  uninstallTranslations() 
  {
    XtUninstallTranslations(widget);
  }
  
  void  unmanage() 
  {
    XtUnmanageChild(widget);
  }
  
  void  unmap() 
  {
    XtUnmapWidget(widget);
  }
 
  void  update() 
  {
    XClearArea(XtDisplay(widget), XtWindow(widget), 
      0, 0, 0, 0, TRUE);
  }
  //2015/01/18  
  void processTraversal()
  {
    XmProcessTraversal(widget, XmTRAVERSE_CURRENT);
  }

  //RevertToParent, RevertToPointerRoot, or RevertToNone
  void setInputFocus(int revert_to=RevertToNone, Time time=CurrentTime)
  {
    XSetInputFocus(getDisplay(), getWindow(), revert_to, time);
  }
  
  //2015/01/18
  Screen*  getScreen()
  {
    Display* display = getDisplay();
    return ScreenOfDisplay(display, DefaultScreen(display));    
  }

  Window  getRootWindow()
  {
    Screen* scr = getScreen();
    Display* display = getDisplay();
    return RootWindow(display, XScreenNumberOfScreen(scr));
  }
  
  int grabPointer(Cursor cursor)
  {
    Window root   = getRootWindow();
    Window window   = getWindow();
    Display* display = getDisplay();
    return XGrabPointer(
        display, window, False,
        ButtonMotionMask | ButtonPressMask | ButtonReleaseMask,
        GrabModeAsync, GrabModeAsync, window, cursor, CurrentTime);
  }  
  
  int ungrabPointer()
  {
    Display* display = getDisplay();
    int rc = XUngrabPointer(display, CurrentTime);
    return rc;
  }
/*
        typedef struct {
             int x, y;
             int width, height;
             int border_width;
             int depth;
             Visual *visual;
             Window root;
             int class;
             int bit_gravity;
             int win_gravity;
             int backing_store;
             unsigned long backing_planes;
             unsigned long backing_pixel;
             Bool save_under;
             Colormap colormap;
             Bool map_installed;
             int map_state;
             long all_event_masks;
             long your_event_mask;
             long do_not_propagate_mask;
             Bool override_redirect;
             Screen *screen;
        } XWindowAttributes;
  */
  Status getAttributes(XWindowAttributes* attributes)
  {
   return XGetWindowAttributes(getDisplay(), getWindow(), attributes);
  }
  
  //2015/04/01
  void changeAttributes(unsigned long valueMask, XSetWindowAttributes& attr)
  {
    XChangeWindowAttributes(getDisplay(), getWindow(), valueMask, &attr);    
  }
    
  //2015/04/01
  void changeColormap(Colormap colormap)
  {
    XSetWindowAttributes attr;
    memset(&attr, 0, sizeof(attr));
    attr.colormap = colormap;
    XChangeWindowAttributes(getDisplay(), getWindow(), CWColormap, &attr);
  }

  int getDepth()
  {
    XWindowAttributes attributes;
    XGetWindowAttributes(getDisplay(), getWindow(), &attributes);
    return attributes.depth;
  }
     
  void queryMousePointer(int& root_x, int& root_y)
  {
  Display* display = getDisplay();
  Window root = XDefaultRootWindow(display);
  Window ret_root;
  Window ret_child;
  int win_x;
  int win_y;
  unsigned int mask;

  XQueryPointer(display, root, &ret_root, &ret_child, &root_x, &root_y,
           &win_x, &win_y, &mask);
  }
  
  void queryMousePointer(int& root_x, int& root_y, int& win_x, int& win_y)
  {
  Display* display = getDisplay();
  Window root = XDefaultRootWindow(display);
  Window ret_root;
  Window ret_child;
  unsigned int mask;

  XQueryPointer(display, root, &ret_root, &ret_child, &root_x, &root_y,
           &win_x, &win_y, &mask);
  }
  
  //2015/02/18
  Pixel whitePixel()
  {
     return WhitePixel(getDisplay(), XDefaultScreen(getDisplay()) );
  }
  
  Pixel blackPixel()
  {
     return BlackPixel(getDisplay(), XDefaultScreen(getDisplay()) );
  }
  
  Pixel grayPixel()
  {
    return color("lightgray");
  }
  
  Colormap getDefaultColormap()
  {
    return DefaultColormap(getDisplay(), XDefaultScreen(getDisplay()));
  }

  Pixel colorPixel(const char* name) 
  {
    XColor color;
    XColor exact;
    XAllocNamedColor(getDisplay(), getDefaultColormap(), name, &color, &exact);
    return color.pixel;
  }  

  Pixel color(const char* name) 
  {
    XColor color;
    XColor exact;
    XAllocNamedColor(getDisplay(), getDefaultColormap(), name, &color, &exact);
    return color.pixel;
  }  
  
  Status sendEvent(Boolean propagate, long event_mask, XEvent* event_send)
  {
    return XSendEvent(getDisplay(), getWindow(), propagate, event_mask, event_send);
  }

  void mapRaisedWindow()
  {
    XMapRaised(getDisplay(), getWindow());
  }

  void mapSubWindows()
  {
    XMapSubwindows(getDisplay(), getWindow());
  }

  void unmapSubWindows()
  {
    XUnmapSubwindows(getDisplay(), getWindow());
  }

  void raiseWindow()
  {
    XRaiseWindow(getDisplay(), getWindow());
  }
  
  void lowerWindow()
  {
    XLowerWindow(getDisplay(), getWindow());
  }
  
  void changeWindowAttributes(unsigned long valuemask, XSetWindowAttributes *attributes)
  {
    XChangeWindowAttributes(getDisplay(), getWindow(), valuemask, attributes);
  }

  //2015/03/27
  virtual void updateView()
  {
  }

  //2015/03/27
  virtual void updateModel(CommonObject* object)
  {
  }

  //2015/03/27
  void flush()
  {
    XFlush(getDisplay());
  }

  //2015/08/10

  Boolean primarySource(Time time)
  {
    return XmePrimarySource(getWidget(), time);
  }

  Boolean namedSource(Atom atom, Time time)
  {
    return XmeNamedSource(getWidget(), atom, time);
  }

  Boolean scondarySource(Time time)
  {
    return XmeSecondarySource(getWidget(), time);
  }

  void secondaryTransfer(Atom atom, XtEnum xtEnum, Time time)
  {
    XmeSecondaryTransfer(getWidget(), atom, xtEnum, time);
  }

  Boolean clipboardSource(XtEnum xtEnum, Time time)
  {
    return XmeClipboardSource(getWidget(), xtEnum, time);
  }

  Widget dragSource(XtPointer data, XEvent* xev, Args& args)
  {
     return XmeDragSource(getWidget(), data, xev, args.getArgList(), 
               args.count());
  }

  Boolean primarySink(XtEnum xtEnum, XtPointer data, Time time)
  {
    return XmePrimarySink(getWidget(), xtEnum, data, time);
  }

  Boolean namedSink(Atom atom, XtEnum xtEnum, XtPointer data, Time time)
  {
    return XmeNamedSink(getWidget(), atom, xtEnum, data, time);
  }

  Boolean secondarySink(Time time)
  {
    return XmeSecondarySink(getWidget(), time);
  }

  Boolean clipboardSink(XtEnum xtEnum, XtPointer data)
  {
     XmeClipboardSink(getWidget(), xtEnum, data);
  }

  void dropSink(Args& args)
  {
    return XmeDropSink(getWidget(), args.getArgList(),  
             args.count());
  }

  Atom* standardTargets(int target, int* targets)
  {
    return XmeStandardTargets(getWidget(), target, targets);
  }

  void standardConvert(XtPointer data, XmConvertCallbackStruct* cbs)
  {
    XmeStandardConvert(getWidget(), data, cbs);
  }

  Atom getEncodingAtom()
  {
    return XmeGetEncodingAtom(getWidget());
  }

  //2016/03/27
  void sendExposeEvent() 
  {
    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = Expose;
    xev.xexpose.window = this->getWindow();
    this -> sendEvent(false, ExposureMask, &xev);
    this -> flush(); //This may be needed to update the view on a multithreaded
                     //application.
  }

  void sendRenderRequest()
  {
    sendExposeEvent();  
  }

  void sendFocusInEvent()
  {
    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = FocusIn;
    xev.xfocus.window = this->getWindow();

    this -> sendEvent(false, FocusChangeMask, &xev);
    this -> flush(); //This may be needed to update the view
  }

  void sendConfigureEvent()
  {
    XEvent e;
    memset(&e, 0, sizeof(e));
    
    Display* display = getDisplay();
    Window   window  = getWindow();
    
    Dimension width;
    Dimension height;
    get(XmNwidth, (XtArgVal)&width);
    get(XmNheight, (XtArgVal)&height);

    e.type = ConfigureNotify;
    e.xconfigure.display = getDisplay();
    //e.xconfigure.event  = window;
    e.xconfigure.window = window;
    e.xconfigure.x = 0;
    e.xconfigure.y = 0;
    e.xconfigure.width  = width;
    e.xconfigure.height = height;
    e.xconfigure.border_width = 0; 

    this->sendEvent(False, StructureNotifyMask, &e);
    this->flush(); //This may be needed to update the view
  }

  //2016/09/15
  virtual View* getParent() 
  {
    return NULL;
  }

  //2016/09/15
  operator Widget()
  {
    return widget;
  }

  virtual void display()
  {
  }
};

}
