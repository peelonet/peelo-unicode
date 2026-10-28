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

namespace peelo::unicode::encoding::bocu1
{
  namespace internal
  {
    // UTS #40 / UTN #6 constants (from the reference implementation).
    static constexpr std::int32_t ascii_prev = 0x40;
    static constexpr std::int32_t min_lead = 0x21;
    static constexpr std::int32_t middle = 0x90;
    static constexpr std::int32_t max_lead = 0xfe;
    static constexpr std::int32_t reset = 0xff;
    static constexpr std::int32_t trail_controls_count = 20;
    static constexpr std::int32_t trail_byte_offset = min_lead - trail_controls_count;
    static constexpr std::int32_t trail_count =
      (0xff - min_lead + 1) + trail_controls_count;
    static constexpr std::int32_t single = 64;
    static constexpr std::int32_t lead_2 = 43;
    static constexpr std::int32_t lead_3 = 3;
    static constexpr std::int32_t reach_pos_1 = single - 1;
    static constexpr std::int32_t reach_neg_1 = -single;
    static constexpr std::int32_t reach_pos_2 =
      reach_pos_1 + lead_2 * trail_count;
    static constexpr std::int32_t reach_neg_2 =
      reach_neg_1 - lead_2 * trail_count;
    static constexpr std::int32_t reach_pos_3 =
      reach_pos_2 + lead_3 * trail_count * trail_count;
    static constexpr std::int32_t reach_neg_3 =
      reach_neg_2 - lead_3 * trail_count * trail_count;
    static constexpr std::int32_t start_pos_2 = middle + reach_pos_1 + 1;
    static constexpr std::int32_t start_pos_3 = start_pos_2 + lead_2;
    static constexpr std::int32_t start_pos_4 = start_pos_3 + lead_3;
    static constexpr std::int32_t start_neg_2 = middle + reach_neg_1;
    static constexpr std::int32_t start_neg_3 = start_neg_2 - lead_2;
    static constexpr std::int32_t start_neg_4 = start_neg_3 - lead_3;

    // Maps external trail bytes 0x00..0x20 to trail values 0..19; -1 = illegal.
    static constexpr std::array<std::int8_t, min_lead> byte_to_trail = {{
      -1, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, -1,
      -1, -1, -1, -1, -1, -1, -1, -1,
      0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
      0x0e, 0x0f, -1, -1, 0x10, 0x11, 0x12, 0x13,
      -1
    }};

    // Maps trail values 0..19 to external trail bytes.
    static constexpr std::array<std::uint8_t, trail_controls_count> trail_to_byte = {{
      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x10, 0x11,
      0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
      0x1c, 0x1d, 0x1e, 0x1f
    }};

    inline unsigned char
    encode_trail(std::int32_t t)
    {
      if (t >= trail_controls_count)
      {
        return static_cast<unsigned char>(t + trail_byte_offset);
      }

      return trail_to_byte[t];
    }

    inline bool
    decode_trail(unsigned char byte, std::int32_t& t)
    {
      if (byte <= 0x20)
      {
        const auto mapped = byte_to_trail[byte];

        if (mapped < 0)
        {
          return false;
        }
        t = mapped;
        return true;
      }

      t = static_cast<std::int32_t>(byte) - trail_byte_offset;
      return true;
    }

    inline std::int32_t
    next_prev(std::int32_t c)
    {
      if (c >= 0x3040 && c <= 0x309f)
      {
        return 0x3070;
      }
      else if (c >= 0x4e00 && c <= 0x9fa5)
      {
        return 0x4e00 - reach_neg_2;
      }
      else if (c >= 0xac00 && c <= 0xd7a3)
      {
        return (0xd7a3 + 0xac00) / 2;
      }

      return (c & ~0x7f) + ascii_prev;
    }

    inline void
    negdivmod(std::int32_t& n, std::int32_t d, std::int32_t& m)
    {
      m = n % d;
      n /= d;
      if (m < 0)
      {
        --n;
        m += d;
      }
    }

    inline void
    pack_diff(std::int32_t diff, std::string& output)
    {
      std::int32_t lead;
      std::int32_t count;
      unsigned char trails[3];

      if (diff >= reach_neg_1)
      {
        if (diff <= reach_pos_1)
        {
          output.append(1, static_cast<char>(middle + diff));
          return;
        }
        else if (diff <= reach_pos_2)
        {
          diff -= reach_pos_1 + 1;
          lead = start_pos_2;
          count = 1;
        }
        else if (diff <= reach_pos_3)
        {
          diff -= reach_pos_2 + 1;
          lead = start_pos_3;
          count = 2;
        }
        else
        {
          diff -= reach_pos_3 + 1;
          lead = start_pos_4;
          count = 3;
        }
      }
      else
      {
        if (diff >= reach_neg_2)
        {
          diff -= reach_neg_1;
          lead = start_neg_2;
          count = 1;
        }
        else if (diff >= reach_neg_3)
        {
          diff -= reach_neg_2;
          lead = start_neg_3;
          count = 2;
        }
        else
        {
          diff -= reach_neg_3;
          lead = start_neg_4;
          count = 3;
        }
      }

      for (std::int32_t i = count - 1; i >= 0; --i)
      {
        std::int32_t m;

        negdivmod(diff, trail_count, m);
        trails[i] = encode_trail(m);
      }

      output.append(1, static_cast<char>(lead + diff));
      for (std::int32_t i = 0; i < count; ++i)
      {
        output.append(1, static_cast<char>(trails[i]));
      }
    }

    inline void
    encode_codepoint_stateful(
      char32_t c,
      std::int32_t& prev,
      std::string& output
    )
    {
      const auto code = static_cast<std::int32_t>(c);

      if (code <= 0x20)
      {
        if (code != 0x20)
        {
          prev = ascii_prev;
        }
        output.append(1, static_cast<char>(code));
        return;
      }

      pack_diff(code - prev, output);
      prev = next_prev(code);
    }

    struct decode_state
    {
      std::int32_t prev = ascii_prev;
      std::int32_t count = 0;
      std::int32_t diff = 0;
    };

    inline bool
    decode_lead_byte(decode_state& state, unsigned char byte)
    {
      std::int32_t c;
      std::int32_t count;

      if (byte >= start_neg_2)
      {
        if (byte < start_pos_3)
        {
          c = (static_cast<std::int32_t>(byte) - start_pos_2) * trail_count
            + reach_pos_1 + 1;
          count = 1;
        }
        else if (byte < start_pos_4)
        {
          c = (static_cast<std::int32_t>(byte) - start_pos_3)
            * trail_count * trail_count
            + reach_pos_2 + 1;
          count = 2;
        }
        else
        {
          c = reach_pos_3 + 1;
          count = 3;
        }
      }
      else
      {
        if (byte >= start_neg_3)
        {
          c = (static_cast<std::int32_t>(byte) - start_neg_2) * trail_count
            + reach_neg_1;
          count = 1;
        }
        else if (byte > min_lead)
        {
          c = (static_cast<std::int32_t>(byte) - start_neg_3)
            * trail_count * trail_count
            + reach_neg_2;
          count = 2;
        }
        else
        {
          c = -trail_count * trail_count * trail_count + reach_neg_3;
          count = 3;
        }
      }

      state.diff = c;
      state.count = count;
      return true;
    }

    inline bool
    decode_trail_byte(
      decode_state& state,
      unsigned char byte,
      char32_t& result
    )
    {
      std::int32_t t;

      if (!decode_trail(byte, t))
      {
        state.prev = ascii_prev;
        state.count = 0;
        return false;
      }

      auto c = state.diff;
      auto count = state.count;

      if (count == 1)
      {
        c = state.prev + c + t;
        if (c < 0 || c > 0x10ffff)
        {
          state.prev = ascii_prev;
          state.count = 0;
          return false;
        }
        state.prev = next_prev(c);
        state.count = 0;
        result = static_cast<char32_t>(c);
        return true;
      }

      if (count == 2)
      {
        state.diff = c + t * trail_count;
      }
      else
      {
        state.diff = c + t * trail_count * trail_count;
      }
      state.count = count - 1;
      return true;
    }

    // Returns: 1 = codepoint ready, 0 = need more input, -1 = error
    inline int
    decode_byte(
      decode_state& state,
      unsigned char byte,
      char32_t& result
    )
    {
      if (state.count == 0)
      {
        if (byte <= 0x20)
        {
          if (byte != 0x20)
          {
            state.prev = ascii_prev;
          }
          result = byte;
          return 1;
        }

        if (byte >= start_neg_2 && byte < start_pos_2)
        {
          const auto c = state.prev
            + (static_cast<std::int32_t>(byte) - middle);

          state.prev = next_prev(c);
          result = static_cast<char32_t>(c);
          return 1;
        }

        if (byte == reset)
        {
          state.prev = ascii_prev;
          return 0;
        }

        decode_lead_byte(state, byte);
        return 0;
      }

      if (!decode_trail_byte(state, byte, result))
      {
        return -1;
      }
      return state.count == 0 ? 1 : 0;
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

      for (std::size_t i = 0; i < length; ++i)
      {
        char32_t codepoint;
        const auto status = decode_byte(
          state,
          static_cast<unsigned char>(input[i]),
          codepoint
        );

        if (status < 0)
        {
          if (validate)
          {
            return false;
          }
          state = decode_state{};
          continue;
        }
        if (status > 0)
        {
          output.append(1, codepoint);
        }
      }

      if (validate && state.count != 0)
      {
        return false;
      }

      return true;
    }
  }

  /**
   * Encodes a single Unicode code point using BOCU-1 from the initial encoder
   * state. Prefer encode() for multi-character text so difference state is kept.
   */
  inline void
  encode_codepoint(char32_t c, std::string& output)
  {
    std::int32_t prev = internal::ascii_prev;

    internal::encode_codepoint_stateful(c, prev, output);
  }

  /**
   * Encodes given Unicode character sequence into a byte string using BOCU-1.
   * Encoding errors are ignored.
   */
  inline std::string
  encode(const char32_t* input, std::size_t length)
  {
    std::string result;
    std::int32_t prev = internal::ascii_prev;

    for (std::size_t i = 0; i < length; ++i)
    {
      const auto c = input[i];

      if (!ctype::isvalid(c))
      {
        continue;
      }
      internal::encode_codepoint_stateful(c, prev, result);
    }

    return result;
  }

  /**
   * Encodes given Unicode string into a byte string using BOCU-1. Encoding
   * errors are ignored.
   */
  inline std::string
  encode(const std::u32string& input)
  {
    return encode(input.c_str(), input.length());
  }

  /**
   * Encodes given Unicode character sequence into a byte string using BOCU-1.
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
    std::int32_t prev = internal::ascii_prev;

    for (std::size_t i = 0; i < length; ++i)
    {
      const auto c = input[i];

      if (!ctype::isvalid(c))
      {
        return false;
      }
      internal::encode_codepoint_stateful(c, prev, output);
    }

    return true;
  }

  /**
   * Encodes given Unicode string into a byte string using BOCU-1. Result will
   * be stored into given byte string. If encoding error is encountered, this
   * function will return false, otherwise it returns true.
   */
  inline bool
  encode_validate(const std::u32string& input, std::string& output)
  {
    return encode_validate(input.c_str(), input.length(), output);
  }

  /**
   * Decodes given byte sequence into Unicode string using BOCU-1. Decoding
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
   * Decodes given byte string into Unicode string using BOCU-1. Decoding
   * errors are ignored.
   */
  inline std::u32string
  decode(const std::string& input)
  {
    return decode(input.c_str(), input.length());
  }

  /**
   * Decodes given byte sequence into Unicode string using BOCU-1. Result will
   * be stored into given Unicode string. If decoding error is encountered,
   * this function will return false, otherwise it returns true.
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
   * Decodes given byte string into Unicode string using BOCU-1. Result will be
   * stored into given Unicode string. If decoding error is encountered, this
   * function will return false, otherwise it returns true.
   */
  inline bool
  decode_validate(const std::string& input, std::u32string& output)
  {
    return decode_validate(input.c_str(), input.length(), output);
  }
}
