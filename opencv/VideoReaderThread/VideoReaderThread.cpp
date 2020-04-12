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
 *  VideoReaderThread.cpp
 *
 *****************************************************************************/

//2017/12/25

//2019/01/03
#include <opencv2/stitching.hpp>
#define String _XtString

#include <oz++/Mutex.h>
#include <oz++/motif/RowColumn.h>

#undef String

#include <oz++/opencv/OpenCVVideoReaderThread.h>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVScaleComboBox.h>
#include <oz++/opencv/OpenCVScrolledImageView.h>

namespace OZ {

class MainView :public OpenCVMainView {

private:
  //Inner class starts.
  class VideoImageView: public OpenCVScrolledImageView {
  private:
    cv::Mat scaledImage;
    int     scalingRatio;
    Mutex   mutex;

  public:
    void display()
    {
      if (!scaledImage.empty()) {
        show(scaledImage);
      }
    }

  public:
    VideoImageView(View* parent, const char* name, Args& args)
    :OpenCVScrolledImageView(parent, name, args)
    {
      scalingRatio = (int)args.get(XmNimageScalingRatio);
      setKeepScrollBarPosition(true);
    } 

    void setScalingRatio(int scaling)
    {
      scalingRatio = scaling;
    }

    void render(cv::Mat& frame)
    {
      mutex.lock();

      float ratio = (float)scalingRatio/100.0f;
      cv::Mat mat = frame.clone();
      cv::resize(mat, scaledImage, cv::Size(),
                            ratio, ratio);
      invalidate();

      display();
      flush();
      mutex.unlock();
    }
  };
  //Inner class ends
	
private:
  int                       videoIndex;

  int                       imageScalingRatio;

  SmartPtr<VideoImageView>  videoImage;
  SmartPtr<RowColumn>       controlPane;

  SmartPtr<OpenCVScaleComboBox> scaleComboBox;

  SmartPtr<OpenCVVideoReaderThread> videoReaderThread;

  OpenCVVideoCapture        videoCapture;

  void resize(Dimension w, Dimension h)
  {
    MenuBar* mbar = getMenuBar();
    int mh = mbar->height();
    int CP_WIDTH = 160;
    w = w - CP_WIDTH;
    h = h - mh;
    if (videoImage && controlPane) {
      videoImage  ->reshape(0,   0, w,          h);
      controlPane ->reshape(w+2, 0, CP_WIDTH-4, h);
    }
  }

  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      videoImage->setScalingRatio(val);
      printf("ScalingRatio %d\n", val);
    }
  } 

  virtual void mapNotify(Event& event)
  {
    printf("mapNotify\n");
    try {
      //1 Open videoCapture in this main thread.
      videoCapture.open(videoIndex);

      //2 Create OpenCVVideoReaderThread by using videoCapture.
      videoReaderThread = new OpenCVVideoReaderThread(
                videoImage,
                videoCapture);

      videoReaderThread -> start();

    } catch (Exception& ex) {
      caught(ex);
    }
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args) 
  {
    try {
      BulletinBoard* bboard = getBulletinBoard();

      const char* defaultScale = "60%";
      imageScalingRatio = atoi(defaultScale);

      videoIndex = (int)args.get(XmNvideoDeviceIndex);

      Args ar; 
      ar.reset();
      ar.set(XmNimageScalingRatio, imageScalingRatio);
      videoImage = new VideoImageView(bboard, "", ar);

      ar.reset();
      controlPane = new RowColumn(bboard, "", ar);

      ar.reset();
      CompoundString scaler("Scale");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNdefaultScale, defaultScale);
      scaleComboBox = new OpenCVScaleComboBox(controlPane, "", ar);
      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);

      sendConfigureEvent(); 

    } catch(OZ::Exception& ex) {
      caught(ex);
    }
  }


  ~MainView()
  {
    if (videoReaderThread) {
      videoReaderThread->terminate();
      videoReaderThread->wait();
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
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run(); 
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

