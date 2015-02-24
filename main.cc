#include <iostream>
#include "src/commandline_parser.h"

using namespace std;

const char *usage[] = {
        "This is the usage of commanline_parser, it's very easy to use.",
        "You only need two files in src/ without depending a huge library like boost.",
        "Just like this simple program you run, ",
        "",
        "1) Include the header 'commandline_parser.h'.",
        "2) Declare an object associate with CParser.",
        "3) Call CParser::parse() manually.",
        "4) Verify an option by calling has(),has_or(),has_and()",
        "5) To get a value from specified option by calling get() and then as<T>().",
        "",
        "For more details and examples, see test/*.cc",
        nullptr
};

int main(int argc, const char *argv[]) {
  parser::CParser parser;
  try {
    parser.parse(argc, argv);
  } catch (const parser::OptionError &err) {
    cout << err.what() << endl;
  }

  parser.dump();

  int n = 0;
  while (usage[n])
    cout << usage[n++] << endl;

  return 0;
}