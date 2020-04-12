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
 *  NonPhotorealisticRendering.cpp
 *
 *****************************************************************************/

//2017/08/20
// See: https://github.com/opencv/opencv/blob/master/samples/cpp/tutorial_code/photo/non_photorealistic_rendering/npr_demo.cpp

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
     EdgePreserveSmoothingByNormalizedConvolutionFilter,
     EdgePreserveSmoothingByRecursiveFilter,
    
     DetailEnhancement,    
     MonochromePencilSketch, 
     ColorPencilSketch,
     Stylization,          
  } RendererType;

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

  class NonPhotorealisticView: public OpenCVImageView {
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
    NonPhotorealisticView(View* parent, const char* name, Args& args)
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

    ~NonPhotorealisticView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      destImage = originalImage.clone();
      scaleImage(destImage, scaledImage, scalingRatio);
      refresh(); 
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(destImage, scaledImage, scalingRatio);
    }

    void transform(RendererType rendererTypeIndex, int scalingRatio)
    {
      switch(rendererTypeIndex) {
        case EdgePreserveSmoothingByNormalizedConvolutionFilter: {
          edgePreservingFilter(originalImage, destImage,1);
        }
        break;
        
        case EdgePreserveSmoothingByRecursiveFilter: {
          edgePreservingFilter(originalImage, destImage,2);
        }
        break;
        
        case DetailEnhancement: {
          cv::detailEnhance(originalImage, destImage);
        }
        break;
        
        case MonochromePencilSketch: {
          Mat img; 
          cv::pencilSketch(originalImage, destImage, img, 10 , 0.1f, 0.03f); 
        }
        break;

        case ColorPencilSketch: {
          Mat img; 
          cv::pencilSketch(originalImage, img, destImage, 10 , 0.1f, 0.03f); 
        }
        break;
        
        case Stylization: {
          cv::stylization(originalImage, destImage);
        }
        break;
        
       default:
        break;
      }
      scaleImage(destImage, scaledImage, scalingRatio);
    }
  };
  //Inner classes end.
	
private:

  StringT<char>                imageFile;
  int                          imageLoadingFlag;
  int                          imageScalingRatio; //percentage 

  SmartPtr<Label>              label;
  SmartPtr<OriginalImageView>  originalImage;
  SmartPtr<NonPhotorealisticView>   nonPhotorealisticImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  RendererType                 rendererIndex;
  SmartPtr<LabeledComboBox>    rendererComboBox;

  SmartPtr<FileOpenDialog>     fileDialog;

public:
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalImage -> rescale(imageScalingRatio); 
      nonPhotorealisticImage -> rescale(imageScalingRatio); 
    }
  }  

  void rendererSelChanged(Action& event)
  {
    rendererIndex = (RendererType)rendererComboBox -> getSelectedPosition();
    nonPhotorealisticImage -> transform( rendererIndex,
                                imageScalingRatio );
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
    imageFile  = fileDialog->getFileName();
    const char* filename = (const char*)imageFile;
    printf("filename: %s\n", filename);
    fileDialog->popdown();
    bool result = true;
    try {  
    
      originalImage->invalidate();
      originalImage->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio);

      originalImage->invalidate();
      nonPhotorealisticImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      nonPhotorealisticImage -> transform( rendererIndex,
                                imageScalingRatio );

      updateLabel(filename);

      resize(width(), height());
      flush();
    } catch (OZ::Exception& ex) {
      caught(ex);
    } 
  }

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 300;
    int LB_HEIGHT = 30;
    int ww =  w-CP_WIDTH;
    int hh = h - LB_HEIGHT;
    if (label && originalImage && nonPhotorealisticImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT); 
      originalImage-> reshape(0, LB_HEIGHT, ww/2, hh);
      nonPhotorealisticImage -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
     
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
      nonPhotorealisticImage   = new NonPhotorealisticView(bboard, "", ar);

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

      //Create an rendererComboBox in controlPane.
      const char* methods[] = {
        "EdgePreserve Smoothing By Normalized Convolution Filter",
        "EdgePreserve Smoothing By Recursive Filter",
        "Detail Enhancement",       
        "Monochrome Pencil Sketch",
        "Color Pencil Sketch",
        "Stylization" 
      };
      CompoundStringList csl(methods, CountOf(methods));

      rendererIndex = EdgePreserveSmoothingByRecursiveFilter;
      ar.reset();
      ar.set(XmNitems, csl);
      ar.set(XmNselectedPosition, rendererIndex);
      ar.set(XmNitemCount, CountOf(methods));
      rendererComboBox = new LabeledComboBox(controlPane, "", ar);
      //Add a selChanged callback.
      rendererComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::rendererSelChanged, NULL);
     
      nonPhotorealisticImage -> transform( rendererIndex,
                                imageScalingRatio );
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
    args.set(XmNwidth,  1100);
    args.set(XmNheight, 380);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

