#include "generator/subroutine.h"
#include <algorithm>
#include <iostream>
#include <sstream>

namespace program_options {

Subroutine::Subroutine() : first_line_(nullptr) {}

Subroutine::Subroutine(const char* name, const char* description)
    : first_line_(nullptr), description_(description), name_(name) {
  usages_.reserve(5);
}

void Subroutine::add_usage_line(const Row& row) {
  usages_.push_back(row);
}

std::ostream& operator<<(std::ostream& out, Subroutine& subroutine) {
  // print the subroutine name and its description
  //  out << subroutine.get_name() << "\t" << subroutine.get_description()
  //      << std::endl;
  if (subroutine.get_first_line()) {
    // print the first line
    out << subroutine.get_first_line() << std::endl;
  }
  auto begin = subroutine.begin();
  auto end = subroutine.end();

  std::vector<std::stringstream> row_list;
  row_list.reserve(subroutine.size());

  // build usage rows without description field,
  // find the max-len row at the same time.
  size_t max_len = 0;
  std::for_each(begin, end, [&max_len, &row_list](const Row& row) {
    std::stringstream ss;
    ss << "  ";
    if (!row.oshort().empty()) {
      ss << "-" << row.oshort() << " ";
    }
    if (!row.olong().empty()) {
      if (!row.oshort().empty())
        ss << "[ --" << row.olong() << " ] ";
      else
        ss << "--" << row.olong() << " ";
    }

    if (row.required()) {
      ss << "arg ";
      if (!row.value().empty()) {
        ss << "= " << row.value() << " ";
      }
    }

    max_len = std::max(max_len, ss.str().size());
    row_list.push_back(std::move(ss));
  });

  // show all rows and align description field
  size_t row_count = subroutine.size();
  for (size_t i = 0; i < row_count; ++i) {
    std::string str_row(std::move(row_list[i].str()));
    // print row without description
    out << str_row;
    // print spaces
    size_t spaces = 0;
    size_t len = str_row.size();
    if (max_len > len) spaces = max_len - len;

    while (spaces--) {
      out << " ";
    }
    // print description
    out << subroutine.at(i).desc() << std::endl;
  }
  out << std::endl;

  return out;
}
}
