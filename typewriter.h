#include <string>

enum Boldness {
        kNormal,
        kBold
};

class Typewriter {
public:
Typewriter();
~Typewriter() {
        set_bold(kNormal);
        enable_mod(false);
        enable_shift(false);
        enable_code (false);
};
void print_char(char c, Boldness b=kNormal);
void print_string(const std::string& s, Boldness b=kNormal);
void press_key(int key);
void enable_shift(bool enable);
void enable_mod(bool enable);
void enable_code(bool enable);
void wait_for_space();
bool should_stop() const;
private:
void reset_lf();
void print_char_base(char c, bool undead=false);
void set_bold(Boldness b);
Boldness bold_ = kNormal;
bool code_enabled_ = false;
bool mod_enabled_ = false;
bool shift_enabled_ = false;
};
