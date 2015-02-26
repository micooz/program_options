/*
* Command Line Parser
* (c) Copyright 2015 Micooz
*
* Released under the Apache License.
* See the LICENSE file or
* http://www.apache.org/licenses/LICENSE-2.0.txt for more information.
*/
#ifndef COMMANDLINE_PARSER_H_
#define COMMANDLINE_PARSER_H_

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "usage_generator.h"

namespace parser
{

// class OptionError

class OptionError : public std::exception {
 public:

  explicit OptionError(const std::string& msg);

  const char* what() const throw();

  ~OptionError() throw();

 private:
  std::string _msg;
};

// class CParseItem

class CParseItem {
 public:

  explicit CParseItem(const std::string& val);

  /*
   * dynamic type cast, support base data types including std::string
   */
  template<typename T>
  T as() {
    T r;
    std::stringstream buf;
    buf << _val;
    buf >> r;
    return r;
  }

  /*
   * alias of as<std::string>()
   */
  std::string val() const;

 private:
  std::string _val;
};

// class CommandLineParser

class CParser {
 public:
  typedef std::vector<std::string> ParameterList;
  typedef std::map<std::string, CParseItem*> ParseResult;

  CParser();

  /*
  * release memory allocated in parse()
  */
  ~CParser();

  /*
   * parse the cmd line by given argc and argv
   */
  ParseResult* parse(const int argc, const char** argv);

  /*
   * check whether a certain option exist
   */
  bool has(const char* key);

  /*
   * check whether a sequence of options exist
   * example: has_or(3, "he", "or", "she");
   */
  bool has_or(int n, ...);

  /*
   * check whether a sequence of options exist
   * example: has_and(3, "he", "and", "she");
   */
  bool has_and(int n, ...);

  /*
   * get the specified option value
   */
  CParseItem* get(const std::string& key);

  /*
   * output all ParseResult
   */
  void dump();

  inline void set_usage_chain(const std::vector<Generator::Row> *chain) {
    chain_ = chain;
  }

 private:
  void init();

  void cleanup();

  void set_addition();

  const char** argv_;
  const std::vector<Generator::Row> *chain_;
  int argc_;
  ParameterList args_;
  ParseResult* pr_;
};

}

#endif // COMMANDLINE_PARSER_H_
