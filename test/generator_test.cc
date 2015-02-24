#include <iostream>
#include "../src/usage_generator.h"

using namespace std;

int main() {
  parser::Generator generator;

  generator.MakeUsage("Usage for example:")
          ("h,help", "show help imformation")
          ("o,option", "this is an option")
          (",option-2", "this is the option 2")
          ("c,", "this is option c")
          ("p,position", "position of a place").SetDefault("0.0");

  cout << generator;

  return 0;
}
