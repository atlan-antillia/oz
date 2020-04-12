/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  OpenCVDefaultMenu.h 
 *
 *****************************************************************************/

#pragma once
#include <oz++/CommonObject.h>

#include <oz++/motif/Menu.h>
#include <oz++/motif/MenuItem.h>

namespace OZ {

enum FILE_MENU {NEW=100, OPEN, SAVE, SAVE_AS, EXIT};
enum EDIT_MENU {CUT=200, COPY, PASTE,SELECT_ALL, FIND};
enum HELP_MENU {HELP=600, VERSION, OPENCV_VERSION};

static MenuItem FileItems[] = {
  {PUSHBUTTON,  "new",   "New",  NEW,        'N', "Ctrl<Key>N", "Ctrl+N", NULL},
  {PUSHBUTTON,  "open",  "Open", OPEN,       'O', "Ctrl<Key>O", "Ctrl+O", NULL},
  {PUSHBUTTON,  "save",  "Save", SAVE,       'S', "Ctrl<Key>S", "Ctrl+S", NULL},
  {PUSHBUTTON,  "save_as","SaveAs", SAVE_AS, ZERO, NULL,        NULL,     NULL},
  {SEPARATOR,   "sep",    "",       0,       ZERO, NULL,        NULL,     NULL},
  {PUSHBUTTON,  "exit",  "Exit", EXIT,       ZERO, NULL,        NULL,     NULL}
  };

static MenuItem EditItems[] = {
  {PUSHBUTTON,  "cut",      "Cut",  CUT,    'X', "Ctrl<Key>X", "Ctrl+X", NULL},
  {PUSHBUTTON,  "copy",     "Copy", COPY,   'C', "Ctrl<Key>C", "Ctrl+C", NULL},
  {PUSHBUTTON,  "paste",    "Paste", PASTE, 'P', "Ctrl<Key>V", "Ctrl+V", NULL},
  {PUSHBUTTON,  "select_a", "SelectAll", SELECT_ALL, 'A', "Ctrl<Key>A", "Ctrl+A", NULL},
  {PUSHBUTTON,  "find",     "Find", FIND,  'F', "Ctrl<Key>F", "Ctrl+F", NULL}
 };

static MenuItem HelpItems[] = {
  {PUSHBUTTON,  "help",      "Help",  HELP,      ZERO, NULL, NULL, NULL},
  {PUSHBUTTON,  "version",   "Version", VERSION, ZERO, NULL, NULL,NULL},
  {PUSHBUTTON,  "cversion",  "OpenCV Version", OPENCV_VERSION, ZERO, NULL, NULL,NULL}
 };
}


