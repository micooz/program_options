/*
* generator
* (c) Copyright 2015 Micooz
*
* Released under the Apache License.
* See the LICENSE file or
* http://www.apache.org/licenses/LICENSE-2.0.txt for more information.
*/
#ifndef GENERATOR_GENERATOR_H_
#define GENERATOR_GENERATOR_H_

#include <iostream>
#include <vector>
#include <string>

namespace program_options {

class Parser;

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

  ~Generator();

  inline Generator& MakeUsage(const char* first_line) {
    chain_.clear();
    first_line_ = first_line;
    return *this;
  }

  Parser* MakeParser();

  inline Generator& operator()(const char* option, const char* description) {
    this->add_usage_line(option, "", description);
    return *this;
  }

  inline Generator& operator()(const char* option, const char* default_value,
                               const char* description) {
    bool added = this->add_usage_line(option, default_value, description);
    if (added) {
      auto last = chain_.end() - 1;
      last->require_value = true;
    }
    return *this;
  }

  inline std::vector<Row>& GetChain() { return chain_; }

 private:
  friend std::ostream& operator<<(std::ostream& out,
                                  const Generator& generator);

  bool add_usage_line(const char* option, const char* default_value,
                      const char* description);

  const char kDelimeter = ',';
  const char* first_line_;
  std::vector<Row> chain_;
  Parser* parser_;
};
}

#endif  // GENERATOR_GENERATOR_H_
