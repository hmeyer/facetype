#include "cv_detector.h"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

CvDetector::CvDetector(const std::string& cascade_name) {
        if (!cascade_.load(cascade_name)) {
                throw std::invalid_argument(std::string("Error loading ") + cascade_name);
        }
}

namespace {

cv::Rect mergeRects(const cv::Rect& a, const cv::Rect& b) {
        cv::Rect r;
        r.x = std::min(a.x, b.x);
        r.y = std::min(a.y, b.y);
        r.width = std::max(a.x + a.width, b.x + b.width) - r.x;
        r.height = std::max(a.y + a.height, b.y + b.height) - r.y;
        return r;
}

bool mergeCloseFaces(const std::vector<cv::Rect>& faces, cv::Rect* result, double *facewidth) {
        cout << "trying to merge " << faces.size() << " faces" << endl;
        const double kSingleRatioThreshold = 1.5;
        const double kTotalRatioThreshold = 4.0;
        if (faces.size() == 0) {
                return false;
        }
        cv::Rect initial = faces[0];
        *facewidth = initial.width;
        *result = initial;
        size_t i;
        for (i = 1; i < faces.size(); i++) {
                const cv::Rect& f = faces[i];
                double wr = f.width * 1.0 / initial.width;
                double hr = f.height * 1.0 / initial.height;
                if (std::max(wr,  1.0 / wr) > kSingleRatioThreshold) {
                        cout << "wr = " << wr << " this exceeds " << kSingleRatioThreshold << endl;
                        break;
                }
                if (std::max(hr, 1.0 / hr) > kSingleRatioThreshold) {
                        cout << "hr = " << hr << " this exceeds " << kSingleRatioThreshold << endl;
                        break;
                }
                *facewidth += f.width;
                cv::Rect t = mergeRects(*result, f);
                double area_r = t.width * t.height * 1.0 / (result->width * result->height);
                if (area_r > kTotalRatioThreshold) {
                        cout << "area_r = " << area_r << " this exceeds " << kTotalRatioThreshold << endl;
                        break;
                }
                cout << "merging rects" << endl;
                *result = t;
        }
        *facewidth /= i;
        return true;
}

}  // namespace

bool CvDetector::detect(const cv::Mat& img, cv::Rect* r, double *facewidth) {
        std::vector<cv::Rect> faces;
        cascade_.detectMultiScale(img, faces,
                                  1.1, 2, 0
                                  //|cv::CASCADE_FIND_BIGGEST_OBJECT
                                  //|cv::CASCADE_DO_ROUGH_SEARCH
                                  |cv::CASCADE_SCALE_IMAGE
                                  ,
                                  cv::Size(20, 20) );
        return mergeCloseFaces(faces, r, facewidth);
}
