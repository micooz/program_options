#include "gtest/gtest.h"
#include "generator/generator.h"

using namespace program_options;

TEST(TestGenerator, generating) {
  Generator gen;

  gen.make_usage("");
  std::cout << gen << "----------\n";

  gen.make_usage("this is an usage information");
  std::cout << gen << "----------\n";

  gen.make_usage("usage of subroutine_1:").add_subroutine
    ("subroutine_1", "description of subroutine_1")
    ("a,", "option with a short-form")
    (",long", "option with a long-form")
    ("option", "this option will be ignored")
    ("o,option", "option with both short and long form");

  gen.add_subroutine
    ("subroutine_2", "description of subroutine_2")
    ("o,option", "", "option require a value")
    ("p,person", "micooz", "option with default value");

  auto kv = gen.get_subroutine_list();
  for(auto sub : kv) {
    std::cout << "\t" << sub.first << "\t" << sub.second << std::endl;
  }
  std::cout << "----------\n";
  
  std::cout << gen("subroutine_1") << "----------\n";
  std::cout << gen("subroutine_2") << "----------\n";
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
