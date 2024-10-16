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

#include <array>
#include <cstring>
#include <optional>
#include <string>

namespace peelo::unicode::bom
{
  /**
   * Enumeration of different recognized BOM types.
   */
  enum class type
  {
    utf8,
    utf16_be,
    utf16_le,
    utf32_be,
    utf32_le,
    utf7,
    utf1,
    utf_ebcdic,
    scsu,
    bocu_1,
    gb18030,
  };

  /**
   * Tests whether given byte string contains byte order mark or not.
   *
   * @param input Byte string to test.
   * @param length Length of the given byte string.
   * @return Byte order mark detected in the given byte string, or null option
   *         if the given byte string does not contain byte order mark.
   */
  inline std::optional<type>
  detect(const char* input, std::size_t length)
  {
    struct bom_info
    {
      const char* bytes;
      std::size_t length;
      enum type type;
    };
    static constexpr std::size_t bom_array_size = 11;
    static const std::array<bom_info, bom_array_size> bom_array =
    {{
      {
        "\xef\xbb\xbf",
        3,
        type::utf8,
      },
      {
        "\0\0\xfe\xff",
        4,
        type::utf32_be,
      },
      {
        "\xff\xfe\0\0",
        4,
        type::utf32_le,
      },
      {
        "\xfe\xff",
        2,
        type::utf16_be,
      },
      {
        "\xff\xfe",
        2,
        type::utf16_le,
      },
      {
        "\x2b\x2f\x76",
        3,
        type::utf7,
      },
      {
        "\xf7\x64\x4c",
        3,
        type::utf1,
      },
      {
        "\xdd\x73\x66\x73",
        4,
        type::utf_ebcdic
      },
      {
        "\x0e\xfe\xff",
        3,
        type::scsu
      },
      {
        "\xfb\xee\x28",
        3,
        type::bocu_1
      },
      {
        "\x84\x31\x95\x33",
        4,
        type::gb18030
      },
    }};

    for (std::size_t i = 0; i < bom_array_size; ++i)
    {
      const auto& info = bom_array[i];

      if (length < info.length)
      {
        continue;
      }
      else if (!std::memcmp(input, info.bytes, info.length))
      {
        return info.type;
      }
    }

    return std::nullopt;
  }

  /**
   * Tests whether given string contains byte order mark or not.
   *
   * @param input String to test.
   * @return Byte order mark detected in the given byte string, or null option
   *         if the given byte string does not contain byte order mark.
   */
  inline std::optional<type>
  detect(const std::string& input)
  {
    return detect(input.c_str(), input.length());
  }
}
