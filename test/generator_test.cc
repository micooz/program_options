#include <iostream>
#include "generator/generator.h"

using namespace std;

int main() {
  program_options::Generator generator;

  generator.MakeUsage("Usage for example:")
          ("h,help", "show help imformation")
          ("o,option", "", "this is an option")
          (",option-2", "this is the option 2")
          ("x,", "this is the option x")
          // invalid option format (no , found), it will be ignored.
          ("c", "this is option c")
          ("p,position", "0.0", "position of a place");

  cout << generator;

  return 0;
}
