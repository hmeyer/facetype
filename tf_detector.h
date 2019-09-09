#ifndef TF_DETECTOR_H
#define TF_DETECTOR_H

#include "detector.h"
#include <string>
#include <array>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model.h"
//#include "tensorflow/lite/graph_info.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/stderr_reporter.h"

class TfDetector : public Detector {
public:
  TfDetector(const std::string& model_filename, const std::string& label_filename);
protected:
  bool detect(const cv::Mat& img, cv::Rect* r) override;
private:
  bool process_outputs(int img_width, int img_height, cv::Rect* r);

  float threshold_ = 0.1;
  tflite::StderrReporter error_reporter_;
  std::unique_ptr<tflite::FlatBufferModel> model_;
  tflite::ops::builtin::BuiltinOpResolver resolver_;
  std::unique_ptr<tflite::Interpreter> interpreter_;
  int width_, height_, channels_;
  // Outputs
  std::array<TfLiteTensor*, 4> outputs_;
  std::vector<std::string> labels_;
};

#endif
