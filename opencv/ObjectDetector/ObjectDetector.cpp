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
 *  ObjectDetector.cpp
 *
 *****************************************************************************/

//2017/5/20 Modified to use OpenCVScrolledImage instead of OpenCVImageView
//         to reduce excessive screen flickering on a window resizing.

//2019/01/03
#include <opencv2/stitching.hpp>
#include <oz++/motif/Label.h>
#include <oz++/motif/TextField.h>
#include <oz++/motif/PushButton.h>
#include <oz++/motif/RowColumn.h>
#include <oz++/motif/FileDropdownComboBox.h>
#include <oz++/motif/FileOpenDialog.h>
#include <oz++/motif/FolderOpenDialog.h>
#include <oz++/opencv/OpenCVMainView.h>
#include <oz++/opencv/OpenCVImageView.h>
#include <oz++/opencv/OpenCVScrolledImageView.h>
#include <oz++/opencv/OpenCVScaleComboBox.h>
#include <opencv2/objdetect/objdetect.hpp>

namespace OZ {

class MainView : public OpenCVMainView {

private:
  //////////////////////////////////////////////
  //Inner class starts.

  class SimpleView  :public OpenCVScrolledImageView {
  private:
    cv::Mat originalImage;
    cv::Mat scaledImage;

    void display()
    {
      show(scaledImage);
    }

  public:
    SimpleView(View* parent, const char* name, Args& args)
    :OpenCVScrolledImageView(parent, name, args)
    {
      const char* filename = "";
      int   loadingFlag    = 1;
      int   scalingRatio   = 100;

      if (args.has(XmNimageFileName)) {
        filename = (const char*)args.get(XmNimageFileName);
      }

      if (args.has(XmNimageLoadingFlag)) {
        loadingFlag = (int)args.get(XmNimageLoadingFlag); 
      }
      if (args.has(XmNimageScalingRatio)) {
        scalingRatio = (int)args.get(XmNimageScalingRatio); 
      }

      loadImage(filename, loadingFlag, scalingRatio);
    }

    void loadImage(const char* filename, int loadingFlag, int scalingRatio)
    {
      originalImage = readImage(filename, loadingFlag);
      scaleImage(originalImage, scaledImage, scalingRatio); 
    }

    void rescale(int scalingRatio)
    {
      scaledImage.release();
      scaleImage(originalImage, scaledImage, scalingRatio); 
    }
 
    void clear(int scalingRatio)
    {
      rescale(scalingRatio); 
    }

    void detect(const char* path, int scalingRatio)
    {
      try {
        CascadeClassifier classifier;
        classifier.load(path);
      
        cv::vector<Rect> faces;
       classifier.detectMultiScale(originalImage, faces, 1.1, 3, 0, cv::Size(20,20));
        if (faces.size() == 0) {
         // MessageBox(NULL, "Not detected by detectMultiScale.", 
         //     "DetectOperation", MB_OK|MB_ICONINFORMATION); 
        }
	for (int i = 0; i < faces.size(); i++){
          rectangle(originalImage, Point(faces[i].x, faces[i].y), 
	       Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
              Scalar(0, 200, 0), 3, CV_AA);
        }
        scaleImage(originalImage, scaledImage, scalingRatio); 
      } catch (Exception& ex) {
        caught(ex);
      }
    }
  };
  //Inner class ends.
  //////////////////////////////////////////////

  //SmartPtr<BulletinBoard>   bboard;
    SmartPtr<TextField>       filePath;
    SmartPtr<Label>            classifierLabel;
    SmartPtr<FileDropdownComboBox> classifierComboBox;
    SmartPtr<PushButton>      folderButton;
    StringT<char>            classifierFolder;

    SmartPtr<SimpleView>     view;

  SmartPtr<RowColumn>      controlPane;
    SmartPtr<OpenCVScaleComboBox>  scaleComboBox;
    SmartPtr<PushButton>   reloadButton;
    SmartPtr<PushButton>   detectButton;

  SmartPtr<FileOpenDialog> fileDialog;
  SmartPtr<FolderOpenDialog>  folderDialog;

  StringT<char>            imageFileName;
  int                      imageLoadingFlag;
  int                      imageScalingRatio;

  void fileOpen(Action& action)
  {
    fileDialog->popup();
  }

  void updateLabel(const char* filename)
  {
     CompoundString cs(filename);
     filePath->set(XmNlabelString, cs);    
     filePath->setString(filename);//, cs);    
  }

  void scaleChanged(Action& action)
  {
    int val = scaleComboBox->getScale();

    if (val > 0 && imageScalingRatio != val) {
      imageScalingRatio = val;
      view -> rescale(imageScalingRatio); 
    }
  }  

  void showFolderBrowser(Action& action)
  {
    folderDialog->popup();
  }

  void ok(Action& action)
  {
    try {  
      const char* filename = fileDialog->getFileName();
      imageFileName = filename;
      printf("filename: %s\n", filename);
      fileDialog->popdown();
      view->invalidate();
      view->loadImage(filename, 
		imageLoadingFlag, imageScalingRatio);

      updateLabel(filename);

      resize(width(), height());
      //flush();
    } catch (OZ::Exception& ex) {
       caught(ex);
    } 
  }

  void selectFolder(Action& action)
  {
    classifierFolder = folderDialog->getFolderName();
    printf("selectFolder %s\n", (const char*)classifierFolder);
   
    folderDialog->popdown();
    classifierComboBox->listupFiles((const char*)classifierFolder, "*.xml");

  }

  void reload(Action& action)
  {
     view -> loadImage(imageFileName,
                       imageLoadingFlag,
                       imageScalingRatio);
  }

  void detect(Action& action)
  {
    XmString xms;
    classifierComboBox->get(XmNselectedItem, (XtArgVal)&xms);
    CompoundString cs(xms);
    char* item = NULL;
    cs.get(&item);
    if (item != NULL) {
      char classifierxml[PATH_MAX];
      sprintf(classifierxml, "%s/%s", (const char*)classifierFolder,
           item); 
      printf("detect xml=%s\n", classifierxml);
      view -> detect(classifierxml, imageScalingRatio); 
    }
    XtFree(item);
  }

  void resize(Dimension w, Dimension h)
  {
    int CP_WIDTH = 160;
    int ww =  w-CP_WIDTH;
    static  int LH = 0;
    if (LH == 0) {
      LH = filePath->height();
    }

    static  int CH = 0;
    if (CH == 0) {
      CH = classifierComboBox->height();
    }
    static const int CL_WIDTH = 100;
    static const int FB_WIDTH = 60;
    if (filePath && classifierComboBox && view  && controlPane ) {
      BulletinBoard* bboard = getBulletinBoard();
      //bboard->unmap();
      //bboard->unmanage();

      int lh = LH;
      int ch = CH;
      int m = 2; 
      filePath              -> reshape(m, m, w, lh+m);
      classifierLabel    -> reshape(m, lh+2*m, CL_WIDTH, CH); 
      classifierComboBox -> reshape(2*m+CL_WIDTH, lh+2*m, w-CL_WIDTH-FB_WIDTH-4*m, CH);
      folderButton       -> reshape(w - FB_WIDTH, lh+2*m+m, FB_WIDTH-m, LH);

      view               -> reshape(m, lh+2*m + ch+2*m, ww-2*m, 
					h - lh - ch -4*m-ch);
      Window cw = controlPane->getWindow();
      controlPane        -> reshape(ww-1, lh+2*m + ch+2*m, CP_WIDTH+1, 
					h - lh - ch-4*m);

      //controlPane -> unmap();
      //controlPane -> map();

      //bboard->manage();
      //bboard->map();
    }
  }

public:
  MainView(OpenCVApplication& applet, const char* name, Args& args)
  :OpenCVMainView(applet, name, args)
  {
    BulletinBoard* bboard = getBulletinBoard();

    try {
      Args ar;
      ar.reset();

      imageFileName     = "../images/WafukuMannequin.png";

      imageLoadingFlag  = CV_LOAD_IMAGE_COLOR;
      imageScalingRatio = 100;

      //1 Create a filePath to display a filename.
      ar.set(XmNalignment,  XmALIGNMENT_BEGINNING);
      filePath = new TextField(bboard, "", ar);
      filePath -> setEditable(FALSE);
      updateLabel(imageFileName);

      CompoundString classifiercs("Classifier");
      ar.reset();
      ar.set(XmNalignment,  XmALIGNMENT_BEGINNING);
      ar.set(XmNlabelString, classifiercs);
      classifierLabel = new Label(bboard, "", ar);

      CompoundString foldercs("...");
      ar.reset();
      ar.set(XmNlabelString, foldercs);
      folderButton = new PushButton(bboard, "", ar);
      folderButton -> addCallback(XmNactivateCallback, this,
		(Callback)&MainView::showFolderBrowser, NULL);

      ar.reset();
      classifierComboBox = new FileDropdownComboBox(bboard, "", ar);

      //2 Create a simpleview.
      ar.reset();
      ar.set(XmNimageFileName,    imageFileName);
      ar.set(XmNimageLoadingFlag, imageLoadingFlag);
      ar.set(XmNimageScalingRatio, imageScalingRatio);
      view = new SimpleView(bboard, "", ar);

      //3 Create a controlPane.
      ar.reset();
      controlPane = new RowColumn(bboard, "", ar);

      //4 Create a scaleComboBox.
      const char* defaultScale = "100%";
      ar.reset();
      CompoundString scaler("Scale");
      ar.set(XmNlabelString, scaler);
      ar.set(XmNdefaultScale, defaultScale);
      scaleComboBox = new OpenCVScaleComboBox(controlPane, "", ar);
      scaleComboBox->addCallback(XmNselectionCallback, this,
        (Callback)&MainView::scaleChanged, NULL);

      CompoundString reloadcs("Reload");
      ar.reset();
      ar.set(XmNlabelString, reloadcs);
      reloadButton = new PushButton(controlPane, "", ar);
      reloadButton -> addCallback(XmNactivateCallback, this,
		(Callback)&MainView::reload, NULL);

      CompoundString detectcs("Detect");
      ar.reset();
      ar.set(XmNlabelString, detectcs);
      detectButton = new PushButton(controlPane, "", ar);
      detectButton -> addCallback(XmNactivateCallback, this,
		(Callback)&MainView::detect, NULL);

      const char* home = getenv("HOME");

      //5 Create a fileOpendialog
      ar.reset();
      ar.set(XmNrootFolderName, home);
      fileDialog = new FileOpenDialog(this, "FileOpenDialog", ar);
      fileDialog  -> getOkButton()
                  -> addCallback(XmNactivateCallback, this,
                          (Callback)&MainView::ok, NULL);  

      //5 Create a folderOpenDialog
      ar.reset();
      ar.set(XmNrootFolderName, home);
      folderDialog = new FolderOpenDialog(this, "FolderOpenDialog", ar);
      folderDialog  -> getOkButton()
                  -> addCallback(XmNactivateCallback, this,
                          (Callback)&MainView::selectFolder, NULL);  
    } catch (OZ::Exception& ex) {
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
    args.set(XmNwidth,  800);
    args.set(XmNheight, 460);
    MainView view(applet, argv[0], args);
    view.realize();

    applet.run();
  } catch (OZ::Exception& ex) {
    caught(ex);
  }
  return 0;
}

