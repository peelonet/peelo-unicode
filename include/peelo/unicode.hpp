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
#include <cstddef>
#include <string>

namespace peelo
{
  namespace unicode
  {
    /**
     * Determines whether given Unicode code point is valid or not.
     */
    bool isvalid(char32_t);

    /**
     * Determines whether the given Unicode code point is alphanumeric.
     */
    bool isalnum(char32_t);

    /**
     * Determines whether the given Unicode code point is alphabetic.
     */
    bool isalpha(char32_t);

    /**
     * Determines whether the given Unicode code point is lowercase.
     */
    bool islower(char32_t);

    /**
     * Determines whether the given Unicode code point is an uppercase
     * character.
     */
    bool isupper(char32_t);

    /**
     * Determines whether the given Unicode code point is a digit.
     */
    bool isdigit(char32_t);

    /**
     * Determines whether the given Unicode code point is a hexadecimal
     * character.
     */
    bool isxdigit(char32_t);

    /**
     * Determines whether the given Unicode code point is a control character.
     */
    bool iscntrl(char32_t);

    /**
     * Determines whether the given Unicode code point is a graphical
     * character.
     */
    bool isgraph(char32_t);

    /**
     * Determines whether the given Unicode code point is a space character.
     */
    bool isspace(char32_t);

    /**
     * Determines whether the given Unicode code point is a blank character.
     */
    bool isblank(char32_t);

    /**
     * Determines whether the given Unicode code point is a printing character.
     */
    bool isprint(char32_t);

    /**
     * Determines whether the given Unicode code point is a punctuation
     * character.
     */
    bool ispunct(char32_t);

    /**
     * Converts given Unicode code point into lowercase.
     */
    char32_t tolower(char32_t);

    /**
     * Converts given Unicode code point into uppercase.
     */
    char32_t toupper(char32_t);

    namespace utf8
    {
      /**
       * Encodes given Unicode string into a byte string using UTF-8 character
       * encoding. Encoding errors are ignored.
       */
      std::string encode(const std::u32string&);

      /**
       * Decodes given byte string into Unicode string using UTF-8 character
       * encoding. Decoding errors are ignored.
       */
      std::u32string decode(const std::string&);

      /**
       * Encodes given Unicode string into a byte string using UTF-8 character
       * encoding. Result will be stored into given byte string. If encoding
       * error is encountered, this function will return false, otherwise it
       * returns true.
       */
      bool encode_validate(const std::u32string&, std::string&);

      /**
       * Decodes given byte string into Unicode string using UTF-8 character
       * encoding. Result will be stroed into given Unicode string. If decoding
       * error is encountered, this function will return false, otherwise it
       * returns true.
       */
      bool decode_validate(const std::string&, std::u32string&);

      /**
       * Attempts to determine length (in bytes) of UTF-8 sequence which begins
       * with the given byte. If the length cannot be determined (i.e.
       * beginning of sequence is invalid according to the UTF-8
       * specification), 0 will be returned instead.
       */
      std::size_t sequence_length(unsigned char);

      /**
       * Attempts to determine the amount of bytes which are required to encode
       * given Unicode code point with UTF-8 character encoding. If the given
       * code point cannot be encoded, 0 will be returned instead.
       */
      std::size_t codepoint_length(char32_t);
    }
  }
}
