/*
 * Copyright (c) 2018-2026, peelo.net
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

#include <array>
#include <cstdint>

#include <peelo/unicode/encoding/_utils.hpp>

namespace peelo::unicode::encoding::utf_ebcdic
{
  namespace internal
  {
    // UTR #16 Table 2: I8-sequence byte -> UTF-EBCDIC byte
    static constexpr std::array<unsigned char, 256> i8_to_ebcdic = {{
      0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
      0x16, 0x05, 0x15, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
      0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26,
      0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
      0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
      0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
      0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
      0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
      0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
      0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
      0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
      0xe7, 0xe8, 0xe9, 0xad, 0xe0, 0xbd, 0x5f, 0x6d,
      0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
      0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
      0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
      0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0xa1, 0x07,
      0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x06, 0x17,
      0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b,
      0x30, 0x31, 0x1a, 0x33, 0x34, 0x35, 0x36, 0x08,
      0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14, 0x3e, 0xff,
      0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
      0x49, 0x4a, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
      0x57, 0x58, 0x59, 0x62, 0x63, 0x64, 0x65, 0x66,
      0x67, 0x68, 0x69, 0x6a, 0x70, 0x71, 0x72, 0x73,
      0x74, 0x75, 0x76, 0x77, 0x78, 0x80, 0x8a, 0x8b,
      0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x9a, 0x9b, 0x9c,
      0x9d, 0x9e, 0x9f, 0xa0, 0xaa, 0xab, 0xac, 0xae,
      0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
      0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbe, 0xbf,
      0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xda, 0xdb,
      0xdc, 0xdd, 0xde, 0xdf, 0xe1, 0xea, 0xeb, 0xec,
      0xed, 0xee, 0xef, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe
    }};

    // UTR #16 Table 3: UTF-EBCDIC byte -> I8-sequence byte
    static constexpr std::array<unsigned char, 256> ebcdic_to_i8 = {{
      0x00, 0x01, 0x02, 0x03, 0x9c, 0x09, 0x86, 0x7f,
      0x97, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
      0x10, 0x11, 0x12, 0x13, 0x9d, 0x0a, 0x08, 0x87,
      0x18, 0x19, 0x92, 0x8f, 0x1c, 0x1d, 0x1e, 0x1f,
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x17, 0x1b,
      0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07,
      0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,
      0x98, 0x99, 0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a,
      0x20, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
      0xa7, 0xa8, 0xa9, 0x2e, 0x3c, 0x28, 0x2b, 0x7c,
      0x26, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0,
      0xb1, 0xb2, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x5e,
      0x2d, 0x2f, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
      0xb9, 0xba, 0xbb, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
      0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3,
      0xc4, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
      0xc5, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
      0x68, 0x69, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb,
      0xcc, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
      0x71, 0x72, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2,
      0xd3, 0x7e, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
      0x79, 0x7a, 0xd4, 0xd5, 0xd6, 0x5b, 0xd7, 0xd8,
      0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0,
      0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0x5d, 0xe6, 0xe7,
      0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
      0x48, 0x49, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed,
      0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
      0x51, 0x52, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3,
      0x5c, 0xf4, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
      0x59, 0x5a, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa,
      0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
      0x38, 0x39, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 0x9f
    }};

    inline bool
    is_i8_trailing(unsigned char byte)
    {
      return (byte & 0xe0) == 0xa0;
    }

    inline std::size_t
    i8_sequence_length(unsigned char lead)
    {
      if (lead <= 0x9f)
      {
        return 1;
      }
      else if (is_i8_trailing(lead))
      {
        return 0;
      }
      else if ((lead & 0xe0) == 0xc0)
      {
        return 2;
      }
      else if ((lead & 0xf0) == 0xe0)
      {
        return 3;
      }
      else if ((lead & 0xf8) == 0xf0)
      {
        return 4;
      }
      else if ((lead & 0xfc) == 0xf8)
      {
        return 5;
      }

      // 6- and 7-byte forms exist for UCS-4 beyond Unicode; reject them.
      return 0;
    }

    inline void
    append_mapped(unsigned char i8_byte, std::string& output)
    {
      output.append(1, static_cast<char>(i8_to_ebcdic[i8_byte]));
    }
  }

  /**
   * Attempts to determine length (in bytes) of UTF-EBCDIC sequence which
   * begins with the given byte. Returns 0 if the byte cannot start a
   * sequence.
   */
  inline std::size_t
  sequence_length(unsigned char byte)
  {
    return internal::i8_sequence_length(internal::ebcdic_to_i8[byte]);
  }

  /**
   * Attempts to determine the amount of bytes which are required to encode
   * given Unicode code point with UTF-EBCDIC. If the given code point cannot
   * be encoded, 0 will be returned instead.
   */
  inline std::size_t
  codepoint_length(char32_t c)
  {
    if (!ctype::isvalid(c))
    {
      return 0;
    }
    else if (c <= 0x9f)
    {
      return 1;
    }
    else if (c <= 0x3ff)
    {
      return 2;
    }
    else if (c <= 0x3fff)
    {
      return 3;
    }
    else if (c <= 0x3ffff)
    {
      return 4;
    }

    return 5;
  }

  inline void
  encode_codepoint(char32_t c, std::string& output)
  {
    if (c <= 0x9f)
    {
      internal::append_mapped(static_cast<unsigned char>(c), output);
    }
    else if (c <= 0x3ff)
    {
      internal::append_mapped(
        static_cast<unsigned char>(0xc0 | (c >> 5)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | (c & 0x1f)),
        output
      );
    }
    else if (c <= 0x3fff)
    {
      internal::append_mapped(
        static_cast<unsigned char>(0xe0 | (c >> 10)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | ((c >> 5) & 0x1f)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | (c & 0x1f)),
        output
      );
    }
    else if (c <= 0x3ffff)
    {
      internal::append_mapped(
        static_cast<unsigned char>(0xf0 | (c >> 15)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | ((c >> 10) & 0x1f)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | ((c >> 5) & 0x1f)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | (c & 0x1f)),
        output
      );
    }
    else
    {
      internal::append_mapped(
        static_cast<unsigned char>(0xf8 | (c >> 20)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | ((c >> 15) & 0x1f)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | ((c >> 10) & 0x1f)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | ((c >> 5) & 0x1f)),
        output
      );
      internal::append_mapped(
        static_cast<unsigned char>(0xa0 | (c & 0x1f)),
        output
      );
    }
  }

  /**
   * Encodes given Unicode character sequence into a byte string using
   * UTF-EBCDIC character encoding. Encoding errors are ignored.
   */
  inline std::string
  encode(const char32_t* input, std::size_t length)
  {
    return utils::encode(input, length, encode_codepoint);
  }

  /**
   * Encodes given Unicode string into a byte string using UTF-EBCDIC character
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
    if (i >= length)
    {
      return false;
    }

    const auto lead_i8 = internal::ebcdic_to_i8[
      static_cast<unsigned char>(input[i])
    ];
    const auto seq_length = internal::i8_sequence_length(lead_i8);

    if (!seq_length || i + seq_length > length)
    {
      ++i;
      return false;
    }

    if (seq_length == 1)
    {
      result = static_cast<char32_t>(lead_i8);
      ++i;
      return true;
    }

    unsigned char i8_bytes[5];

    i8_bytes[0] = lead_i8;
    for (std::size_t j = 1; j < seq_length; ++j)
    {
      i8_bytes[j] = internal::ebcdic_to_i8[
        static_cast<unsigned char>(input[i + j])
      ];
      if (!internal::is_i8_trailing(i8_bytes[j]))
      {
        ++i;
        return false;
      }
    }

    switch (seq_length)
    {
      case 2:
        result = static_cast<char32_t>(
          ((i8_bytes[0] & 0x1f) << 5) | (i8_bytes[1] & 0x1f)
        );
        break;

      case 3:
        result = static_cast<char32_t>(
          ((i8_bytes[0] & 0x0f) << 10)
          | ((i8_bytes[1] & 0x1f) << 5)
          | (i8_bytes[2] & 0x1f)
        );
        break;

      case 4:
        result = static_cast<char32_t>(
          ((i8_bytes[0] & 0x07) << 15)
          | ((i8_bytes[1] & 0x1f) << 10)
          | ((i8_bytes[2] & 0x1f) << 5)
          | (i8_bytes[3] & 0x1f)
        );
        break;

      case 5:
        result = static_cast<char32_t>(
          ((i8_bytes[0] & 0x03) << 20)
          | ((i8_bytes[1] & 0x1f) << 15)
          | ((i8_bytes[2] & 0x1f) << 10)
          | ((i8_bytes[3] & 0x1f) << 5)
          | (i8_bytes[4] & 0x1f)
        );
        break;

      default:
        ++i;
        return false;
    }

    i += seq_length;

    return true;
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-EBCDIC character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string
  decode(const char* input, const std::size_t length)
  {
    return utils::decode(input, length, decode_advance);
  }

  /**
   * Decodes given byte string into Unicode string using UTF-EBCDIC character
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
   * Encodes given Unicode character sequence into a byte string using
   * UTF-EBCDIC character encoding. Result will be stored into given byte
   * string. If encoding error is encountered, this function will return false,
   * otherwise it returns true.
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
   * Encodes given Unicode string into a byte string using UTF-EBCDIC character
   * encoding. Result will be stored into given byte string. If encoding error
   * is encountered, this function will return false, otherwise it returns
   * true.
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
   * Decodes given byte sequence into Unicode string using UTF-EBCDIC character
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
   * Decodes given byte string into Unicode string using UTF-EBCDIC character
   * encoding. Result will be stored into given Unicode string. If decoding
   * error is encountered, this function will return false, otherwise it
   * returns true.
   */
  inline bool
  decode_validate(const std::string& input, std::u32string& output)
  {
    return decode_validate(input.c_str(), input.length(), output);
  }
}
