#include "usage_generator.h"

namespace parser
{

Generator::Generator()
        : first_line_(nullptr) {

}

Generator& Generator::MakeUsage(const char* first_line) {
  chain_.clear();
  first_line_ = first_line;
  return *this;
}

Generator& Generator::operator()(const char* option, const char* description) {
  this->add_usage_line(option, "", description);
  return *this;
}

Generator& Generator::operator()(const char* option, const char* default_value,
                                 const char* description) {
  this->add_usage_line(option, default_value, description);
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Generator& generator) {
  auto begin = generator.chain_.begin();
  auto end = generator.chain_.end();

  std::for_each(begin, end, [&out](const Generator::Row& row) {
    out << "  ";
    if (!row.option_short.empty()) {
      out << "-" << row.option_short << " ";
    }
    if (!row.option_long.empty()) {
      if (!row.option_short.empty())
        out << "[ --" << row.option_long << " ] ";
      else
        out << "--" << row.option_long << " ";
    }

    if (!row.default_value.empty()) {
      out << "arg = " << row.default_value << " ";
    }

    out << row.description << std::endl;
  });

  return out;
}

void Generator::add_usage_line(const char* option, const char* default_value,
                               const char* description) {
  std::string option_str(option);
  auto delimeter_pos = option_str.find(kDelimeter);

  std::string option_short;
  std::string option_long;

  if (delimeter_pos != std::string::npos) {
    option_short.assign(std::move(option_str.substr(0, delimeter_pos)));
    option_long.assign(std::move(option_str.substr(delimeter_pos + 1)));
  }

  Generator::Row row;
  row.option_short = option_short;
  row.option_long = option_long;
  row.default_value = std::string(default_value);
  row.description = std::string(description);

  chain_.push_back(row);
}

}
