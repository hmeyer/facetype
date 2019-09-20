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
Ascii(int width, int height);
~Ascii();
bool eventHappened();
double aspect() const;
typedef std::function<void (char c, bool bold)> Printer;
void displayImage(const cv::Mat* im, Printer printer);
protected:
aa_context* ctx_;
std::unique_ptr<aa_renderparams> p_;
};

#endif // ASCII_H
