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

#include <cstdint>

#include <peelo/unicode/encoding/_utils.hpp>
#include <peelo/unicode/encoding/gb18030_tables.hpp>

namespace peelo::unicode::encoding::gb18030
{
  namespace internal
  {
    // GB 18030 byte ranges:
    // 1-byte: 0x00-0x7F (ASCII)
    // 2-byte: [0x81-0xFE][0x40-0x7E | 0x80-0xFE]
    // 4-byte: [0x81-0xFE][0x30-0x39][0x81-0xFE][0x30-0x39]

    // Binary search in two-byte table by GB code (for decoding)
    inline char32_t
    lookup_two_byte_to_unicode(std::uint16_t gb_code)
    {
      std::size_t low = 0;
      std::size_t high = tables::gb_to_unicode.size();

      while (low < high)
      {
        const auto mid = low + (high - low) / 2;
        if (tables::gb_to_unicode[mid].gb < gb_code)
        {
          low = mid + 1;
        }
        else if (tables::gb_to_unicode[mid].gb > gb_code)
        {
          high = mid;
        }
        else
        {
          return tables::gb_to_unicode[mid].unicode;
        }
      }

      return 0xFFFFFFFF; // Not found
    }

    // Binary search by Unicode codepoint (for encoding)
    inline std::uint16_t
    lookup_unicode_to_two_byte(char32_t unicode)
    {
      std::size_t low = 0;
      std::size_t high = tables::unicode_to_gb.size();

      while (low < high)
      {
        const auto mid = low + (high - low) / 2;
        if (tables::unicode_to_gb[mid].unicode < unicode)
        {
          low = mid + 1;
        }
        else if (tables::unicode_to_gb[mid].unicode > unicode)
        {
          high = mid;
        }
        else
        {
          return tables::unicode_to_gb[mid].gb;
        }
      }

      return 0; // Not found
    }

    // Convert 4-byte GB18030 sequence to linear offset
    inline std::uint32_t
    four_byte_to_offset(
      unsigned char b1,
      unsigned char b2,
      unsigned char b3,
      unsigned char b4
    )
    {
      return ((b1 - 0x81) * 12600) +
             ((b2 - 0x30) * 1260) +
             ((b3 - 0x81) * 10) +
             (b4 - 0x30);
    }

    // Convert linear offset to 4-byte GB18030 sequence
    inline void
    offset_to_four_byte(
      std::uint32_t offset,
      unsigned char& b1,
      unsigned char& b2,
      unsigned char& b3,
      unsigned char& b4
    )
    {
      b4 = static_cast<unsigned char>((offset % 10) + 0x30);
      offset /= 10;
      b3 = static_cast<unsigned char>((offset % 126) + 0x81);
      offset /= 126;
      b2 = static_cast<unsigned char>((offset % 10) + 0x30);
      offset /= 10;
      b1 = static_cast<unsigned char>(offset + 0x81);
    }

    // Convert 4-byte GB offset to Unicode using ranges table
    inline char32_t
    four_byte_offset_to_unicode(std::uint32_t offset)
    {
      // Find the range that contains this offset
      // The ranges are sorted by gb_offset, so we can find the last range
      // where gb_offset <= our offset
      std::size_t range_idx = 0;
      for (std::size_t i = 1; i < tables::four_byte_ranges.size(); ++i)
      {
        if (tables::four_byte_ranges[i].gb_offset <= offset)
        {
          range_idx = i;
        }
        else
        {
          break;
        }
      }

      const auto& range = tables::four_byte_ranges[range_idx];
      return range.unicode_start + (offset - range.gb_offset);
    }

    // Convert Unicode to 4-byte GB offset using ranges table
    inline std::uint32_t
    unicode_to_four_byte_offset(char32_t unicode)
    {
      // Find the range that contains this unicode codepoint
      for (std::size_t i = tables::four_byte_ranges.size(); i > 0; --i)
      {
        const auto& range = tables::four_byte_ranges[i - 1];
        if (unicode >= range.unicode_start)
        {
          return range.gb_offset + (unicode - range.unicode_start);
        }
      }
      return 0xFFFFFFFF; // Not found
    }

  } // namespace internal

  /**
   * Determines if a byte is a valid first byte of a GB18030 sequence.
   */
  inline bool
  is_first_byte(unsigned char byte)
  {
    return byte <= 0x7F || (byte >= 0x81 && byte <= 0xFE);
  }

  /**
   * Determines if a byte is a valid second byte of a 2-byte GB18030 sequence.
   */
  inline bool
  is_two_byte_second(unsigned char byte)
  {
    return (byte >= 0x40 && byte <= 0x7E) || (byte >= 0x80 && byte <= 0xFE);
  }

  /**
   * Determines if a byte is a valid second/fourth byte of a 4-byte sequence.
   */
  inline bool
  is_four_byte_second_fourth(unsigned char byte)
  {
    return byte >= 0x30 && byte <= 0x39;
  }

  inline void
  encode_codepoint(char32_t c, std::string& output)
  {
    // ASCII range - 1 byte
    if (c <= 0x7F)
    {
      output.append(1, static_cast<char>(c));
      return;
    }

    // Try two-byte encoding first
    const auto two_byte = internal::lookup_unicode_to_two_byte(c);

    if (two_byte != 0)
    {
      output.append(1, static_cast<char>(two_byte >> 8));
      output.append(1, static_cast<char>(two_byte & 0xFF));
      return;
    }

    // Four-byte encoding
    const auto offset = internal::unicode_to_four_byte_offset(c);
    if (offset != 0xFFFFFFFF)
    {
      unsigned char b1, b2, b3, b4;

      internal::offset_to_four_byte(offset, b1, b2, b3, b4);
      output.append(1, static_cast<char>(b1));
      output.append(1, static_cast<char>(b2));
      output.append(1, static_cast<char>(b3));
      output.append(1, static_cast<char>(b4));
    }
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

    const auto b1 = static_cast<unsigned char>(input[i]);

    // ASCII - 1 byte
    if (b1 <= 0x7F)
    {
      result = static_cast<char32_t>(b1);
      ++i;
      return true;
    }

    // Must be 0x81-0xFE for multi-byte
    if (b1 < 0x81 || b1 > 0xFE)
    {
      ++i;
      return false;
    }

    if (i + 1 >= length)
    {
      ++i;
      return false;
    }

    const auto b2 = static_cast<unsigned char>(input[i + 1]);

    // Check for 4-byte sequence (second byte is 0x30-0x39)
    if (is_four_byte_second_fourth(b2))
    {
      if (i + 3 >= length)
      {
        ++i;
        return false;
      }

      const auto b3 = static_cast<unsigned char>(input[i + 2]);
      const auto b4 = static_cast<unsigned char>(input[i + 3]);

      if (b3 < 0x81 || b3 > 0xFE || !is_four_byte_second_fourth(b4))
      {
        ++i;
        return false;
      }

      const auto offset = internal::four_byte_to_offset(b1, b2, b3, b4);
      result = internal::four_byte_offset_to_unicode(offset);

      if (result == 0xFFFFFFFF)
      {
        ++i;
        return false;
      }

      i += 4;
      return true;
    }

    // Two-byte sequence
    if (!is_two_byte_second(b2))
    {
      ++i;
      return false;
    }

    const std::uint16_t gb_code = (static_cast<std::uint16_t>(b1) << 8) | b2;
    result = internal::lookup_two_byte_to_unicode(gb_code);

    if (result == 0xFFFFFFFF)
    {
      ++i;
      return false;
    }

    i += 2;
    return true;
  }

  /**
   * Encodes given Unicode character sequence into a byte string using GB18030
   * character encoding. Encoding errors are ignored.
   */
  inline std::string
  encode(const char32_t* input, std::size_t length)
  {
    return utils::encode(input, length, encode_codepoint);
  }

  /**
   * Encodes given Unicode string into a byte string using GB18030 character
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

  /**
   * Decodes given byte sequence into Unicode string using GB18030 character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string
  decode(const char* input, const std::size_t length)
  {
    return utils::decode(input, length, decode_advance);
  }

  /**
   * Decodes given byte string into Unicode string using GB18030 character
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
   * Encodes given Unicode character sequence into a byte string using GB18030
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
   * Encodes given Unicode string into a byte string using GB18030 character
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
   * Decodes given byte sequence into Unicode string using GB18030 character
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
   * Decodes given byte string into Unicode string using GB18030 character
   * encoding. Result will be stored into given Unicode string. If decoding
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
