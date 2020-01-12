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
#ifndef PEELO_UNICODE_CTYPE_TOLOWER_HPP_GUARD
#define PEELO_UNICODE_CTYPE_TOLOWER_HPP_GUARD

namespace peelo::unicode::ctype
{
  /**
   * Converts given Unicode code point into lowercase.
   */
  inline char32_t tolower(char32_t c)
  {
    if (c >= 'A' && c <= 'Z')
    {
      return c + 32;
    }
    if (c >= 0x00C0)
    {
      if ((c >= 0x00C0 && c <= 0x00D6) || (c >= 0x00D8 && c <= 0x00DE))
      {
        return c + 32;
      }
      else if ((c >= 0x0100 && c < 0x0138) || (c > 0x0149 && c < 0x0178))
      {
        if (c == 0x0130)
        {
          return 0x0069;
        }
        else if ((c & 1) == 0)
        {
          return c + 1;
        }
      }
      else if (c == 0x0178)
      {
        return 0x00FF;
      }
      else if ((c >= 0x0139 && c < 0x0149) || (c > 0x0178 && c < 0x017F))
      {
        if (c & 1)
        {
          return c + 1;
        }
      }
      else if (c >= 0x0200 && c <= 0x0217)
      {
        if ((c & 1) == 0)
        {
          return c + 1;
        }
      }
      else if ((c >= 0x0401 && c <= 0x040C) || (c >= 0x040E && c <= 0x040F))
      {
        return c + 80;
      }
      else if (c >= 0x410 && c <= 0x042F)
      {
        return c + 32;
      }
      else if (c >= 0x0460 && c <= 0x047F)
      {
        if ((c & 1) == 0)
        {
          return c + 1;
        }
      }
      else if (c >= 0x0531 && c <= 0x0556)
      {
        return c + 48;
      }
      else if (c >= 0x10A0 && c <= 0x10C5)
      {
        return c + 48;
      }
      else if (c >= 0xFF21 && c <= 0xFF3A)
      {
        return c + 32;
      }
    }

    return c;
  }
}

#endif /* !PEELO_UNICODE_CTYPE_TOLOWER_HPP_GUARD */
