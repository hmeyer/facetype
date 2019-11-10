#ifndef DETECTOR_H
#define DETECTOR_H

#include "opencv2/core/core.hpp"

class Detector {
public:
bool detectAndCrop(const cv::Mat& img, double crop_aspect, cv::Mat* cropped);
void set_detection_size(int width, int height) {
        detection_width_ = width;
        detection_height_ = height;
}
protected:
virtual bool detect(const cv::Mat& img, cv::Rect* r, double* facewidth) = 0;
private:
int detection_width_ = 0;
int detection_height_ = 0;
};

#endif
