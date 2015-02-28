#include <assert.h>
#include <iostream>
#include "../src/usage_generator.h"
#include "../src/commandline_parser.h"

using namespace std;
using namespace parser;

int main() {
  Generator generator;
  generator.MakeUsage("Usage:")
          ("h,help", "show this information")
          ("a,alpha", "0", "alpha")
          ("r,", "255", "color red")
          ("g,green", "color green")
          (",blue", "1", "color blue");
  cout << generator;

  CParser* parser = generator.MakeParser();
  const char* argv = "path/to/this -h -ra 10 --green";
  parser->parse(5, argv);
  parser->dump();

  assert(parser->has("a"));
  assert(parser->has("a") == parser->has("alpha"));
  assert(parser->has("green"));
  assert(parser->get("a")->as<int>() == 10);
  assert(parser->get("blue")->as<int>() == 1);

  return 0;
}