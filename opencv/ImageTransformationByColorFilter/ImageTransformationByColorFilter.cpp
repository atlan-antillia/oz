/**************************************************************************
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
 *  ImageTransformationByColorFilter.cpp
 *
 *****************************************************************************/

//2017/10/10

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
  typedef enum {
    GOLD = 0,
    GRAY,
    GREEN,
    SEPIA,
    VIOLET,
  } FILTER;

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

  class TransformedImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat filteredImage; 
    cv::Mat scaledImage;

    //The scale image is displayed on this image view.
    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    TransformedImageView(View* parent, const char* name, Args& args)
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

    ~TransformedImageView()
    {
    }
    
    void loadImage(const char* filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scalingRatio=100)
    {
      originalImage = readImage(filename, imageLoadingFlag);
      filteredImage  = originalImage.clone();
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(filteredImage, scaledImage, scalingRatio);
    }

    void transform(FILTER filter, int scalingRatio)
    {
        cv::Mat filters[5];
        //gold
        filters[GOLD] = (cv::Mat_<float>(3,3) <<
          0.114f, 0.114f, 0.114f,
          0.299f, 0.299f, 0.299f,
          0.587f, 0.587f, 0.587f
        );
        //gray
        filters[GRAY] = (cv::Mat_<float>(3,3) <<
          0.5f, 0.5f, 0.5f,
          0.5f, 0.5f, 0.5f,
          0.5f, 0.5f, 0.5f);
        //green
        filters[GREEN] = (cv::Mat_<float>(3,3) <<
          0.114f, 0.114f, 0.114f,
          0.587f, 0.587f, 0.587f, 
          0.299f, 0.299f, 0.299f
        );
        //sepia
        filters[SEPIA] = (cv::Mat_<float>(3,3) << 
          0.272, 0.534, 0.131,
          0.349, 0.686, 0.168, 
          0.393, 0.769, 0.189);
        //violet
        filters[VIOLET] = (cv::Mat_<float>(3,3) <<
          0.587f, 0.587f, 0.587f,
          0.114f, 0.114f, 0.114f,
          0.299f, 0.299f, 0.299f
        );
        
        cv::Mat newImage = originalImage.clone();
      
        cv::transform(originalImage, 
                   newImage, 
                   filters[filter]); 
        filteredImage = newImage;
      
      //The transformedImage is scaled in the following way, and the scaled image
      //is displayed in this image view.
      
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
  SmartPtr<TransformedImageView>   transformedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  SmartPtr<LabeledComboBox>    filterComboBox;
  FILTER                       filter;

  SmartPtr<FileOpenDialog>     fileDialog;

public:
  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalImage -> rescale(imageScalingRatio); 
      transformedImage -> rescale(imageScalingRatio); 
    }
  }  

  void filterChanged(Action& action)
  {
    filter = (FILTER)filterComboBox->getSelectedPosition();

    printf("filterChanged filter=%d\n", filter);
    transformedImage -> transform(filter,   imageScalingRatio);
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
      transformedImage ->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      transformedImage -> transform(filter,   imageScalingRatio);
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
    if (label && originalImage && transformedImage && controlPane 
     ) {
      label        -> reshape(0, 0, w, LB_HEIGHT);      
      originalImage-> reshape(0, LB_HEIGHT, ww/2, hh);
      transformedImage -> reshape(ww/2, LB_HEIGHT, ww/2-1, hh);
     
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
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING);
      label = new Label(bboard, "", ar);
      updateLabel(imageFile);

      ar.reset();
      ar.set(XmNimageFileName, imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);

      originalImage = new OriginalImageView(bboard, "", ar);

      ar.reset();
      ar.set(XmNimageFileName,  imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);

      transformedImage   = new TransformedImageView(bboard, "", ar);

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

      ar.reset();
      CompoundString filtercs("Filter");
      const char* items[] = {
        "Gold", "Gray", "Green", "Sepia", "Violet"};

      filter = SEPIA;

      CompoundStringList csl(items, CountOf(items));
      ar.set(XmNitems, csl);
      ar.set(XmNitemCount, CountOf(items));
      ar.set(XmNselectedPosition, filter);

      filterComboBox   = new LabeledComboBox(controlPane, "", ar);
      filterComboBox -> addCallback(XmNselectionCallback, this,
        (Callback)&MainView::filterChanged, NULL);

      transformedImage -> transform(filter, imageScalingRatio);

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

