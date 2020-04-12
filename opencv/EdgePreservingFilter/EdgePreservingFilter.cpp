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
 *  EdgePreservingFilter.cpp
 *
 *****************************************************************************/

//2018/04/10
//photo.hpp

/** @brief Filtering is the fundamental operation in image and video processing. Edge-preserving smoothing
filters are used in many different applications @cite EM11 .

@param src Input 8-bit 3-channel image.
@param dst Output 8-bit 3-channel image.
@param flags Edge preserving filters:
-   **RECURS_FILTER** = 1
-   **NORMCONV_FILTER** = 2
@param sigma_s Range between 0 to 200.
@param sigma_r Range between 0 to 1.

CV_EXPORTS_W void edgePreservingFilter(InputArray src, OutputArray dst, int flags = 1,
        float sigma_s = 60, float sigma_r = 0.4f);

*/

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

  class FilteredImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat filteredImage; 
    cv::Mat scaledImage;

    //The scaled image is displayed on this image view.
    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    FilteredImageView(View* parent, const char* name, Args& args)
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

    ~FilteredImageView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      filteredImage  = originalImage.clone();

      scaleImage(filteredImage, scaledImage, scalingRatio);
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(filteredImage, scaledImage, scalingRatio);
    }

    void filter(int sigmaColor, int sigmaSpace, int scalingRatio)
    {
      try {
        int flag = RECURS_FILTER ; //NORMCONV_FILTER is very slow;
        cv::edgePreservingFilter(
            originalImage, 
            filteredImage, 
            flag,
            (double)sigmaSpace, 
            (double)sigmaColor/100.0f);
      } catch (cv::Exception& ex) {
        ; //Ignore
      }     
      scaleImage(filteredImage, scaledImage, scalingRatio);
    }
  };
  //Inner classes end.
	
private:

  StringT<char>                imageFile;
  int                          imageLoadingFlag;
  int                          imageScalingRatio; //percentage 

  SmartPtr<Label>              label;
  SmartPtr<OriginalImageView>  originalImage;
  SmartPtr<FilteredImageView>   filteredImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  static const int             SIGMA_COLOR_MAX =200;
  int                          sigmaColor;
  SmartPtr<LabeledTrackBar>    sigmaColorTrackBar;
  
  static const int             SIGMA_SPACE_MAX=100;
  int                          sigmaSpace;
  SmartPtr<LabeledTrackBar>    sigmaSpaceTrackBar;

  SmartPtr<FileOpenDialog>     fileDialog;

public:
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalImage -> rescale(imageScalingRatio); 
      filteredImage -> rescale(imageScalingRatio); 
    }
  }  

  void trackBarScrolled1(Action& action)
  {
    sigmaColor = sigmaColorTrackBar->getPosition();
    sigmaSpace = sigmaSpaceTrackBar->getPosition();
    filteredImage->filter(sigmaColor, sigmaSpace, imageScalingRatio);
  }

  void trackBarScrolled2(Action& action)
  {
    sigmaColor = sigmaColorTrackBar->getPosition();
    sigmaSpace = sigmaSpaceTrackBar->getPosition();
    
    filteredImage->filter(sigmaColor, sigmaSpace, imageScalingRatio);
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
      filteredImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      sigmaColor = sigmaColorTrackBar->getPosition();
      sigmaSpace = sigmaSpaceTrackBar->getPosition();

      filteredImage->filter(sigmaColor, sigmaSpace, imageScalingRatio);

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
    if (label && originalImage && filteredImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      originalImage-> reshape(0, LB_HEIGHT, ww/2, hh);
      filteredImage -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
     
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
    imageFile = "../images/flower.png";
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

      filteredImage   = new FilteredImageView(bboard, "", ar);

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

      sigmaColor = 160;
      sigmaSpace = 40;
        
      ar.reset();
      CompoundString th1Title("SigmaColor:[1,200]");
      ar.set(XmNminimum, 1);
      ar.set(XmNmaximum, SIGMA_COLOR_MAX); 
      ar.set(XmNvalue,    sigmaColor); 
      ar.set(XmNtitleString, th1Title);
      sigmaColorTrackBar = new LabeledTrackBar(controlPane, "", ar);
      sigmaColorTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled1, NULL);

      ar.reset();
      CompoundString th2Title("SigmaSpace:[0,100]");
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  SIGMA_SPACE_MAX);
      ar.set(XmNvalue,    sigmaSpace);
      ar.set(XmNtitleString, th2Title);
      sigmaSpaceTrackBar = new LabeledTrackBar(controlPane, "", ar);
      sigmaSpaceTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled2, NULL);


      filteredImage->filter(sigmaColor, sigmaSpace, imageScalingRatio);

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

