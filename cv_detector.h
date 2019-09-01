#ifndef CV_DETECTOR_H
#define CV_DETECTOR_H

#include <opencv2/objdetect.hpp>

#include <string>
#include "detector.h"

class CvDetector : public Detector {
public:
  CvDetector(const std::string& cascade_name);
protected:
  bool detect(const cv::Mat& img, cv::Rect* r) override;
private:
  cv::CascadeClassifier cascade_;
};

#endif
