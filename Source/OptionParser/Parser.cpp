#include "Parser.hpp"
#include <algorithm>
#include <sstream>

namespace Common::OptionParser {
  namespace Detail {
    std::vector<std::string> toVec(int argc, const char** argv) {
      std::vector<std::string> args;

      for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
      }

      return args;
    }
  } // namespace Detail

  void Parser::parse(int argc, const char** argv) {
    parse(Detail::toVec(argc, argv));

    if (argc >= 1) {
      name_ = argv[0];
    }
  }

  void Parser::parse(std::vector<std::string> args) {
    std::shared_ptr<Detail::Option> opt;
    size_t arity = 0u;

    for (auto& arg : args) {
      auto kt = std::find_if(
        std::begin(options_), std::end(options_), [&arg](auto weakOpt) {
          if (auto o = weakOpt.lock()) {
            return o->name() == arg;
          }

          return false;
        });

      if (kt != std::end(options_)) {
        opt = kt->lock();
        arity = opt->arity();

        if (arity == 0u) {
          opt->set("");
          opt = nullptr;
        }
      } else {
        if (opt) {
          opt->set(arg);
          if (--arity == 0u) {
            opt = nullptr;
          }
        } else {
          args_.emplace_back(std::move(arg));
        }
      }
    }
  }

  std::string_view Parser::name() const {
    return name_;
  }

  std::string Parser::help() const {
    std::stringstream ss;
    if (name_.size() > 0) {
      ss << "Usage: " << name_ << " [options] args...\n";
    } else {
      ss << "Usage: [options] args...\n";
    }
    ss << "\n";

    for (const auto& o : options_) {
      if (auto opt = o.lock()) {
        ss << opt->name() << "\n";
      }
    }

    return ss.str();
  }
} // namespace Common::OptionParser
