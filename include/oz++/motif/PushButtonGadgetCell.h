/+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAq-
 +ACo-
 +ACo- Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 +ACo-
 +ACo- Redistribution and use in source and binary forms, with or without
 +ACo- modification, are permitted provided that the following conditions
 +ACo- are met:
 +ACo- 1. Redistributions of source code must retain the above copyright
 +ACo-    notice, this list of conditions, and the following disclaimer.
 +ACo-
 +ACo- 2. The name of the author may not be used to endorse or promote products
 +ACo-    derived from this software without specific prior written permission.
 +ACo-
 +ACo- THIS SOFTWARE IS PROVIDED BY THE AUTHOR +AGAAYA-AS IS'' AND ANY EXPRESS OR
 +ACo- IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 +ACo- OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 +ACo- IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 +ACo- SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 +ACo- PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES+ADs- LOSS OF USE, DATA, OR PROFITS+ADs-
 +ACo- OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 +ACo- WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 +ACo- OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 +ACo- ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 +ACo-
 +ACo-
 +ACo-      PushButtonGadgetCell.h
 +ACo-
 +ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKg-/

//2017/10/10 Modified to use create method.

+ACM-pragma once

+ACM-include +ADw-oz++/motif/PushButtonGadget-.h+AD4-

namespace OZ +AHs-

class PushButtonGadgetCell :public PushButtonGadget +AHs-

public:
  PushButtonGadgetCell(View+ACo- parent, const char+ACo- name, Args+ACY- args)
  :PushButtonGadget()
  +AHs-
    PushButtonGadgetCell::create(parent, name, args)+ADs-
  +AH0-

  PushButtonGadgetCell()
  :PushButtonGadget()
  +AHs-
  +AH0-

  void create(View+ACo- parent, const char+ACo- name, Args+ACY- args)
  +AHs-
    PushButtonGadget::create(parent, name, 
               args.set(XmNborderWidth, 1)
                   .set(XmNborder, parent-+AD4-grayPixel() )
                   .set(XmNhighlightThickness, 1) 
                   .set(XmNshadowThickness, 0) 
                   .set(XmNtraversalOn, TRUE)  
                   .set(XmNrecomputeSize, False)
                   .set(XmNbackground, parent-+AD4-whitePixel()) )+ADs- 
  +AH0-
+AH0AOw-

+AH0-

