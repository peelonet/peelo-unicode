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
#ifndef PEELO_UNICODE_CTYPE_ISDIGIT_HPP_GUARD
#define PEELO_UNICODE_CTYPE_ISDIGIT_HPP_GUARD

namespace peelo::unicode::ctype
{
  /**
   * Determines whether the given Unicode code point is a digit.
   */
  inline bool isdigit(char32_t c)
  {
    static const char32_t digit_table[23][2] =
    {
      { 0x0030, 0x0039 }, { 0x0660, 0x0669 }, { 0x06f0, 0x06f9 },
      { 0x0966, 0x096f }, { 0x09e6, 0x09ef }, { 0x0a66, 0x0a6f },
      { 0x0ae6, 0x0aef }, { 0x0b66, 0x0b6f }, { 0x0be6, 0x0bef },
      { 0x0c66, 0x0c6f }, { 0x0ce6, 0x0cef }, { 0x0d66, 0x0d6f },
      { 0x0e50, 0x0e59 }, { 0x0ed0, 0x0ed9 }, { 0x0f20, 0x0f29 },
      { 0x1040, 0x1049 }, { 0x17e0, 0x17e9 }, { 0x1810, 0x1819 },
      { 0x1946, 0x194f }, { 0x19d0, 0x19d9 }, { 0xff10, 0xff19 },
      { 0x104a0, 0x104a9 }, { 0x1d7ce, 0x1d7ff }
    };

    for (int i = 0; i < 23; ++i)
    {
      if (c >= digit_table[i][0] && c <= digit_table[i][1])
      {
        return true;
      }
    }

    return false;
  }
}

#endif /* !PEELO_UNICODE_CTYPE_ISDIGIT_HPP_GUARD */
