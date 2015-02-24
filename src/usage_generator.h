/*
* Command Line Usage Generator
* (c) Copyright 2015 Micooz
*
* Released under the Apache License.
* See the LICENSE file or
* http://www.apache.org/licenses/LICENSE-2.0.txt for more information.
*/
#ifndef USAGE_GENERATOR_H_
#define USAGE_GENERATOR_H_

#include <iostream>
#include <vector>
#include <string>

namespace parser
{

class Generator {
 public:
  struct Row {
    std::string option_short;
    std::string option_long;
    std::string default_value;
    std::string description;
  };

  Generator();

  Generator& MakeUsage(const char* first_line);

  Generator& operator()(const char* option, const char* description);

  Generator& SetDefault(const char* value);

 private:
  const char kDelimeter = ',';
  const char* first_line_;
  std::vector<Row> chain_;

  friend std::ostream& operator<<(std::ostream& out, const Generator& generator);
};

}

#endif // USAGE_GENERATOR_H_