/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ImageMorphing.cpp
 *
 *****************************************************************************/

//2017/05/28

//2019/01/03
#include <opencv2/stitching.hpp>
#include <oz++/Pair.h>
#include <oz++/motif/Label.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/LabeledTrackBar.h>
#include <oz++/opencv/OpenCVScaleComboBox.h>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/motif/FileOpenDialog.h>

namespace OZ {

class MainView :public OpenCVMainView {

private:
  ///////////////////////////////////////////////
  //Inner classes start.
  class OriginalImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat scaledImage;
    
    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    OriginalImageView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
      try {
        const char* filename = (const char*)args.get(XmNimageFileName);
        int imageLoadingFlag = args.get(XmNimageLoadingFlag);
        int scalingRatio = (int)args.get(XmNimageScalingRatio);

        loadImage(filename, imageLoadingFlag, scalingRatio);
 
      } catch (OZ::Exception ex) {
        caught(ex);
      }
    } 

    ~OriginalImageView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      scaleImage(originalImage, scaledImage, scalingRatio);
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(originalImage, scaledImage, scalingRatio);
    }
  };

  class TransformedImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat destImage; 
    cv::Mat grayImage; 
    cv::Mat scaledImage;

    //The scale image is displayed on this image view.
    virtual void display()
    {
      show(scaledImage);
    }
 
  public:
    TransformedImageView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
      try {
        const char* filename = (const char*)args.get(XmNimageFileName);
        int imageLoadingFlag = args.get(XmNimageLoadingFlag);

        int scalingRatio = (int)args.get(XmNimageScalingRatio);
        loadImage(filename, imageLoadingFlag, scalingRatio);

      } catch (OZ::Exception ex) {
        caught(ex);
      }
    } 

    ~TransformedImageView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      cv::cvtColor( originalImage, grayImage, COLOR_BGR2GRAY); 
      refresh(); 
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(destImage, scaledImage, scalingRatio);
    }

    void transform(int morphShape, int morphType, 
                int ksize, int scalingRatio)
    {
      cv::Mat element = getStructuringElement(morphShape,
                       cv::Size( (ksize/2)*2 +1, (ksize/2)*2 +1),
                       cv::Point( -1, -1) );
      
      cv::morphologyEx(originalImage, destImage, morphType, element);
      if (destImage.empty()) {
        destImage = originalImage.clone();
      } else {
        ; 
      }
      scaleImage(destImage, scaledImage, scalingRatio);
    }

  };
  //Inner classes end.
	
private:

  StringT<char>                imageFile;
  int                          imageLoadingFlag;
  int                          imageScalingRatio; //percentage 

  SmartPtr<Label>              label;
  SmartPtr<OriginalImageView>  originalImage;
  SmartPtr<TransformedImageView>   transformedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  int                           morphShape;  
  SmartPtr<LabeledComboBox>     morphShapeComboBox;
  
  int                           morphType;
  SmartPtr<LabeledComboBox>     morphTypeComboBox;

  int                           ksize;
  int                           maxKernelSize;
  SmartPtr<LabeledTrackBar>     ksizeTrackBar;
 
  SmartPtr<FileOpenDialog>      fileDialog;

  int getMorphShape()
  {
    int n = 0;
    Pair<int, int> shapes[] = {
      {n++, MORPH_RECT},
      {n++, MORPH_CROSS},
      {n++, MORPH_ELLIPSE}};
    int shape = MORPH_RECT;
    
    for (int i = 0; i<CountOf(shapes); i++) {
      if (shapes[i].first == morphShape) {
        shape = shapes[i].second;
        break;
      }
    }
    return shape;
  }

  int getMorphType()
  {
    int n = 0;
    Pair<int, int> types[] = {
      {n++, MORPH_OPEN},
      {n++, MORPH_CLOSE},
      {n++, MORPH_GRADIENT},
      {n++, MORPH_TOPHAT},
      {n++, MORPH_BLACKHAT}}
    ;
    int type = MORPH_OPEN;
    for (int i = 0; i<CountOf(types); i++) {
      if (types[i].first == morphType) {
        type = types[i].second;
        break;
      }
    }
    return type;
  }

  
  void selMorphShapeChanged(Action& event)
  {
    morphShape = morphShapeComboBox -> getSelectedPosition();
    dprintf("selMorphShapedChagned %d\n", morphShape);
    transformedImage -> transform(getMorphShape(), getMorphType(), ksize,
                                imageScalingRatio );
  }

  void selMorphTypeChanged(Action& event)
  {
    morphType = morphTypeComboBox -> getSelectedPosition();
    dprintf("selMorphTypeChanged %d\n", morphType);
    transformedImage -> transform(getMorphShape(), getMorphType(), ksize,
                                imageScalingRatio );
  }


  //Horizontal Scroll event by a horizontal TrackBar
  void trackBarScrolled(Action& action)
  {
    ksize = ksizeTrackBar->getPosition();
    transformedImage -> transform(getMorphShape(), getMorphType(), ksize,
                                imageScalingRatio );
  }
    

public:
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalImage -> rescale(imageScalingRatio); 
      transformedImage -> rescale(imageScalingRatio); 
    }
  }  

  void fileOpen(Action& action)
  {
    fileDialog->popup();
  }

  void updateLabel(const char* filename)
  {
     CompoundString cs(filename);
     label->set(XmNlabelString, cs);    
  }

  void ok(Action& action)
  {
    try {  
      imageFile  = fileDialog->getFileName();
      const char* filename = (const char*)imageFile;
      printf("filename: %s\n", filename);
      fileDialog->popdown();
      originalImage->invalidate();
      originalImage->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio);

      originalImage->invalidate();
      transformedImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      transformedImage -> transform(getMorphShape(), getMorphType(), ksize,
                                imageScalingRatio );

      updateLabel(filename);

      resize(width(), height());
      flush();
    } catch (OZ::Exception& ex) {
      caught(ex);
    } 
  }

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 200;
    int LB_HEIGHT = 30;
    int ww =  w-CP_WIDTH;
    int hh = h - LB_HEIGHT;
    if (label && originalImage && transformedImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      originalImage-> reshape(0, LB_HEIGHT, ww/2, hh);
      transformedImage -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
     
      controlPane  -> reshape(ww-1, LB_HEIGHT, CP_WIDTH+1, hh);

      controlPane -> unmap();
      controlPane -> map();
    }

    flush();
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args) 
  {
    BulletinBoard* bboard = getBulletinBoard();
    imageFile = "../images/HelloWorld.png";

    imageLoadingFlag = CV_LOAD_IMAGE_COLOR;
    imageScalingRatio = 60; //%

    try {
      Args ar;
      CompoundString fileNamecs(imageFile);
      ar.set(XmNlabelString, fileNamecs); 
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING); 
      label = new Label(bboard, "", ar);

      ar.reset();
      ar.set(XmNimageFileName, imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);
      originalImage = new OriginalImageView(bboard, "", ar);

      ar.reset();
      ar.set(XmNimageFileName,  imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);
      transformedImage   = new TransformedImageView(bboard, "", ar);

      ar.reset();
      controlPane = new RowColumn(bboard, "", ar);

      //Create a scaleComboBox in the controlPane
      const char* defaultScale = "60%";
      ar.reset();
      CompoundString scaler("Scale");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNdefaultScale, defaultScale);
      scaleComboBox = new OpenCVScaleComboBox(controlPane, "", ar);
      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);
     
      ar.reset();
      const char* shapes[] = {
        "MORPH_RECT",   //0
        "MORPH_CROSS",  //1
        "MORPH_ELLIPSE" //2
      };
      morphShape = 2;
      CompoundStringList shapesCsl(shapes, CountOf(shapes));
      ar.set(XmNitems, shapesCsl);
      ar.set(XmNitemCount, CountOf(shapes));
      ar.set(XmNvisibleItemCount, CountOf(shapes));
      ar.set(XmNselectedPosition, morphShape);
      morphShapeComboBox = new LabeledComboBox(controlPane, "MorphShape", ar);
      morphShapeComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::selMorphShapeChanged, NULL);
      
      ar.reset();
      const char* types[] = {
        "MORPH_OPEN",
        "MORPH_CLOSE",
        "MORPH_GRADIENT",
        "MORPH_TOPHAT",
        "MORPH_BLACKHAT",
      };
      morphType = 2;
      CompoundStringList typeCsl(types, CountOf(types));
      ar.set(XmNitems, typeCsl);
      ar.set(XmNitemCount, CountOf(types));
      ar.set(XmNvisibleItemCount, CountOf(types));
      ar.set(XmNselectedPosition, morphType);
      morphTypeComboBox = new LabeledComboBox(controlPane, "MorphType", ar);
      
      //morphTypeComboBox -> setCurSel(morphType);
      morphTypeComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::selMorphTypeChanged, NULL);

      maxKernelSize = 31;
      ksize = 4;          //starting kernelSize
      CompoundString ksizeCsl("KernelSize:[0, 31]"); 
      ar.reset();
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum, maxKernelSize);
      ar.set(XmNvalue, ksize);
      ar.set(XmNtitleString, ksizeCsl);
      ksizeTrackBar = new LabeledTrackBar(controlPane, "KernelSize", ar);
      ksizeTrackBar -> addCallback(XmNvalueChangedCallback, this,  
        (Callback)&MainView::trackBarScrolled, NULL);
 
 
      transformedImage -> transform(getMorphShape(), getMorphType(), ksize,
                                imageScalingRatio );
      ar.reset();
      fileDialog = new FileOpenDialog(this, "FileOpenDialog", ar);
      fileDialog  -> getOkButton()
                  -> addCallback(XmNactivateCallback, this,
                          (Callback)&MainView::ok, NULL); 
      sendConfigureEvent(); 
    } catch(OZ::Exception& ex) {
      caught(ex);
    }
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
    const char*  appclass = argv[0];
    OpenCVApplication applet(appclass, argc, argv);

    Args args;
    args.set(XmNwidth,  900);
    args.set(XmNheight, 380);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

