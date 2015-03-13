#include <cassert>
#include <iostream>
#include "../src/commandline_parser.h"

using namespace std;

int main() {
  parser::CParser parser;

  cout << "=====> Basic Tests <=====" << endl;
  // const int argc = 11;
  // const char *argv[] = {"path/to/program", "-o", "--option", "-O", "3",
  //                      "--option", "value", "-xyz", "3.14", "-O", "33"};
  // parser.parse(argc, argv);
  parser.parse(
      "path/to/program -o --option -O 3 --option=v1,v2 -xyz 3.14 -O=33");
  {
    cout << "# show parse results, ";
    cout << "'set' means the option was set but without value." << endl;

    parser.dump();
  }
  cout << endl;
  {
    cout << "# get value with specified type." << endl;

    cout << parser.get("O")->as<int>() << endl;
    cout << parser.get("option")->as<string>() << endl;
    cout << parser.get("z")->as<double>() << endl;
  }
  cout << endl;
  {
    cout << "# verify whether option exist." << endl;

    cout << parser.has("t") << endl;
    cout << parser.has("option") << endl;
    cout << parser.has("3") << endl;
    cout << parser.has("-yxz") << endl;
  }
  cout << endl;
  {
    cout << "# verify whether a sequence of option exist." << endl;

    // 'has_or' and 'has_and' are deprecated,
    // use std::initializer_list instead.
    // cout << parser.has_and(3, "x", "y", "option") << endl;
    // cout << parser.has_and(2, "a", "x") << endl;
    // cout << parser.has_or(2, "a", "x") << endl;
    // cout << parser.has_or(2, "xy", "z") << endl;

    cout << parser.has_and({"x", "y", "option"}) << endl;
    cout << parser.has_and({"a", "x"}) << endl;
    cout << parser.has_or({"a", "x"}) << endl;
    cout << parser.has_or({"xy", "z"}) << endl;
  }
  cout << endl;

  cout << "=====> Accuracy Tests <=====" << endl;
  {
    cout << "# return nullptr string when option is not exist." << endl;
    assert(parser.get("happy") == nullptr);
    assert(parser.get("value") == nullptr);
    assert(parser.get("option") != nullptr);
  }
  cout << endl;
  {
    const char *argv_accuracy[] = {"path/to/program", "-o", "-"};
    try {
      parser.parse(3, argv_accuracy);
    } catch (const parser::OptionError &err) {
      cout << err.what() << endl;
    }
  }
  cout << endl;
  {
    const char *argv_accuracy[] = {"path/to/program", "--"};
    try {
      parser.parse(2, argv_accuracy);
    } catch (const parser::OptionError &err) {
      cout << err.what() << endl;
    }
  }
  cout << endl;
  {
    const char *argv_accuracy[] = {"path/to/program", "abc", "xyz"};
    try {
      parser.parse(3, argv_accuracy);
    } catch (const parser::OptionError &err) {
      cout << err.what() << endl;
    }
  }

  return 0;
}
