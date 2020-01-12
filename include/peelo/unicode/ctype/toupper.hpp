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
#ifndef PEELO_UNICODE_CTYPE_TOUPPER_HPP_GUARD
#define PEELO_UNICODE_CTYPE_TOUPPER_HPP_GUARD

namespace peelo::unicode::ctype
{
  /**
   * Converts given Unicode code point into uppercase.
   */
  inline char32_t toupper(char32_t c)
  {
    if (c >= 'a' && c <= 'z')
    {
      return c - 32;
    }
    if (c >= 0x00E0)
    {
      if ((c >= 0x00E0 && c <= 0x00F6) || (c >= 0x00F8 && c <= 0x00FE))
      {
        return c - 32;
      }
      else if (c == 0x00FF)
      {
        return 0x0178;
      }
      else if ((c >= 0x0100 && c < 0x0138) || (c > 0x0149 && c < 0x0178))
      {
        if (c == 0x0131)
        {
          return 0x0049;
        }
        else if (c & 1)
        {
          return c - 1;
        }
      }
      else if ((c >= 0x0139 && c < 0x0149) || (c > 0x0178 && c < 0x017F))
      {
        if ((c & 1) == 0)
        {
          return c - 1;
        }
      }
      else if (c == 0x017F)
      {
        return 0x0053;
      }
      else if (c >= 0x0200 && c <= 0x0217)
      {
        if (c & 1)
        {
          return c - 1;
        }
      }
      else if (c >= 0x0430 && c <= 0x044F)
      {
        return c - 32;
      }
      else if ((c >= 0x0451 && c <= 0x045C) || (c >= 0x045E && c <= 0x045F))
      {
        return c - 80;
      }
      else if (c >= 0x0460 && c <= 0x047F)
      {
        if (c & 1)
        {
          return c - 1;
        }
      }
      else if (c >= 0x0561 && c < 0x0587)
      {
        return c - 48;
      }
      else if (c >= 0xFF41 && c <= 0xFF5A)
      {
        return c - 32;
      }
    }

    return c;
  }
}

#endif /* !PEELO_UNICODE_CTYPE_TOUPPER_HPP_GUARD */
