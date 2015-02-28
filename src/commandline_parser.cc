#include "commandline_parser.h"

using namespace std;

namespace parser
{

// class OptionError

OptionError::OptionError(const string& msg)
        : _msg(msg) {

}

const char* OptionError::what() const throw() {
  string msg;
  msg.append("Command line parse error: ").append(_msg).push_back('.');
  return msg.c_str();
}

OptionError::~OptionError() throw() {

}


// class CCParseItem

CParseItem::CParseItem(const string& val)
        : _val(val) {

}

CParseItem* CParser::get(const string& key) {
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
        : chain_(nullptr), pr_(nullptr) {

}

CParser::~CParser() {
  this->cleanup();
}

CParser::ParseResult* CParser::parse(const int argc, const char** argv) {
  if (!this->init(argc, argv)) {
    return nullptr;
  }
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

  if (chain_) {
    this->set_addition();
  }

  return pr_;
}

CParser::ParseResult* CParser::parse(const char* command_line) {
  int i = 0;
  string block;
  vector<string> blocks;
  char c;
  while ((c = command_line[i++]) != '\0') {
    if (c != ' ') {
      block.push_back(c);
    } else {
      if (!block.empty()) {
        blocks.push_back(block);
      }
      block.clear();
    }
  }
  if (!block.empty()) {
    blocks.push_back(block);
  }
  size_t size = blocks.size(); // argc
  char** argv = new char* [size];
  i = 0;
  std::for_each(blocks.begin(), blocks.end(), [&argv, &i](const string& b) {
    argv[i++] = const_cast<char*>(b.c_str());
  });
  auto pr = this->parse(static_cast<const int>(size),
                        const_cast<const char**>(argv));

  delete[] argv;
  argv = nullptr;

  return pr;
}

bool CParser::has(const char* key) {
  string skey(key);

  if (pr_ && !pr_->empty() && !skey.empty()) {
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
    const char* key = va_arg(keys, const char *);
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
    const char* key = va_arg(keys, const char *);
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

bool CParser::init(const int argc, const char** argv) {
  argc_ = argc;
  argv_ = argv;
  if (argc > 0) {
    this->cleanup();

    args_.reserve(static_cast<size_t>(argc_));

    for (int i = 0; i < argc_; ++i) {
      args_.push_back(argv_[i]);
    }

    pr_ = new CParser::ParseResult;
    return true;
  }
  return false;
}

void CParser::cleanup() {
  args_.clear();
  if (pr_) {
    auto ibegin = pr_->begin();
    auto iend = pr_->end();
    auto it = ibegin;
    for (; it != iend; ++it) {
      CParseItem* item = it->second;
      if (item)
        delete item;
    }
    delete pr_;
    pr_ = nullptr;
  }
}

void CParser::set_addition() {
  auto begin = chain_->begin();
  auto end = chain_->end();

  std::for_each(begin, end, [this](const Generator::Row& row) {
    // assume both -o and --option are allowed,
    // but only provide -o,
    // then set the another --option.
    // vice versa.
    const string& def = row.default_value;
    const string& ops = row.option_short;
    const string& opl = row.option_long;
    ParseResult& pr = *pr_;

    bool has_short = this->has(ops.c_str());
    bool has_long = this->has(opl.c_str());

    // assume -o [ --option ] arg = 1
    // but not provide option value,
    // then set to default 1.
    // otherwise, both set to user defined value

    if (!ops.empty()) {
      if (has_short) {
        if (pr[ops] != nullptr && !opl.empty()) {
          pr[opl] = new CParseItem(std::move(pr[ops]->val()));
        } else if (pr[ops] == nullptr && !def.empty()) {
          pr[ops] = new CParseItem(std::move(def));
          if (!opl.empty())
            pr[opl] = new CParseItem(std::move(def));
        } else {
          pr[opl] = nullptr;
        }
      }
    }

    if (!opl.empty()) {
      if (has_long) {
        if (pr[opl] != nullptr && !ops.empty()) {
          pr[ops] = new CParseItem(std::move(pr[opl]->val()));
        } else if (pr[opl] == nullptr && !def.empty()) {
          if (!ops.empty())
            pr[ops] = new CParseItem(std::move(def));
          pr[opl] = new CParseItem(std::move(def));
        } else {
          pr[ops] = nullptr;
        }
      }
    }

    if (!has_long && !has_short && !def.empty()) {
      if (!opl.empty())
        pr[opl] = new CParseItem(std::move(def));
      if (!ops.empty())
        pr[ops] = new CParseItem(std::move(def));
    }
  }

  );
}

}
