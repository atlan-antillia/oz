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
 *  CustomYoloObjectDetector.cpp
 *
 *****************************************************************************/



//2019/06/09
//#define OPENCV

#include <opencv2/stitching.hpp>
#include <opencv2/opencv.hpp>
#include <oz++/motif/Label.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/LabeledComboBox.h>
#include <oz++/motif/LabeledTrackBar.h>
#include <oz++/motif/DropdownScaler.h>

#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVScrolledImageView.h>
#include <oz++/motif/FileOpenDialog.h>
#include <oz++/motif/ScrolledTableView.h>
#include <oz++/ConfigParser.h>

#include <oz++/yolov3/Detector3.h>
#include <oz++/yolov3/DetectedObjectList.h>

namespace OZ {

class MainView :public OpenCVMainView {
   
private:
  ///////////////////////////////////////////////

  class DetectedImageView: public OpenCVScrolledImageView {
  private:
    cv::Mat originalImage; 
    cv::Mat scaledImage;
    
    virtual void display()
    {
       show(scaledImage);
    }
 
  public:
    DetectedImageView(View* parent, const char* name, Args& args)
    :OpenCVScrolledImageView(parent, name, args)
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
      scaleImage(originalImage, scaledImage, scalingRatio);
      refresh();
    }    

    void setImage(cv::Mat& image, int scalingRatio=100 )
    {
      originalImage = image;
      rescale(scalingRatio);
    }
 
    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(originalImage, scaledImage, scalingRatio);
    }
  };
	
private:

  std::string         imageFile;
  std::string         csv_filename;

  int                 imageLoadingFlag = CV_LOAD_IMAGE_COLOR;
  int                 imageScalingRatio =80; //percentage 

  ConfigParser        parser;

  SmartPtr<Detector3> detector;
  std::string         darknet_root;

  std::string         cfg_filename;
  std::string         coco_filename;
  std::string         weight_filename;

  SmartPtr<DetectedImageView>   detectedImage;
  SmartPtr<Form>                controlPane;
  SmartPtr<DetectedObjectList>  objectList;

  SmartPtr<RowColumn>        topPane;
  SmartPtr<Label>            label;
  SmartPtr<DropdownScaler>   scaleComboBox;
  SmartPtr<PushButton>       reloadButton;
  SmartPtr<PushButton>       detectButton;
 
  SmartPtr<FileOpenDialog>   fileDialog;

public:
  void cancel(Action& action)
  {
    fileDialog->popdown();
  }

  void fileOpen(Action& action)
  {
    fileDialog->popup();
  }

  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();
    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      detectedImage -> rescale(imageScalingRatio); 
    }
  }  

  void updateTitle(const char* filename)
  {
     std::string appname = getAppName();
     std::string title = filename;
     title += std::string(" - ");
     title += appname; 
     setTitle(title.c_str());
  }

  void ok(Action& action)
  {
    try {  
      imageFile  = fileDialog->getFileName();
      const char* filename = (const char*)imageFile.c_str();
 
      printf("filename: %s\n", filename);
      fileDialog->popdown();
    
      detectedImage ->loadImage(filename, imageLoadingFlag, imageScalingRatio);
      detectedImage->unmap();
      detectedImage->map();

      csv_filename = createCSVFilename(imageFile);
      //resize(width(), height());
      updateTitle(filename);

      flush();
    } catch (OZ::Exception& ex) {
       caught(ex);
    } 
  }

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 330;
    int LB_HEIGHT = 30;
    int ww =  w-CP_WIDTH;
    int hh = h - LB_HEIGHT;
    
    if (topPane && detectButton && detectedImage && controlPane ) {
      detectButton->get(XmNheight, LB_HEIGHT);
      hh = h - LB_HEIGHT -15;
      detectedImage -> reshape(0, LB_HEIGHT+5, ww-2, hh-30);
     
      controlPane  -> reshape(ww+1, LB_HEIGHT+5, CP_WIDTH-4, hh-30);
    }
    flush();
  }

  void reload(Action& event)
  {
    detectedImage -> loadImage(imageFile.c_str(),
                imageLoadingFlag,
                imageScalingRatio);
  }

  std::string createCSVFilename(std::string& filename)
  {
    std::string csv_fname = filename;
    auto pos = filename.rfind("/");
    if (pos != std::string::npos) { 
      std::string nameonly= filename.substr(pos+1);
      csv_fname = std::string("./") + nameonly + std::string(".csv"); 
      printf("csv_filename %s\n", csv_fname.c_str());
    }
    return csv_fname;
  }

  void detect(Action& event)
  {
    image timage = detector->detect_image(imageFile.c_str(), csv_filename.c_str());

    cv::Mat mat = detector->image_to_mat(timage);
    cv::Mat mbgr;
    cv::cvtColor(mat, mbgr, cv::COLOR_RGB2BGR);
    free_image(timage);
    detectedImage->setImage(mbgr, imageScalingRatio);
    TableView* tablev = objectList->getTableView();
    tablev->removeAllItems();
    tablev->readCSVFile(csv_filename);
  }


public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args) 
  {
    imageScalingRatio = 80;

    char defaultScale[20] = {0};
    sprintf(defaultScale, "%d%%", imageScalingRatio);
    printf("%s¥n", defaultScale);

    imageLoadingFlag = CV_LOAD_IMAGE_COLOR;

    std::string config = "./CustomYoloObjectDetector.ini";

    parser.parse(config);

    darknet_root    = parser.getString("DARKNET_ROOT", "filename");

    cfg_filename    = parser.getString("CFG_FILE", "filename");
    coco_filename   = parser.getString("COCO_FILE", "filename");
    weight_filename = parser.getString("WEIGHT_FILE", "filename");

    BulletinBoard* bboard = getBulletinBoard();
    imageFile = "../../images/JRShinjukuStation.2.jpg";
    csv_filename = createCSVFilename(imageFile);

    // Create a instance oF OZ::Detector3.
    detector = new OZ::Detector3(darknet_root, cfg_filename, weight_filename, coco_filename);

    try {
      Args ar;
      ar.set(XmNx, 0);
      ar.set(XmNy, 0);
      ar.set(XmNmarginHeight, 1);
      ar.set(XmNmarginWidth, 1);
      ar.set(XmNorientation, XmHORIZONTAL);
      ar.set(XmNspacing, 20);
      topPane = new RowColumn(bboard, "", ar);

      ar.reset();
      label = new Label(topPane, "Scale", ar);
      ar.reset();
      ar.set(XmNdefaultScale, defaultScale);
      scaleComboBox = new DropdownScaler(topPane, "", ar);
      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);

      ar.set(XmNx, 0);
      ar.set(XmNy, 0);
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING); 
      reloadButton = new PushButton(topPane, " Reload ", ar);
      reloadButton->addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::reload, NULL);

      ar.reset(); 
      ar.set(XmNx, 120);
      ar.set(XmNy, 0);
      ar.set(XmNalignment, XmALIGNMENT_BEGINNING); 
      detectButton = new PushButton(topPane, " Detect ", ar);
      detectButton->addCallback(XmNactivateCallback, this,
                 (Callback)&MainView::detect, NULL);

      updateTitle((const char*)imageFile.c_str());


      ar.reset();
      ar.set(XmNimageFileName, imageFile.c_str());
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);

      detectedImage   = new DetectedImageView(bboard, "", ar);

      ar.reset();
      controlPane = new Form(bboard, "", ar);

      ar.reset();

      objectList = new DetectedObjectList(controlPane, "objectlist", ar);

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
    args.set(XmNwidth,  1000);
    args.set(XmNheight, 480);
    MainView view(applet, appclass, args);
    view.realize();

    applet.run();
    
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

