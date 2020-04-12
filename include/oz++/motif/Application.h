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
 *      Application.h
 *
 *****************************************************************************/


#pragma once

#include <oz++/CommonObject.h>
#include <oz++/motif/ApplicationContext.h>
#include <oz++/motif/IView.h>

namespace OZ {
  
class Application :public CommonObject {

private:
  int      mode;  
  ApplicationContext* appContext;

  Display *display;
  char*   progname; 
  char*    appclass;
	
  bool    cleanUp;
	
public:
  Application(const char* appclass1, int argc, char** argv)
  :appContext(NULL),
  display(NULL),
  progname(NULL), 
  appclass(NULL)
  {
    XtToolkitInitialize ();

    XtSetLanguageProc(NULL, NULL, NULL);

    appContext = new ApplicationContext();

    char* name = argv[0];
    char* p = strrchr(name, '/');
    if(p) name = ++p;
    progname = __strdup(name);
    appclass = __strdup(appclass1);

    display = XtOpenDisplay (appContext->getContext(), NULL, 
    	progname, appclass, NULL, 0, &argc, argv);
    if (!display) {
      XtWarning ("Can't open display, exiting...");
      ::exit(0);
    }
    mode = TRUE;
    cleanUp = true;
  }

  Application(const char* appclass1, char** resources, 
      int argc, char** argv)
  :appContext(NULL),
  display(NULL),
  progname(NULL), 
  appclass(NULL)
  {
    XtToolkitInitialize ();

    XtSetLanguageProc(NULL, NULL, NULL);

   appContext = new ApplicationContext();
    if(resources) {
      appContext -> setFallbackResources(resources);
    }
    char* name = argv[0];

    char* p = strrchr(name, '/');
    if(p) name = ++p;
    progname = __strdup(name);
    appclass = __strdup(appclass1);

    display = XtOpenDisplay (appContext->getContext(), NULL,
    	progname, appclass, NULL, 0, &argc, argv);
    if (!display) {
        XtWarning ("Can't open display, exiting...");
        ::exit(0);
    }
    mode = TRUE;
    cleanUp = true;

  }


  ~Application() 
  {
    if (cleanUp) {
      cleanup();
    }
  }
	
  void cleanup()
  {
    if (display) {
      XtCloseDisplay(display);
      display = NULL;	
    }

    if (appContext) {
      delete appContext;
      appContext = NULL;
    
      delete [] progname;
      progname = NULL;
      delete [] appclass;
      appclass = NULL;
    }
  }

  Display* getDisplay() const 
  {
    return display;
  }

  //2015/04/02
  Screen*  getScreen()
  {
    Display* display = getDisplay();
    return ScreenOfDisplay(display, DefaultScreen(display));		
  }
	
  Colormap defaultColorMap(int screen_number)
  {
    return XDefaultColormap(getDisplay(), screen_number);
  }

  int defaultDepth(int screen_number)
  {
    return XDefaultDepth(getDisplay(), screen_number);
  }

  Window defaultRootWindow()
  {
    return XDefaultRootWindow(getDisplay());
  }
	

  int defaultScreen()
  {
    return DefaultScreen(getDisplay());
  }
	
/*
typedef struct {
	Visual *visual;
	VisualID visualid;
	int screen;
	unsigned int depth;
	int class;
	unsigned long red_mask;
	unsigned long green_mask;
	unsigned long blue_mask;
	int colormap_size;
	int bits_per_rgb;
} XVisualInfo;
 */
	
  Status matchVisualInfo(int depth, int vclass, XVisualInfo *vinfo)
  {	
    return XMatchVisualInfo(getDisplay(), defaultScreen(), depth, vclass, vinfo);
  }

  Colormap createColormap(Visual* visual, int alloc = AllocNone)
  {
    return XCreateColormap(getDisplay(), defaultRootWindow(), visual, alloc);
  }
	
  char*  getProgName() const 
  {
    return progname;
  }

  char*  getAppClass() const 
  {
    return appclass;
  }

  ApplicationContext* getAppContext() const 
  {
    return appContext;
  }

  void  terminate() 
  {
    mode = False;
    XtAppContext context = appContext->getContext();
    XtAppSetExitFlag(context);
  }

  void   run()
  {
    XEvent event; 
    XtAppContext context = appContext->getContext();
 
    do {
      XtAppNextEvent(context, &event);
      XtDispatchEvent(&event);
    } while(XtAppGetExitFlag(context) == FALSE);
  }
 
  bool  processPendingEvent()
  {
    XEvent event;
    XtAppContext context = appContext->getContext();

    if (XtAppPending(context) && XtAppPeekEvent(context, &event)) {
        XtAppNextEvent(context, &event);
        XtDispatchEvent(&event);
      return true;
    } else {
      return false;
    }
  }

  //Please specify millisecond for the delay parameter.
  void   run(IView* view, int delay=40)
  {
    if (delay < 1 && delay >1000) {
      delay = 10;
    }
    int microsec = delay * 1000;

    XEvent event;
    XtAppContext context = appContext->getContext();

    do {
      if (XtAppPending(context) && XtAppPeekEvent(context, &event)) {
        XtAppNextEvent(context, &event);
        XtDispatchEvent(&event);
      } else {
        //Use idling time to render something in a view. 
        if (view) {
          view->render();
        }
      }
      usleep(microsec);

    } while(XtAppGetExitFlag(context) == FALSE);
  }

  
  void noCleanup()
  {
  	cleanUp = false;
  }
};

}

