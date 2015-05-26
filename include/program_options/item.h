/*
* parser item
* (c) Copyright 2015 Micooz
*
* Released under the Apache License.
* See the LICENSE file or
* http://www.apache.org/licenses/LICENSE-2.0.txt for more information.
*/
#ifndef PARSER_ITEM_H_
#define PARSER_ITEM_H_

#include <sstream>

namespace program_options {

class ParseItem {
 public:
  ParseItem(const std::string& value);

  /*
   * dynamic type cast, support base data types including std::string
   */
  template<typename T>
  T as() {
    T r;
    std::stringstream buf;
    buf << value_;
    buf >> r;
    return r;
  }

  /*
   * alias of as<std::string>()
   */
  inline std::string val() const { return value_; }

  /*
   * returns c-style string, will be useful if you want a const char*
   */
  inline const char* c_str() const { return value_.c_str(); }

 private:
  std::string value_;
};
}

#endif  // PARSER_ITEM_H_