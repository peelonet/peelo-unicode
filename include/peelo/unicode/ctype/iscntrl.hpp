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
#ifndef PEELO_UNICODE_CTYPE_ISCNTRL_HPP_GUARD
#define PEELO_UNICODE_CTYPE_ISCNTRL_HPP_GUARD

namespace peelo::unicode::ctype
{
  /**
   * Determines whether the given Unicode code point is a control character.
   */
  inline bool iscntrl(char32_t c)
  {
    static const char32_t cntrl_table[19][2] =
    {
      { 0x0000, 0x001f }, { 0x007f, 0x009f }, { 0x00ad, 0x00ad },
      { 0x0600, 0x0603 }, { 0x06dd, 0x06dd }, { 0x070f, 0x070f },
      { 0x17b4, 0x17b5 }, { 0x200b, 0x200f }, { 0x202a, 0x202e },
      { 0x2060, 0x2063 }, { 0x206a, 0x206f }, { 0xd800, 0xf8ff },
      { 0xfeff, 0xfeff }, { 0xfff9, 0xfffb }, { 0x1d173, 0x1d17a },
      { 0xe0001, 0xe0001 }, { 0xe0020, 0xe007f }, { 0xf0000, 0xffffd },
      { 0x100000, 0x10fffd }
    };

    for (int i = 0; i < 19; ++i)
    {
      if (c >= cntrl_table[i][0] && c <= cntrl_table[i][1])
      {
        return true;
      }
    }

    return false;
  }
}

#endif /* !PEELO_UNICODE_CTYPE_ISCNTRL_HPP_GUARD */
