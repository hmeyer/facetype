#include <string>

enum Boldness {
        kNormal,
        kBold
};

class BaseTypewriter {
 public:
  BaseTypewriter() {}
  virtual ~BaseTypewriter() {};
  virtual void print_char(char, Boldness =kNormal) {}
  virtual void print_string(const std::string&, Boldness =kNormal) {}
  virtual void print_align_right(const std::string&, Boldness =kNormal) {}
  virtual void wait_for_space() {}
  virtual bool should_stop() const { return false; }
};

class Typewriter : public BaseTypewriter {
 public:
  Typewriter();
  ~Typewriter() override {
    set_bold(kNormal);
    enable_mod(false);
    enable_shift(false);
    enable_code(false);
  };
  static const int kWidth = 70;
  static const int kHeight = 55;
  void print_char(char c, Boldness b=kNormal) override;
  void print_string(const std::string& s, Boldness b=kNormal) override;
  void print_align_right(const std::string& s, Boldness b=kNormal) override;
  void wait_for_space() override;
  bool should_stop() const override;
 private:
  void press_key(int key);
  void enable_shift(bool enable);
  void enable_mod(bool enable);
  void enable_code(bool enable);
  void reset_lf();
  void print_char_base(char c, bool undead=false);
  void set_bold(Boldness b);
  Boldness bold_ = kNormal;
  bool code_enabled_ = false;
  bool mod_enabled_ = false;
  bool shift_enabled_ = false;
};

