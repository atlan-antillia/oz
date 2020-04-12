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
 *  FeatureDetector.cpp
 *
 *****************************************************************************/

//2017/05/15

//2019/01/03
#include <opencv2/stitching.hpp>
#include <oz++/motif/Label.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/LabeledComboBox.h>
#include <oz++/motif/LabeledTrackBar.h>
#include <oz++/opencv/OpenCVScaleComboBox.h>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/motif/FileOpenDialog.h>
#include <opencv2/features2d/features2d.hpp>

namespace OZ {

class MainView :public OpenCVMainView {
private:
  typedef enum {
      /* AgastFeatureDetector */   DETECTOR_AGAST,
      /* AKAZEFeatureDetector */   DETECTOR_AKAZE,
      /* BRISKFeatureDetector */   DETECTOR_BRISK,
      /* FastFeatureDetector  */   DETECTOR_FAST,
      /* GFTTDetector         */   DETECTOR_GFTT,
      /* KAZEFeatureDetector  */   DETECTOR_KAZE,
      /* MSERFeatureDetector  */   DETECTOR_MSER,
      /* ORBFeatureDetector   */   DETECTOR_ORB,
  } DETECTOR;
  
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
      detectedImage  = originalImage.clone();
      detectedImage.create( originalImage.size(), originalImage.type() );

      scaleImage(detectedImage, scaledImage, scalingRatio);
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(detectedImage, scaledImage, scalingRatio);
    }

    void clear(int scalingRatio)
    {
      cv::Mat image = originalImage.clone();
      detectedImage = image;
      scaleImage(detectedImage, scaledImage, scalingRatio);
      refresh();
    }

    void detect(DETECTOR detectorIndex,int scalingRatio)
    {
      cv::Mat image = originalImage.clone();
      
      std::vector<cv::KeyPoint> keypoints;

      switch (detectorIndex) {
        case DETECTOR_AGAST: {      
              Ptr<AgastFeatureDetector> detector =  cv::AgastFeatureDetector::create(); 
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_AKAZE:{
              Ptr<AKAZE> detector =  cv::AKAZE::create();
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_BRISK: {
              Ptr<BRISK> detector =  cv::BRISK::create();
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_FAST: {
              Ptr<FastFeatureDetector> detector =  cv::FastFeatureDetector::create();
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_GFTT:{
              Ptr<GFTTDetector> detector =  cv::GFTTDetector::create();
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_KAZE:{
              Ptr<KAZE> detector =  cv::KAZE::create();
              detector->detect(image, keypoints);              
            }
            break;
      case DETECTOR_MSER: {
              Ptr<MSER> detector =  cv::MSER::create();
              detector->detect(image, keypoints);
            }
            break;
      case DETECTOR_ORB:{
              Ptr<ORB> detector =  cv::ORB::create();  //use default argments
              detector->detect(image, keypoints);
            }
            break;
      default:
        break;
      } 
      drawKeypoints(detectedImage, keypoints, detectedImage, 
                                    cv::Scalar(0x00, 0x00, 0xff)); //RED in BGR
      scaleImage(detectedImage, scaledImage, scalingRatio);
    }
  };
  //Inner classes end.
	
private:

  StringT<char>                 imageFile;
  int                           imageLoadingFlag;
  int                           imageScalingRatio; //percentage 

  SmartPtr<Label>               label;
  SmartPtr<OriginalImageView>   originalImage;
  SmartPtr<DetectedImageView>   detectedImage;
  SmartPtr<RowColumn>           controlPane;
  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  DETECTOR                      detectorIndex;
  SmartPtr<LabeledComboBox>     detectorComboBox;

  SmartPtr<PushButton>          clearButton;
  SmartPtr<PushButton>          detectButton;
  
  SmartPtr<FileOpenDialog>      fileDialog;

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

      detectedImage->detect(detectorIndex, imageScalingRatio);

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

  void detectorChanged(Action& event)
  {
    detectorIndex = (DETECTOR)detectorComboBox -> getSelectedPosition();
    
    detectedImage -> detect(detectorIndex, 
                                imageScalingRatio);
  }

  void clear(Action& event)
  {
    detectedImage -> clear(imageScalingRatio);
  }

  void detect(Action& event)
  {
    detectedImage -> detect(detectorIndex,
                                imageScalingRatio);
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

      const char* methods[] = {
      "AgastFeatureDetector",
      "AKAZEFeatureDetector",
      "BRISKFeatureDetector",
      "FastFeatureDetector", 
      "GFTTDetector",       
      "KAZEFeatureDetector", 
      "MSERFeatureDetector",
      "ORBFeatureDetector",
      };
      CompoundStringList csl(methods, CountOf(methods));

      detectorIndex = DETECTOR_AGAST;
      CompoundString detector("Detector");
      ar.reset();
      ar.set(XmNlabelString, detector);
      ar.set(XmNitems, csl);
      ar.set(XmNselectedPosition, detectorIndex);
      ar.set(XmNitemCount, CountOf(methods));
      detectorComboBox = new LabeledComboBox(controlPane, "", ar);

      detectorComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::detectorChanged, NULL);
     
      ar.reset();
      clearButton = new PushButton(controlPane, "Clear", ar);
      clearButton->addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::clear, NULL);

      ar.reset();
      detectButton = new PushButton(controlPane, "Detect", ar);
      detectButton->addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::detect, NULL);

      detectedImage->detect(detectorIndex, imageScalingRatio);

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

