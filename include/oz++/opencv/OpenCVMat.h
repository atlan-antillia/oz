#pragma once

#include <oz++/opencv/OpenCVObject.h>

namespace OZ {

class OpenCVMat : public OpenCVObject {
private:
  cv::Mat mat;

  OpenCVMat()
  {
  }

public:
  OpenCVMat(cv::Mat& m)
  {
    mat = m.clone();
  }

  ~OpenCVMat()
  {
    mat.release();
  }

  cv::Mat& getMat()
  {
    return mat;
  }

  cv::Mat getClone()
  {
    return mat.clone();
  }
};

}

