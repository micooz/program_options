/*
 * subroutine
 * (c) Copyright 2015 Micooz
 *
 * Released under the Apache License.
 * See the LICENSE file or
 * http://www.apache.org/licenses/LICENSE-2.0.txt for more information.
 */
#ifndef GENERATOR_SUBROUTINE_H_
#define GENERATOR_SUBROUTINE_H_

#include <vector>
#include <string>
#include "row.h"

namespace program_options {

class Subroutine {
 public:
  typedef std::vector<Row> Usages;

  Subroutine();

  Subroutine(const char* name, const char* description);

  inline std::string get_name() const { return name_; }

  inline void set_first_line(const char* line) { first_line_ = line; }

  inline void set_description(const char* description) {
    description_ = description;
  }

  inline const char* get_description() const { return description_; }

  inline const char* get_first_line() const { return first_line_; }

  void add_usage_line(const Row& row);

  inline Usages::iterator begin() { return usages_.begin(); }

  inline Usages::iterator end() { return usages_.end(); }

  inline size_t size() { return usages_.size(); }

  inline Row& at(size_t i) { return usages_.at(i); }

  inline const Usages& get_usage() const { return usages_; }

  inline static const char* get_default_name() { return "EmptySubroutine"; }

 private:
  friend std::ostream& operator<<(std::ostream& out, Subroutine& subroutine);

  Usages usages_;
  const char* first_line_;
  const char* description_;
  std::string name_;
};
}
#endif  // GENERATOR_SUBROUTINE_H_