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
 *  FeatureMatcher.cpp
 *
 *****************************************************************************/

// This is an elementary feature matching sample program, based on OpenCV2.4.8
//

//2017/05/30

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
#include "PrivateFileMenu.h"

namespace OZ {

class MainView :public OpenCVMainView {

private:
  ///////////////////////////////////////////////
  //Inner classes start.
  class SourceImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat scaledImage;
    
    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    SourceImageView(View* parent, const char* name, Args& args)
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

    ~SourceImageView()
    {
    }
    
    cv::Mat& getMat()
    {
      return originalImage;
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

  class MatchedImageView: public OpenCVImageView {
  private:
    cv::Mat matchedImage; 
    cv::Mat scaledImage;

    //The scale image is displayed on this image view.
    virtual void display()
    {
      show(scaledImage);
    }
 
  public:
    MatchedImageView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
      refresh();
    } 

    ~MatchedImageView()
    {
    }
    
    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(matchedImage, scaledImage, scalingRatio);
    }

    void setMatched(cv::Mat& mat, int scalingRatio)
    {
      matchedImage.release();
      
      matchedImage = mat;
      scaleImage(matchedImage, scaledImage, scalingRatio);
    }

  };
  //Inner classes end.
	
private:
  typedef enum {
      /* AKAZEFeatureDetector */   DETECTOR_AKAZE=0,
      /* BRISKFeatureDetector */   DETECTOR_BRISK,
      /* ORBFeatureDetector   */   DETECTOR_ORB,
  } DETECTOR;


  static const int          IMAGES_COUNT = 2;
  static const int          FIRST  = 0;
  static const int          SECOND = 1;
  StringT<char>             imageFiles[IMAGES_COUNT];
  
  int                       fileOpenIndex;

  SmartPtr<SourceImageView> sourceImage[IMAGES_COUNT];

  int                       imageLoadingFlag;
  int                       imageScalingRatio; //percentage 

  SmartPtr<Label>           label;
  
  SmartPtr<MatchedImageView>   matchedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  int                       detectorIndex;  
  SmartPtr<LabeledComboBox> detectorComboBox;

  SmartPtr<Label>           matchedNumber;
  int                       bestTopNumberIndex;
  SmartPtr<LabeledComboBox> bestTopNumberComboBox;
  

  SmartPtr<PushButton>      matchButton; 

  SmartPtr<FileOpenDialog>  fileDialog;

  
  //Callback for the matchButton
  void match(Action& action)
  {
    matchFeature(); 
  }
  
  void setMatchedNumber(int bestMatched, int totalMatchedNumber)
  {
    char number[128];
    sprintf(number, //CountOf(number), 
         "Matched Number: %d / %d", bestMatched, totalMatchedNumber);
    
    matchedNumber->setText(number);
  }
  
  //Matching operation. 
  void matchFeature()
  {
    try {
      cv::Ptr<cv::Feature2D> feature = cv::ORB::create();

      switch (detectorIndex) {
        
      case DETECTOR_AKAZE:
            feature =  cv::AKAZE::create();
            break;
        
      case DETECTOR_BRISK:
            feature =  cv::BRISK::create();
            break;
        
        case DETECTOR_ORB:
            feature =  cv::ORB::create();  //use default argments
            break;
      default:
        break;
      } 
      std::vector<cv::KeyPoint> keypoints[IMAGES_COUNT]; 
      cv::Mat                   descriptors[IMAGES_COUNT];
      cv::Mat                   mats[IMAGES_COUNT];
    
      //Detect features and compute descriptors.
      for (int i = 0; i<IMAGES_COUNT; i++) {
        mats[i] = sourceImage[i] -> getMat();
        feature -> detect(mats[i], keypoints[i]);
        feature -> compute(mats[i], keypoints[i], descriptors[i]);
      }
      
      //Brute force matcher
      cv::BFMatcher    matcher(NORM_HAMMING, true);
    
      std::vector<cv::DMatch> matches;
      matcher.match(descriptors[FIRST], descriptors[SECOND], matches);

      int totalMatchedNumber = matches.size();
      
      int bestTop = (bestTopNumberIndex + 1) * 10;
    
      if (bestTop <= totalMatchedNumber) {
        std::nth_element(matches.begin(), matches.begin() + bestTop - 1, matches.end());
        matches.erase(matches.begin() + bestTop, matches.end());
      }
        
      int bestMatchedNumber = matches.size();
        
      Mat matchedImg;
      drawMatches(mats[FIRST],  keypoints[FIRST], 
                mats[SECOND], keypoints[SECOND], matches, matchedImg);
      
      matchedImage ->setMatched(matchedImg, imageScalingRatio);
      
      setMatchedNumber(bestMatchedNumber, totalMatchedNumber);

    } catch (cv::Exception& ex) {
      //MessageBox(NULL, "Caught an excption", "Exception", MB_OK);
    }
  }
  
  //Callback for the detectorComboBox
  void detectorChanged(Action& action)
  {
    detectorIndex = (DETECTOR)detectorComboBox->getSelectedPosition();
    printf("detectorChanged: %d\n", detectorIndex);
    matchFeature(); 
  }

public:
  //Callback for the bestTopNumberComboBox
  void bestTopNumberChanged(Action& action)
  {
    bestTopNumberIndex = bestTopNumberComboBox->getSelectedPosition();
    printf("bestTopNumberChanged %d\n", bestTopNumberIndex); 
    matchFeature();
  }
  
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      sourceImage[FIRST] -> rescale(imageScalingRatio); 
      sourceImage[SECOND] -> rescale(imageScalingRatio); 
      matchedImage -> rescale(imageScalingRatio); 
    }
  }  

  void fileOpen(int index, Action& action)
  {
    fileOpenIndex = index;
    fileDialog->popup();
  }

  void updateLabel()
  {
    char path[PATH_MAX];
    sprintf(path, "%s, %s", 
               (const char*)imageFiles[0], (const char*)imageFiles[1]);
    label -> setText(path);
  }

  //OK button callback for FileOpenDialog 
  void ok(Action& action)
  {
    try {  
      imageFiles[fileOpenIndex]  = fileDialog->getFileName();
      const char* filename = (const char*)imageFiles[fileOpenIndex];
      printf("filename: %s\n", filename);
      fileDialog->popdown();
      sourceImage[fileOpenIndex]->invalidate();
      sourceImage[fileOpenIndex]->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio);

      updateLabel();

      resize(width(), height());
      flush();

    } catch (OZ::Exception& ex) {
      caught(ex);
    } 
  }

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 220;
    int LB_HEIGHT = 30;
    int ww =  w-CP_WIDTH;
    int hh = h - LB_HEIGHT;
    if (label && sourceImage[0] && matchedImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      sourceImage[0]-> reshape(0, LB_HEIGHT, ww/2, hh/2-4 );
      sourceImage[1] -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh/2-4);
      matchedImage   -> reshape(0, hh/2-2, ww, hh/2); 
      controlPane  -> reshape(ww-1, LB_HEIGHT, CP_WIDTH+1, hh);

      controlPane -> unmap();
      controlPane -> map();
    }

    flush();
  }

  //Callback to handle file pulldown events. 
  virtual void privateFileMenu(Action& action)
  {
    menuId = action.getClientIntData();

    switch(menuId) {
    case NEW: 
        fileNew(action);
        break;
        
    case OPEN_FILE1: 
        fileOpen(FIRST, action);
        break;
       
    case OPEN_FILE2: 
        fileOpen(SECOND, action);
        break;
       
    case SAVE: 
        fileSave(action);
        break;
        
    case SAVE_AS: 
        fileSaveAs(action);
        break;
        
    case EXIT: 
        confirm(action); 
        break;
    }
  }
 
  virtual void addFilePulldownMenu()
  {
    //1 Recreate pulldown menu for File.
    MenuBar* menubar = getMenuBar();
    getFileMenuButton() -> recreatePulldownMenu(menubar, "file");

    //2 Add PrivateFileItems to the FilePulldown.
    getFileMenuButton() -> addItems(PrivateFileItems, XtNumber(PrivateFileItems),
                    this, (Callback)&MainView::privateFileMenu);
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args) 
  {
    addFilePulldownMenu();

    BulletinBoard* bboard = getBulletinBoard();
    imageLoadingFlag = CV_LOAD_IMAGE_COLOR;
    imageScalingRatio = 60; //%

    try {
      imageFiles[FIRST]  = "../images/Tower1.png";
      imageFiles[SECOND] = "../images/Tower2.png";

      //1 Create a label to display image file names.
      Args ar;
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING); 
      label = new Label(bboard, "", ar);
      updateLabel();

      int  imageLoadingFlag = CV_LOAD_IMAGE_COLOR;

      //2 Create two sourceImageViews.
      for (int i = 0; i< IMAGES_COUNT; i++) {
        ar.reset();
        const StringT<char> fullpath = getFullpath(imageFiles[i]);
        ar.set(XmNimageFileName,  fullpath);
        ar.set(XmNimageLoadingFlag, imageLoadingFlag);
        sourceImage[i] = new SourceImageView(bboard, "", ar);
        sourceImage[i]->rescale(imageScalingRatio);
      }

      //3 Create a feature matched imageView
      ar.reset();
      matchedImage = new MatchedImageView(bboard, "", ar);
      
      //4 Create a rowColumn controlPane to manage controls.
      ar.reset();
      controlPane = new RowColumn(bboard, "", ar);

      //5 Create a scaleComboBox in the controlPane
      const char* defaultScale = "60%";
      ar.reset();
      CompoundString scaler("Scale");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNdefaultScale, defaultScale);
      scaleComboBox = new OpenCVScaleComboBox(controlPane, "", ar);
      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);
     
      //6 Create a detectorComboBox.
      ar.reset();
      const char* items[] =  {
              "AKAZEFeatureDetector",
              "BRISKFeatureDetector",
              "ORBFeatureDetector"};

      detectorIndex  = DETECTOR_ORB;
      CompoundStringList csl(items, CountOf(items));
      ar.set(XmNitems, csl);
      ar.set(XmNitemCount, CountOf(items));
      ar.set(XmNselectedPosition, detectorIndex);
      detectorComboBox   = new LabeledComboBox(controlPane, "FeatureDetector", ar);
      detectorComboBox -> addCallback(XmNselectionCallback, this,
            (Callback)&MainView::detectorChanged, NULL);


      //7 Create a bestTopNumbercombobox
      const char* numbers[] = {
         "10", "20", "30", "40", "50", "60", "70", "80", "90", "100",
      };
      CompoundStringList numbersCsl(numbers, CountOf(numbers));
      bestTopNumberIndex = 1;
      ar.reset();
      ar.set(XmNitems, numbersCsl);
      ar.set(XmNitemCount, CountOf(numbers));
      ar.set(XmNselectedPosition, bestTopNumberIndex); 
      bestTopNumberComboBox = new LabeledComboBox(controlPane, "BestTopNumber", ar);
      
      bestTopNumberComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::bestTopNumberChanged, NULL);

      //8 Create a match push button.
      ar.reset();
      matchButton = new PushButton(controlPane, "Match", ar);
      matchButton -> addCallback(XmNactivateCallback, this,
         (Callback)&MainView::match, NULL);

      //9 Create a label to display the number of matched features.
      ar.reset();
      matchedNumber = new Label(controlPane, "", ar);

      //10 Call matchFeature method to detect features in sourceimages
      // and to compute the matched descriptors from the detected features.
      matchFeature();

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
    args.set(XmNheight, 500);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

