#include "gtest/gtest.h"
#include "generator/generator.h"
#include "parser/parser.h"

using namespace program_options;

TEST(TestConbination, combination){
  Generator gen;
  gen.make_usage("Usage:").add_subroutine("make")
    ("h,help", "show this information")
    ("a,alpha", "0", "alpha")
    ("r,", "255", "color red")
    ("g,green", "color green")
    (",blue", "1", "color blue");
  std::cout << gen;
  
  Parser* parser = gen.make_parser();
  parser->parse("path/to/this make -h -ra 10 --green=0");
  
  EXPECT_STREQ(parser->get_subroutine_name().c_str(), "make");
  EXPECT_TRUE(parser->has("a"));
  EXPECT_TRUE(parser->has("green"));
  EXPECT_EQ(parser->has("a"), parser->has("alpha"));
  EXPECT_EQ(parser->get("a")->as<int>(), 10);
  EXPECT_EQ(parser->get("blue")->as<int>(), 1);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
