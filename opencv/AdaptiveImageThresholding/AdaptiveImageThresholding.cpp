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

    static const int MAX_PIXEL_VALUE = 255;
    static const int C               = 9;    //Constant subtracted from the mean or weighted mean
    
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

    void binarize(int adaptiveMethod, int thresholdType, int blockSize, int scalingRatio)
    {
      try {
        blockSize = (blockSize/2)*2 + 1;
        if (blockSize <3) {
          blockSize=3;
        }
        
        cv::adaptiveThreshold( grayImage, destImage, MAX_PIXEL_VALUE, 
            adaptiveMethod, thresholdType, blockSize,  (double)C);
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

  int                          adaptiveMethodIndex;
  SmartPtr<LabeledComboBox>    adaptiveMethodComboBox;

  int                          thresholdTypeIndex;
  SmartPtr<LabeledComboBox>    thresholdTypeComboBox;
  
  static const int             BLOCK_SIZE_MIN = 3;
  static const int             BLOCK_SIZE_MAX = 43;
  int                          blockSize;
  SmartPtr<LabeledTrackBar>    blockSizeTrackBar;
  
  SmartPtr<FileOpenDialog>     fileDialog;

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

  static int getAdaptiveMethod(int index) 
  {
    int n = 0;
    //We don't include the THRESH_OTSU type
    Pair<int, int> methods[] = {
      {n++, ADAPTIVE_THRESH_MEAN_C},  
      {n++, ADAPTIVE_THRESH_GAUSSIAN_C},  
    };
    int method = ADAPTIVE_THRESH_MEAN_C; 
    if (index >= 0 && index <n) {
      method = methods[index].second;
    } 
    return method;
  }
  

  static int getThresholdType(int index) 
  {
    int n = 0;
    Pair<int, int> types[] = {
      {n++, cv::THRESH_BINARY     },
      {n++, cv::THRESH_BINARY_INV },
    };
    int type = THRESH_BINARY; 
    if (index >= 0 && index <n) {
      type = types[index].second;
    } 
    return type;
  }

  void adaptiveMethodSelChanged(Action& event)
  {
    adaptiveMethodIndex = adaptiveMethodComboBox -> getSelectedPosition();
    thresholdTypeIndex  = thresholdTypeComboBox -> getSelectedPosition();

    blockSize           = blockSizeTrackBar -> getPosition();
    binarizedImage -> binarize( getAdaptiveMethod(adaptiveMethodIndex),
                                getThresholdType(thresholdTypeIndex),
                                blockSize,
                                imageScalingRatio );
  }
  
  void thresholdTypeSelChanged(Action& event)
  {
    
    adaptiveMethodIndex = adaptiveMethodComboBox -> getSelectedPosition();
    thresholdTypeIndex  = thresholdTypeComboBox -> getSelectedPosition();
    
    blockSize           = blockSizeTrackBar -> getPosition();
    binarizedImage -> binarize( getAdaptiveMethod(adaptiveMethodIndex), 
                                getThresholdType(thresholdTypeIndex),
                                blockSize,
                                imageScalingRatio);
  }

  void trackBarScrolled(Action& event)
  {
    adaptiveMethodIndex = adaptiveMethodComboBox -> getSelectedPosition();
    thresholdTypeIndex  = thresholdTypeComboBox -> getSelectedPosition();
 
    blockSize           = blockSizeTrackBar -> getPosition();
    binarizedImage -> binarize( getAdaptiveMethod(adaptiveMethodIndex), 
                                getThresholdType(thresholdTypeIndex),
                                blockSize,
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
    imageFile  = fileDialog->getFileName();
    const char* filename = (const char*)imageFile;
    printf("filename: %s\n", filename);
    fileDialog->popdown();
    bool result = true;
    try {  
    
      originalImage->invalidate();
      originalImage->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio);

      originalImage->invalidate();
      binarizedImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      binarizedImage -> binarize( getAdaptiveMethod(adaptiveMethodIndex), 
                                getThresholdType(thresholdTypeIndex),
                                blockSize,
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

      //The following two lines are a workaround to erase garbage.
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
    imageFile = "../images/GinzaWako.png";

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
      ar.set(XmNimageFileName, imageFile);
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

      //Create an adaptiveMethodComboBox in controlPane.
      const char* methods[] = {
        "Adaptive_Thresh Mean_C",
        "Adaptive_Thresh_Gaussian_C",
      };
      CompoundStringList csl(methods, CountOf(methods));

      adaptiveMethodIndex = 0;
      ar.reset();
      ar.set(XmNitems, csl);
      ar.set(XmNselectedPosition, adaptiveMethodIndex);
      ar.set(XmNitemCount, CountOf(methods));
      adaptiveMethodComboBox = new LabeledComboBox(controlPane, "", ar);
      //Add a selChanged callback.
      adaptiveMethodComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::adaptiveMethodSelChanged, NULL);
     
      //Create a thresholdTypeComboBox in controlPane.
      const char* types[] = {
        "Binary",
        "Binary Inverted",
      };
      thresholdTypeIndex = 0;
      CompoundStringList typescsl(types, CountOf(types));
      ar.reset();
      ar.set(XmNitems, typescsl);
      ar.set(XmNselectedPosition, thresholdTypeIndex);
      ar.set(XmNitemCount, CountOf(methods));
      thresholdTypeComboBox = new LabeledComboBox(controlPane, "", ar);
      //Add a selChanged callback.
      thresholdTypeComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::thresholdTypeSelChanged, NULL);
     
      //Create a blockSizeTrackBar in controlPane.
      CompoundString titlecs("BlockValue:[3,43]"); 
      blockSize = 7;
      ar.reset();
      ar.set(XmNtitleString, titlecs);
      ar.set(XmNminimum, BLOCK_SIZE_MIN); //3);
      ar.set(XmNmaximum, BLOCK_SIZE_MAX);
      ar.set(XmNvalue, blockSize);
      blockSizeTrackBar = new LabeledTrackBar(controlPane, "BlockValue", ar);
      blockSizeTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled, NULL);
 
      binarizedImage -> binarize( getAdaptiveMethod(adaptiveMethodIndex), 
                                getThresholdType(thresholdTypeIndex),
                                blockSize,
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

