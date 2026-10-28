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

namespace peelo::unicode::encoding::scsu
{
  namespace internal
  {
    // UTS #6 tag values
    static constexpr unsigned char SQ0 = 0x01;
    static constexpr unsigned char SDX = 0x0b;
    static constexpr unsigned char SQU = 0x0e;
    static constexpr unsigned char SCU = 0x0f;
    static constexpr unsigned char SC0 = 0x10;
    static constexpr unsigned char SD0 = 0x18;
    static constexpr unsigned char UC0 = 0xe0;
    static constexpr unsigned char UD0 = 0xe8;
    static constexpr unsigned char UQU = 0xf0;
    static constexpr unsigned char UDX = 0xf1;

    // Table 4: static window starting offsets
    static constexpr std::array<char32_t, 8> static_offsets = {{
      0x0000, 0x0080, 0x0100, 0x0300, 0x2000, 0x2080, 0x2100, 0x3000
    }};

    // Table 5: default dynamic window starting offsets
    static constexpr std::array<char32_t, 8> dynamic_offsets_init = {{
      0x0080, 0x00c0, 0x0400, 0x0600, 0x0900, 0x3040, 0x30a0, 0xff00
    }};

    inline bool
    offset_table(unsigned char index, char32_t& offset)
    {
      if (index >= 0x01 && index <= 0x67)
      {
        offset = static_cast<char32_t>(index) * 0x80;
        return true;
      }
      else if (index >= 0x68 && index <= 0xa7)
      {
        offset = static_cast<char32_t>(index) * 0x80 + 0xac00;
        return true;
      }

      switch (index)
      {
        case 0xf9: offset = 0x00c0; return true;
        case 0xfa: offset = 0x0250; return true;
        case 0xfb: offset = 0x0370; return true;
        case 0xfc: offset = 0x0530; return true;
        case 0xfd: offset = 0x3040; return true;
        case 0xfe: offset = 0x30a0; return true;
        case 0xff: offset = 0xff60; return true;
        default:   return false;
      }
    }

    inline bool
    is_in_window(char32_t offset, char32_t c)
    {
      return c >= offset && c <= offset + 0x7f;
    }

    /**
     * Returns dynamic window 0..7, static window 8..15, or -1 if none of the
     * predefined windows contain c.
     */
    inline int
    find_window(char32_t c)
    {
      for (int i = 0; i < 8; ++i)
      {
        if (is_in_window(dynamic_offsets_init[i], c))
        {
          return i;
        }
      }
      for (int i = 0; i < 8; ++i)
      {
        if (is_in_window(static_offsets[i], c))
        {
          return i + 8;
        }
      }

      return -1;
    }

    struct encode_state
    {
      bool unicode_mode = false;
      unsigned char window = 0;
    };

    inline void
    encode_codepoint_stateful(
      char32_t c,
      encode_state& state,
      std::string& output
    )
    {
      if (c > 0xffff)
      {
        encode_codepoint_stateful(0xd7c0 + (c >> 10), state, output);
        encode_codepoint_stateful(0xdc00 + (c & 0x3ff), state, output);
        return;
      }

      if (!state.unicode_mode)
      {
        if (c < 0x20)
        {
          // Quote C0 controls other than NUL, HT, LF, CR.
          if (c <= 0x0f && ((1u << c) & 0x2601u) == 0)
          {
            output.append(1, static_cast<char>(SQ0));
          }
          output.append(1, static_cast<char>(c));
        }
        else if (c <= 0x7f)
        {
          output.append(1, static_cast<char>(c));
        }
        else if (is_in_window(dynamic_offsets_init[state.window], c))
        {
          output.append(
            1,
            static_cast<char>(0x80 + (c - dynamic_offsets_init[state.window]))
          );
        }
        else
        {
          const auto w = find_window(c);

          if (w >= 0 && w <= 7)
          {
            output.append(1, static_cast<char>(SC0 + w));
            output.append(
              1,
              static_cast<char>(0x80 + (c - dynamic_offsets_init[w]))
            );
            state.window = static_cast<unsigned char>(w);
          }
          else if (w >= 8)
          {
            const auto static_index = w - 8;

            output.append(1, static_cast<char>(SQ0 + static_index));
            output.append(
              1,
              static_cast<char>(c - static_offsets[static_index])
            );
          }
          else if (c == 0xfeff)
          {
            output.append(1, static_cast<char>(SQU));
            output.append(1, static_cast<char>(0xfe));
            output.append(1, static_cast<char>(0xff));
          }
          else
          {
            output.append(1, static_cast<char>(SCU));
            state.unicode_mode = true;
            encode_codepoint_stateful(c, state, output);
          }
        }
      }
      else
      {
        if (c <= 0x7f)
        {
          state.unicode_mode = false;
          output.append(1, static_cast<char>(UC0 + state.window));
          encode_codepoint_stateful(c, state, output);
        }
        else
        {
          const auto w = find_window(c);

          if (w >= 0 && w <= 7)
          {
            output.append(1, static_cast<char>(UC0 + w));
            state.window = static_cast<unsigned char>(w);
            state.unicode_mode = false;
            encode_codepoint_stateful(c, state, output);
          }
          else
          {
            if (c >= 0xe000 && c <= 0xf2ff)
            {
              output.append(1, static_cast<char>(UQU));
            }
            output.append(1, static_cast<char>((c >> 8) & 0xff));
            output.append(1, static_cast<char>(c & 0xff));
          }
        }
      }
    }

    struct decode_state
    {
      bool unicode_mode = false;
      unsigned char window = 0;
      std::array<char32_t, 8> dynamic = dynamic_offsets_init;
    };

    inline bool
    read_byte(
      const char* input,
      std::size_t& i,
      const std::size_t length,
      unsigned char& byte
    )
    {
      if (i >= length)
      {
        return false;
      }
      byte = static_cast<unsigned char>(input[i++]);
      return true;
    }

    inline bool
    decode_one(
      const char* input,
      std::size_t& i,
      const std::size_t length,
      decode_state& state,
      char32_t& result
    )
    {
      unsigned char byte;

      if (!read_byte(input, i, length, byte))
      {
        return false;
      }

      if (!state.unicode_mode)
      {
        if (byte == SQU)
        {
          unsigned char hi;
          unsigned char lo;

          if (!read_byte(input, i, length, hi)
            || !read_byte(input, i, length, lo))
          {
            return false;
          }
          result = (static_cast<char32_t>(hi) << 8) | lo;
          return true;
        }
        else if (byte == SCU)
        {
          state.unicode_mode = true;
          return decode_one(input, i, length, state, result);
        }
        else if (byte >= SQ0 && byte <= SQ0 + 7)
        {
          const auto n = byte - SQ0;
          unsigned char quoted;

          if (!read_byte(input, i, length, quoted))
          {
            return false;
          }
          if (quoted <= 0x7f)
          {
            result = static_offsets[n] + quoted;
          }
          else
          {
            result = state.dynamic[n] + (quoted - 0x80);
          }
          return true;
        }
        else if (byte >= SC0 && byte <= SC0 + 7)
        {
          state.window = static_cast<unsigned char>(byte - SC0);
          return decode_one(input, i, length, state, result);
        }
        else if (byte >= SD0 && byte <= SD0 + 7)
        {
          const auto n = byte - SD0;
          unsigned char index;
          char32_t offset;

          if (!read_byte(input, i, length, index)
            || !offset_table(index, offset))
          {
            return false;
          }
          state.dynamic[n] = offset;
          state.window = static_cast<unsigned char>(n);
          return decode_one(input, i, length, state, result);
        }
        else if (byte == SDX)
        {
          unsigned char hi;
          unsigned char lo;

          if (!read_byte(input, i, length, hi)
            || !read_byte(input, i, length, lo))
          {
            return false;
          }
          {
            const auto n = static_cast<unsigned char>(hi >> 5);

            state.dynamic[n] = 0x10000
              + (0x80 * (((hi & 0x1f) << 8) | lo));
            state.window = n;
          }
          return decode_one(input, i, length, state, result);
        }
        else if (byte == 0x0c)
        {
          return false;
        }
        else if (byte == 0x00
          || byte == 0x09
          || byte == 0x0a
          || byte == 0x0d
          || (byte >= 0x20 && byte <= 0x7f))
        {
          result = byte;
          return true;
        }
        else if (byte >= 0x80)
        {
          result = state.dynamic[state.window] + (byte - 0x80);
          return true;
        }

        return false;
      }

      // Unicode mode
      if (byte >= UC0 && byte <= UC0 + 7)
      {
        state.window = static_cast<unsigned char>(byte - UC0);
        state.unicode_mode = false;
        return decode_one(input, i, length, state, result);
      }
      else if (byte >= UD0 && byte <= UD0 + 7)
      {
        const auto n = byte - UD0;
        unsigned char index;
        char32_t offset;

        if (!read_byte(input, i, length, index)
          || !offset_table(index, offset))
        {
          return false;
        }
        state.dynamic[n] = offset;
        state.window = static_cast<unsigned char>(n);
        state.unicode_mode = false;
        return decode_one(input, i, length, state, result);
      }
      else if (byte == UQU)
      {
        unsigned char hi;
        unsigned char lo;

        if (!read_byte(input, i, length, hi)
          || !read_byte(input, i, length, lo))
        {
          return false;
        }
        result = (static_cast<char32_t>(hi) << 8) | lo;
        return true;
      }
      else if (byte == UDX)
      {
        unsigned char hi;
        unsigned char lo;

        if (!read_byte(input, i, length, hi)
          || !read_byte(input, i, length, lo))
        {
          return false;
        }
        {
          const auto n = static_cast<unsigned char>(hi >> 5);

          state.dynamic[n] = 0x10000
            + (0x80 * (((hi & 0x1f) << 8) | lo));
          state.window = n;
        }
        state.unicode_mode = false;
        return decode_one(input, i, length, state, result);
      }
      else if (byte == 0xf2)
      {
        return false;
      }
      else
      {
        unsigned char lo;

        if (!read_byte(input, i, length, lo))
        {
          return false;
        }

        const auto unit = static_cast<char32_t>((byte << 8) | lo);

        if (unit >= 0xd800 && unit <= 0xdbff)
        {
          unsigned char hi2;
          unsigned char lo2;

          if (!read_byte(input, i, length, hi2)
            || !read_byte(input, i, length, lo2))
          {
            return false;
          }

          const auto low = static_cast<char32_t>((hi2 << 8) | lo2);

          if (low < 0xdc00 || low > 0xdfff)
          {
            return false;
          }
          result = 0x10000
            + (((unit - 0xd800) << 10) | (low - 0xdc00));
          return true;
        }
        else if (unit >= 0xdc00 && unit <= 0xdfff)
        {
          return false;
        }

        result = unit;
        return true;
      }
    }

    inline bool
    decode_impl(
      const char* input,
      const std::size_t length,
      std::u32string& output,
      bool validate
    )
    {
      decode_state state;

      for (std::size_t i = 0; i < length;)
      {
        const auto start = i;
        char32_t codepoint;

        if (!decode_one(input, i, length, state, codepoint))
        {
          if (validate)
          {
            return false;
          }
          if (i == start)
          {
            ++i;
          }
          continue;
        }
        output.append(1, codepoint);
      }

      return true;
    }
  }

  /**
   * Encodes a single Unicode code point using SCSU from the initial encoder
   * state. Prefer encode() for multi-character text so window state is kept.
   */
  inline void
  encode_codepoint(char32_t c, std::string& output)
  {
    internal::encode_state state;

    internal::encode_codepoint_stateful(c, state, output);
  }

  /**
   * Encodes given Unicode character sequence into a byte string using SCSU.
   * Encoding errors are ignored. Uses a minimal encoder (UTS #6 §8.4) that
   * produces conformant, XML-suitable output without defining new windows.
   */
  inline std::string
  encode(const char32_t* input, std::size_t length)
  {
    std::string result;
    internal::encode_state state;

    for (std::size_t i = 0; i < length; ++i)
    {
      const auto c = input[i];

      if (!ctype::isvalid(c))
      {
        continue;
      }
      internal::encode_codepoint_stateful(c, state, result);
    }

    return result;
  }

  /**
   * Encodes given Unicode string into a byte string using SCSU. Encoding
   * errors are ignored.
   */
  inline std::string
  encode(const std::u32string& input)
  {
    return encode(input.c_str(), input.length());
  }

  /**
   * Encodes given Unicode character sequence into a byte string using SCSU.
   * Result will be stored into given byte string. If encoding error is
   * encountered, this function will return false, otherwise it returns true.
   */
  inline bool
  encode_validate(
    const char32_t* input,
    const std::size_t length,
    std::string& output
  )
  {
    internal::encode_state state;

    for (std::size_t i = 0; i < length; ++i)
    {
      const auto c = input[i];

      if (!ctype::isvalid(c))
      {
        return false;
      }
      internal::encode_codepoint_stateful(c, state, output);
    }

    return true;
  }

  /**
   * Encodes given Unicode string into a byte string using SCSU. Result will be
   * stored into given byte string. If encoding error is encountered, this
   * function will return false, otherwise it returns true.
   */
  inline bool
  encode_validate(const std::u32string& input, std::string& output)
  {
    return encode_validate(input.c_str(), input.length(), output);
  }

  /**
   * Decodes given byte sequence into Unicode string using SCSU. Decoding
   * errors are ignored.
   */
  inline std::u32string
  decode(const char* input, const std::size_t length)
  {
    std::u32string result;

    internal::decode_impl(input, length, result, false);

    return result;
  }

  /**
   * Decodes given byte string into Unicode string using SCSU. Decoding errors
   * are ignored.
   */
  inline std::u32string
  decode(const std::string& input)
  {
    return decode(input.c_str(), input.length());
  }

  /**
   * Decodes given byte sequence into Unicode string using SCSU. Result will be
   * stored into given Unicode string. If decoding error is encountered, this
   * function will return false, otherwise it returns true.
   */
  inline bool
  decode_validate(
    const char* input,
    const std::size_t length,
    std::u32string& output
  )
  {
    return internal::decode_impl(input, length, output, true);
  }

  /**
   * Decodes given byte string into Unicode string using SCSU. Result will be
   * stored into given Unicode string. If decoding error is encountered, this
   * function will return false, otherwise it returns true.
   */
  inline bool
  decode_validate(const std::string& input, std::u32string& output)
  {
    return decode_validate(input.c_str(), input.length(), output);
  }
}
