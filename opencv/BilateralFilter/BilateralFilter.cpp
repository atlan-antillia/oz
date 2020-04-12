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
 *  BilateralFilter.cpp
 *
 *****************************************************************************/

//2017/09/25
// See: "http://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html"

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

  class BlurredImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat blurredImage; 
    cv::Mat scaledImage;

    //The scale image is displayed on this image view.
    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    BlurredImageView(View* parent, const char* name, Args& args)
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

    ~BlurredImageView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      blurredImage  = originalImage.clone();
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(blurredImage, scaledImage, scalingRatio);
    }

    void blur(int diameter, int sigmaSpaceColor, int sigmaSpaceSpace, int scalingRatio)
    {
      //void bilateralFilter(InputArray src, OutputArray dst, int diameter, 
      //            double sigmaSpaceColor, 
      //            double sigmaSpaceSpace, 
      //            int borderType=BORDER_DEFAULT )
      
      cv::Mat newImage = cv::Mat::zeros(originalImage.size(), originalImage.type() );
      cv::bilateralFilter(originalImage, newImage, diameter, 
            (double)sigmaSpaceColor, //sigmaSpaceX, 
            (double)sigmaSpaceSpace, //sigmaSpaceY
            BORDER_DEFAULT);
      blurredImage = newImage;
      scaleImage(blurredImage, scaledImage, scalingRatio);
    }
  };
  //Inner classes end.
	
private:

  StringT<char>                imageFile;
  int                          imageLoadingFlag;
  int                          imageScalingRatio; //percentage 

  SmartPtr<Label>              label;
  SmartPtr<OriginalImageView>  originalImage;
  SmartPtr<BlurredImageView>   blurredImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  SmartPtr<LabeledTrackBar>    diameterTrackBar;
  int                          diameter;

  SmartPtr<LabeledTrackBar>    sigmaColorTrackBar;
  int                          sigmaColor;

  SmartPtr<LabeledTrackBar>    sigmaSpaceTrackBar;
  int                          sigmaSpace;

  SmartPtr<FileOpenDialog>     fileDialog;

public:
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalImage -> rescale(imageScalingRatio); 
      blurredImage -> rescale(imageScalingRatio); 
    }
  }  

  void diameterChanged(Action& action)
  {
    diameter = (int)diameterTrackBar->get(XmNvalue);
    printf("diameterChanged diameter=%d\n", diameter);
    blurredImage -> blur(diameter, sigmaColor, sigmaSpace, imageScalingRatio);
  }

  void sigmaColorChanged(Action& action)
  {
    sigmaColor = (int)sigmaColorTrackBar->get(XmNvalue);
    printf("sigmaColorChanged sigmaColor=%d\n", sigmaColor);
    blurredImage -> blur(diameter, sigmaColor, sigmaSpace, imageScalingRatio);
  }

  void sigmaSpaceChanged(Action& action)
  {
    sigmaSpace = sigmaSpaceTrackBar->get(XmNvalue);
    printf("sigmaSpaceChanged sigmaSpace=%d\n", sigmaSpace);
    
    blurredImage -> blur(diameter, sigmaColor, sigmaSpace, imageScalingRatio);
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
      blurredImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      blurredImage -> blur(diameter, sigmaColor, sigmaSpace, imageScalingRatio);
      updateLabel(imageFile);

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
    if (label && originalImage && blurredImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT);      
      originalImage-> reshape(0, LB_HEIGHT, ww/2, hh);
      blurredImage -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
     
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

    imageFile = "../images/MeshedNioh.png";
    imageLoadingFlag = CV_LOAD_IMAGE_COLOR;
    imageScalingRatio = 60; //%

    try {
      Args ar;
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING);
      label = new Label(bboard, "", ar);
      updateLabel(imageFile);

      ar.reset();
      ar.set(XmNimageFileName,    imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);

      originalImage = new OriginalImageView(bboard, "", ar);

      ar.reset();
      ar.set(XmNimageFileName,   imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);

      blurredImage   = new BlurredImageView(bboard, "", ar);

      ar.reset();
      controlPane = new RowColumn(bboard, "", ar);
      const char* defaultScale= "60%";

      ar.reset();
      CompoundString scaler("Scale");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNdefaultScale, defaultScale); 
      scaleComboBox = new OpenCVScaleComboBox(controlPane, "", ar);

      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);

      diameter = 4;
      CompoundString diametercs("Diameter:[0,5]");
      ar.reset();
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum, 5);
      ar.set(XmNvalue, diameter);
      ar.set(XmNtitleString, diametercs);
      diameterTrackBar = new LabeledTrackBar(controlPane, "", ar);
      diameterTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::diameterChanged, NULL);

      sigmaColor = 160;
      ar.reset();
      CompoundString sigmaColorcs("KernelSize:[0,255]");
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  255);
      ar.set(XmNvalue,    sigmaColor); 
      ar.set(XmNtitleString, sigmaColorcs);
      sigmaColorTrackBar = new LabeledTrackBar(controlPane, "", ar);
      sigmaColorTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::sigmaColorChanged, NULL);

      ar.reset();
      CompoundString sigmaSpacecs("Sigma:[0,255]");
      sigmaSpace = 40;
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum, 255);
      ar.set(XmNvalue,   sigmaSpace );
      ar.set(XmNtitleString, sigmaSpacecs);
      sigmaSpaceTrackBar = new LabeledTrackBar(controlPane, "", ar);
      sigmaSpaceTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::sigmaSpaceChanged, NULL);

      blurredImage -> blur(diameter, sigmaColor, sigmaSpace, imageScalingRatio);

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
    args.set(XmNwidth, 900);
    args.set(XmNheight, 380);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

