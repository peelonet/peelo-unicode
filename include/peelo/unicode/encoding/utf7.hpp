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

namespace peelo::unicode::encoding::utf7
{
  namespace internal
  {
    static constexpr const char* base64_alphabet =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    inline bool
    is_base64(unsigned char byte)
    {
      return (byte >= 'A' && byte <= 'Z')
        || (byte >= 'a' && byte <= 'z')
        || (byte >= '0' && byte <= '9')
        || byte == '+'
        || byte == '/';
    }

    inline int
    base64_value(unsigned char byte)
    {
      if (byte >= 'A' && byte <= 'Z')
      {
        return byte - 'A';
      }
      else if (byte >= 'a' && byte <= 'z')
      {
        return byte - 'a' + 26;
      }
      else if (byte >= '0' && byte <= '9')
      {
        return byte - '0' + 52;
      }
      else if (byte == '+')
      {
        return 62;
      }
      else if (byte == '/')
      {
        return 63;
      }

      return -1;
    }

    /**
     * Characters that may be encoded directly as US-ASCII (Set D, Set O, and
     * the whitespace characters allowed by RFC 2152). '+' is never direct.
     */
    inline bool
    is_direct(char32_t c)
    {
      if (c > 0x7f)
      {
        return false;
      }
      if ((c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z')
        || (c >= '0' && c <= '9'))
      {
        return true;
      }

      switch (c)
      {
        // Whitespace
        case '\t':
        case '\n':
        case '\r':
        case ' ':
        // Set D
        case '\'':
        case '(':
        case ')':
        case ',':
        case '-':
        case '.':
        case '/':
        case ':':
        case '?':
        // Set O
        case '!':
        case '"':
        case '#':
        case '$':
        case '%':
        case '&':
        case '*':
        case ';':
        case '<':
        case '=':
        case '>':
        case '@':
        case '[':
        case ']':
        case '^':
        case '_':
        case '`':
        case '{':
        case '|':
        case '}':
          return true;

        default:
          return false;
      }
    }

    inline void
    append_utf16(char32_t c, std::string& bytes)
    {
      if (c > 0xffff)
      {
        const auto high = static_cast<std::uint16_t>((c >> 10) + 0xd7c0);
        const auto low = static_cast<std::uint16_t>((c & 0x3ff) + 0xdc00);

        bytes.append(1, static_cast<char>((high >> 8) & 0xff));
        bytes.append(1, static_cast<char>(high & 0xff));
        bytes.append(1, static_cast<char>((low >> 8) & 0xff));
        bytes.append(1, static_cast<char>(low & 0xff));
      }
      else
      {
        bytes.append(1, static_cast<char>((c >> 8) & 0xff));
        bytes.append(1, static_cast<char>(c & 0xff));
      }
    }

    inline void
    encode_modified_base64(const std::string& bytes, std::string& output)
    {
      std::uint32_t buffer = 0;
      int bits = 0;

      for (std::size_t i = 0; i < bytes.length(); ++i)
      {
        buffer = (buffer << 8) | static_cast<unsigned char>(bytes[i]);
        bits += 8;

        while (bits >= 6)
        {
          bits -= 6;
          output.append(1, base64_alphabet[(buffer >> bits) & 0x3f]);
        }
      }

      if (bits > 0)
      {
        output.append(1, base64_alphabet[(buffer << (6 - bits)) & 0x3f]);
      }
    }

    /**
     * Writes a shifted sequence for the given UTF-16BE bytes. A terminating
     * '-' is emitted when required by RFC 2152: at end of input, when the
     * following character is in the Base64 set, or when it is '-'.
     */
    inline void
    flush_shifted(
      const std::string& bytes,
      std::string& output,
      bool require_terminator
    )
    {
      if (bytes.empty())
      {
        return;
      }

      output.append(1, '+');
      encode_modified_base64(bytes, output);
      if (require_terminator)
      {
        output.append(1, '-');
      }
    }

    inline bool
    requires_shift_terminator(char32_t next)
    {
      return next == '-'
        || (next <= 0x7f && is_base64(static_cast<unsigned char>(next)));
    }

    inline bool
    decode_modified_base64(
      const char* input,
      std::size_t length,
      std::string& bytes,
      bool validate
    )
    {
      std::uint32_t buffer = 0;
      int bits = 0;

      for (std::size_t i = 0; i < length; ++i)
      {
        const auto value = base64_value(static_cast<unsigned char>(input[i]));

        if (value < 0)
        {
          return false;
        }

        buffer = (buffer << 6) | static_cast<std::uint32_t>(value);
        bits += 6;

        if (bits >= 8)
        {
          bits -= 8;
          bytes.append(1, static_cast<char>((buffer >> bits) & 0xff));
        }
      }

      // Remaining bits that do not form a complete octet, and any trailing
      // odd octet that does not form a complete UTF-16 code unit, must be
      // zero when validating (RFC 2152).
      if (bits > 0)
      {
        const auto remaining = buffer & ((1u << bits) - 1u);

        if (remaining != 0)
        {
          if (validate)
          {
            return false;
          }
        }
      }

      if ((bytes.length() % 2) != 0)
      {
        const auto orphan = static_cast<unsigned char>(bytes.back());

        bytes.pop_back();
        if (orphan != 0 && validate)
        {
          return false;
        }
      }

      return true;
    }

    inline bool
    utf16be_to_codepoints(
      const std::string& bytes,
      std::u32string& output,
      bool validate
    )
    {
      for (std::size_t i = 0; i < bytes.length();)
      {
        if (i + 1 >= bytes.length())
        {
          return !validate;
        }

        const auto unit = static_cast<std::uint16_t>(
          (static_cast<unsigned char>(bytes[i]) << 8)
          | static_cast<unsigned char>(bytes[i + 1])
        );

        if (unit >= 0xd800 && unit <= 0xdbff)
        {
          if (i + 3 >= bytes.length())
          {
            if (validate)
            {
              return false;
            }
            i += 2;
            continue;
          }

          const auto low = static_cast<std::uint16_t>(
            (static_cast<unsigned char>(bytes[i + 2]) << 8)
            | static_cast<unsigned char>(bytes[i + 3])
          );

          if (low < 0xdc00 || low > 0xdfff)
          {
            if (validate)
            {
              return false;
            }
            i += 2;
            continue;
          }

          const auto codepoint = static_cast<char32_t>(
            (((unit & 0x3ff) << 10) | (low & 0x3ff)) + 0x10000
          );

          if (!ctype::isvalid(codepoint))
          {
            if (validate)
            {
              return false;
            }
          }
          else
          {
            output.append(1, codepoint);
          }
          i += 4;
        }
        else if (unit >= 0xdc00 && unit <= 0xdfff)
        {
          if (validate)
          {
            return false;
          }
          i += 2;
        }
        else
        {
          const auto codepoint = static_cast<char32_t>(unit);

          if (!ctype::isvalid(codepoint))
          {
            if (validate)
            {
              return false;
            }
          }
          else
          {
            output.append(1, codepoint);
          }
          i += 2;
        }
      }

      return true;
    }
  }

  /**
   * Determines whether the given Unicode code point can be encoded directly
   * as US-ASCII in UTF-7 (Set D, Set O, or allowed whitespace).
   */
  inline bool
  is_direct(char32_t c)
  {
    return internal::is_direct(c);
  }

  inline void
  encode_codepoint(char32_t c, std::string& output)
  {
    if (c == '+')
    {
      output.append("+-");
      return;
    }

    if (internal::is_direct(c))
    {
      output.append(1, static_cast<char>(c));
      return;
    }

    std::string bytes;

    internal::append_utf16(c, bytes);
    internal::flush_shifted(bytes, output, true);
  }

  /**
   * Encodes given Unicode character sequence into a byte string using UTF-7
   * character encoding. Encoding errors are ignored.
   */
  inline std::string
  encode(const char32_t* input, std::size_t length)
  {
    std::string result;
    std::string pending;

    for (std::size_t i = 0; i < length; ++i)
    {
      const auto c = input[i];

      if (!ctype::isvalid(c))
      {
        continue;
      }

      if (c == '+')
      {
        internal::flush_shifted(pending, result, true);
        pending.clear();
        result.append("+-");
      }
      else if (internal::is_direct(c))
      {
        internal::flush_shifted(
          pending,
          result,
          internal::requires_shift_terminator(c)
        );
        pending.clear();
        result.append(1, static_cast<char>(c));
      }
      else
      {
        internal::append_utf16(c, pending);
      }
    }

    internal::flush_shifted(pending, result, true);

    return result;
  }

  /**
   * Encodes given Unicode string into a byte string using UTF-7 character
   * encoding. Encoding errors are ignored.
   */
  inline std::string
  encode(const std::u32string& input)
  {
    return encode(input.c_str(), input.length());
  }

  /**
   * Encodes given Unicode character sequence into a byte string using UTF-7
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
    std::string pending;

    for (std::size_t i = 0; i < length; ++i)
    {
      const auto c = input[i];

      if (!ctype::isvalid(c))
      {
        return false;
      }

      if (c == '+')
      {
        internal::flush_shifted(pending, output, true);
        pending.clear();
        output.append("+-");
      }
      else if (internal::is_direct(c))
      {
        internal::flush_shifted(
          pending,
          output,
          internal::requires_shift_terminator(c)
        );
        pending.clear();
        output.append(1, static_cast<char>(c));
      }
      else
      {
        internal::append_utf16(c, pending);
      }
    }

    internal::flush_shifted(pending, output, true);

    return true;
  }

  /**
   * Encodes given Unicode string into a byte string using UTF-7 character
   * encoding. Result will be stored into given byte string. If encoding
   * error is encountered, this function will return false, otherwise it
   * returns true.
   */
  inline bool
  encode_validate(const std::u32string& input, std::string& output)
  {
    return encode_validate(input.c_str(), input.length(), output);
  }

  namespace internal
  {
    inline bool
    decode_shifted(
      const char* input,
      std::size_t& i,
      const std::size_t length,
      std::u32string& output,
      bool validate
    )
    {
      // Caller has already consumed the leading '+'; i points at the next byte.
      if (i >= length)
      {
        // Lone trailing '+' with an empty shifted sequence.
        return true;
      }

      if (input[i] == '-')
      {
        output.append(1, U'+');
        ++i;
        return true;
      }

      if (!is_base64(static_cast<unsigned char>(input[i])))
      {
        // '+' followed by a non-base64, non-'-' character is ill-formed.
        if (validate)
        {
          return false;
        }
        ++i;
        return true;
      }

      const auto base64_start = i;

      while (i < length && is_base64(static_cast<unsigned char>(input[i])))
      {
        ++i;
      }

      std::string bytes;

      if (!decode_modified_base64(
        input + base64_start,
        i - base64_start,
        bytes,
        validate
      ))
      {
        if (validate)
        {
          return false;
        }
        bytes.clear();
      }

      // A shifted sequence that yields no complete UTF-16 code unit is
      // ill-formed, except for the empty sequence after a lone '+'.
      if (bytes.empty() && i > base64_start)
      {
        if (validate)
        {
          return false;
        }
      }
      else if (!utf16be_to_codepoints(bytes, output, validate))
      {
        return false;
      }

      // Absorb optional terminating '-'. End of input also ends the sequence.
      if (i < length && input[i] == '-')
      {
        ++i;
      }

      return true;
    }

    inline bool
    decode_impl(
      const char* input,
      const std::size_t length,
      std::u32string& output,
      bool validate
    )
    {
      for (std::size_t i = 0; i < length;)
      {
        const auto byte = static_cast<unsigned char>(input[i]);

        if (byte == '+')
        {
          ++i;
          if (!decode_shifted(input, i, length, output, validate))
          {
            return false;
          }
        }
        else if (byte > 0x7f)
        {
          if (validate)
          {
            return false;
          }
          ++i;
        }
        else
        {
          // Outside a shifted sequence, any US-ASCII octet maps to itself.
          output.append(1, static_cast<char32_t>(byte));
          ++i;
        }
      }

      return true;
    }
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-7 character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string
  decode(const char* input, const std::size_t length)
  {
    std::u32string result;

    internal::decode_impl(input, length, result, false);

    return result;
  }

  /**
   * Decodes given byte string into Unicode string using UTF-7 character
   * encoding. Decoding errors are ignored.
   */
  inline std::u32string
  decode(const std::string& input)
  {
    return decode(input.c_str(), input.length());
  }

  /**
   * Decodes given byte sequence into Unicode string using UTF-7 character
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
    return internal::decode_impl(input, length, output, true);
  }

  /**
   * Decodes given byte string into Unicode string using UTF-7 character
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
