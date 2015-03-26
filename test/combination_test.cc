#include <assert.h>
#include <iostream>
#include "generator/generator.h"
#include "parser/parser.h"

using namespace std;
using namespace program_options;

int main() {
  Generator generator;
  generator.MakeUsage("Usage:")
          ("h,help", "show this information")
          ("a,alpha", "0", "alpha")
          ("r,", "255", "color red")
          ("g,green", "color green")
          (",blue", "1", "color blue");
  cout << generator;

  Parser* parser = generator.MakeParser();
  parser->parse("path/to/this -h -ra 10 --green=0");
  parser->dump();

  assert(parser->has("a"));
  assert(parser->has("a") == parser->has("alpha"));
  assert(parser->has("green"));
  assert(parser->get("a")->as<int>() == 10);
  assert(parser->get("blue")->as<int>() == 1);

  return 0;
}