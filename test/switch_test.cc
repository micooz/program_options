#include "program_options/switch.hpp"
#include <string>

using namespace program_options;

class User {
 public:
  explicit User(int age) : age_(age) {}

  bool operator<(const User& user) const { return this->age_ < user.age(); }

  int age() const { return age_; }

 private:
  int age_;
};

int main(int argc, char* argv[]) {
  #define printl(line) printf((line)); printf("\n")
  std::string expression("windows");

  Switch<std::string> switcher(expression);
  switcher
    .found("apple", []() {
      printl("it's apple");
    })
    .found("windows", []() {
      printl("it's windows");
    })
    .found("linux", []() {
      printl("it's linux");
    }).done();

  expression = "unix";
  program_options::select(expression)
    .found("unix", []() {
      printl("it's unix");
    })
    .found("Unix", []() {
      printl("it's Unix");
    })
    .others([]() {
      printl("nothing found");
    });

  program_options::select(100)
    .found(10, []() {
      printl("it's 10");
    })
    .found(20, []() {
      printl("it's 20");
    })
    .others([]() {
      printl("nothing found");
    });

  User u1(20), u2(22), ux(20);
  program_options::select(ux)
    .found(u1, []() {
      printl("it's u1");
    })
    .found(u2, []() {
      printl("it's u2");
    }).done();

  return 0;
}