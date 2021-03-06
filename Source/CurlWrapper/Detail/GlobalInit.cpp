/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2017, <https://github.com/chaos0x8>
 *
 *  \copyright
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  \copyright
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "CurlWrapper/Detail/GlobalInit.hpp"
#include <curl/curl.h>

namespace Common
{
  namespace CurlWrapper
  {
    namespace Detail
    {
      GlobalInit::GlobalInit()
      {
        curl_global_init(CURL_GLOBAL_DEFAULT);
      }

      GlobalInit::~GlobalInit()
      {
        curl_global_cleanup();
      }

      std::shared_ptr<GlobalInit> GlobalInit::getInstance()
      {
        if (auto result = instance.lock())
          return result;

        auto result = std::make_shared<GlobalInit>();
        instance = result;
        return result;
      }

      std::weak_ptr<GlobalInit> GlobalInit::instance;
    }
  }
}
