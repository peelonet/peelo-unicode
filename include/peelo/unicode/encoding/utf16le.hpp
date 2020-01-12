/*
 * Copyright (c) 2018-2020, peelo.net
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
#include <cstddef>
#include <string>

#include <peelo/unicode/ctype/isvalid.hpp>

namespace peelo::unicode::encoding::utf16le
{
  namespace internal
  {
    inline void encode_codepoint(char32_t c, std::string& output)
    {
      if (c > 0xffff)
      {
        const auto high = (c >> 10) + 0xd7c0;
        const auto low = (c & 0x3ff) + 0xdc00;

        output.append(1, static_cast<unsigned char>(high & 0xff));
        output.append(1, static_cast<unsigned char>((high >> 8) & 0xff));
        output.append(1, static_cast<unsigned char>(low & 0xff));
        output.append(1, static_cast<unsigned char>((low >> 8) & 0xff));
      } else {
        output.append(1, static_cast<unsigned char>(c & 0xff));
        output.append(1, static_cast<unsigned char>((c & 0xff00) >> 8));
      }
    }
  }

  /**
   * Encodes given Unicode character sequence into a byte string using UTF-16LE
   * character encoding. Encoding errors are ignored.
   */
  inline std::string encode(const char32_t* input, const std::size_t length)
  {
    std::string output;

    for (std::u32string::size_type i = 0; i < length; ++i)
    {
      const auto& c = input[i];

      if (!ctype::isvalid(c))
      {
        continue;
      }
      internal::encode_codepoint(c, output);
    }

    return output;
  }

  /**
   * Encodes given Unicode string into a byte string using UTF-16LE character
   * encoding. Encoding errors are ignored.
   */
  inline std::string encode(const std::u32string& input)
  {
    return encode(input.c_str(), input.length());
  }

  namespace internal
  {
    inline bool decode_advance(
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

      if (i + 1 < length && (input[i + 1] & 0xfc) == 0xd8)
      {
        if (i + 3 >= length)
        {
          return false;
        }
        p0 = static_cast<unsigned char>(input[i]);
        p1 = static_cast<unsigned char>(input[i + 1]);
        p2 = static_cast<unsigned char>(input[i + 2]);
        p3 = static_cast<unsigned char>(input[i + 3]);
        output = static_cast<char32_t>(
          ((((p1 << 8) + p0) & 0x03ff) << 10)
           + (((p3 << 8) + p2) & 0x03ff) + 0x10000
        );
        i += 4;
      } else {
        if (i + 1 >= length)
        {
          return false;
        }
        p0 = static_cast<unsigned char>(input[i]);
        p1 = static_cast<unsigned char>(input[i + 1]);
        output = static_cast<char32_t>(p1 * 256 + p0);
        i += 2;
      }

      return true;
    }
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-16LE character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string decode(const char* input, const std::size_t length)
  {
    std::u32string output;

    for (std::string::size_type i = 0; i < length;)
    {
      char32_t c;

      if (!internal::decode_advance(input, i, length, c))
      {
        break;
      }
      output.append(1, c);
    }

    return output;
  }

  /**
   * Decodes given byte string into Unicode string using UTF-16LE character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string decode(const std::string& input)
  {
    return decode(input.c_str(), input.length());
  }

  /**
   * Encodes given Unicode character sequence into a byte string using UTF-16LE
   * character encoding. Result will be stored into given byte string. If
   * encoding error is encountered, this function will return false, otherwise
   * it returns true.
   */
  inline bool encode_validate(
    const char32_t* input,
    const std::size_t length,
    std::string& output
  )
  {
    for (std::size_t i = 0; i < length; ++i)
    {
      const auto& c = input[i];

      if (!ctype::isvalid(c))
      {
        return false;
      }
      internal::encode_codepoint(c, output);
    }

    return true;
  }

  /**
   * Encodes given Unicode string into a byte string using UTF-16LE character
   * encoding. Result will be stored into given byte string. If encoding error
   * is encountered, this function will return false, otherwise it returns
   * true.
   */
  inline bool encode_validate(
    const std::u32string& input,
    std::string& output
  )
  {
    return encode_validate(input.c_str(), input.length(), output);
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-16LE character
   * encoding. Result will be stored into given Unicode string. If decoding
   * error is encountered, this function will return false, otherwise it
   * returns true.
   */
  inline bool decode_validate(
    const char* input,
    const std::size_t length,
    std::u32string& output
  )
  {
    for (std::size_t i = 0; i < length;)
    {
      char32_t c;

      if (!internal::decode_advance(input, i, length, c))
      {
        return false;
      }
      output.append(1, c);
    }

    return true;
  }

  /**
   * Decodes given byte string into Unicode string using UTF-16LE character
   * encoding. Result will be stored into given Unicode string. If decoding
   * error is encountered, this function will return false, otherwise it
   * returns true.
   */
  inline bool decode_validate(
    const std::string& input,
    std::u32string& output
  )
  {
    return decode_validate(input.c_str(), input.length(), output);
  }
}
