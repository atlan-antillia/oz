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
 *  ScreenCapture.cpp
 *
 *****************************************************************************/

#include <oz++/motif/Application.h>
#include <oz++/motif/ScreenCapture.h>
#include <oz++/motif/Image.h>
#include <oz++/JPGFileWriter.h>


//
int main(int argc, char** argv) 
{
  int x = 0;
  int y = 0;
  int width = 640;
  int height = 480;

  const char* filename = "./screen.jpg";
  int n = 1;
  if (argc == 6 ) {
    x      = atoi(argv[n++]);
    y      = atoi(argv[n++]);
    width  = atoi(argv[n++]);
    height = atoi(argv[n++]);
    filename = argv[n];
  } else { 
    printf("Usage: %s x y width height filename\n", argv[0]); 
  } 

  try {
    const char*  appclass = argv[0];
    Application applet(appclass, argc, argv);
    ScreenCapture capture(applet);

    capture.capture(x, y, width, height);

    int quality = 80;
    capture.saveAsJPG(filename, quality);
 
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

