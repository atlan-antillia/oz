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
 *  AdaptiveImageThresholding.cpp
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

namespace OZ {

class MainView :public OpenCVMainView {
private:
  typedef enum {
    COLOR_MODEL_HSV = 0,
    COLOR_MODEL_YCRCB
  } COLOR_MODEL;

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

  class HistgrammedImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat destImage; 
    cv::Mat scaledImage;

    //The scale image is displayed on this image view.
    virtual void display()
    {
      show(scaledImage);
    }
 
  public:
    HistgrammedImageView(View* parent, const char* name, Args& args)
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

    ~HistgrammedImageView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      destImage = originalImage.clone();
      refresh(); 
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(destImage, scaledImage, scalingRatio);
    }

    
    void equalizeHistgram(COLOR_MODEL colorModel, int scalingRatio)
    {
      try {
        int equalizeChannel  = 0;
        int toNewColorFormat = COLOR_BGR2HSV;
        int toOldColorFormat = COLOR_HSV2BGR;
        
        switch (colorModel) {
        case COLOR_MODEL_HSV:
            equalizeChannel  = 2;
            toNewColorFormat = COLOR_BGR2HSV;
            toOldColorFormat = COLOR_HSV2BGR;
          break;
          
        case COLOR_MODEL_YCRCB:
            equalizeChannel = 0;
            toNewColorFormat = COLOR_BGR2YCrCb;
            toOldColorFormat = COLOR_YCrCb2BGR;          
          break;
          
        default:
          break;
        }
    
        cv::Mat colorModeledImage;

        //Convert color-format from BGR to HSV or YCrCB
        cvtColor(originalImage, colorModeledImage, toNewColorFormat);
        
        //Split ycrcbImage to three channles.
        std::vector<Mat> channels(3);
        
        cv::split(colorModeledImage, channels);
    
        //Equalize a channel of channels specified equalizedChannel index.
        cv::equalizeHist(channels[equalizeChannel], channels[equalizeChannel]);
    
        //Merge three channels images to ycrcbImage
        cv::merge(channels, colorModeledImage);
        
        //Convert color-format HSV orYCrCb to  BGR
        cv::cvtColor(colorModeledImage, destImage, toOldColorFormat);
        scaleImage(destImage, scaledImage, scalingRatio);

      } catch (cv::Exception& ex) {
        ; //
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
  SmartPtr<HistgrammedImageView>   histgrammedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  COLOR_MODEL                  colorModel;
  SmartPtr<LabeledComboBox>    colorModelComboBox;

  SmartPtr<FileOpenDialog>     fileDialog;

public:
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalImage -> rescale(imageScalingRatio); 
      histgrammedImage -> rescale(imageScalingRatio); 
    }
  }  

  
  //Horizontal Scroll event by TrackBars
  void colorModelChanged(Action& action)
  {
    colorModel = (COLOR_MODEL)colorModelComboBox->getSelectedPosition();
    printf("colorModelChanged %d\n", colorModel);
        
    histgrammedImage -> equalizeHistgram(colorModel, imageScalingRatio);
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
      histgrammedImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      histgrammedImage -> equalizeHistgram(colorModel, imageScalingRatio);

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
    if (label && originalImage && histgrammedImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      originalImage-> reshape(0, LB_HEIGHT, ww/2, hh);
      histgrammedImage -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
     
      controlPane  -> reshape(ww-1, LB_HEIGHT, CP_WIDTH+1, hh);

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
      histgrammedImage   = new HistgrammedImageView(bboard, "", ar);

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
     
      //Create a colorModelComboBx
      const char* models[] = {
        "HSV",
        "YCrCb",
      };
      
      CompoundStringList csl(models, CountOf(models));

      colorModel = COLOR_MODEL_HSV;
      ar.reset();
      ar.set(XmNitems, csl);
      ar.set(XmNitemCount, CountOf(models));
      ar.set(XmNvalue, colorModel);

      colorModelComboBox = new LabeledComboBox(controlPane, "ColorModel", ar);
      //Add a colorModelChanged callback to the colorModelComboBox.
      colorModelComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::colorModelChanged, NULL);
      
      histgrammedImage -> equalizeHistgram(colorModel, imageScalingRatio);

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

