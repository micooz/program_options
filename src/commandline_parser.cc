#include "commandline_parser.h"
#include <iostream>

using namespace std;

namespace parser
{

// class OptionError

OptionError::OptionError(const string &msg)
        : _msg(msg) {

}

const char *OptionError::what() const throw() {
  string msg;
  msg.append("Command line parse error: ").append(_msg).push_back('.');
  return msg.c_str();
}

OptionError::~OptionError() throw() {

}


// class CCParseItem

CParseItem::CParseItem(const string &val)
        : _val(val) {

}

CParseItem *CParser::get(const string &key) {
  if (pr_->find(key) != pr_->end()) {
    return (*pr_)[key];
  }
  return nullptr;
}

string CParseItem::val() const {
  return _val;
}


// class CParser

CParser::CParser()
        : pr_(nullptr) {

}

CParser::~CParser() {
  this->cleanup();
}

CParser::ParseResult *CParser::parse(const int argc, const char **argv) {
  argc_ = argc;
  argv_ = argv;
  this->init();

  auto ibegin = args_.begin() + 1; // ignore the first cmd name
  auto iend = args_.end();
  auto it = ibegin;

  string block;
  string previous(*ibegin);

  for (; it != iend; ++it) {
    block.assign(*it);

    switch (block.size()) {
      case 1:
        if (block == "-") {
          throw OptionError("single '-' is not allowed");
        }
        break;
      case 2:
        if (block[0] == '-') {
          if (block[1] == '-') {
            throw OptionError("option '--' is incomplete");
          } else {
            // single option
            // e.g., ./exec -s
            (*pr_)[block.substr(1)] = nullptr;
          }
        }
        break;
      default: // >=3
        if (block[0] == '-') {
          if (block[1] == '-') {
            // a long format option
            // e.g., ./exec --option
            (*pr_)[block.substr(2)] = nullptr;
          } else {
            // a combination options
            // e.g., ./exec -ab[...]
            auto tbegin = block.begin() + 1; // ignore the first '-'
            auto tend = block.end();
            auto t = tbegin;

            for (; t != tend; ++t) {
              string key;
              key.push_back(*t);
              (*pr_)[key] = nullptr;
            }
          }
        }
        break;
    }// switch

    if (block[0] != '-' && previous != block //not the first option
            ) {

      if (previous[0] != '-') {
        // previous is not an option, error occur
        // e.g., ./exec abc def
        throw OptionError("'" + block + "' is not allowed here");
      }

      string key;

      if (previous[0] == '-' && previous[1] == '-') {
        // previous is a long format option.
        // e.g., ./exec --option value
        key = previous.substr(2);
      } else {
        // it's the value of previous option.
        // e.g., ./exec -o   [...]
        // e.g., ./exec -opq [...]
        key.push_back(*(previous.end() - 1));
      }

      if (pr_->find(key) != pr_->end()) {
        (*pr_)[key] = new CParseItem(block);
      }
    }

    previous = block;
  }// for

  return pr_;
}

bool CParser::has(const char *key) {
  string skey(key);

  if (pr_ && !skey.empty()) {
    if (skey[0] == '-') {
      //check combination options, e.g., CParser::has("-xyz")
      for (size_t i = 1; i < skey.size(); ++i) {
        string tkey;
        tkey.push_back(skey[i]);
        if (pr_->find(tkey) == pr_->end()) {
          return false;
        }
      }
      return true;
    } else {
      // check single option, e.g., CParser::has("x")
      return pr_->find(skey) != pr_->end();
    }
  }
  return false;
}

bool CParser::has_or(int n, ...) {
  va_list keys;
  va_start(keys, n);
  while (n--) {
    const char *key = va_arg(keys, const char *);
    if (this->has(key)) {
      return true;
    }
  }
  va_end(keys);
  return false;
}

bool CParser::has_and(int n, ...) {
  va_list keys;
  va_start(keys, n);
  while (n--) {
    const char *key = va_arg(keys, const char *);
    if (!this->has(key)) {
      return false;
    }
  }
  va_end(keys);
  return true;
}

void CParser::dump() {
  if (pr_) {
    auto ibegin = pr_->begin();
    auto iend = pr_->end();
    auto it = ibegin;
    for (; it != iend; ++it) {
      cout << it->first;
      if (it->second) {
        cout << " = " << it->second->val() << endl;
      } else {
        cout << " set" << endl;
      }
    }
  }
}

void CParser::init() {
  this->cleanup();

  args_.reserve(static_cast<size_t>(argc_));

  for (int i = 0; i < argc_; ++i) {
    args_.push_back(argv_[i]);
  }

  pr_ = new CParser::ParseResult;
}

void CParser::cleanup() {
  args_.clear();
  if (pr_) {
    auto ibegin = pr_->begin();
    auto iend = pr_->end();
    auto it = ibegin;
    for (; it != iend; ++it) {
      CParseItem *item = it->second;
      if (item)
        delete item;
    }
    delete pr_;
    pr_ = nullptr;
  }
}

}
