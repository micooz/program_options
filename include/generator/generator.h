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
#include <map>
#include <string>
#include "subroutine.h"

namespace program_options {

class Parser;

class Generator {
 public:
  typedef std::map<std::string, Subroutine*> SubroutineCollection;

  Generator();

  ~Generator();

  // begin a new usage generate process and
  // create the first line that you will see in the result.
  // if you don't want to create the first line,
  // it's ok to create your usage lines without calling make_usage() first.
  Generator& make_usage(const char* first_line);

  Parser* make_parser();

  // generate usage lines for a subroutine.
  Generator& add_subroutine(const char* name);

  Generator& add_subroutine(const char* name, const char* description);

  // get pairs of subroutine name and its description.
  std::map<std::string, std::string> get_subroutine_list();

  // add an usage line, it doesn't require a value
  inline Generator& operator()(const char* option, const char* description) {
    this->add_usage_line(option, "", description);
    (get_subroutine()->end() - 1)->required(false);
    return *this;
  }

  // add an usage line, it requires a value(may has a default value)
  inline Generator& operator()(const char* option, const char* default_value,
                               const char* description) {
    this->add_usage_line(option, default_value, description);
    return *this;
  }

  // this method is for subroutine usage printing.
  // e.g., cout << generator("subroutine_name");
  inline Subroutine& operator()(const char* name) {
    return *subroutines_.at(name);
  }

  inline size_t get_subroutines_num() const { return subroutines_.size(); }

 private:
  inline Subroutine* get_subroutine() {
    return subroutines_.at(current_subroutine);
  }

  friend std::ostream& operator<<(std::ostream& out, Generator& generator);

  // add a usage line.
  // if delimeter doesn't exist, return false
  bool add_usage_line(const char* option, const char* default_value,
                      const char* description);

  const char kDelimeter = ',';
  SubroutineCollection subroutines_;
  std::string current_subroutine;
  Parser* parser_;
};
}

#endif  // GENERATOR_GENERATOR_H_