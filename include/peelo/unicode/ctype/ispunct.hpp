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
#ifndef PEELO_UNICODE_CTYPE_ISPUNCT_HPP_GUARD
#define PEELO_UNICODE_CTYPE_ISPUNCT_HPP_GUARD

namespace peelo::unicode::ctype
{
  /**
   * Determines whether the given Unicode code point is a punctuation
   * character.
   */
  inline bool ispunct(char32_t c)
  {
    static const char32_t punct_table[96][2] =
    {
      { 0x0021, 0x0023 }, { 0x0025, 0x002a }, { 0x002c, 0x002f },
      { 0x003a, 0x003b }, { 0x003f, 0x0040 }, { 0x005b, 0x005d },
      { 0x005f, 0x005f }, { 0x007b, 0x007b }, { 0x007d, 0x007d },
      { 0x00a1, 0x00a1 }, { 0x00ab, 0x00ab }, { 0x00b7, 0x00b7 },
      { 0x00bb, 0x00bb }, { 0x00bf, 0x00bf }, { 0x037e, 0x037e },
      { 0x0387, 0x0387 }, { 0x055a, 0x055f }, { 0x0589, 0x058a },
      { 0x05be, 0x05be }, { 0x05c0, 0x05c0 }, { 0x05c3, 0x05c3 },
      { 0x05c6, 0x05c6 }, { 0x05f3, 0x05f4 }, { 0x060c, 0x060d },
      { 0x061b, 0x061b }, { 0x061e, 0x061f }, { 0x066a, 0x066d },
      { 0x06d4, 0x06d4 }, { 0x0700, 0x070d }, { 0x0964, 0x0965 },
      { 0x0970, 0x0970 }, { 0x0df4, 0x0df4 }, { 0x0e4f, 0x0e4f },
      { 0x0e5a, 0x0e5b }, { 0x0f04, 0x0f12 }, { 0x0f3a, 0x0f3d },
      { 0x0f85, 0x0f85 }, { 0x0fd0, 0x0fd1 }, { 0x104a, 0x104f },
      { 0x10fb, 0x10fb }, { 0x1361, 0x1368 }, { 0x166d, 0x166e },
      { 0x169b, 0x169c }, { 0x16eb, 0x16ed }, { 0x1735, 0x1736 },
      { 0x17d4, 0x17d6 }, { 0x17d8, 0x17da }, { 0x1800, 0x180a },
      { 0x1944, 0x1945 }, { 0x19de, 0x19df }, { 0x1a1e, 0x1a1f },
      { 0x2010, 0x2027 }, { 0x2030, 0x2043 }, { 0x2045, 0x2051 },
      { 0x2053, 0x205e }, { 0x207d, 0x207e }, { 0x208d, 0x208e },
      { 0x2329, 0x232a }, { 0x23b4, 0x23b6 }, { 0x2768, 0x2775 },
      { 0x27c5, 0x27c6 }, { 0x27e6, 0x27eb }, { 0x2983, 0x2998 },
      { 0x29d8, 0x29db }, { 0x29fc, 0x29fd }, { 0x2cf9, 0x2cfc },
      { 0x2cfe, 0x2cff }, { 0x2e00, 0x2e17 }, { 0x2e1c, 0x2e1d },
      { 0x3001, 0x3003 }, { 0x3008, 0x3011 }, { 0x3014, 0x301f },
      { 0x3030, 0x3030 }, { 0x303d, 0x303d }, { 0x30a0, 0x30a0 },
      { 0x30fb, 0x30fb }, { 0xfd3e, 0xfd3f }, { 0xfe10, 0xfe19 },
      { 0xfe30, 0xfe52 }, { 0xfe54, 0xfe61 }, { 0xfe63, 0xfe63 },
      { 0xfe68, 0xfe68 }, { 0xfe6a, 0xfe6b }, { 0xff01, 0xff03 },
      { 0xff05, 0xff0a }, { 0xff0c, 0xff0f }, { 0xff1a, 0xff1b },
      { 0xff1f, 0xff20 }, { 0xff3b, 0xff3d }, { 0xff3f, 0xff3f },
      { 0xff5b, 0xff5b }, { 0xff5d, 0xff5d }, { 0xff5f, 0xff65 },
      { 0x10100, 0x10101 }, { 0x1039f, 0x1039f }, { 0x10a50, 0x10a58 }
    };

    for (int i = 0; i < 96; ++i)
    {
      if (c >= punct_table[i][0] && c <= punct_table[i][1])
      {
        return true;
      }
    }

    return false;
  }
}

#endif /* !PEELO_UNICODE_CTYPE_ISPUNCT_HPP_GUARD */
