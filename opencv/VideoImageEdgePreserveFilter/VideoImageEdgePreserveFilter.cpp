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
 *  VideoImageEdgePreserveFilter.cpp
 *
 *****************************************************************************/

#include <oz++/Mutex.h>
#include <opencv2/stitching.hpp>

#include <oz++/motif/RowColumn.h>
#include <oz++/motif/LabeledTrackBar.h>
#include <oz++/opencv/OpenCVVideoCaptureView.h>
#include <oz++/opencv/OpenCVImageProcessingThread.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/opencv/OpenCVScaleComboBox.h>
#include <oz++/opencv/OpenCVScrolledImageView.h>

namespace OZ {

class MainView :public OpenCVVideoCaptureView {

private:
  //Inner classes start.
  class OriginalImageView: public OpenCVImageView {
  private:
    cv::Mat scaledImage;
    int scalingRatio;

  public:
    void display()
    {
      show(scaledImage);
    }

  public:
    OriginalImageView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
      scalingRatio = 60;
    } 

    void setScaling(int scaling)
    {
      scalingRatio = scaling;
    }

    void render(cv::Mat& frame)
    {
      rescale(frame, scaledImage, scalingRatio);

      invalidate();
      refresh();
    }
  };

  class SharpenedImageView: public OpenCVImageView {
  private:
    cv::Mat scaledImage;
    int     scalingRatio;

    int     ksize;
    int     sigma;

  public:
    void display()
    {
      show(scaledImage);
    }

  public:
    SharpenedImageView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    ,ksize(3)
    ,sigma(5) 
    {
      scalingRatio = 60;
    }

    void setScaling(int scaling)
    {
      scalingRatio = scaling;
    }

    void setKsize(int k)
    {
      ksize = (k*2)/2 +1;
    }

    void setSigma(int s)
    {
      sigma = s;
    }

    void render(cv::Mat& frame)
    {
      cv::Mat newImage = cv::Mat::zeros(frame.size(), frame.type() );
      cv::GaussianBlur(frame, newImage, cv::Size(ksize, ksize), 
            (double)sigma, //sigmaX, 
            (double)sigma, //sigmaY
            BORDER_DEFAULT);
      
      double alpha = 2.5;
      double beta  = 1.0 - alpha;
      cv::addWeighted(frame, alpha, newImage, beta, 0.0, newImage);
      
      rescale(newImage, scaledImage, scalingRatio);

      invalidate();
      refresh();
    }
  };

  class ImageProcessor : public OpenCVImageProcessingThread {
  private:
    OriginalImageView*    originalView;
    SharpenedImageView*   sharpenedImage;
    bool                  looping;
    int                   interval; //msec

  public:
    ImageProcessor(OriginalImageView* original,
                   SharpenedImageView* sharpened)
    :OpenCVImageProcessingThread()
    ,originalView(original)
    ,sharpenedImage(sharpened) 
    ,interval(40)
    {
    }
    
    void run()
    {
       int micros = interval * 1000;

       while(looping) {
         OpenCVMat* cvmat = get();
         if (cvmat) {
           cv::Mat& m = cvmat->getMat();
           originalView->render(m); 
           sharpenedImage->render(m); 
         }
         usleep(micros);
       }
    }
  };

  //Inner classes end
	
private:
  SmartPtr<OriginalImageView>  originalView;
  SmartPtr<SharpenedImageView> sharpenedView;

  SmartPtr<RowColumn>          controlPane;

  int                          imageScalingRatio;
  SmartPtr<LabeledTrackBar>    ksizeTrackBar;
  int                          ksize;

  SmartPtr<LabeledTrackBar>    sigmaTrackBar;
  int                          sigma;

  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  SmartPtr<ImageProcessor>      imageProcessor;

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 160;
    w = w- CP_WIDTH;
    if (originalView && sharpenedView && controlPane) {
      originalView-> resize(0,     0, w/2-1, h, 0);
      sharpenedView -> resize(w/2+1, 0, w/2-2, h, 0);
      controlPane -> reshape(w+2,  0, CP_WIDTH-4, h);
    }
  }

  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      originalView -> setScaling(imageScalingRatio);
      sharpenedView-> setScaling(imageScalingRatio);
      
      printf("ScalingRatio %d\n", val);
    }
  } 

  void ksizeChanged(Action& action)
  {
    ksize = (int)ksizeTrackBar->get(XmNvalue);
    sharpenedView->setKsize(ksize);
  }

  void sigmaChanged(Action& action)
  {
    sigma = sigmaTrackBar->get(XmNvalue);
    sharpenedView->setSigma(sigma);
    printf("sigmaChanged sigma=%d\n", sigma);
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVVideoCaptureView(applet, name, args) 
  ,imageScalingRatio(100)
  ,ksize(8)
  {
    try {
      BulletinBoard* bboard = getBulletinBoard();
      Args ar; 
      ar.reset();
      originalView = new OriginalImageView(bboard, "", ar);

      ar.reset();
      sharpenedView = new SharpenedImageView(bboard, "", ar);

      ar.reset();
      controlPane = new RowColumn(bboard, "", ar);
      const char* defaultScale = "100%";

      ar.reset();
      CompoundString scaler("Scale");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNdefaultScale, defaultScale);
      scaleComboBox = new OpenCVScaleComboBox(controlPane, "", ar);
      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);

      ksize =3;
      ar.reset();
      CompoundString ksizecs("KernelSize:[0,31]");
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum,  31);
      ar.set(XmNvalue,    ksize); 
      ar.set(XmNtitleString, ksizecs);
      ksizeTrackBar = new LabeledTrackBar(controlPane, "", ar);
      ksizeTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::ksizeChanged, NULL);

      ar.reset();
      CompoundString sigmacs("Sigma[0,20]");
      sigma = 12;
      ar.set(XmNminimum, 0);
      ar.set(XmNmaximum, 20);
      ar.set(XmNvalue,   sigma );
      ar.set(XmNtitleString, sigmacs);
      sigmaTrackBar = new LabeledTrackBar(controlPane, "", ar);
      sigmaTrackBar->addCallback(XmNvalueChangedCallback, this,
        (Callback)&MainView::sigmaChanged, NULL);

      sharpenedView->setKsize(ksize);
      sharpenedView->setSigma(sigma);

      imageProcessor = new ImageProcessor(originalView, sharpenedView);
      imageProcessor -> start();

      sendConfigureEvent(); 

    } catch(OZ::Exception& ex) {
      caught(ex);
    }
  }

  ~MainView()
  {
    if (imageProcessor) {
      imageProcessor->terminate();
      imageProcessor->wait();
    }
  }

  virtual void render()
  {
    try {
      cv::Mat frame;
      if (readVideoFrame(frame)) {
        imageProcessor -> add(new OpenCVMat(frame));
      }
    } catch (...) {
    }
  }
};

}

//
int main(int argc, char** argv) 
{
  XInitThreads();

  try {
    const char*  appclass = argv[0];
    OpenCVApplication applet(appclass, argc, argv);

    Args args;
    args.set(XmNwidth,  900);
    args.set(XmNheight, 400);
    args.set(XmNvideoDeviceIndex, 0);
    args.set(XmNcaptureAutoStart, true);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run(&view, 40); //100msec
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

