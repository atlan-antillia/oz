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
 *  TemplateMatcher.cpp
 *
 *****************************************************************************/

// This is an elementary template matching sample program, 
// based on OpenCV3.3
// See  http://docs.opencv.org/3.2.0/de/da9/tutorial_template_matching.html
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
#include <opencv2/features2d/features2d.hpp>
#include "PrivateFileMenu.h"

namespace OZ {

class MainView :public OpenCVMainView {

private:
  ///////////////////////////////////////////////
  //Inner classes start.
  class SourceImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat matchedImage;
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

    void setMatchedImage(cv::Mat& image, int scalingRatio)
    {
      matchedImage = image;
      scaleImage(matchedImage, scaledImage, scalingRatio);      
    }

    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      matchedImage = originalImage.clone();
      scaleImage(matchedImage, scaledImage, scalingRatio);
      invalidate();
      refresh();
    }    

    void clear(int scalingRatio)
    {
      matchedImage = originalImage.clone();
      scaleImage(matchedImage, scaledImage, scalingRatio);
    }

    void rescale(int scalingRatio)
    {
      scaleImage(matchedImage, scaledImage, scalingRatio);
    }
  };

  //Template image class starts.
  class TemplateImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat scaledImage;
    
    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    TemplateImageView(View* parent, const char* name, Args& args)
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

    ~TemplateImageView()
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
      invalidate();
      refresh();
    }    

    void rescale(int scalingRatio)
    {
      scaleImage(originalImage, scaledImage, scalingRatio);
    }
  };
  //Inner classes end.
	
private:
  typedef enum {
       SQDIFF = 0,
       SQDIFF_NORMED,
       CCORR,
       CCORR_NORMED,
       CCOEFF,
       CCOEFF_NORMED,
  } MATCHING_METHOD;

  StringT<char>             sourceImageFile;
  StringT<char>             templateImageFile;
  static const int          FIRST  = 0;
  static const int          SECOND = 1;
 
  int                       fileOpenIndex;

  SmartPtr<SourceImageView>   sourceImage;
  SmartPtr<TemplateImageView> templateImage;

  int                       imageLoadingFlag;
  int                       imageScalingRatio; //percentage 

  SmartPtr<Label>           label;
  
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  int                       matchingMethod;  
  SmartPtr<LabeledComboBox> matchingMethodComboBox;

  SmartPtr<PushButton>      clearButton; 
  SmartPtr<PushButton>      matchButton; 

  SmartPtr<FileOpenDialog>  fileDialog;

  
  //Callback for the clearButton
  void clear(Action& action)
  {
    sourceImage->clear(imageScalingRatio);
  }

  //Callback for the matchButton
  void match(Action& action)
  {
    templateMatch(); 
  }
  
  //Template matching operation. 
  void templateMatch()
  {
    try {
      static const Pair<MATCHING_METHOD, int> methods[] = {
       {SQDIFF,        CV_TM_SQDIFF},
       {SQDIFF_NORMED, CV_TM_SQDIFF_NORMED},
       {CCORR,         CV_TM_CCORR},
       {CCORR_NORMED,  CV_TM_CCORR_NORMED},
       {CCOEFF,        CV_TM_CCOEFF},
       {CCOEFF_NORMED, CV_TM_CCOEFF_NORMED},
      };
      int match_method = CV_TM_SQDIFF;
      for (int i = 0; i<CountOf(methods); i++) {
        if (matchingMethod == methods[i].first) {
          match_method = methods[i].second;
        }
      }

      cv::Mat& simage = sourceImage->getMat();
      cv::Mat& timage = templateImage->getMat();
 
      int result_cols = simage.cols - timage.cols + 1;
      int result_rows = simage.rows - timage.rows + 1;

      cv::Mat result;

      result.create( result_rows, result_cols, CV_32FC1 );

      cv::matchTemplate(simage, timage, result, match_method); 
      cv::normalize( result, result, 0, 1, NORM_MINMAX, -1, cv::Mat() );

      double minVal, maxVal; 
      cv::Point minLoc, maxLoc;
      cv::Point matchLoc;

      cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

      if (match_method == TM_SQDIFF || 
         match_method == TM_SQDIFF_NORMED ) { 
        matchLoc = minLoc; 

      } else { 
        matchLoc = maxLoc; 
      }

      cv::Mat matchedImage = simage.clone();

      cv::rectangle(matchedImage, matchLoc, 
            Point( matchLoc.x + timage.cols , matchLoc.y + timage.rows ), 
            CV_RGB(255, 0, 0), 3 );

      sourceImage ->setMatchedImage(matchedImage, imageScalingRatio);
      
    } catch (cv::Exception& ex) {
      //MessageBox(NULL, "Caught an excption", "Exception", MB_OK);
    }
  }
  
  //Callback for the matchingMethodComboBox
  void matchingMethodChanged(Action& action)
  {
    matchingMethod = (MATCHING_METHOD)matchingMethodComboBox->getSelectedPosition();
    printf("matchingMethodChanged: %d\n", matchingMethod);
    templateMatch(); 
  }

public:
  
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      sourceImage  -> rescale(imageScalingRatio); 
      templateImage -> rescale(imageScalingRatio); 
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
               (const char*)sourceImageFile, 
               (const char*)templateImageFile);
    label -> setText(path);
  }

  //OK button callback for FileOpenDialog 
  void ok(Action& action)
  {
    try {  
      fileDialog->popdown();
      if (fileOpenIndex == 0) {
        sourceImageFile  = fileDialog->getFileName();
        sourceImage->invalidate();
        sourceImage->loadImage((const char*)sourceImageFile, 
		imageLoadingFlag, imageScalingRatio);
      }
      if (fileOpenIndex == 1) {
        templateImageFile  = fileDialog->getFileName();
        templateImage->invalidate();
        templateImage->loadImage((const char*)templateImageFile,
		imageLoadingFlag, imageScalingRatio);
      }
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
    if (label && sourceImage && templateImage
        && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      sourceImage  -> reshape(0,    LB_HEIGHT, ww/2,   hh );
      templateImage-> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
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
    getFileMenuButton() -> addItems(PrivateFileItems, 
                    XtNumber(PrivateFileItems),
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
      //sourceImageFile   = "../images/SlantFigure.png";
      //templateImageFile = "../images/FigureFace.png";
      //sourceImageFile   = "../images/Tokyo2020.png";
      //templateImageFile = "../images/Tokyo.png";
      sourceImageFile   = "../images/MaterializedToruses.png";
      templateImageFile = "../images/GreenMaterializedTorus.png";

      //1 Create a label to display image file names.
      Args ar;
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING); 
      label = new Label(bboard, "", ar);
      updateLabel();

      int  imageLoadingFlag = CV_LOAD_IMAGE_COLOR;
      //2 Create a sourceImageView.
      ar.reset();
      ar.set(XmNimageFileName,  sourceImageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      sourceImage = new SourceImageView(bboard, "", ar);
      sourceImage->rescale(imageScalingRatio);

      //3 Create a templateImageView.
      ar.reset();
      ar.set(XmNimageFileName, templateImageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      templateImage = new TemplateImageView(bboard, "", ar);
      templateImage->rescale(imageScalingRatio);


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
     
      //6 Create a matchingMethodComboBox.
      ar.reset();
      const char* items[] =  {
               "SQDIFF",
               "SQDIFF_NORMED",
               "CCORR",
               "CCORR_NORMED",
               "CCOEFF",
               "CCOEFF_NORMED" };

      matchingMethod  = SQDIFF;
      CompoundStringList csl(items, CountOf(items));
      ar.set(XmNitems, csl);
      ar.set(XmNitemCount, CountOf(items));
      ar.set(XmNselectedPosition, matchingMethod);
      matchingMethodComboBox   = new LabeledComboBox(controlPane, "MatchingMethod", ar);
      matchingMethodComboBox -> addCallback(XmNselectionCallback, this,
            (Callback)&MainView::matchingMethodChanged, NULL);

      //7 Create a clear push button.
      ar.reset();
      clearButton = new PushButton(controlPane, "Clear", ar);
      clearButton -> addCallback(XmNactivateCallback, this,
         (Callback)&MainView::clear, NULL);

      //8 Create a match push button.
      ar.reset();
      matchButton = new PushButton(controlPane, "Match", ar);
      matchButton -> addCallback(XmNactivateCallback, this,
         (Callback)&MainView::match, NULL);


      //9 Call match method to match template image in sourceimages
      templateMatch();

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

