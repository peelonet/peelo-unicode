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
#ifndef PEELO_UNICODE_CTYPE_ISSPACE_HPP_GUARD
#define PEELO_UNICODE_CTYPE_ISSPACE_HPP_GUARD

namespace peelo::unicode::ctype
{
  /**
   * Determines whether the given Unicode code point is a space character.
   */
  inline bool isspace(char32_t c)
  {
    static const char32_t space_table[11][2] =
    {
      { 0x0009, 0x000d }, { 0x0020, 0x0020 },
      { 0x0085, 0x0085 }, { 0x00a0, 0x00a0 },
      { 0x1680, 0x1680 }, { 0x180e, 0x180e },
      { 0x2000, 0x200a }, { 0x2028, 0x2029 },
      { 0x202f, 0x202f }, { 0x205f, 0x205f },
      { 0x3000, 0x3000 }
    };

    for (int i = 0; i < 11; ++i)
    {
      if (c >= space_table[i][0] && c <= space_table[i][1])
      {
        return true;
      }
    }

    return false;
  }
}

#endif /* !PEELO_UNICODE_CTYPE_ISSPACE_HPP_GUARD */
