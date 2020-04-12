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
 *  HarrisCornerDetector.cpp
 *
 *****************************************************************************/

//2017/06/10
// See: http://docs.opencv.org/2.4/modules/imgproc/doc/
//        feature_detection.html?highlight=cornerharris#cornerharris
//
//      http://www.swarthmore.edu/NatSci/mzucker1/opencv-2.4.10-docs/
//        doc/tutorials/features2d/trackingmotion/harris_detector/harris_detector.html

//2019/01/03
#include <opencv2/stitching.hpp>
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

  class DetectedImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat grayImage; 
    cv::Mat detectedImage; 
    cv::Mat scaledImage;

    //The scale image is displayed on this image view.
    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    DetectedImageView(View* parent, const char* name, Args& args)
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

    ~DetectedImageView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      cv::cvtColor( originalImage, grayImage, COLOR_BGR2GRAY );

      detectedImage  = grayImage.clone(); //cv::Mat::zeros(originalImage.size(), CV_32FC1 );

      scaleImage(grayImage, scaledImage, scalingRatio);
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(detectedImage, scaledImage, scalingRatio);
    }

    //The following detectCornert method  is based on the code:
    //http://www.swarthmore.edu/NatSci/mzucker1/opencv-2.4.10-docs/
    //  doc/tutorials/features2d/trackingmotion/harris_detector/harris_detector.html

    int detectCorner(int blockSize, int ksize, int threshold, int scalingRatio)
    {
      //blockSize: Neighborhood size.

      //ksize : Aperture parameter for the Sobel operator, 
      // which should be odd.
      ksize = (ksize/2)*2 + 1;

      //k: Harris detector free parameter.
      double k = 0.04;

      detectedImage.release();

      cv::Mat destImage  = cv::Mat::zeros(originalImage.size(), CV_32FC1 );

      cv::cornerHarris(grayImage, destImage,
            blockSize, ksize, k, BORDER_DEFAULT );

      cv::Mat destNormalized; 
      cv::normalize(destImage, destNormalized, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );

      //We draw circles at the corners detected by cornerHarris on the originalImage 
      // by using the destNormalized image data and the threshold parameter 
      // which is used to draw the circles..

      detectedImage = originalImage.clone(); 
     int detectedNumber = 0;
      for( int j = 0; j < destNormalized.rows ; j++ ) { 
        for( int i = 0; i < destNormalized.cols; i++ ) {
          if( (int) destNormalized.at<float>(j,i) > threshold ) {
            cv::circle(detectedImage, cv::Point( i, j ), 5,  
                           cv::Scalar(0, 0, 255),  //red
                           1, 8, 0);
            detectedNumber++;
          }
        }
      }
      scaleImage(detectedImage, scaledImage, scalingRatio);
      return detectedNumber;
    }
  };
  //Inner classes end.
	
private:

  StringT<char>                imageFile;
  int                          imageLoadingFlag;
  int                          imageScalingRatio; //percentage 

  SmartPtr<Label>              label;
  SmartPtr<OriginalImageView>  originalImage;
  SmartPtr<DetectedImageView>   detectedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  int                          blockSizeMax;
  int                          blockSize;
  SmartPtr<LabeledTrackBar>    blockSizeTrackBar;
  
  int                          ksizeMax;
  int                          ksize;
  SmartPtr<LabeledTrackBar>    ksizeTrackBar;

  int                          detectorThreshold;
  SmartPtr<LabeledTrackBar>    detectorThresholdTrackBar;

  SmartPtr<Label>              detectedNumber;

  SmartPtr<FileOpenDialog>     fileDialog;

public:
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalImage -> rescale(imageScalingRatio); 
      detectedImage -> rescale(imageScalingRatio); 
    }
  }  

  void trackBarScrolled1(Action& action)
  {
    blockSize = blockSizeTrackBar->getPosition();
    ksize = ksizeTrackBar->getPosition();
    detectorThreshold = detectorThresholdTrackBar->getPosition();
    int number = detectedImage->detectCorner(blockSize, ksize, 
                     detectorThreshold, imageScalingRatio);
    setDetectedNumber(number);
  }

  void trackBarScrolled2(Action& action)
  {
    blockSize = blockSizeTrackBar->getPosition();
    ksize = ksizeTrackBar->getPosition();
    
    detectorThreshold = detectorThresholdTrackBar->getPosition();
    int number = detectedImage->detectCorner(blockSize, ksize, 
                     detectorThreshold, imageScalingRatio);
    setDetectedNumber(number);
  }

  void trackBarScrolled3(Action& action)
  {
    blockSize = blockSizeTrackBar->getPosition();
    ksize = ksizeTrackBar->getPosition();
    detectorThreshold = detectorThresholdTrackBar->getPosition();
    int number = detectedImage->detectCorner(blockSize, ksize, 
                   detectorThreshold, imageScalingRatio);
    setDetectedNumber(number);
  }

  void cancel(Action& action)
  {
    fileDialog->popdown();
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

  void setDetectedNumber(int number)
  {
    char string[128];
    sprintf(string, "DetectedNumber:%d", number);
    CompoundString cs(string);
    detectedNumber->set(XmNlabelString, cs);
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
      detectedImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      blockSize = blockSizeTrackBar->getPosition();
      ksize = ksizeTrackBar->getPosition();
      detectorThreshold = detectorThresholdTrackBar->getPosition();
      int number = detectedImage->detectCorner(blockSize, ksize, 
                                 detectorThreshold, imageScalingRatio);

      setDetectedNumber(number);

      updateLabel(filename);

      resize(width(), height());
      flush();
    } catch (OZ::Exception& ex) {
       caught(ex);
    } 
  }

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 240;
    int LB_HEIGHT = 30;
    int ww =  w-CP_WIDTH;
    int hh = h - LB_HEIGHT;
    if (label && originalImage && detectedImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      originalImage-> reshape(0, LB_HEIGHT, ww/2, hh);
      detectedImage -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
     
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
    imageFile = "../images/Geometry.png";

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

      detectedImage   = new DetectedImageView(bboard, "", ar);

      ar.reset();
      controlPane = new RowColumn(bboard, "", ar);
      const char* defaultScale = "60%";

      ar.reset();
      CompoundString scaler("Scale");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNdefaultScale, defaultScale);
      scaleComboBox = new OpenCVScaleComboBox(controlPane, "", ar);
      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);

      blockSizeMax = 100;
      ksizeMax = 31;

      blockSize = 7;
      ksize = 5;
        
      ar.reset();
      CompoundString th1Title("BlockSize:[2,100]");
      ar.set(XmNminimum, 2);
      ar.set(XmNmaximum,  blockSizeMax);
      ar.set(XmNvalue,    blockSize); 
      ar.set(XmNtitleString, th1Title);
      blockSizeTrackBar = new LabeledTrackBar(controlPane, "", ar);
      blockSizeTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled1, NULL);

      ar.reset();
      CompoundString th2Title("ApertureSize:[0,31]");
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  ksizeMax);
      ar.set(XmNvalue,    ksize);
      ar.set(XmNtitleString, th2Title);
      ksizeTrackBar = new LabeledTrackBar(controlPane, "", ar);
      ksizeTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled2, NULL);

      detectorThreshold = 120;
      ar.reset();
      CompoundString th3Title("DetectThreshold:[100,240]");
      ar.set(XmNminimum, 100);
      ar.set(XmNmaximum,  240);
      ar.set(XmNvalue,    detectorThreshold);
      ar.set(XmNtitleString, th3Title);
      detectorThresholdTrackBar = new LabeledTrackBar(controlPane, "", ar);
      detectorThresholdTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled3, NULL);

      ar.reset();
      detectedNumber = new Label(controlPane, "", ar);

      int number = detectedImage->detectCorner(blockSize, ksize, 
                                  detectorThreshold, imageScalingRatio);
      setDetectedNumber(number);

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
    args.set(XmNwidth,  1000);
    args.set(XmNheight, 380);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

