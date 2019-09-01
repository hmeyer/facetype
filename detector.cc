#include "detector.h"
#include <opencv2/imgproc.hpp>

bool Detector::detectAndCrop(const cv::Mat& img, double crop_aspect, cv::Mat* cropped) {
  cv::Rect r;
  cv::Mat eq_img;
  cv::equalizeHist(img, eq_img);
  if (!detect(eq_img, &r)) return false;

  double aspect = r.width * 1.0 / r.height;

  if (crop_aspect > aspect) {
    int nw = crop_aspect * r.height;
    r.x = std::max(0, r.x - (nw - r.width) / 2);
    r.width = std::min(nw, img.size().width - r.x);
  } else {
    int nh = r.width / crop_aspect;
    r.y = std::max(0, r.y - (nh - r.height) / 2);
    r.height = std::min(nh, img.size().height - r.y);
  }

  *cropped = img(r).clone();
  equalizeHist( *cropped, *cropped);
  return true;
}