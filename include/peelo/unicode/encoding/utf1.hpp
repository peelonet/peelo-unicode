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

namespace peelo::unicode::encoding::utf1
{
  namespace internal
  {
    // UTF-1 uses modulo-190 arithmetic after excluding 66 protected byte
    // values (C0/C1 controls and space): 256 - 66 = 190.
    static constexpr std::uint32_t modulus = 190;
    static constexpr std::uint32_t modulus2 = modulus * modulus;
    static constexpr std::uint32_t modulus3 = modulus2 * modulus;
    static constexpr std::uint32_t modulus4 = modulus3 * modulus;

    // Range starts taken from ISO/IEC 10646-1:1993 Annex G / Wikipedia.
    static constexpr char32_t range_2byte_start = 0x0100;
    static constexpr char32_t range_3byte_start = 0x4016;
    static constexpr char32_t range_5byte_start = 0x38e2e;

    /**
     * Maps a value in 0..189 to a UTF-1 continuation byte (0x21-0x7E or
     * 0xA0-0xFF).
     */
    inline unsigned char
    encode_mod(std::uint32_t value)
    {
      if (value < 94)
      {
        return static_cast<unsigned char>(value + 33);
      }

      return static_cast<unsigned char>(value + 66);
    }

    /**
     * Inverse of encode_mod. Returns true and writes the 0..189 value when
     * the byte is a valid continuation byte.
     */
    inline bool
    decode_mod(unsigned char byte, std::uint32_t& value)
    {
      if (byte >= 0x21 && byte <= 0x7e)
      {
        value = byte - 33;
        return true;
      }
      else if (byte >= 0xa0)
      {
        value = byte - 66;
        return true;
      }

      return false;
    }
  }

  /**
   * Attempts to determine length (in bytes) of UTF-1 sequence which begins
   * with the given byte. Returns 0 if the byte cannot start a sequence.
   */
  inline std::size_t
  sequence_length(unsigned char byte)
  {
    if (byte <= 0x9f)
    {
      return 1;
    }
    else if (byte <= 0xf5)
    {
      // 0xA0: U+00A0..U+00FF; 0xA1..0xF5: U+0100..U+4015
      return 2;
    }
    else if (byte <= 0xfb)
    {
      return 3;
    }

    // 0xFC..0xFF
    return 5;
  }

  /**
   * Attempts to determine the amount of bytes which are required to encode
   * given Unicode code point with UTF-1 character encoding. If the given
   * code point cannot be encoded, 0 will be returned instead.
   */
  inline std::size_t
  codepoint_length(char32_t c)
  {
    if (!ctype::isvalid(c))
    {
      return 0;
    }
    else if (c < 0xa0)
    {
      return 1;
    }
    else if (c < internal::range_2byte_start)
    {
      return 2;
    }
    else if (c < internal::range_3byte_start)
    {
      return 2;
    }
    else if (c < internal::range_5byte_start)
    {
      return 3;
    }

    return 5;
  }

  inline void
  encode_codepoint(char32_t c, std::string& output)
  {
    if (c < 0xa0)
    {
      output.append(1, static_cast<char>(c));
    }
    else if (c < internal::range_2byte_start)
    {
      output.append(1, static_cast<char>(0xa0));
      output.append(1, static_cast<char>(c));
    }
    else if (c < internal::range_3byte_start)
    {
      const auto x = static_cast<std::uint32_t>(c - internal::range_2byte_start);

      output.append(1, static_cast<char>(0xa1 + x / internal::modulus));
      output.append(1, static_cast<char>(internal::encode_mod(x % internal::modulus)));
    }
    else if (c < internal::range_5byte_start)
    {
      const auto x = static_cast<std::uint32_t>(c - internal::range_3byte_start);

      output.append(1, static_cast<char>(0xf6 + x / internal::modulus2));
      output.append(
        1,
        static_cast<char>(internal::encode_mod((x / internal::modulus) % internal::modulus))
      );
      output.append(1, static_cast<char>(internal::encode_mod(x % internal::modulus)));
    }
    else
    {
      const auto x = static_cast<std::uint32_t>(c - internal::range_5byte_start);

      output.append(1, static_cast<char>(0xfc + x / internal::modulus4));
      output.append(
        1,
        static_cast<char>(internal::encode_mod((x / internal::modulus3) % internal::modulus))
      );
      output.append(
        1,
        static_cast<char>(internal::encode_mod((x / internal::modulus2) % internal::modulus))
      );
      output.append(
        1,
        static_cast<char>(internal::encode_mod((x / internal::modulus) % internal::modulus))
      );
      output.append(1, static_cast<char>(internal::encode_mod(x % internal::modulus)));
    }
  }

  /**
   * Encodes given Unicode character sequence into a byte string using UTF-1
   * character encoding. Encoding errors are ignored.
   */
  inline std::string
  encode(const char32_t* input, std::size_t length)
  {
    return utils::encode(input, length, encode_codepoint);
  }

  /**
   * Encodes given Unicode string into a byte string using UTF-1 character
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

    const auto b0 = static_cast<unsigned char>(input[i]);
    const auto seq_length = sequence_length(b0);

    if (!seq_length || i + seq_length > length)
    {
      ++i;
      return false;
    }

    if (seq_length == 1)
    {
      result = static_cast<char32_t>(b0);
      ++i;
      return true;
    }

    if (b0 == 0xa0)
    {
      const auto b1 = static_cast<unsigned char>(input[i + 1]);

      // Second byte encodes U+00A0..U+00FF directly.
      if (b1 < 0xa0)
      {
        ++i;
        return false;
      }

      result = static_cast<char32_t>(b1);
      i += 2;
      return true;
    }

    if (seq_length == 2)
    {
      std::uint32_t mod;

      if (!internal::decode_mod(static_cast<unsigned char>(input[i + 1]), mod))
      {
        ++i;
        return false;
      }

      result = static_cast<char32_t>(
        internal::range_2byte_start
        + (b0 - 0xa1) * internal::modulus
        + mod
      );
      i += 2;
      return true;
    }

    if (seq_length == 3)
    {
      std::uint32_t mod1;
      std::uint32_t mod2;

      if (!internal::decode_mod(static_cast<unsigned char>(input[i + 1]), mod1)
        || !internal::decode_mod(static_cast<unsigned char>(input[i + 2]), mod2))
      {
        ++i;
        return false;
      }

      result = static_cast<char32_t>(
        internal::range_3byte_start
        + (b0 - 0xf6) * internal::modulus2
        + mod1 * internal::modulus
        + mod2
      );
      i += 3;
      return true;
    }

    // seq_length == 5
    {
      std::uint32_t mod1;
      std::uint32_t mod2;
      std::uint32_t mod3;
      std::uint32_t mod4;

      if (!internal::decode_mod(static_cast<unsigned char>(input[i + 1]), mod1)
        || !internal::decode_mod(static_cast<unsigned char>(input[i + 2]), mod2)
        || !internal::decode_mod(static_cast<unsigned char>(input[i + 3]), mod3)
        || !internal::decode_mod(static_cast<unsigned char>(input[i + 4]), mod4))
      {
        ++i;
        return false;
      }

      result = static_cast<char32_t>(
        internal::range_5byte_start
        + (b0 - 0xfc) * internal::modulus4
        + mod1 * internal::modulus3
        + mod2 * internal::modulus2
        + mod3 * internal::modulus
        + mod4
      );
      i += 5;
      return true;
    }
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-1 character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string
  decode(const char* input, const std::size_t length)
  {
    return utils::decode(input, length, decode_advance);
  }

  /**
   * Decodes given byte string into Unicode string using UTF-1 character
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
   * Encodes given Unicode character sequence into a byte string using UTF-1
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
   * Encodes given Unicode string into a byte string using UTF-1 character
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
   * Decodes given byte sequence into Unicode string using UTF-1 character
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
   * Decodes given byte string into Unicode string using UTF-1 character
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
