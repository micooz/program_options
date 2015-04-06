#include "generator/generator.h"
#include "parser/parser.h"

namespace program_options {

Generator::Generator() : parser_(nullptr) {}

Generator::~Generator() {
  if (parser_) {
    delete parser_;
    parser_ = nullptr;
  }
  for (auto it = subroutines_.begin(); it != subroutines_.end(); ++it) {
    if (it->second) {
      delete it->second;
      it->second = nullptr;
    }
  }
}

Generator& Generator::make_usage(const char* first_line) {
  if (current_subroutine.empty()) {
    current_subroutine.assign(Subroutine::get_default_name());
    add_subroutine(Subroutine::get_default_name());
  }
  get_subroutine()->set_first_line(first_line);
  return *this;
}

Parser* Generator::make_parser() {
  if (parser_) delete parser_;

  parser_ = new Parser;
  parser_->set_usage_subroutines(&subroutines_);

  return parser_;
}

Generator& Generator::add_subroutine(const char* name) {
  if (subroutines_.find(name) == subroutines_.end()) {
    current_subroutine.assign(name);
    Subroutine* routine = new Subroutine(current_subroutine.c_str(), "");
    subroutines_.insert({current_subroutine, routine});
  }
  return *this;
}

Generator& Generator::add_subroutine(const char* name,
                                     const char* description) {
  add_subroutine(name);
  if (subroutines_.find(name) != subroutines_.end()) {
    get_subroutine()->set_description(description);
  }
  return *this;
}

std::map<std::string, std::string> Generator::get_subroutine_list() {
  std::map<std::string, std::string> kv;
  for (auto pr : subroutines_) {
    Subroutine* subroutine = pr.second;
    if (subroutine->get_name() != Subroutine::get_default_name())
      kv[subroutine->get_name()] = subroutine->get_description();
  }
  return std::move(kv);
}

std::ostream& operator<<(std::ostream& out, Generator& generator) {
  for (auto pr : generator.subroutines_) {
    Subroutine* subroutine = pr.second;
    if (subroutine->get_name() != Subroutine::get_default_name()) {
      out << subroutine->get_name() << "\t";
    }
    out << subroutine->get_description() << std::endl;
    out << *subroutine;
  }
  return out;
}

bool Generator::add_usage_line(const char* option, const char* default_value,
                               const char* description) {
  std::string option_str(option);
  auto delimeter_pos = option_str.find(kDelimeter);

  std::string option_short;
  std::string option_long;

  if (delimeter_pos != std::string::npos) {
    option_short.assign(std::move(option_str.substr(0, delimeter_pos)));
    option_long.assign(std::move(option_str.substr(delimeter_pos + 1)));

    Row row;
    row.oshort(option_short);
    row.olong(option_long);
    row.value(default_value);
    row.desc(description);

    get_subroutine()->add_usage_line(row);
    return true;
  }
  return false;
}
}
