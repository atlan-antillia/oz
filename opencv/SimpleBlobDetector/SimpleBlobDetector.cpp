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
 *  SimpleBlobDetector.cpp
 *
 *****************************************************************************/

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
    cv::Mat grayScaleImage; 
    cv::Mat detectedImage; 
    cv::Mat scaledImage;

    //The scaled image is displayed on this image view.
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
      cv::cvtColor(originalImage, grayScaleImage, cv::COLOR_BGR2GRAY);
      detectedImage  = grayScaleImage.clone();
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(detectedImage, scaledImage, scalingRatio);
    }

    void dump(cv::SimpleBlobDetector::Params& params)
    {
      printf("thresholdStep = %f\n", params.thresholdStep);
      printf("minThreshold  = %f\n",   params.minThreshold);
      printf("maxThreshold = %f\n",    params.maxThreshold);

      printf("filterByArea  = %d\n",   params.filterByArea);
      printf("minArea = %f\n",   params.minArea);
      printf("maxArea = %f\n",   params.maxArea);

      printf("filterByColor = %d\n",   params.filterByColor);
      printf("blobColor = %d\n",   params.blobColor);

      printf("filterByCircularity = %d\n",   params.filterByCircularity);
      printf("minCircularity = %f\n",   params.minCircularity);
      printf("maxCircularity = %f\n",   params.maxCircularity);

      printf("filterByConvexity = %d\n",   params.filterByConvexity);
      printf("minConvexity = %f\n",   params.minConvexity);
      printf("maxConvexity = %f\n",   params.maxConvexity);

      printf("filterByInertia = %d\n",    params.filterByInertia);
      printf("minInertiaRatio = %f\n",   params.minInertiaRatio);
      printf("maxInertiaRatio = %f\n",   params.maxInertiaRatio);

      printf("minRepeatability = %zd\n",   params.minRepeatability);

      printf("minDistBetweenBlobs = %f\n",   params.minDistBetweenBlobs);
    }

    void detect(int minDist, int minArea, 
            int maxArea, int scalingRatio)
    {
      try {
        cv::SimpleBlobDetector::Params params;
        dump(params);

        params.thresholdStep = 10.0;
        params.minThreshold = 50.0;
        params.maxThreshold = 220.0;
 
        params.filterByArea = true;
        params.minArea = minArea;
        params.maxArea = maxArea;

        params.filterByColor = true;
        params.blobColor     = 0;

        params.filterByCircularity = true;
        params.minCircularity = 0.5;

        params.filterByConvexity = true;
        params.minConvexity = 0.8;
 
        params.filterByInertia = true;
        params.minInertiaRatio = 0.1;
 
        params.minRepeatability = 2.0;
        params.minDistBetweenBlobs= 5.0;
        params.minDistBetweenBlobs= (double)minDist;

 
        Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
        std::vector<KeyPoint> keypoints;
 
        detector->detect(grayScaleImage, keypoints);
        
        drawKeypoints(grayScaleImage, keypoints, 
              detectedImage, CV_RGB(255, 0, 0), 
              DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

        scaleImage(detectedImage, scaledImage, scalingRatio);
      } catch (cv::Exception& ex) {
      }
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

  static const int          TEMPLATE_WINDOW_SIZE_MAX = 31;
  int                       minAreaSize;  //7
  SmartPtr<LabeledTrackBar> minAreaSizeTrackBar;
 
  static const int          SEARCH_WINDOW_SIZE_MAX = 31;
  int                       maxAreaSize;  //21
  SmartPtr<LabeledTrackBar> maxAreaSizeTrackBar;

  static const int          H_PARAMETER_MAX = 31;
  int                       minDistBetweenBlobs;  //3
  SmartPtr<LabeledTrackBar> minDistBetweenBlobsTrackBar;

  SmartPtr<FileOpenDialog>  fileDialog;

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

  void minAreaSizeScrolled(Action& action)
  {
    minAreaSize = minAreaSizeTrackBar->getPosition();
    detectedImage -> detect(minDistBetweenBlobs, minAreaSize, 
	maxAreaSize, imageScalingRatio);
  }
  
  void maxAreaSizeScrolled(Action& action)
  {
    maxAreaSize = maxAreaSizeTrackBar->getPosition();
    detectedImage -> detect(minDistBetweenBlobs, minAreaSize, 
	maxAreaSize, imageScalingRatio);
  }

  void minDistBetweenBlobsScrolled(Action& action)
  {
    minDistBetweenBlobs = minDistBetweenBlobsTrackBar->getPosition();
    detectedImage -> detect(minDistBetweenBlobs, minAreaSize, 
	maxAreaSize, imageScalingRatio);
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
      detectedImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      detectedImage -> detect(minDistBetweenBlobs, minAreaSize, 
	maxAreaSize, imageScalingRatio);

      updateLabel(filename);

      resize(width(), height());
      flush();
    } catch (OZ::Exception& ex) {
       caught(ex);
    } 
  }

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 260;
    int LB_HEIGHT = 30;
    int ww =  w-CP_WIDTH;
    int hh = h - LB_HEIGHT;
    if (label && originalImage && detectedImage && controlPane ) {
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
    imageFile = "../images/cat.jpg";
    imageLoadingFlag = CV_LOAD_IMAGE_COLOR;
    imageScalingRatio = 100; //%

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

      const char* defaultScale = "100%";
      ar.reset();
      CompoundString scaler("Scale");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNdefaultScale, defaultScale);
      scaleComboBox = new OpenCVScaleComboBox(controlPane, "", ar);
      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);

      CompoundString templateLabelcs("MinArea[1,100]");
      CompoundString searchLabelcs("MaxArea[100,2000]");
      CompoundString hparameterLabelcs("MinDistBetweenBlobs[5,100]");
 
      //3 Create a minDistBetweenBlobsTrackBar.
      minDistBetweenBlobs    = 9;
      ar.reset();
      ar.set(XmNminimum, 5);
      ar.set(XmNmaximum, 100);
      ar.set(XmNvalue, minDistBetweenBlobs);
      ar.set(XmNtitleString, hparameterLabelcs); 
      minDistBetweenBlobsTrackBar = new LabeledTrackBar(controlPane, "MinDistanceBetweenBlob", ar);
      minDistBetweenBlobsTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::minDistBetweenBlobsScrolled, NULL);


      //4 Create a minAreaSizeTrackBar.
      minAreaSize    = 15;
      ar.reset();
      ar.set(XmNminimum, 1);
      ar.set(XmNmaximum, 100);
      ar.set(XmNvalue, minAreaSize);
      ar.set(XmNtitleString, templateLabelcs); 
      minAreaSizeTrackBar = new LabeledTrackBar(controlPane, "MinAreaSize", ar);
      minAreaSizeTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::minAreaSizeScrolled, NULL);
            
      //5 Create a maxAreaSizeTrackBar.
      maxAreaSize    = 130;
      ar.reset();
      ar.set(XmNminimum, 100);
      ar.set(XmNmaximum, 2000);
      ar.set(XmNvalue, maxAreaSize);
      ar.set(XmNtitleString, searchLabelcs); 
      maxAreaSizeTrackBar = new LabeledTrackBar(controlPane, "MaxAreaSize", ar);
      maxAreaSizeTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::maxAreaSizeScrolled, NULL);

      detectedImage -> detect(minDistBetweenBlobs, minAreaSize, 
		maxAreaSize, imageScalingRatio);
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
    args.set(XmNwidth,  940);
    args.set(XmNheight, 380);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();

  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;

}

