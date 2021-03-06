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
 * ShapeMatcher.cpp
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
#include "PrivateFileMenu.h"

namespace OZ {

class MainView :public OpenCVMainView {

private:
  ///////////////////////////////////////////////
  //Inner classes start.
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

    cv::Mat& getOriginalImage()
    {
      return originalImage;
    }

    cv::Mat& getBinarizedImage()
    {
      return destImage;
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
      refresh();
    }

  };
  //Inner classes end.
	
private:
  static const int          IMAGES_COUNT = 2;
  static const int          FIRST  = 0;
  static const int          SECOND = 1;
  StringT<char>             imageFiles[IMAGES_COUNT];
  
  int                       fileOpenIndex;

  int                          imageLoadingFlag;
  int                          imageScalingRatio; //percentage 

  SmartPtr<Label>              label;
  
  SmartPtr<BinarizedImageView>  binarizedImage[IMAGES_COUNT];

  SmartPtr<MatchedImageView>   matchedImage;

  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  int                          thresholdTypeIndex;
  SmartPtr<LabeledComboBox>    thresholdTypeComboBox;
  
  int                          thresholdValue;
  SmartPtr<LabeledTrackBar>    thresholdValueTrackBar;

  SmartPtr<Label>              space;

  int                          minimumSize;

  SmartPtr<LabeledTrackBar>    minimumSizeTrackBar;

  int                          maximumSize;

  SmartPtr<LabeledTrackBar>    maximumSizeTrackBar;

  SmartPtr<PushButton>         clearButton;

  SmartPtr<PushButton>         matchButton;
 
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
      binarizedImage[FIRST] -> rescale(imageScalingRatio); 
      binarizedImage[SECOND] -> rescale(imageScalingRatio); 
      matchedImage -> rescale(imageScalingRatio); 
    }
  }  

  void thresholdTypeSelChanged(Action& action)
  {
    thresholdTypeIndex  = thresholdTypeComboBox -> getSelectedPosition();
    
    thresholdValue      = thresholdValueTrackBar -> getPosition();
    binarizedImage[FIRST] -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
    binarizedImage[SECOND] -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
    shapeMatching();
  }

  void trackBarScrolled(Action& action)
  {
    thresholdTypeIndex  = thresholdTypeComboBox -> getSelectedPosition();
 
    thresholdValue      = thresholdValueTrackBar -> getPosition();
    binarizedImage[FIRST] -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
    binarizedImage[SECOND] -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
    shapeMatching();
  }

  void minimumSizeScrolled(Action& action)
  {
    minimumSize         = minimumSizeTrackBar ->getPosition();

    thresholdTypeIndex  = thresholdTypeComboBox -> getSelectedPosition();

    thresholdValue      = thresholdValueTrackBar -> getPosition();
    binarizedImage[FIRST] -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
    binarizedImage[SECOND] -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
    shapeMatching();
  }

  void maximumSizeScrolled(Action& action)
  {
    maximumSize         = maximumSizeTrackBar ->getPosition();

    thresholdTypeIndex  = thresholdTypeComboBox -> getSelectedPosition();

    thresholdValue      = thresholdValueTrackBar -> getPosition();
    binarizedImage[FIRST] -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
    binarizedImage[SECOND] -> binarize( getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio);
    shapeMatching();
  }

  void fileOpen(Action& action)
  {
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
      binarizedImage[fileOpenIndex]->invalidate();
      binarizedImage[fileOpenIndex]->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio);
      binarizedImage[fileOpenIndex] -> binarize(
                                getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio );

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
    int hh = h - LB_HEIGHT/2;
    if (label && binarizedImage[FIRST] && 
                 binarizedImage[SECOND] && 
                 matchedImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      binarizedImage[FIRST]-> reshape(0, LB_HEIGHT, ww/2, hh/2);
      binarizedImage[SECOND] -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh/2);
      matchedImage          -> reshape(0, hh/2+1, ww, hh/2-2);
 
      controlPane  -> reshape(ww-1, LB_HEIGHT, CP_WIDTH+1, hh);

      controlPane -> unmap();
      controlPane -> map();
    }

    flush();
  }

  void clear(Action& action)
  {
    //Detect connectedCommponentsWithStats on destImage 
    cv::Mat& originalImage = binarizedImage[FIRST ]->getOriginalImage();
    cv::Mat destImage      = originalImage.clone(); 
    matchedImage->setMatched(destImage, imageScalingRatio); 
  }

  void match(Action& action)
  {
    shapeMatching();
  }

  void shapeMatching()
  {
    //Detect connectedCommponentsWithStats on destImage 
    Mat labels, stats, centroids; 
    cv::Mat& originalImage = binarizedImage[FIRST ]->getOriginalImage();
    cv::Mat& srcBin = binarizedImage[FIRST ]->getBinarizedImage();
    cv::Mat& tmpBin = binarizedImage[SECOND]->getBinarizedImage();

    cv::connectedComponentsWithStats(srcBin,
            labels, stats, centroids);
     
    int tw = tmpBin.rows;
    int th = tmpBin.cols;
       
    cv::Mat destImage = originalImage.clone();
 
    const double MATCHING_THRESHOLD = 0.005;

    cv::Rect minimum(0, 0, 0, 0);
    double MIN_SIMILARITY = 1.0;
    bool found = false;
 
    for(int i=0; i < stats.rows; i++)  { 
      int x = stats.at<int>(Point(0, i)); 
      int y = stats.at<int>(Point(1, i)); 
      int w = stats.at<int>(Point(2, i)); 
      int h = stats.at<int>(Point(3, i)); 
      cv::Rect rect(x, y, w, h);

      cv::Mat rectOfInterest = srcBin(rect); 
      double similarity = cv::matchShapes(tmpBin, 
               rectOfInterest, CV_CONTOURS_MATCH_I1, 0);

      if ( (w >= minimumSize || h >= minimumSize ) && 
           (w <= maximumSize || h <= maximumSize )) { 
        if (similarity <= MIN_SIMILARITY) {
          MIN_SIMILARITY = similarity;       
          minimum = rect;
            printf("matching similarity=%.04f  x=%d y=%d w=%d h=%d\n", 
             similarity, x, y, w, h);
          found = true;
        }
      }
    } 

    if (found) {
      cv::rectangle(destImage, minimum, CV_RGB(255, 0, 0), 3);
    }

    matchedImage->setMatched(destImage, imageScalingRatio); 
  }

  void fileOpen(int index, Action& action)
  {
    fileOpenIndex = index;
    fileDialog->popup();
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
    imageFiles[FIRST]  = "../images/CatImage.png";
    imageFiles[SECOND] = "../images/CatFace.png";

    imageLoadingFlag = CV_LOAD_IMAGE_COLOR;
    imageScalingRatio = 60; //%

    try {
      Args ar;
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING); 
      label = new Label(bboard, "", ar);
      updateLabel();

      for (int i = 0; i<IMAGES_COUNT; i++) {
        ar.reset();
        ar.set(XmNimageFileName,  imageFiles[i]);
        ar.set(XmNimageLoadingFlag, imageLoadingFlag);
        ar.set(XmNimageScalingRatio, imageScalingRatio);
        binarizedImage[i] = new BinarizedImageView(bboard, "", ar);
      }
      ar.reset();
      matchedImage = new MatchedImageView(bboard, "", ar);


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
      CompoundString thresholdcs("ThresholdType"); 
      thresholdTypeIndex = 0; // Binary
      CompoundStringList typescsl(types, CountOf(types));
      ar.reset();
      ar.set(XmNitems, typescsl);
      ar.set(XmNselectedPosition, thresholdTypeIndex);
      ar.set(XmNitemCount, CountOf(types));
      ar.set(XmNlabelString, thresholdcs);
      thresholdTypeComboBox = new LabeledComboBox(controlPane, "", ar);
      thresholdTypeComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::thresholdTypeSelChanged, NULL);
     
      //Create a blockSizeTrackBar in controlPane.
      CompoundString titlecs("ThresholdValue:[0,255]"); 
     
      //Create a thresholdValueTrackBar
      thresholdValue = 60;
      ar.reset();
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum, THRESHOLD_VALUE_MAX);
      ar.set(XmNvalue, thresholdValue);
      ar.set(XmNtitleString, titlecs);
      thresholdValueTrackBar = new LabeledTrackBar(controlPane, "ThresholdValue", ar);
      thresholdValueTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled, NULL);
 
      //Create a minimumSizeTrackBar
      CompoundString minsizecs("MatchMinSize:[10,100]"); 
      minimumSize = 60;
      ar.reset();
      ar.set(XmNminimum, 10);
      ar.set(XmNmaximum, 100);
      ar.set(XmNvalue,   minimumSize);
      ar.set(XmNtitleString, minsizecs);
      minimumSizeTrackBar = new LabeledTrackBar(controlPane, "MinimumSize", ar);
      minimumSizeTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::minimumSizeScrolled, NULL);

      //Create a minimumSizeTrackBar
      CompoundString maxsizecs("MatchMaxSize:[200,400]");    
      maximumSize = 240;
      ar.reset();
      ar.set(XmNminimum, 100);
      ar.set(XmNmaximum, 400);
      ar.set(XmNvalue,   maximumSize);
      ar.set(XmNtitleString, maxsizecs);
      maximumSizeTrackBar = new LabeledTrackBar(controlPane, "MaximumSize", ar);
      maximumSizeTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::maximumSizeScrolled, NULL);

      ar.reset();
      clearButton = new PushButton(controlPane, "Clear", ar);
      clearButton -> addCallback(XmNactivateCallback, this,
        (Callback)&MainView::clear, NULL);
 
      ar.reset();
      matchButton = new PushButton(controlPane, "Match", ar);
      matchButton -> addCallback(XmNactivateCallback, this,
        (Callback)&MainView::match, NULL);
 
      binarizedImage[FIRST] -> binarize(
                                getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio );
      binarizedImage[SECOND] -> binarize(
                                getThresholdType(thresholdTypeIndex),
                                thresholdValue,
                                imageScalingRatio );
      shapeMatching();

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
    args.set(XmNheight, 560);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

