#ifndef DETECTOR_H
#define DETECTOR_H

#include "opencv2/core/core.hpp"

class Detector {
public:
  bool detectAndCrop(const cv::Mat& img, double crop_aspect, cv::Mat* cropped);
protected:
  virtual bool detect(const cv::Mat& img, cv::Rect* r) = 0;
};

#endif
