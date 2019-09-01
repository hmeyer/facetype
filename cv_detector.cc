#include "cv_detector.h"
#include <stdexcept>
#include <vector>

CvDetector::CvDetector(const std::string& cascade_name) {
  if (!cascade_.load(cascade_name)) {
    throw std::invalid_argument(std::string("Error loading ") + cascade_name);
  }
}

bool CvDetector::detect(const cv::Mat& img, cv::Rect* r) {
  std::vector<cv::Rect> faces;
  cascade_.detectMultiScale(img, faces,
			    1.1, 2, 0
			    //|cv::CASCADE_FIND_BIGGEST_OBJECT
			    //|cv::CASCADE_DO_ROUGH_SEARCH
			    |cv::CASCADE_SCALE_IMAGE
			    ,
			    cv::Size(20, 20) );
  if (faces.size() > 0) {
    *r = faces[0];
    return true;
  }
  return false;
}
