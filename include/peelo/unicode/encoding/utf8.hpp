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

namespace peelo::unicode::encoding::utf8
{
  /**
   * Attempts to determine length (in bytes) of UTF-8 sequence which begins
   * with the given byte. If the length cannot be determined (i.e.
   * beginning of sequence is invalid according to the UTF-8
   * specification), 0 will be returned instead.
   */
  inline std::size_t
  sequence_length(unsigned char byte)
  {
    if ((byte & 0x80) == 0x00)
    {
      return 1;
    }
    else if ((byte & 0xc0) == 0x80)
    {
      return 0;
    }
    else if ((byte & 0xe0) == 0xc0)
    {
      return 2;
    }
    else if ((byte & 0xf0) == 0xe0)
    {
      return 3;
    }
    else if ((byte & 0xf8) == 0xf0)
    {
      return 4;
    } else {
      return 0;
    }
  }

  /**
   * Attempts to determine the amount of bytes which are required to encode
   * given Unicode code point with UTF-8 character encoding. If the given
   * code point cannot be encoded, 0 will be returned instead.
   */
  inline std::size_t
  codepoint_length(char32_t c)
  {
    if (c < 0x007f)
    {
      return 1;
    }
    else if (c < 0x07ff)
    {
      return 2;
    }
    else if (c < 0xffff)
    {
      return 3;
    }
    else if (c < 0x10ffff)
    {
      return 4;
    } else {
      return 0;
    }
  }

  inline void
  encode_codepoint(char32_t c, std::string& output)
  {
    if (c <= 0x7f)
    {
      output.append(1, static_cast<char>(c));
    }
    else if (c <= 0x07ff)
    {
      output.append(1, static_cast<char>(0xc0 | ((c & 0x7c0) >> 6)));
      output.append(1, static_cast<char>(0x80 | (c & 0x3f)));
    }
    else if (c <= 0xffff)
    {
      output.append(1, static_cast<char>(0xe0 | ((c & 0xf000)) >> 12));
      output.append(1, static_cast<char>(0x80 | ((c & 0xfc0)) >> 6));
      output.append(1, static_cast<char>(0x80 | (c & 0x3f)));
    } else {
      output.append(1, static_cast<char>(0xf0 | ((c & 0x1c0000) >> 18)));
      output.append(1, static_cast<char>(0x80 | ((c & 0x3f000) >> 12)));
      output.append(1, static_cast<char>(0x80 | ((c & 0xfc0) >> 6)));
      output.append(1, static_cast<char>(0x80 | (c & 0x3f)));
    }
  }

  /**
   * Encodes given Unicode character sequence into a byte string using UTF-8
   * character encoding. Encoding errors are ignored.
   */
  inline std::string
  encode(const char32_t* input, std::size_t length)
  {
    return utils::encode(input, length, encode_codepoint);
  }

  /**
   * Encodes given Unicode string into a byte string using UTF-8 character
   * encoding. Encoding errors are ignored.
   */
  inline std::string
  encode(const std::u32string& input)
  {
    return utils::encode(
      input.c_str(),
      input.length(),
      encode_codepoint
    );
  }

  inline bool
  decode_advance(
    const char* input,
    std::size_t& i,
    const std::size_t length,
    char32_t& result
  )
  {
    const auto seq_length = sequence_length(input[i]);

    if (!seq_length || i + (seq_length - 1) >= length)
    {
      return false;
    }

    switch (seq_length)
    {
      case 1:
        result = static_cast<char32_t>(input[i]);
        break;

      case 2:
        result = static_cast<char32_t>(input[i] & 0x1f);
        break;

      case 3:
        result = static_cast<char32_t>(input[i] & 0x0f);
        break;

      case 4:
        result = static_cast<char32_t>(input[i] & 0x07);
        break;

      default:
        return false;
    }

    for (std::size_t j = 1; j < seq_length; ++j)
    {
      if ((input[i + j] & 0xc0) != 0x80)
      {
        return false;
      }
      result = (result << 6) | (input[i + j] & 0x3f);
    }

    i += seq_length;

    return true;
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-8 character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string
  decode(const char* input, const std::size_t length)
  {
    return utils::decode(input, length, decode_advance);
  }

  /**
   * Decodes given byte string into Unicode string using UTF-8 character
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
   * Encodes given Unicode character sequence into a byte string using UTF-8
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
   * Encodes given Unicode string into a byte string using UTF-8 character
   * encoding. Result will be stored into given byte string. If encoding
   * error is encountered, this function will return false, otherwise it
   * returns true.
   */
  inline bool
  encode_validate(const std::u32string& input, std::string& output)
  {
    return utils::encode_validate(
      input.c_str(),
      input.length(),
      output,
      encode_codepoint
    );
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-8 character
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
   * Decodes given byte string into Unicode string using UTF-8 character
   * encoding. Result will be stroed into given Unicode string. If decoding
   * error is encountered, this function will return false, otherwise it
   * returns true.
   */
  inline bool
  decode_validate(const std::string& input, std::u32string& output)
  {
    return utils::decode_validate(
      input.c_str(),
      input.length(),
      output,
      decode_advance
    );
  }
}
