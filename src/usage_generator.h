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
    bool require_value;
  };

  Generator();

  Generator& MakeUsage(const char* first_line);

  Generator& operator()(const char* option, const char* description);

  Generator& operator()(const char* option, const char* default_value,
                        const char* description);

 private:
  friend std::ostream& operator<<(std::ostream& out,
                                  const Generator& generator);

  std::vector<Row>::iterator add_usage_line(const char* option,
                                            const char* default_value,
                                            const char* description);

  const char kDelimeter = ',';
  const char* first_line_;
  std::vector<Row> chain_;
};

}

#endif // USAGE_GENERATOR_H_