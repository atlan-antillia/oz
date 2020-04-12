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
 *  AdaptiveImageThresholding.cpp
 *
 *****************************************************************************/

//2017/05/15

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

  class BinarizedImageView: public OpenCVImageView {
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
    BinarizedImageView(View* parent, const char* name, Args& args)
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

    ~BinarizedImageView()
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

    void binarize(int thresholdType, int thresholdValue, int scalingRatio)
    {
      thresholdValue = (thresholdValue/2)*2 + 1;

      try {
        cv::threshold( grayImage, destImage, thresholdValue, 
             THRESHOLD_VALUE_MAX, thresholdType );
      } catch (cv::Exception& ex) {
        //Sometimes we get an exception; I don't know the reason why it happens.
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
  SmartPtr<BinarizedImageView>   binarizedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  int                          thresholdTypeIndex;
  SmartPtr<LabeledComboBox>    thresholdTypeComboBox;
  
  int                          thresholdValue;
  SmartPtr<LabeledTrackBar>    thresholdValueTrackBar;
 
  SmartPtr<FileOpenDialog>     fileDialog;

  static const int THRESHOLD_VALUE_MAX = 255;

  static int getThresholdType(int index) 
  {
    int n = 0;
    //We don't include the THRESH_OTSU type
    Pair<int, int> types[] = {
      {n++, THRESH_BINARY     },
      {n++, THRESH_BINARY_INV },
      {n++, THRESH_TRUNC      },
      {n++, THRESH_TOZERO     },
      {n++, THRESH_TOZERO_INV },
      {n++, THRESH_OTSU       }, 
    };
    int type = THRESH_BINARY; 
    if (index >= 0 && index <n) {
      type = types[index].second;
    } 
    return type;
  }

public:
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalImage -> rescale(imageScalingRatio); 
      binarizedImage -> rescale(imageScalingRatio); 
    }
  }  

  
  void thresholdTypeSelChanged(Action& event)
  {
    thresholdTypeIndex  = thresholdTypeComboBox -> getSelectedPosition();
    
    thresholdValue      = thresholdValueTrackBar -> getPosition();
    binarizedImage -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
  }

  void trackBarScrolled(Action& event)
  {
    thresholdTypeIndex  = thresholdTypeComboBox -> getSelectedPosition();
 
    thresholdValue      = thresholdValueTrackBar -> getPosition();
    binarizedImage -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
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
      binarizedImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      binarizedImage -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
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
    if (label && originalImage && binarizedImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      originalImage-> reshape(0, LB_HEIGHT, ww/2, hh);
      binarizedImage -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
     
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
    imageFile = "../images/flower.png";
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
      ar.set(XmNimageFileName,   imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);
      binarizedImage   = new BinarizedImageView(bboard, "", ar);

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
     
      //Create a thresholdTypeComboBox in controlPane.
      const char* types[] = {
        "Binary",
        "Binary Inverted",
        "Threshold Truncated",
        "Threshold to Zero",
        "Threshold to Zero Inverted",
        //"Mask", 
        "Otsu", 
      };
      thresholdTypeIndex = 0;
      CompoundStringList typescsl(types, CountOf(types));
      ar.reset();
      ar.set(XmNitems, typescsl);
      ar.set(XmNselectedPosition, thresholdTypeIndex);
      ar.set(XmNitemCount, CountOf(types));
      thresholdTypeComboBox = new LabeledComboBox(controlPane, "", ar);
      thresholdTypeComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::thresholdTypeSelChanged, NULL);
     
      //Create a blockSizeTrackBar in controlPane.
      CompoundString titlecs("ThresholdValue:[0,255]"); 
     
      //Create a thresholdValueTrackBar
      thresholdValue = 80;
      ar.reset();
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum, THRESHOLD_VALUE_MAX);
      ar.set(XmNvalue, thresholdValue);
      ar.set(XmNtitleString, titlecs);
      thresholdValueTrackBar = new LabeledTrackBar(controlPane, "ThresholdValue", ar);
      thresholdValueTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled, NULL);
 
 
      binarizedImage -> binarize(
                                getThresholdType(thresholdTypeIndex),
                                thresholdValue,
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

