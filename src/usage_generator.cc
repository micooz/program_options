#include "usage_generator.h"
#include <sstream>
#include "commandline_parser.h"

namespace parser
{

Generator::Generator()
        : first_line_(nullptr), parser_(nullptr) {

}

Generator::~Generator() {
  if (parser_) {
    delete parser_;
    parser_ = nullptr;
  }
}

Generator& Generator::MakeUsage(const char* first_line) {
  chain_.clear();
  first_line_ = first_line;
  return *this;
}

CParser* Generator::MakeParser() {
  if (parser_)
    delete parser_;

  parser_ = new CParser;
  parser_->set_usage_chain(&chain_);

  return parser_;
}

Generator& Generator::operator()(const char* option, const char* description) {
  this->add_usage_line(option, "", description);
  return *this;
}

Generator& Generator::operator()(const char* option, const char* default_value,
                                 const char* description) {
  bool added = this->add_usage_line(option, default_value, description);
  if (added) {
    auto last = chain_.end() - 1;
    last->require_value = true;
  }
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Generator& generator) {
  out << generator.first_line_ << std::endl;

  auto begin = generator.chain_.begin();
  auto end = generator.chain_.end();

  std::vector<std::stringstream> row_list;
  row_list.reserve(generator.chain_.size());

  // build usage rows without description field,
  // find the max-len row at the same time.
  size_t max_len = 0;
  std::for_each(begin, end, [&max_len, &row_list](const Generator::Row& row) {
    std::stringstream ss;
    ss << "  ";
    if (!row.option_short.empty()) {
      ss << "-" << row.option_short << " ";
    }
    if (!row.option_long.empty()) {
      if (!row.option_short.empty())
        ss << "[ --" << row.option_long << " ] ";
      else
        ss << "--" << row.option_long << " ";
    }

    if (row.require_value) {
      ss << "arg ";
      if (!row.default_value.empty()) {
        ss << "= " << row.default_value << " ";
      }
    }

    max_len = std::max(max_len, ss.str().size());
    row_list.push_back(std::move(ss));
  });

  // show all rows and align description field
  size_t row_count = generator.chain_.size();
  for (size_t i = 0; i < row_count; ++i) {
    std::string str_row(std::move(row_list[i].str()));
    // print row without description
    out << str_row;
    // print spaces
    size_t spaces = 0;
    size_t len = str_row.size();
    if (max_len > len)
      spaces = max_len - len;

    while (spaces--) {
      out << " ";
    }
    // print description
    out << generator.chain_[i].description << std::endl;
  }

  return out;
}

bool Generator::add_usage_line(
        const char* option,
        const char* default_value,
        const char* description) {
  std::string option_str(option);
  auto delimeter_pos = option_str.find(kDelimeter);

  std::string option_short;
  std::string option_long;

  if (delimeter_pos != std::string::npos) {
    option_short.assign(std::move(option_str.substr(0, delimeter_pos)));
    option_long.assign(std::move(option_str.substr(delimeter_pos + 1)));

    Generator::Row row;
    row.option_short = option_short;
    row.option_long = option_long;
    row.default_value = std::string(default_value);
    row.description = std::string(description);

    chain_.push_back(row);
    return true;
  }
  return false;
}

}
