#ifndef ASCII_H
#define ASCII_H

#include <memory>

namespace cv {
class Mat;
} // namespace cv

struct aa_context;
struct aa_renderparams;

class Ascii {
public:
Ascii();
~Ascii();
bool eventHappened();
double aspect() const;
void displayImage(const cv::Mat* im);
protected:
aa_context* ctx_;
std::unique_ptr<aa_renderparams> p_;
};

#endif // ASCII_H
