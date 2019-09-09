#include "tf_detector.h"
#include "tensorflow/lite/c/c_api_internal.h"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

namespace {
std::string DescribeTensor(const TfLiteTensor& t) {
  TfLiteIntArray* dims = t.dims;
  std::string result = TfLiteTypeGetName(t.type);
  result += "[";
  for (int i = 0; i < dims->size; i++) {
    result += std::to_string(dims->data[i]);
    if (i + 1 < dims->size) result += "x";
  }
  result += "]";
  return result;
}

std::vector<std::string> GetLabels(const std::string& filename) {
  std::ifstream in(filename);
  if(!in) {
    throw std::invalid_argument("cannot open " + filename);
  }
  std::string str;
  std::vector<std::string> result;
  while (std::getline(in, str)) {
    result.push_back(str);
  }
  in.close();
  return result;
}
}

TfDetector::TfDetector(const std::string& model_filename, const std::string& label_filename) {
  labels_ = GetLabels(label_filename);
  model_ = tflite::FlatBufferModel::BuildFromFile(model_filename.c_str(), &error_reporter_);
  if (model_ == nullptr) {
    throw std::invalid_argument("Cannot load " + model_filename);
  }
  tflite::InterpreterBuilder builder(*model_, resolver_);
  if (builder(&interpreter_) != kTfLiteOk) {
    throw std::runtime_error("unable to initialize tflite interpreter");
  }
  if (interpreter_->AllocateTensors() != kTfLiteOk) {
    throw std::runtime_error("unable to allocate tensors");
  }
  const int kInputSize = interpreter_->inputs().size();
  if (kInputSize != 1) {
    throw std::invalid_argument("Models has " + std::to_string(kInputSize) + " inputs. Expected 1 input.");
  }
  int input = interpreter_->inputs()[0];
  TfLiteIntArray* dims = interpreter_->tensor(input)->dims;
  assert(dims->size >= 4);
  assert(interpreter_->tensor(input)->type == kTfLiteUInt8);
  assert(dims->data[3] == 3);
  height_ = dims->data[1];
  width_ = dims->data[2];
  channels_ = dims->data[3];
  std::cout << "input: " << DescribeTensor(*interpreter_->tensor(input)) << std::endl;
  const int kOutputSize = interpreter_->outputs().size();
  if (kOutputSize != 4) {
    throw std::invalid_argument("Models has " + std::to_string(kOutputSize) + " outputs. Expected 4 outputs.");
  }
  for(int i = 0; i < kOutputSize; i++) {
    outputs_[i] = interpreter_->tensor(interpreter_->outputs()[i]);
    std::cout << "output: " << DescribeTensor(*outputs_[i]) << std::endl;
  }
}

namespace {
int TensorDataOffset(TfLiteTensor* tensor) {
  int nelems = 1;
  for (int i = 1; i < tensor->dims->size; i++) {
    nelems *= tensor->dims->data[i];
  }
  return nelems;
}

float* TensorDataF(TfLiteTensor* tensor, int batch_index) {
  if (tensor->type != kTfLiteFloat32) {
    const std::string msg = "invalid tensor type (expected float32): ";
    throw std::invalid_argument(msg + TfLiteTypeGetName(tensor->type));
  }
  return tensor->data.f + TensorDataOffset(tensor) * batch_index;
}

uint8_t* TensorDataU8(TfLiteTensor* tensor, int batch_index) {
  if (tensor->type != kTfLiteUInt8) {
    const std::string msg = "invalid tensor type (expected uint8): ";
    throw std::invalid_argument(msg + TfLiteTypeGetName(tensor->type));
  }
  return tensor->data.uint8 + TensorDataOffset(tensor) * batch_index;
}
}

bool TfDetector::process_outputs(int img_width, int img_height, cv::Rect* r) {
  const int    num_detections    = *TensorDataF(outputs_[3], 0);
  const float* detection_classes =  TensorDataF(outputs_[1], 0);
  const float* detection_scores  =  TensorDataF(outputs_[2], 0);
  const float* detection_boxes   =  TensorDataF(outputs_[0], 0);
  float best_threshold = 0.0f;
  for (int i=0; i<num_detections; i++) {
    // Get class
    const int cls = detection_classes[i] + 1;

    // Ignore first one
    if (cls == 0) continue;

    // Get score
    float score = detection_scores[i];

    // Check minimum score
    if (score < threshold_) break;

    if (score > best_threshold) {
      best_threshold = score;
      // Get coordinates
      const float top    = detection_boxes[4 * i]     * img_height;
      const float left   = detection_boxes[4 * i + 1] * img_width;
      const float bottom = detection_boxes[4 * i + 2] * img_height;
      const float right  = detection_boxes[4 * i + 3] * img_width;
      r->x = left;
      r->y = top;
      r->width = right - left;
      r->height = bottom - top;
      std::cout << "detection: cls: " << labels_.at(cls) << " score: " << score;
      std::cout << " [" << r->x << ", " << r->y << ", " << r->width << ", " << r->height << "]";
      std::cout << std::endl;
    }
  }
  return best_threshold > 0.0f;
}

bool TfDetector::detect(const cv::Mat& img, cv::Rect* r) {
  cv::Size size(width_, height_);
  cv::Mat resized;
  std::cout << "resizing" << std::endl;
  cv::resize(img, resized, size);
  int input = interpreter_->inputs()[0];
  uint8_t *tf_input = interpreter_->typed_tensor<uint8_t>(input);
  const int row_bytes = width_ * channels_;
  uint8_t* im_ptr = resized.data;
  for(int y = 0; y < height_; y++) {
    memcpy(tf_input, im_ptr, row_bytes);
    tf_input += row_bytes;
    im_ptr += resized.step;
  }
  std::cout << "invoking tflite interpreter" << std::endl;
  if (interpreter_->Invoke() != kTfLiteOk) {
    throw std::runtime_error("Failed to invoke interpreter");
  }
  return process_outputs(img.size[1], img.size[0], r);
}
