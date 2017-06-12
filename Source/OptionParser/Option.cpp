#include "Option.hpp"
#include <algorithm>

namespace Common::OptionParser::Detail
{
  bool isTrueAlike(std::string val)
  {
    std::transform(std::begin(val), std::end(val), std::begin(val), ::tolower);

    return val == "true" or val == "yes" or val == "1";
  }
}
