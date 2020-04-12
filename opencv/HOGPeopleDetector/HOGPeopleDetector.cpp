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
 *  HOGPeopleDetector.cpp
 *
 *****************************************************************************/

//2018/04/10
//On cv::HOGDescriptor Struct, see:  https://docs.opencv.org/3.4.1/d5/d33/structcv_1_1HOGDescriptor.html
/*
//Constructor
cv::HOGDescriptor::HOGDescriptor ( Size  _winSize, //Detection window size. Align to block size and block stride. Default value is Size(64,128). 
  Size  _blockSize,                                //Block size in pixels. Align to cell size. Default value is Size(16,16). 
  Size  _blockStride,                              //Block stride. It must be a multiple of cell size. Default value is Size(8,8). 
  Size  _cellSize,                                 //Cell size. Default value is Size(8,8). 
  int  _nbins,                                     //Number of bins used in the calculation of histogram of gradients. Default value is 9. 
  int  _derivAperture = 1,                         //not documented ?
  double  _winSigma = -1,  
  int  _histogramNormType = HOGDescriptor::L2Hys,  //histogramNormType 
  double  _L2HysThreshold = 0.2,                   //L2-Hys normalization method shrinkage. 
  bool  _gammaCorrection = false,                  //Flag to specify whether the gamma correction preprocessing is required or not. 
  int  _nlevels = HOGDescriptor::DEFAULT_NLEVELS,  //Maximum number of detection window increases. Default value is 64. 
  bool  _signedGradient = false                    //Indicates signed gradient will be used or not. 
 )  
//////////////////////////////////////////////////////
Please note the following attributes.

Public Attributes 
Size  blockSize 
  Block size in pixels. Align to cell size. Default value is Size(16,16). More...
Size  blockStride 
  Block stride. It must be a multiple of cell size. Default value is Size(8,8). More...
Size  cellSize 
  Cell size. Default value is Size(8,8). More...
int  derivAperture 
  not documented More...
float  free_coef 
  not documented More...
bool  gammaCorrection 
  Flag to specify whether the gamma correction preprocessing is required or not. More...
int  histogramNormType 
  histogramNormType More...
double  L2HysThreshold 
  L2-Hys normalization method shrinkage. More...
int  nbins 
  Number of bins used in the calculation of histogram of gradients. Default value is 9. More...
int  nlevels 
  Maximum number of detection window increases. Default value is 64. More...
UMat  oclSvmDetector 
  coefficients for the linear SVM classifier used when OpenCL is enabled More...
bool  signedGradient 
  Indicates signed gradient will be used or not. More...
std::vector< float >  svmDetector 
  coefficients for the linear SVM classifier. More...
double  winSigma 
  Gaussian smoothing window parameter. More...
Size  winSize 
  Detection window size. Align to block size and block stri 
*/

/*
train_HOG.cpp
See also: https://docs.opencv.org/3.4.1/d5/d77/train_HOG_8cpp-example.html#a34
*/

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

#include <opencv2/objdetect.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace OZ {

class MainView :public OpenCVMainView {
private:
   typedef enum {
    DEFAULT      = 0,
    DAIMLER      = 1,
    USER_DEFINED = 2
  } HOG_TYPE; 
   
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
      detectedImage = originalImage.clone();
      detectedImage.create( originalImage.size(), originalImage.type() );
      //1 Convert it to a gray image.
      cv::cvtColor(originalImage, grayImage, COLOR_BGR2GRAY); 
        
      //2 Apply equalizeHist to the gray image.
      cv::equalizeHist(grayImage, grayImage);
        
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

    void detect(HOG_TYPE descriptor,int scalingRatio)
    {
      detectedImage  = originalImage.clone();

      //Please refer the method size_t HOGDescriptor::getDescriptorSize() const in opencv-3.4.1/module/objdetect/src/hog.cpp
      //Note the following restrictions on blockSize, winSize, blockStride and cellSize:
      // (blockSize.width % cellSize.width == 0 &&
      //  blockSize.height % cellSize.height == 0);
      //((winSize.width - blockSize.width) % blockStride.width == 0 && 
      // (winSize.height - blockSize.height) % blockStride.height == 0) 
      
      cv::HOGDescriptor hog ;
  
      switch(descriptor) {
      case DEFAULT:
        hog.winSize     = cv::Size(64,128);
        hog.blockSize   = cv::Size(16, 16);
        hog.blockStride = cv::Size( 8,  8);
        hog.cellSize    = cv::Size( 8,  8);
        hog.nbins       = 9;
        hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector()); 
        break;
        
      case DAIMLER:
        hog.winSize     = cv::Size(48, 96);
        hog.blockSize   = cv::Size(16, 16);
        hog.blockStride = cv::Size( 8,  8);
        hog.cellSize    = cv::Size( 8,  8);
        hog.nbins       = 9;
        hog.setSVMDetector(cv::HOGDescriptor::getDaimlerPeopleDetector()); 
        break;
        
      case USER_DEFINED:
        hog.winSize     = cv::Size(32,64);
        hog.blockSize   = cv::Size( 8, 8);
        hog.blockStride = cv::Size( 4, 4);
        hog.cellSize    = cv::Size( 4, 4);
        hog.nbins       = 9;
        hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector()); 
        break;
      }
  
      std::vector<cv::Rect> people;
      hog.detectMultiScale(grayImage, people);
      
      for (std::vector<cv::Rect>::const_iterator it = people.begin();
                          it != people.end(); ++it) {
        cv::Rect r = *it;
        cv::rectangle(detectedImage, r.tl(), r.br(), CV_RGB(255, 0, 0), 3);
      }
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

  HOG_TYPE                    descriptorIndex;
  SmartPtr<LabeledComboBox>     descriptorComboBox;

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

      detectedImage->detect(descriptorIndex, imageScalingRatio);

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

  void descriptorChanged(Action& event)
  {
    descriptorIndex = (HOG_TYPE)descriptorComboBox -> getSelectedPosition();
    
    detectedImage -> detect(descriptorIndex, 
                                imageScalingRatio);
  }

  void clear(Action& event)
  {
    detectedImage -> clear(imageScalingRatio);
  }

  void detect(Action& event)
  {
    detectedImage -> detect(descriptorIndex,
                                imageScalingRatio);
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args) 
  {
    BulletinBoard* bboard = getBulletinBoard();
    imageFile = "../images/Pedestrian8.png";
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
      "Default",
      "Diamler",
      "UserDefined",
      };
      CompoundStringList csl(methods, CountOf(methods));

      descriptorIndex = DEFAULT;
      CompoundString descriptor("Descriptor");
      ar.reset();
      ar.set(XmNlabelString, descriptor);
      ar.set(XmNitems, csl);
      ar.set(XmNselectedPosition, descriptorIndex);
      ar.set(XmNitemCount, CountOf(methods));
      descriptorComboBox = new LabeledComboBox(controlPane, "", ar);

      descriptorComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::descriptorChanged, NULL);
     
      ar.reset();
      clearButton = new PushButton(controlPane, "Clear", ar);
      clearButton->addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::clear, NULL);

      ar.reset();
      detectButton = new PushButton(controlPane, "Detect", ar);
      detectButton->addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::detect, NULL);

      detectedImage->detect(descriptorIndex, imageScalingRatio);

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

