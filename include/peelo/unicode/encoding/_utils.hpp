/*
 * Copyright (c) 2018-2024, peelo.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <cstddef>
#include <string>

#include <peelo/unicode/ctype/isvalid.hpp>

namespace peelo::unicode::encoding::utils
{
  using encode_callback = void(*)(
    char32_t codepoint,
    std::string& output
  );
  using decode_callback = bool(*)(
    const char* input,
    std::size_t& i,
    const std::size_t length,
    char32_t& result
  );

  inline std::string
  encode(
    const char32_t* input,
    std::size_t length,
    encode_callback callback
  )
  {
    std::string result;

    for (std::size_t i = 0; i < length; ++i)
    {
      const auto& c = input[i];

      if (!ctype::isvalid(c))
      {
        continue;
      }
      callback(c, result);
    }

    return result;
  }

  inline bool
  encode_validate(
    const char32_t* input,
    std::size_t length,
    std::string& output,
    encode_callback callback
  )
  {
    for (std::size_t i = 0; i < length; ++i)
    {
      const auto& c = input[i];

      if (!ctype::isvalid(c))
      {
        return false;
      }
      callback(c, output);
    }

    return true;
  }

  inline std::u32string
  decode(const char* input, std::size_t length, decode_callback callback)
  {
    std::u32string result;

    for (std::size_t i = 0; i < length;)
    {
      char32_t c;

      if (!callback(input, i, length, c))
      {
        continue;
      }
      result.append(1, c);
    }

    return result;
  }

  inline bool
  decode_validate(
    const char* input,
    std::size_t length,
    std::u32string& output,
    decode_callback callback
  )
  {
    for (std::size_t i = 0; i < length;)
    {
      char32_t c;

      if (!callback(input, i, length, c))
      {
        return false;
      }
      output.append(1, c);
    }

    return true;
  }
}
