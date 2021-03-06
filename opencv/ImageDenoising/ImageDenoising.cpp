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
 *  ImageDenoising.cpp
 *
 *****************************************************************************/

//2017/05/15
// See: /usr/include/opencv2/photo/photo.hpp
// CV_EXPORTS_W void fastNlMeansDenoising( InputArray src, OutputArray dst, 
//                float h = 3,
//                int templateWindowSize = 7, 
//                int searchWindowSize = 21)

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

  class DenoisedImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat denoisedImage; 
    cv::Mat scaledImage;

    //The scaled image is displayed on this image view.
    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    DenoisedImageView(View* parent, const char* name, Args& args)
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

    ~DenoisedImageView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      denoisedImage  = originalImage.clone();
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(denoisedImage, scaledImage, scalingRatio);
    }

    void denoise(int h, int templateWindowSize, 
            int searchWindowSize, int scalingRatio)
    {
      try {
        
        fastNlMeansDenoising(originalImage, denoisedImage, (float)h, 
           templateWindowSize, searchWindowSize);

        scaleImage(denoisedImage, scaledImage, scalingRatio);
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
  SmartPtr<DenoisedImageView>   denoisedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  static const int          TEMPLATE_WINDOW_SIZE_MAX = 31;
  int                       templateWindowSize;  //7
  SmartPtr<LabeledTrackBar> templateWindowSizeTrackBar;
 
  static const int          SEARCH_WINDOW_SIZE_MAX = 31;
  int                       searchWindowSize;  //21
  SmartPtr<LabeledTrackBar> searchWindowSizeTrackBar;

  static const int          H_PARAMETER_MAX = 31;
  int                       hParameter;  //3
  SmartPtr<LabeledTrackBar> hParameterTrackBar;

  SmartPtr<FileOpenDialog>  fileDialog;

public:
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalImage -> rescale(imageScalingRatio); 
      denoisedImage -> rescale(imageScalingRatio); 
    }
  }  

  void templateWindowSizeScrolled(Action& action)
  {
    templateWindowSize = templateWindowSizeTrackBar->getPosition();
    denoisedImage -> denoise(hParameter, templateWindowSize, 
	searchWindowSize, imageScalingRatio);
  }
  
  void searchWindowSizeScrolled(Action& action)
  {
    searchWindowSize = searchWindowSizeTrackBar->getPosition();
    denoisedImage -> denoise(hParameter, templateWindowSize, 
	searchWindowSize, imageScalingRatio);
  }

  void hParameterScrolled(Action& action)
  {
    hParameter = hParameterTrackBar->getPosition();
    denoisedImage -> denoise(hParameter, templateWindowSize, 
	searchWindowSize, imageScalingRatio);
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
      denoisedImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      denoisedImage -> denoise(hParameter, templateWindowSize, 
	searchWindowSize, imageScalingRatio);

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
    if (label && originalImage && denoisedImage && controlPane ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      originalImage-> reshape(0, LB_HEIGHT, ww/2, hh);
      denoisedImage -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
     
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
    imageFile = "../images/YellowFlower.png";
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

      denoisedImage   = new DenoisedImageView(bboard, "", ar);

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

      CompoundString templateLabelcs("TemplateWindowSize[1,31]");
      CompoundString searchLabelcs("SearchWindowSize[1,31]");
      CompoundString hparameterLabelcs("HParameter[1,31]");
 
      //3 Create a hParameterTrackBar.
      hParameter    = 27;
      ar.reset();
      ar.set(XmNminimum, 1);
      ar.set(XmNmaximum, H_PARAMETER_MAX);
      ar.set(XmNvalue, hParameter);
      ar.set(XmNtitleString, hparameterLabelcs); 
      hParameterTrackBar = new LabeledTrackBar(controlPane, "H Parameter", ar);
      hParameterTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::hParameterScrolled, NULL);


      //4 Create a templateWindowSizeTrackBar.
      templateWindowSize    = 15;
      ar.reset();
      ar.set(XmNminimum, 1);
      ar.set(XmNmaximum, TEMPLATE_WINDOW_SIZE_MAX);
      ar.set(XmNvalue, templateWindowSize);
      ar.set(XmNtitleString, templateLabelcs); 
      templateWindowSizeTrackBar = new LabeledTrackBar(controlPane, "TemplateWindowSize", ar);
      templateWindowSizeTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::templateWindowSizeScrolled, NULL);
            
      //5 Create a searchWindowSizeTrackBar.
      searchWindowSize    = 13;
      ar.reset();
      ar.set(XmNminimum, 1);
      ar.set(XmNmaximum, SEARCH_WINDOW_SIZE_MAX);
      ar.set(XmNvalue, searchWindowSize);
      ar.set(XmNtitleString, searchLabelcs); 
      searchWindowSizeTrackBar = new LabeledTrackBar(controlPane, "SearchWindowSize", ar);
      searchWindowSizeTrackBar -> addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::searchWindowSizeScrolled, NULL);

      denoisedImage -> denoise(hParameter, templateWindowSize, 
		searchWindowSize, imageScalingRatio);
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

