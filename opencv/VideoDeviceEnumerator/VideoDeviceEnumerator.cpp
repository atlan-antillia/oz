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
 *  VideoDeviceEnumerator.cpp
 *
 *****************************************************************************/

//2019/01/03
#include <opencv2/stitching.hpp>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/LabeledVideoDeviceComboBox.h>
#include <oz++/opencv/OpenCVVideoCaptureView.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/opencv/OpenCVScaleComboBox.h>
#include <oz++/opencv/OpenCVScrolledImageView.h>

namespace OZ {

class MainView :public OpenCVVideoCaptureView {

private:
  //Inner class starts.
  class VideoImageView: public OpenCVImageView {
  private:
    cv::Mat image; 
    cv::Mat scaledImage;

  public:
    void display()
    {
      show(scaledImage);
    }

  public:
    VideoImageView(View* parent, const char* name, Args& args)
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
  //Inner class ends
	
private:
  SmartPtr<VideoImageView>  videoImage;
  SmartPtr<RowColumn>       controlPane;

  int                       imageScalingRatio;
  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  int                       deviceIndex;

  SmartPtr<LabeledVideoDeviceComboBox> videoDevice;

  SmartPtr<PushButton>     startButton;

  SmartPtr<PushButton>     stopButton;

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 200;
    w = w- CP_WIDTH;
    if (videoImage && controlPane) {
      videoImage  ->resize(0, 0, w, h, 0);
      controlPane ->reshape(w+2, 0, CP_WIDTH-4, h);
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

  void videoDeviceChanged(Action& action)
  {
    deviceIndex = videoDevice->getSelectedPosition();

    printf("deviceIndex %d\n", deviceIndex);
  }

  void start(Action& action)
  {
     setDeviceIndex(deviceIndex);
     startCapture();
     printf("StartCapture: %d\n", deviceIndex);
  }

  void stop(Action& action)
  {
     stopCapture();
     printf("StopCapture\n");
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVVideoCaptureView(applet, name, args) 
  ,imageScalingRatio(100)
  ,deviceIndex(0)
  {
    try {
      BulletinBoard* bboard = getBulletinBoard();
      Args ar; 
      ar.reset();
      videoImage = new VideoImageView(bboard, "", ar);

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

      ar.reset();
      CompoundString device("VideoDevice");
      ar.set(XmNlabelString, device);
      videoDevice = new LabeledVideoDeviceComboBox(controlPane, "", ar);
      videoDevice->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::videoDeviceChanged, NULL);

      ar.reset();
      startButton = new PushButton(controlPane, "Start Capture", ar);
      startButton->addCallback(XmNactivateCallback, this,
        (Callback)&MainView::start, NULL);

      ar.reset();
      stopButton = new PushButton(controlPane, "Stop Capture", ar);
      stopButton->addCallback(XmNactivateCallback, this,
        (Callback)&MainView::stop, NULL);

      sendConfigureEvent(); 

    } catch(OZ::Exception& ex) {
      caught(ex);
    }
  }

  virtual void render()
  {
    try {
      cv::Mat frame;
      if (readVideoFrame(frame)) {
        if (videoImage) {
          videoImage->render(frame, imageScalingRatio);
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
    args.set(XmNwidth,  620);
    args.set(XmNheight, 400);
    //args.set(XmNvideoDeviceIndex, 0);
    args.set(XmNcaptureAutoStart, false);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run(&view, 100); //20 msec
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

