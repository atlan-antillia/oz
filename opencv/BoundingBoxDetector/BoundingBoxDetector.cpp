/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  CannyEdgeDetector.cpp
 *
 *****************************************************************************/

//2017/05/15

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

  class DetectedImageView: public OpenCVImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat edgePreserved;
    cv::Mat grayImage;
    cv::Mat cannyEdgedImage;    
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
      detectedImage  = originalImage.clone();
      detectedImage.create( originalImage.size(), originalImage.type() );
      cv::cvtColor( originalImage, grayImage, COLOR_BGR2GRAY );

      scaleImage(grayImage, scaledImage, scalingRatio);
    }    

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(detectedImage, scaledImage, scalingRatio);
    }

    void applyEdgePreservedFilter(double sigmaColor, double sigmaSpace, 
                           cv::Mat& image, cv::Mat& filtered )
    {
      //const int SIGMA_SPACE = 160;
      //const int SIGMA_COLOR = 70;
      
      try {
        int flag = RECURS_FILTER ; //NORMCONV_FILTER is very slow;
        cv::edgePreservingFilter(
            image, 
            filtered, 
            flag,
            sigmaSpace,        //(double)SIGMA_SPACE, 
            sigmaColor/100.0f); //(double)SIGMA_COLOR/100.0f);
      
      } catch (cv::Exception& ex) {
        ; //Ignore
      }
    }

    void detect(int sigmaColor, int sigmaSpace, 
           int threshold1, int threshold2, int scalingRatio)
    {
      threshold1 = (threshold1/2)*2 + 1;
      threshold2 = (threshold2/2)*2 + 1;
      Mat threshold_output;
      std::vector<vector<Point> > contours;
      std::vector<Vec4i> hierarchy;
      //1 Apply EdgePreservedFilter to originalImage.
      applyEdgePreservedFilter((double)sigmaColor, (double)sigmaSpace, 
                originalImage, edgePreserved);
   
      //2 Convert it to grayImage.
      cv::cvtColor(edgePreserved, grayImage, COLOR_BGR2GRAY );
        

      //3 Apply Canny edge detector to the grayImage.
      cv::Canny(grayImage, cannyEdgedImage, threshold1, threshold2);
      
      //4 Find contours from the detectedImage.
      cv::findContours(cannyEdgedImage, contours, 
           hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

      std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
      std::vector<cv::Rect> boundRect( contours.size() );
 
      //5 Get approximate polygonal coordinates from the contours, and
      // bounding rectangles from the contours_polygons.
      for( int i = 0; i < contours.size(); i++ ) { 
        cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
      }
      detectedImage.release();
      
      detectedImage = originalImage.clone();
      
      //6 Find minimum x and y, and maximum w and h from the boundRect array.
      int width  = detectedImage.size().width;
      int height = detectedImage.size().height;
      int x = width, y = height, w = 0, h = 0;
      int sx = width, sy = height, ex = 0, ey = 0;
      //
      for( int i = 0; i< contours.size(); i++ )  {
        int xx = boundRect[i].x;
        int yy = boundRect[i].y;
        int ww = boundRect[i].width;
        int hh = boundRect[i].height;
        int area = ww * hh;
        int MIN_AREA = 10;
        int MIN_X    = 10;
        int MIN_Y    = 10;
       
        if (area > MIN_AREA) {
          if (xx < sx && xx > MIN_X)
            sx = xx;
          if (yy < sy && yy > MIN_Y)
            sy = yy;
          
          if (xx > ex) 
            ex = xx;
          if (yy > ey)
            ey = yy;
        }
      }
      
      int endx = x + w;
      if (endx >= width)
       endx = width;
      int endy = y + h;
      if (endy >= height)
       endy = height;
      int eex = sx + ex;
      int eey = sy + ey;
      if (eex > width)
        eex = width - 4;
      if (eey > height)
        eey = height- 4;
      //7 Draw a rectangle having a start Point(sx, sy) and end Point(ex, ey) on the contourImage.
      cv::rectangle(detectedImage, Point(sx, sy), Point(ex, ey), CV_RGB(255, 0, 0), 2, 8, 0 );
      
      scaleImage(detectedImage, scaledImage, scalingRatio);
    }
  };
  //Inner classes end.
	
private:

  StringT<char>                imageFile;
  int                          imageLoadingFlag;
  int                          imageScalingRatio; //percentage 

  SmartPtr<Label>              label;
  SmartPtr<OriginalImageView>  originalImage;
  SmartPtr<DetectedImageView>   detectedImage;
  SmartPtr<RowColumn>          controlPane;
  SmartPtr<OpenCVScaleComboBox>  scaleComboBox;

  //Parameters for EdgePreservingFilter
  static const int             SIGMA_COLOR_MAX =200;
  int                          sigmaColor;
  SmartPtr<LabeledTrackBar>    sigmaColorTrackBar;

  static const int             SIGMA_SPACE_MAX=100;
  int                          sigmaSpace;
  SmartPtr<LabeledTrackBar>    sigmaSpaceTrackBar;

  //Parameters for CannyEdgeDetector
  static const int             THRESHOLD1_MAX = 300;
  int                          threshold1;
  SmartPtr<LabeledTrackBar>    threshold1TrackBar;
  
  static const int             THRESHOLD2_MAX = 300;
  int                          threshold2;
  SmartPtr<LabeledTrackBar>    threshold2TrackBar;

  SmartPtr<FileOpenDialog>     fileDialog;

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

  //Common trackBarScrolled callback
  void trackBarScrolled(Action& action)
  {
    sigmaColor = sigmaColorTrackBar->getPosition();
    sigmaSpace = sigmaSpaceTrackBar->getPosition();

    threshold1 = threshold1TrackBar->getPosition();
    threshold2 = threshold2TrackBar->getPosition();
    detectedImage->detect(sigmaColor, sigmaSpace, 
          threshold1, threshold2, imageScalingRatio);
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
      detectedImage->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio); 

      sigmaColor = sigmaColorTrackBar->getPosition();
      sigmaSpace = sigmaSpaceTrackBar->getPosition();
      threshold1 = threshold1TrackBar->getPosition();
      threshold2 = threshold2TrackBar->getPosition();

      detectedImage->detect(sigmaColor, sigmaSpace, 
               threshold1, threshold2, imageScalingRatio);

      updateLabel(filename);

      resize(width(), height());
      flush();
    } catch (OZ::Exception& ex) {
       caught(ex);
    } 
  }

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 240;
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

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args) 
  {
    BulletinBoard* bboard = getBulletinBoard();
    imageFile = "../images/CatImage.png";
    imageLoadingFlag = CV_LOAD_IMAGE_COLOR;
    imageScalingRatio = 60; //%

    try {
      Args ar;
      CompoundString fileNamecs(imageFile);
      ar.set(XmNlabelString, fileNamecs); 
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING); 
      label = new Label(bboard, "", ar);

      ar.reset();
      ar.set(XmNimageFileName,    imageFile);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);

      originalImage = new OriginalImageView(bboard, "", ar);

      ar.reset();
      ar.set(XmNimageFileName,   imageFile);
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

      sigmaColor = 160;
      sigmaSpace = 40;

      ar.reset();
      CompoundString sth1Title("EdgeSigmaColor:[1,200]");
      ar.set(XmNminimum, 1);
      ar.set(XmNmaximum, SIGMA_COLOR_MAX);
      ar.set(XmNvalue,    sigmaColor);
      ar.set(XmNtitleString, sth1Title);
      sigmaColorTrackBar = new LabeledTrackBar(controlPane, "", ar);
      sigmaColorTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled, NULL);

      ar.reset();
      CompoundString sth2Title("EdgeSigmaSpace:[0,100]");
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  SIGMA_SPACE_MAX);
      ar.set(XmNvalue,    sigmaSpace);
      ar.set(XmNtitleString, sth2Title);
      sigmaSpaceTrackBar = new LabeledTrackBar(controlPane, "", ar);
      sigmaSpaceTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled, NULL);

      threshold1 =  50;
      threshold2 = 100;
        
      ar.reset();
      CompoundString th1Title("CannyThreshold1[0,300]");
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  THRESHOLD1_MAX);
      ar.set(XmNvalue,    threshold1); 
      ar.set(XmNtitleString, th1Title);
      threshold1TrackBar = new LabeledTrackBar(controlPane, "", ar);
      threshold1TrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled, NULL);

      ar.reset();
      CompoundString th2Title("CannyThreshold2[0,300]");
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  THRESHOLD1_MAX);
      ar.set(XmNvalue,    threshold1);
      ar.set(XmNtitleString, th2Title);
      threshold2TrackBar = new LabeledTrackBar(controlPane, "", ar);
      threshold2TrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::trackBarScrolled, NULL);

      detectedImage->detect(sigmaColor, sigmaSpace, 
                   threshold1, threshold2, imageScalingRatio);

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
    args.set(XmNheight, 420);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

