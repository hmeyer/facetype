#include "detector.h"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>

namespace {

constexpr double kBlurFaceScaler = 0.2;

// Will rotate img (and adjust r) if this helps to better match target_aspect.
void maybeRotate(double target_aspect, cv::Rect* r, cv::Mat* img) {
        if ( (target_aspect > 1.0) == ((r->width * 1.0 / r->height) > 1.0) ) {
                return;
        }
        cv::Mat t;
        cv::rotate(*img, t, cv::ROTATE_90_CLOCKWISE);
        cv::Rect tr;
        tr.x = img->size().height - (r->y + r->height);
        tr.width = r->height;
        tr.y = r->x;
        tr.height = r->width;
        *img = t;
        *r = tr;
}

void highpass(double sigma, cv::Mat* img) {
        cv::Mat blurred;
        cv::GaussianBlur(*img, blurred, cv::Size(), sigma);
        cv::Mat diff;
        cv::addWeighted(*img, 127.0/128, blurred, -127.0/128, 127, diff);
        *img = diff;
}
}  // namespace

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
        double facewidth;
        bool detect_result = detect(eq_img, &r, &facewidth);
        auto end_time = std::chrono::steady_clock::now();
        std::cout << "detection took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms" << std::endl;
        if (!detect_result) return false;

        r.x *= img.size().width / eq_img.size().width;
        r.width *= img.size().width / eq_img.size().width;
        r.y *= img.size().height / eq_img.size().height;
        r.height *= img.size().height / eq_img.size().height;

        maybeRotate(crop_aspect, &r, &img);

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
        highpass(facewidth * kBlurFaceScaler, cropped);
        equalizeHist( *cropped, *cropped);
        return true;
}
