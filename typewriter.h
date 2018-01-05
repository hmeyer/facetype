#include <string>

enum Boldness {
 kNormal,
 kBold
};

class Typewriter {
 public:
  Typewriter();
  void print_char(char c, Boldness b=kNormal); 
  void print_string(const std::string& s, Boldness b=kNormal); 
  void press_key(int key);
  void enable_shift(bool enable);
  void enable_mod(bool enable);
  void enable_code(bool enable);
private:
  size_t line_chars_ = 0;
};
