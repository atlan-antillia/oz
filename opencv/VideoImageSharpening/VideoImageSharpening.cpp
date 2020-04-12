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
 *  VideoImageSharpening.cpp
 *
 *****************************************************************************/

//#include <oz++/StringT.h>
#include <oz++/Mutex.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/LabeledTrackBar.h>
#include <oz++/opencv/OpenCVVideoCaptureView.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/opencv/OpenCVScaleComboBox.h>
#include <oz++/opencv/OpenCVScrolledImageView.h>

namespace OZ {

class MainView :public OpenCVVideoCaptureView {

private:
  //Inner classes start.
  class OriginalImageView: public OpenCVImageView {
  private:
    cv::Mat image; 
    cv::Mat scaledImage;
  public:
    void display()
    {
      show(scaledImage);
    }

  public:
    OriginalImageView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
    } 

    void render(cv::Mat& frame, int scalingRatio)
    {
      image = frame;
      rescale(image, scaledImage, scalingRatio);

      invalidate();
      refresh();
    }
  };

  class SharpenedImageView: public OpenCVImageView {
  private:
    cv::Mat image;
    cv::Mat scaledImage;
  public:
    void display()
    {
      show(scaledImage);
    }

  public:
    SharpenedImageView(View* parent, const char* name, Args& args)
    :OpenCVImageView(parent, name, args)
    {
    }

    void render(cv::Mat& frame, int ksize, int sigma, int scalingRatio)
    {
      image = frame;
      ksize = (ksize*2)/2 + 1;

      cv::Mat newImage = cv::Mat::zeros(image.size(), image.type() );
      cv::GaussianBlur(image, newImage, cv::Size(ksize, ksize), 
            (double)sigma, //sigmaX, 
            (double)sigma, //sigmaY
            BORDER_DEFAULT);
      
      double alpha = 2.5;
      double beta  = 1.0 - alpha;
      cv::addWeighted(image, alpha, newImage, beta, 0.0, newImage);
      
      rescale(newImage, scaledImage, scalingRatio);

      invalidate();
      refresh();
    }
  };

  //Inner classes end
	
private:
  SmartPtr<OriginalImageView>  originalView;
  SmartPtr<SharpenedImageView>   sharpenedImage;

  SmartPtr<RowColumn>          controlPane;

  int                          imageScalingRatio;
  SmartPtr<LabeledTrackBar>    ksizeTrackBar;
  int                          ksize;

  SmartPtr<LabeledTrackBar>    sigmaTrackBar;
  int                          sigma;

  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 160;
    w = w- CP_WIDTH;
    if (originalView && sharpenedImage && controlPane) {
      originalView-> resize(0,     0, w/2-1, h, 0);
      sharpenedImage -> resize(w/2+1, 0, w/2-2, h, 0);
      controlPane -> reshape(w+2,  0, CP_WIDTH-4, h);
    }
  }

  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      printf("ScalingRatio %d\n", val);
    }
  } 

  void ksizeChanged(Action& action)
  {
    ksize = (int)ksizeTrackBar->get(XmNvalue);
  }

  void sigmaChanged(Action& action)
  {
    sigma = sigmaTrackBar->get(XmNvalue);
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
      sharpenedImage = new SharpenedImageView(bboard, "", ar);

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

      ksize =8;
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

      sendConfigureEvent(); 

    } catch(OZ::Exception& ex) {
      caught(ex);
    }
  }

  ~MainView()
  {
  }

  virtual void render()
  {
    try {
      cv::Mat frame;
      if (readVideoFrame(frame)) {
        if (originalView && sharpenedImage) {
          originalView->render(frame, imageScalingRatio);
          sharpenedImage ->render(frame, ksize, sigma, imageScalingRatio);
        }
      }
    } catch (...) {
    }
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
    args.set(XmNheight, 400);
    args.set(XmNvideoDeviceIndex, 0);
    args.set(XmNcaptureAutoStart, true);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run(&view, 100); //100msec
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

