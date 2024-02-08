/*
 * Copyright (c) 2018-2024, peelo.net
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

#include <peelo/unicode/ctype/_utils.hpp>

namespace peelo::unicode::ctype
{
  /**
   * Determines whether the given Unicode code point is a punctuation
   * character.
   */
  inline bool
  ispunct(char32_t c)
  {
    static const std::array<utils::range, 339> punct_table =
    {{
      { 0x0021, 0x002f, }, { 0x003a, 0x0040, }, { 0x005b, 0x0060, },
      { 0x007b, 0x007e, }, { 0x00a1, 0x00a9, }, { 0x00ab, 0x00ac, },
      { 0x00ae, 0x00b1, }, { 0x00b4, 0x00b4, }, { 0x00b6, 0x00b8, },
      { 0x00bb, 0x00bb, }, { 0x00bf, 0x00bf, }, { 0x00d7, 0x00d7, },
      { 0x00f7, 0x00f7, }, { 0x02c2, 0x02c5, }, { 0x02d2, 0x02df, },
      { 0x02e5, 0x02eb, }, { 0x02ed, 0x02ed, }, { 0x02ef, 0x02ff, },
      { 0x0375, 0x0375, }, { 0x037e, 0x037e, }, { 0x0384, 0x0385, },
      { 0x0387, 0x0387, }, { 0x03f6, 0x03f6, }, { 0x0482, 0x0482, },
      { 0x055a, 0x055f, }, { 0x0589, 0x058a, }, { 0x058d, 0x058f, },
      { 0x05be, 0x05be, }, { 0x05c0, 0x05c0, }, { 0x05c3, 0x05c3, },
      { 0x05c6, 0x05c6, }, { 0x05f3, 0x05f4, }, { 0x0606, 0x060f, },
      { 0x061b, 0x061b, }, { 0x061d, 0x061f, }, { 0x066a, 0x066d, },
      { 0x06d4, 0x06d4, }, { 0x06de, 0x06de, }, { 0x06e9, 0x06e9, },
      { 0x06fd, 0x06fe, }, { 0x0700, 0x070d, }, { 0x07f6, 0x07f9, },
      { 0x07fe, 0x07ff, }, { 0x0830, 0x083e, }, { 0x085e, 0x085e, },
      { 0x0888, 0x0888, }, { 0x0964, 0x0965, }, { 0x0970, 0x0970, },
      { 0x09f2, 0x09f3, }, { 0x09fa, 0x09fb, }, { 0x09fd, 0x09fd, },
      { 0x0a76, 0x0a76, }, { 0x0af0, 0x0af1, }, { 0x0b70, 0x0b70, },
      { 0x0bf3, 0x0bfa, }, { 0x0c77, 0x0c77, }, { 0x0c7f, 0x0c7f, },
      { 0x0c84, 0x0c84, }, { 0x0d4f, 0x0d4f, }, { 0x0d79, 0x0d79, },
      { 0x0df4, 0x0df4, }, { 0x0e3f, 0x0e3f, }, { 0x0e4f, 0x0e4f, },
      { 0x0e5a, 0x0e5b, }, { 0x0f01, 0x0f17, }, { 0x0f1a, 0x0f1f, },
      { 0x0f34, 0x0f34, }, { 0x0f36, 0x0f36, }, { 0x0f38, 0x0f38, },
      { 0x0f3a, 0x0f3d, }, { 0x0f85, 0x0f85, }, { 0x0fbe, 0x0fc5, },
      { 0x0fc7, 0x0fcc, }, { 0x0fce, 0x0fda, }, { 0x104a, 0x104f, },
      { 0x109e, 0x109f, }, { 0x10fb, 0x10fb, }, { 0x1360, 0x1368, },
      { 0x1390, 0x1399, }, { 0x1400, 0x1400, }, { 0x166d, 0x166e, },
      { 0x169b, 0x169c, }, { 0x16eb, 0x16ed, }, { 0x1735, 0x1736, },
      { 0x17d4, 0x17d6, }, { 0x17d8, 0x17db, }, { 0x1800, 0x180a, },
      { 0x1940, 0x1940, }, { 0x1944, 0x1945, }, { 0x19de, 0x19ff, },
      { 0x1a1e, 0x1a1f, }, { 0x1aa0, 0x1aa6, }, { 0x1aa8, 0x1aad, },
      { 0x1b5a, 0x1b6a, }, { 0x1b74, 0x1b7e, }, { 0x1bfc, 0x1bff, },
      { 0x1c3b, 0x1c3f, }, { 0x1c7e, 0x1c7f, }, { 0x1cc0, 0x1cc7, },
      { 0x1cd3, 0x1cd3, }, { 0x1fbd, 0x1fbd, }, { 0x1fbf, 0x1fc1, },
      { 0x1fcd, 0x1fcf, }, { 0x1fdd, 0x1fdf, }, { 0x1fed, 0x1fef, },
      { 0x1ffd, 0x1ffe, }, { 0x2010, 0x2027, }, { 0x2030, 0x205e, },
      { 0x207a, 0x207e, }, { 0x208a, 0x208e, }, { 0x20a0, 0x20c0, },
      { 0x2100, 0x2101, }, { 0x2103, 0x2106, }, { 0x2108, 0x2109, },
      { 0x2114, 0x2114, }, { 0x2116, 0x2118, }, { 0x211e, 0x2123, },
      { 0x2125, 0x2125, }, { 0x2127, 0x2127, }, { 0x2129, 0x2129, },
      { 0x212e, 0x212e, }, { 0x213a, 0x213b, }, { 0x2140, 0x2144, },
      { 0x214a, 0x214d, }, { 0x214f, 0x214f, }, { 0x218a, 0x218b, },
      { 0x2190, 0x2426, }, { 0x2440, 0x244a, }, { 0x249c, 0x24e9, },
      { 0x2500, 0x2775, }, { 0x2794, 0x2b73, }, { 0x2b76, 0x2b95, },
      { 0x2b97, 0x2bff, }, { 0x2ce5, 0x2cea, }, { 0x2cf9, 0x2cfc, },
      { 0x2cfe, 0x2cff, }, { 0x2d70, 0x2d70, }, { 0x2e00, 0x2e2e, },
      { 0x2e30, 0x2e5d, }, { 0x2e80, 0x2e99, }, { 0x2e9b, 0x2ef3, },
      { 0x2f00, 0x2fd5, }, { 0x2ff0, 0x2fff, }, { 0x3001, 0x3004, },
      { 0x3008, 0x3020, }, { 0x3030, 0x3030, }, { 0x3036, 0x3037, },
      { 0x303d, 0x303f, }, { 0x309b, 0x309c, }, { 0x30a0, 0x30a0, },
      { 0x30fb, 0x30fb, }, { 0x3190, 0x3191, }, { 0x3196, 0x319f, },
      { 0x31c0, 0x31e3, }, { 0x31ef, 0x31ef, }, { 0x3200, 0x321e, },
      { 0x322a, 0x3247, }, { 0x3250, 0x3250, }, { 0x3260, 0x327f, },
      { 0x328a, 0x32b0, }, { 0x32c0, 0x33ff, }, { 0x4dc0, 0x4dff, },
      { 0xa490, 0xa4c6, }, { 0xa4fe, 0xa4ff, }, { 0xa60d, 0xa60f, },
      { 0xa673, 0xa673, }, { 0xa67e, 0xa67e, }, { 0xa6f2, 0xa6f7, },
      { 0xa700, 0xa716, }, { 0xa720, 0xa721, }, { 0xa789, 0xa78a, },
      { 0xa828, 0xa82b, }, { 0xa836, 0xa839, }, { 0xa874, 0xa877, },
      { 0xa8ce, 0xa8cf, }, { 0xa8f8, 0xa8fa, }, { 0xa8fc, 0xa8fc, },
      { 0xa92e, 0xa92f, }, { 0xa95f, 0xa95f, }, { 0xa9c1, 0xa9cd, },
      { 0xa9de, 0xa9df, }, { 0xaa5c, 0xaa5f, }, { 0xaa77, 0xaa79, },
      { 0xaade, 0xaadf, }, { 0xaaf0, 0xaaf1, }, { 0xab5b, 0xab5b, },
      { 0xab6a, 0xab6b, }, { 0xabeb, 0xabeb, }, { 0xfb29, 0xfb29, },
      { 0xfbb2, 0xfbc2, }, { 0xfd3e, 0xfd4f, }, { 0xfdcf, 0xfdcf, },
      { 0xfdfc, 0xfdff, }, { 0xfe10, 0xfe19, }, { 0xfe30, 0xfe52, },
      { 0xfe54, 0xfe66, }, { 0xfe68, 0xfe6b, }, { 0xff01, 0xff0f, },
      { 0xff1a, 0xff20, }, { 0xff3b, 0xff40, }, { 0xff5b, 0xff65, },
      { 0xffe0, 0xffe6, }, { 0xffe8, 0xffee, }, { 0xfffc, 0xfffd, },
      { 0x10100, 0x10102, }, { 0x10137, 0x1013f, }, { 0x10179, 0x10189, },
      { 0x1018c, 0x1018e, }, { 0x10190, 0x1019c, }, { 0x101a0, 0x101a0, },
      { 0x101d0, 0x101fc, }, { 0x1039f, 0x1039f, }, { 0x103d0, 0x103d0, },
      { 0x1056f, 0x1056f, }, { 0x10857, 0x10857, }, { 0x10877, 0x10878, },
      { 0x1091f, 0x1091f, }, { 0x1093f, 0x1093f, }, { 0x10a50, 0x10a58, },
      { 0x10a7f, 0x10a7f, }, { 0x10ac8, 0x10ac8, }, { 0x10af0, 0x10af6, },
      { 0x10b39, 0x10b3f, }, { 0x10b99, 0x10b9c, }, { 0x10ead, 0x10ead, },
      { 0x10f55, 0x10f59, }, { 0x10f86, 0x10f89, }, { 0x11047, 0x1104d, },
      { 0x110bb, 0x110bc, }, { 0x110be, 0x110c1, }, { 0x11140, 0x11143, },
      { 0x11174, 0x11175, }, { 0x111c5, 0x111c8, }, { 0x111cd, 0x111cd, },
      { 0x111db, 0x111db, }, { 0x111dd, 0x111df, }, { 0x11238, 0x1123d, },
      { 0x112a9, 0x112a9, }, { 0x1144b, 0x1144f, }, { 0x1145a, 0x1145b, },
      { 0x1145d, 0x1145d, }, { 0x114c6, 0x114c6, }, { 0x115c1, 0x115d7, },
      { 0x11641, 0x11643, }, { 0x11660, 0x1166c, }, { 0x116b9, 0x116b9, },
      { 0x1173c, 0x1173f, }, { 0x1183b, 0x1183b, }, { 0x11944, 0x11946, },
      { 0x119e2, 0x119e2, }, { 0x11a3f, 0x11a46, }, { 0x11a9a, 0x11a9c, },
      { 0x11a9e, 0x11aa2, }, { 0x11b00, 0x11b09, }, { 0x11c41, 0x11c45, },
      { 0x11c70, 0x11c71, }, { 0x11ef7, 0x11ef8, }, { 0x11f43, 0x11f4f, },
      { 0x11fd5, 0x11ff1, }, { 0x11fff, 0x11fff, }, { 0x12470, 0x12474, },
      { 0x12ff1, 0x12ff2, }, { 0x16a6e, 0x16a6f, }, { 0x16af5, 0x16af5, },
      { 0x16b37, 0x16b3f, }, { 0x16b44, 0x16b45, }, { 0x16e97, 0x16e9a, },
      { 0x16fe2, 0x16fe2, }, { 0x1bc9c, 0x1bc9c, }, { 0x1bc9f, 0x1bc9f, },
      { 0x1cf50, 0x1cfc3, }, { 0x1d000, 0x1d0f5, }, { 0x1d100, 0x1d126, },
      { 0x1d129, 0x1d164, }, { 0x1d16a, 0x1d16c, }, { 0x1d183, 0x1d184, },
      { 0x1d18c, 0x1d1a9, }, { 0x1d1ae, 0x1d1ea, }, { 0x1d200, 0x1d241, },
      { 0x1d245, 0x1d245, }, { 0x1d300, 0x1d356, }, { 0x1d6c1, 0x1d6c1, },
      { 0x1d6db, 0x1d6db, }, { 0x1d6fb, 0x1d6fb, }, { 0x1d715, 0x1d715, },
      { 0x1d735, 0x1d735, }, { 0x1d74f, 0x1d74f, }, { 0x1d76f, 0x1d76f, },
      { 0x1d789, 0x1d789, }, { 0x1d7a9, 0x1d7a9, }, { 0x1d7c3, 0x1d7c3, },
      { 0x1d800, 0x1d9ff, }, { 0x1da37, 0x1da3a, }, { 0x1da6d, 0x1da74, },
      { 0x1da76, 0x1da83, }, { 0x1da85, 0x1da8b, }, { 0x1e14f, 0x1e14f, },
      { 0x1e2ff, 0x1e2ff, }, { 0x1e95e, 0x1e95f, }, { 0x1ecac, 0x1ecac, },
      { 0x1ecb0, 0x1ecb0, }, { 0x1ed2e, 0x1ed2e, }, { 0x1eef0, 0x1eef1, },
      { 0x1f000, 0x1f02b, }, { 0x1f030, 0x1f093, }, { 0x1f0a0, 0x1f0ae, },
      { 0x1f0b1, 0x1f0bf, }, { 0x1f0c1, 0x1f0cf, }, { 0x1f0d1, 0x1f0f5, },
      { 0x1f10d, 0x1f1ad, }, { 0x1f1e6, 0x1f202, }, { 0x1f210, 0x1f23b, },
      { 0x1f240, 0x1f248, }, { 0x1f250, 0x1f251, }, { 0x1f260, 0x1f265, },
      { 0x1f300, 0x1f6d7, }, { 0x1f6dc, 0x1f6ec, }, { 0x1f6f0, 0x1f6fc, },
      { 0x1f700, 0x1f776, }, { 0x1f77b, 0x1f7d9, }, { 0x1f7e0, 0x1f7eb, },
      { 0x1f7f0, 0x1f7f0, }, { 0x1f800, 0x1f80b, }, { 0x1f810, 0x1f847, },
      { 0x1f850, 0x1f859, }, { 0x1f860, 0x1f887, }, { 0x1f890, 0x1f8ad, },
      { 0x1f8b0, 0x1f8b1, }, { 0x1f900, 0x1fa53, }, { 0x1fa60, 0x1fa6d, },
      { 0x1fa70, 0x1fa7c, }, { 0x1fa80, 0x1fa88, }, { 0x1fa90, 0x1fabd, },
      { 0x1fabf, 0x1fac5, }, { 0x1face, 0x1fadb, }, { 0x1fae0, 0x1fae8, },
      { 0x1faf0, 0x1faf8, }, { 0x1fb00, 0x1fb92, }, { 0x1fb94, 0x1fbca, },
    }};

    return utils::table_lookup(punct_table, c);
  }
}
