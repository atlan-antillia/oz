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
 *  ImageStitcher.cpp
 *
 *****************************************************************************/

// 2019/01/01 Modified create method not to use the second parameter

//2017/10/10 We redefine Status in X11 as XtStatus to avoid type collision
// in cv::Stitcher::Status.

//#define Status XtStatus

#include <opencv2/stitching.hpp>
#include <oz++/Pair.h>
#include <oz++/SmartPtr.h>

#include <oz++/motif/Label.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/LabeledTrackBar.h>

#include <oz++/opencv/OpenCVScaleComboBox.h>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/motif/FileOpenDialog.h>
#include <opencv2/features2d/features2d.hpp>

#undef Status 

#include <opencv2/stitching.hpp>
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
  
    cv::Mat getClone()
    {
      return originalImage.clone();
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

  class StitchedImageView: public OpenCVImageView {
  private:
    cv::Mat stitchedImage; 
    cv::Mat scaledImage;

    //The scale image is displayed on this image view.
    virtual void display()
    {
      show(scaledImage);
    }
 
  public:
    StitchedImageView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
      refresh();
    } 

    ~StitchedImageView()
    {
    }
    
    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(stitchedImage, scaledImage, scalingRatio);
    }

    void setStitchedImage(cv::Mat stitched, int scalingRatio)
    {
      stitchedImage = stitched;

      scaleImage(stitchedImage, scaledImage, scalingRatio);
    }
  };
  //Inner classes end.
	
private:

  static const int          IMAGES_COUNT = 2;
  static const int          FIRST  = 0;
  static const int          SECOND = 1;
  StringT<char>             imageFiles[IMAGES_COUNT];
  
  int                       fileOpenIndex;

  SmartPtr<SourceImageView> sourceImage[IMAGES_COUNT];

  int                       imageLoadingFlag;
  int                       imageScalingRatio; //percentage 

  SmartPtr<Label>           label;
  
  SmartPtr<StitchedImageView>   stitchedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  int                       stitcherModeIndex;  
  cv::Ptr<cv::Stitcher>     stitcher;


  cv::Stitcher::Mode        mode;

  SmartPtr<LabeledComboBox> stitcherModeComboBox;

  SmartPtr<PushButton>      stitchButton; 

  SmartPtr<FileOpenDialog>  fileDialog;

  cv::Stitcher::Mode getStitcherMode(int index) 
  {
    int n = 0;
    Pair<int, cv::Stitcher::Mode> types[] = {
      {n++, cv::Stitcher::PANORAMA  },
      {n++, cv::Stitcher::SCANS     }
    };
    mode = cv::Stitcher::PANORAMA; 
    if (index >= 0 && index <n) {
      mode = types[index].second;
    } 
    return mode;
  }

  const char* getErrorMessage(cv::Stitcher::Status status)
  {
    static const Pair<const char*, cv::Stitcher::Status> errors[] = {
      {"Need more images",    
                cv::Stitcher::ERR_NEED_MORE_IMGS},
      {"Homography est fail", 
                cv::Stitcher::ERR_HOMOGRAPHY_EST_FAIL},
      {"Camera params adjust fail", 
                cv::Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL}
    };
    const char* err = "";
    for (int i = 0; i <CountOf(errors); i++) {
      if (status == errors[i].second) {
        err = errors[i].first;
        break;
      }
    }
    return err;
  }
 
  void stitch(Action& action)
  {
    stitchImages();
  }

  void stitchImages()
  {
    std::vector<cv::Mat> images;
    images.push_back( sourceImage[FIRST  ]->getClone()  );
    images.push_back( sourceImage[SECOND ]->getClone() );

    cv::Mat panorama;
    cv::Stitcher::Status status = stitcher->stitch(images, panorama);
    if (status == cv::Stitcher::OK) {
      stitchedImage->setStitchedImage(panorama, imageScalingRatio);

    } else {
      char message[1024];
      const char* err = getErrorMessage(status);
      sprintf(message, "Failed to Stitcher::stitch method error:%s", err);
      throw IException("Failed to stitch images:%s", message);      
    }
  }

  //Callback for the stitcherModeComboBox
  void stitcherModeChanged(Action& action)
  {
    int index = stitcherModeComboBox -> getSelectedPosition();
   
    if (stitcherModeIndex != index) {
      stitcherModeIndex = index;
      mode = getStitcherMode(index);
      //Rebuild stitcher.
      stitcher = cv::Stitcher::create(mode);
      stitchImages();
    }
  }

public:
  
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      sourceImage[FIRST] -> rescale(imageScalingRatio); 
      sourceImage[SECOND] -> rescale(imageScalingRatio); 
      stitchedImage -> rescale(imageScalingRatio); 
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
    if (label && sourceImage[0] && stitchedImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      sourceImage[0]-> reshape(0, LB_HEIGHT, ww/2, hh/2-4 );
      sourceImage[1] -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh/2-4);
      stitchedImage   -> reshape(0, hh/2-2, ww, hh/2); 
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
      imageFiles[FIRST]  = "../images/Lake2.png";
      imageFiles[SECOND] = "../images/Lake1.png";

      //1 Create a label to display image file names.
      Args ar;
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING); 
      label = new Label(bboard, "", ar);
      updateLabel();

      int  imageLoadingFlag = CV_LOAD_IMAGE_COLOR;

      //2 Create two sourceImages.
      for (int i = 0; i< IMAGES_COUNT; i++) {
        ar.reset();
        ar.set(XmNimageFileName, imageFiles[i]);
        ar.set(XmNimageLoadingFlag, imageLoadingFlag);
        sourceImage[i] = new SourceImageView(bboard, "", ar);
        sourceImage[i]->rescale(imageScalingRatio);
      }

      //3 Create a feature stitched imageView
      ar.reset();
      stitchedImage = new StitchedImageView(bboard, "", ar);
      
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
     
      //6 Create a stitcherModeComboBox.
      ar.reset();
      const char* items[] =  {
              "Panorama",
              "Scans",};

      stitcherModeIndex  = 0; 
      mode = cv::Stitcher::PANORAMA;

      CompoundStringList csl(items, CountOf(items));
      ar.set(XmNitems, csl);
      ar.set(XmNitemCount, CountOf(items));
      ar.set(XmNselectedPosition, stitcherModeIndex);
      stitcherModeComboBox   = new LabeledComboBox(controlPane, "FeatureDetector", ar);
      stitcherModeComboBox -> addCallback(XmNselectionCallback, this,
            (Callback)&MainView::stitcherModeChanged, NULL);

      //7 Create an instance of cv::Stitcher.
      stitcher = cv::Stitcher::create(mode);

      //8 Create a stitch push button.
      ar.reset();
      stitchButton = new PushButton(controlPane, "Stitch", ar);
      stitchButton -> addCallback(XmNactivateCallback, this,
         (Callback)&MainView::stitch, NULL);

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
    args.set(XmNheight, 540);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

