/*
 * Copyright (c) 2018, peelo.net
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
#include <peelo/unicode.hpp>

namespace peelo
{
  namespace unicode
  {
    namespace utf8
    {
      static void encode_codepoint(char32_t, std::string&);
      static bool decode_advance(
        std::string::const_iterator&,
        const std::string::const_iterator&,
        char32_t&
      );

      std::string encode(const std::u32string& input)
      {
        std::string output;

        for (const auto& c : input)
        {
          if (!peelo::unicode::isvalid(c))
          {
            continue;
          }
          encode_codepoint(c, output);
        }

        return output;
      }

      bool encode_validate(const std::u32string& input, std::string& output)
      {
        for (const auto& c : input)
        {
          if (!peelo::unicode::isvalid(c))
          {
            return false;
          }
          encode_codepoint(c, output);
        }

        return true;
      }

      static void encode_codepoint(char32_t c, std::string& output)
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

      std::u32string decode(const std::string& input)
      {
        auto it = std::begin(input);
        const auto end = std::end(input);
        std::u32string output;

        while (it != end)
        {
          char32_t c;

          if (!decode_advance(it, end, c))
          {
            break;
          }
          output.append(1, c);
        }

        return output;
      }

      bool decode_validate(const std::string& input, std::u32string& output)
      {
        auto it = std::begin(input);
        const auto end = std::end(input);

        while (it != end)
        {
          char32_t c;

          if (!decode_advance(it, end, c))
          {
            return false;
          }
          output.append(1, c);
        }

        return true;
      }

      static bool decode_advance(std::string::const_iterator& it,
                                 const std::string::const_iterator& end,
                                 char32_t& result)
      {
        const auto length = sequence_length(*it);

        if (!length || it + (length - 1) >= end)
        {
          return false;
        }

        switch (length)
        {
          case 1:
            result = static_cast<char32_t>(*it++);
            break;

          case 2:
            result = static_cast<char32_t>(*it++ & 0x1f);
            break;

          case 3:
            result = static_cast<char32_t>(*it++ & 0x0f);
            break;

          case 4:
            result = static_cast<char32_t>(*it++ & 0x07);
            break;

          default:
            return false;
        }

        for (std::size_t i = 1; i < length; ++i)
        {
          if ((*it & 0xc0) != 0x80)
          {
            return false;
          }
          result = (result << 6) | (*it++ & 0x3f);
        }

        return true;
      }

      std::size_t sequence_length(unsigned char byte)
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

      std::size_t codepoint_length(char32_t c)
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
    }
  }
}
