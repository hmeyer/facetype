#include "ascii.h"
#include <aalib.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

Ascii::Ascii() :p_(new aa_renderparams({47, 33, 4.0, AA_FLOYD_S, 1, 0})) {
  aa_recommendhidisplay("linux");
  aa_hardwareparams hp = aa_defparams;
  hp.supported &= ~AA_DIM_MASK;
  hp.supported |= AA_BOLD_MASK;
  hp.supported &= ~AA_BOLDFONT_MASK;
  hp.supported &= ~AA_EXTENDED;
  hp.supported &= ~AA_EIGHT;
  ctx_ = aa_autoinit(&hp);
  if(!ctx_) throw std::runtime_error("could not initialize aalib");
}
Ascii::~Ascii() {
  aa_close(ctx_);
}
bool Ascii::eventHappened() {
  return false;
}
double Ascii::aspect() const {
  return aa_mmwidth(ctx_) * 1.0 / aa_mmheight(ctx_);
}
void Ascii::displayImage(const cv::Mat* im) {
  if (im->depth() != CV_8U) throw std::runtime_error("input image not of depth CV_8U");
  if (im->elemSize() != 1) throw std::runtime_error("input image element size != 1");
  cv::Mat scaledIm;
  int w = aa_imgwidth(ctx_);
  int h = aa_imgheight(ctx_);
  resize(*im, scaledIm, Size(w, h), 0, 0, INTER_AREA);
  uint8_t* aa_ptr = aa_image(ctx_);
  if (!aa_ptr) throw std::runtime_error("invalid aa image ptr");
  uint8_t* im_ptr = scaledIm.data;
  if (!im_ptr) throw std::runtime_error("invalid image ptr");
  for(int y = 0; y < h; y++) {
    memcpy(aa_ptr, im_ptr, w);
    aa_ptr += w;
    im_ptr += scaledIm.step;
  }
  aa_render(ctx_, p_.get(), 0, 0, aa_scrwidth(ctx_), aa_scrheight(ctx_));
  aa_flush(ctx_);
}

