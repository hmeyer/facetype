#include "detector.h"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>

bool Detector::detectAndCrop(const cv::Mat& color_img, double crop_aspect, cv::Mat* cropped) {
  cv::Mat img( color_img.rows, color_img.cols, CV_8UC1 );
  cv::cvtColor( color_img, img, cv::COLOR_RGB2GRAY );
  
  cv::Rect r;
  cv::Mat eq_img;
  if (detection_width_ > 0 && detection_height_ > 0) {
    cv::Mat resized_img;
    cv::resize(img, resized_img, cv::Size(detection_width_, detection_height_));
    cv::equalizeHist(resized_img, eq_img);
  } else {
    cv::equalizeHist(img, eq_img);
  }
  
  auto start_time = std::chrono::steady_clock::now();
  bool detect_result = detect(eq_img, &r);
  auto end_time = std::chrono::steady_clock::now();
  std::cout << "detection took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms" << std::endl;
  if (!detect_result) return false;

  r.x *= img.size().width / eq_img.size().width;
  r.width *= img.size().width / eq_img.size().width;
  r.y *= img.size().height / eq_img.size().height;
  r.height *= img.size().height / eq_img.size().height;

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
