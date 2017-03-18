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

#include "CurlWrapper/Detail/Handle.hpp"
#include "CurlWrapper/Detail/GlobalInit.hpp"
#include "CurlWrapper/Detail/WriteProc.hpp"
#include "CurlWrapper/Error.hpp"
#include <curl/curl.h>

namespace Common
{
  namespace CurlWrapper
  {
    namespace Detail
    {
      Handle::Handle() : global(GlobalInit::getInstance()), handle(curl_easy_init())
      {
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1l);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writeProc);
      }

      Handle::Handle(Handle&& other) : global(std::move(other.global)), handle(other.handle)
      {
        other.handle = nullptr;
      }

      Handle::~Handle()
      {
        if (handle)
          curl_easy_cleanup(handle);
      }

      Handle& Handle::operator=(Handle&& other)
      {
        global = std::move(other.global);
        handle = std::move(other.handle);

        other.handle = nullptr;

        return *this;
      }

      std::string Handle::get(const std::string& url)
      {
        std::string result;

        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);

        if (curl_easy_perform(handle) != CURLE_OK)
          throw CurlWrapper::Error("perform failed!");

        return result;
      }
    }
  }
}
