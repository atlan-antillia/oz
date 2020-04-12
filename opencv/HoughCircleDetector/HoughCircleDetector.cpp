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
 *  HoughCircleDetector.cpp
 *
 *****************************************************************************/

//2017/06/10
// See: http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_circle/hough_circle.html 

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

      detectedImage  = originalImage.clone();
      scaleImage(detectedImage, scaledImage, scalingRatio);
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(detectedImage, scaledImage, scalingRatio);
    }

    //The following method is based on the sample code of
    //http://docs.opencv.org/2.4/doc/tutorials/imgproc/
    //    imgtrans/hough_circle/hough_circle.html

    int detect(int ksize, int sigma, 
         int threshold1, int paramThreshold,
         int scalingRatio)
    {
      int detectedNumber = 0;
      //1 Apply cv::GaussianBlur to the grayImage.
      ksize = (ksize/2)*2 + 1;
      cv::GaussianBlur(grayImage, detectedImage, cv::Size(ksize, ksize), 
            (double)sigma, //sigmaX, 
            (double)sigma); //sigmaY

      //2 Apply cv::HoughCircles to the grayImage to detect circles.
      std::vector<Vec3f> circles;

      cv::HoughCircles(detectedImage, circles, CV_HOUGH_GRADIENT, 1, 
                 detectedImage.rows/8, threshold1, paramThreshold, 0, 0 );

      //3 Put a clone of ogirinalImage to the detectedImage, 
      // since the detectedImage is not necessary any more. 
      detectedImage.release();
      detectedImage = originalImage.clone();

      // Draw the circles detected
      detectedNumber = circles.size();
      for( size_t i = 0; i < circles.size(); i++ ) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        //Draw the center of the circle of radius = 3;
        cv::circle(detectedImage, center, 3, Scalar(0,255,0), -1, 8, 0 );

        //Draw the outline of the circle of radius.
        cv::circle(detectedImage, center, radius, Scalar(0,0,255), 3, 8, 0 );
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
  SmartPtr<DetectedImageView>  detectedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  SmartPtr<LabeledTrackBar>    ksizeTrackBar;
  int                          ksize;

  SmartPtr<LabeledTrackBar>    sigmaTrackBar;
  int                          sigma;

  int                          threshold1Max;
  int                          threshold1;
  SmartPtr<LabeledTrackBar>    threshold1TrackBar;
  
  int                          paramThresholdMax;
  int                          paramThreshold;
  SmartPtr<LabeledTrackBar>    paramThresholdTrackBar;

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

  void ksizeChanged(Action& action)
  {
    ksize = (int)ksizeTrackBar->get(XmNvalue);
    printf("ksizeChanged ksize=%d\n", ksize);
    set(XmNsensitive, FALSE);
    int number = detectedImage->detect(ksize, sigma, threshold1, 
                   paramThreshold, imageScalingRatio);
    set(XmNsensitive, TRUE);
    setDetectedNumber(number);
  }

  void sigmaChanged(Action& action)
  {
    sigma = sigmaTrackBar->get(XmNvalue);
    printf("sigmaChanged sigma=%d\n", sigma);
    
    set(XmNsensitive, FALSE);
    int number = detectedImage->detect(ksize, sigma, threshold1, 
                 paramThreshold, 
                 imageScalingRatio);
    set(XmNsensitive, TRUE);
    setDetectedNumber(number);
  }

  void trackBarScrolled1(Action& action)
  {
    threshold1 = threshold1TrackBar->getPosition();
    set(XmNsensitive, FALSE);
    int number = detectedImage->detect(ksize, sigma, threshold1, 
                 paramThreshold, 
                 imageScalingRatio);
    set(XmNsensitive, TRUE);
    setDetectedNumber(number);
  }

  void trackBarScrolled2(Action& action)
  {
    paramThreshold = paramThresholdTrackBar->getPosition();
    
    set(XmNsensitive, FALSE);
    int number = detectedImage->detect(ksize, sigma, threshold1, 
                 paramThreshold, 
                 imageScalingRatio);
    set(XmNsensitive, TRUE);
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

      set(XmNsensitive, FALSE);
      int number = detectedImage->detect(ksize, sigma, threshold1, 
                 paramThreshold, 
                 imageScalingRatio);
      set(XmNsensitive, TRUE);
      updateLabel(filename);
      setDetectedNumber(number);
 
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
    imageFile = "../images/Geometry3.png";

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

      ksize = 7;
      ar.reset();
      CompoundString ksizecs("GaussianBlur Kernel:[0,31]");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  31);
      ar.set(XmNvalue,    ksize); 
      ar.set(XmNtitleString, ksizecs);
      ksizeTrackBar = new LabeledTrackBar(controlPane, "", ar);
      //ksizeTrackBar->addCallback(XmNdragCallback, this,
      ksizeTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::ksizeChanged, NULL);

      ar.reset();
      CompoundString sigmacs("GaussianBlur Sigma:[0,20]");
      sigma = 2;
      ar.set(XmNlabelString, scaler);
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum, 20);
      ar.set(XmNvalue,   sigma );
      ar.set(XmNtitleString, sigmacs);
      sigmaTrackBar = new LabeledTrackBar(controlPane, "", ar);
      //sigmaTrackBar->addCallback(XmNdragCallback, this,
      sigmaTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::sigmaChanged, NULL);

      threshold1Max = 300;
      paramThresholdMax = 300;

      threshold1 = 120;
      paramThreshold = 50;
        
      ar.reset();
      CompoundString th1Title("Canny Threshold1:[0,300]");
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  threshold1Max);
      ar.set(XmNvalue,    threshold1); 
      ar.set(XmNtitleString, th1Title);
      threshold1TrackBar = new LabeledTrackBar(controlPane, "", ar);
      threshold1TrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled1, NULL);

      ar.reset();
      CompoundString th2Title("Param Threshold:[0,300]");
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  threshold1Max);
      ar.set(XmNvalue,    paramThreshold);
      ar.set(XmNtitleString, th2Title);
      paramThresholdTrackBar = new LabeledTrackBar(controlPane, "", ar);
      paramThresholdTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled2, NULL);

      ar.reset();
      detectedNumber = new Label(controlPane, "", ar);

      set(XmNsensitive, FALSE);
      int number = detectedImage->detect(ksize, sigma, threshold1, 
                 paramThreshold, 
                 imageScalingRatio);
      set(XmNsensitive, TRUE);
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
    args.set(XmNwidth,  900);
    args.set(XmNheight, 440);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

