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

#include <peelo/unicode/encoding/_utils.hpp>

namespace peelo::unicode::encoding::utf32le
{
  inline void
  encode_codepoint(char32_t c, std::string& output)
  {
    output.append(1, static_cast<unsigned char>(c & 0xff));
    output.append(1, static_cast<unsigned char>((c & 0xff00) >> 8));
    output.append(1, static_cast<unsigned char>((c & 0xff0000) >> 16));
    output.append(1, static_cast<unsigned char>((c & 0xff000000) >> 24));
  }

  /**
   * Encodes given Unicode character sequence into a byte string using UTF-32BE
   * character encoding. Encoding errors are ignored.
   */
  inline std::string
  encode(const char32_t* input, const std::size_t length)
  {
    return utils::encode(input, length, encode_codepoint);
  }

  /**
   * Encodes given Unicode string into a byte string using UTF-32BE character
   * encoding. Encoding errors are ignored.
   */
  inline std::string
  encode(const std::u32string& input)
  {
    return encode(input.c_str(), input.length());
  }

  inline bool
  decode_advance(
    const char* input,
    std::string::size_type& i,
    const std::string::size_type length,
    char32_t& output
  )
  {
    unsigned char p0;
    unsigned char p1;
    unsigned char p2;
    unsigned char p3;

    if (i + 3 >= length)
    {
      return false;
    }
    p0 = static_cast<unsigned char>(input[i]);
    p1 = static_cast<unsigned char>(input[i + 1]);
    p2 = static_cast<unsigned char>(input[i + 2]);
    p3 = static_cast<unsigned char>(input[i + 3]);
    output = static_cast<char32_t>(((p3 * 256 + p2) * 256 + p1) * 256 + p0);
    i += 4;

    return true;
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-32BE character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string
  decode(const char* input, const std::size_t length)
  {
    return utils::decode(input, length, decode_advance);
  }

  /**
   * Decodes given byte string into Unicode string using UTF-32BE character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string
  decode(const std::string& input)
  {
    return utils::decode(
      input.c_str(),
      input.length(),
      decode_advance
    );
  }

  /**
   * Encodes given Unicode character sequence into a byte string using UTF-32BE
   * character encoding. Result will be stored into given byte string. If
   * encoding error is encountered, this function will return false, otherwise
   * it returns true.
   */
  inline bool
  encode_validate(
    const char32_t* input,
    const std::size_t length,
    std::string& output
  )
  {
    return utils::encode_validate(
      input,
      length,
      output,
      encode_codepoint
    );
  }

  /**
   * Encodes given Unicode string into a byte string using UTF-32BE character
   * encoding. Result will be stored into given byte string. If encoding error
   * is encountered, this function will return false, otherwise it returns
   * true.
   */
  inline bool
  encode_validate(
    const std::u32string& input,
    std::string& output
  )
  {
    return utils::encode_validate(
      input.c_str(),
      input.length(),
      output,
      encode_codepoint
    );
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-32BE character
   * encoding. Result will be stored into given Unicode string. If decoding
   * error is encountered, this function will return false, otherwise it
   * returns true.
   */
  inline bool
  decode_validate(
    const char* input,
    const std::size_t length,
    std::u32string& output
  )
  {
    return utils::decode_validate(
      input,
      length,
      output,
      decode_advance
    );
  }

  /**
   * Decodes given byte string into Unicode string using UTF-32BE character
   * encoding. Result will be stored into given Unicode string. If decoding
   * error is encountered, this function will return false, otherwise it
   * returns true.
   */
  inline bool
  decode_validate(
    const std::string& input,
    std::u32string& output
  )
  {
    return utils::decode_validate(
      input.c_str(),
      input.length(),
      output,
      decode_advance
    );
  }
}
