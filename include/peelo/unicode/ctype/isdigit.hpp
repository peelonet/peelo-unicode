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
   * Determines whether the given Unicode code point is a digit.
   */
  inline bool
  isdigit(char32_t c)
  {
    static const std::array<utils::range, 64> digit_table =
    {{
      { 0x0030, 0x0039 }, { 0x0660, 0x0669 }, { 0x06F0, 0x06F9 },
      { 0x07C0, 0x07C9 }, { 0x0966, 0x096F }, { 0x09E6, 0x09EF },
      { 0x0A66, 0x0A6F }, { 0x0AE6, 0x0AEF }, { 0x0B66, 0x0B6F },
      { 0x0BE6, 0x0BEF }, { 0x0C66, 0x0C6F }, { 0x0CE6, 0x0CEF },
      { 0x0D66, 0x0D6F }, { 0x0DE6, 0x0DEF }, { 0x0E50, 0x0E59 },
      { 0x0ED0, 0x0ED9 }, { 0x0F20, 0x0F29 }, { 0x1040, 0x1049 },
      { 0x1090, 0x1099 }, { 0x17E0, 0x17E9 }, { 0x1810, 0x1819 },
      { 0x1946, 0x194F }, { 0x19D0, 0x19D9 }, { 0x1A80, 0x1A89 },
      { 0x1A90, 0x1A99 }, { 0x1B50, 0x1B59 }, { 0x1BB0, 0x1BB9 },
      { 0x1C40, 0x1C49 }, { 0x1C50, 0x1C59 }, { 0xA620, 0xA629 },
      { 0xA8D0, 0xA8D9 }, { 0xA900, 0xA909 }, { 0xA9D0, 0xA9D9 },
      { 0xA9F0, 0xA9F9 }, { 0xAA50, 0xAA59 }, { 0xABF0, 0xABF9 },
      { 0xFF10, 0xFF19 }, { 0x104A0, 0x104A9 }, { 0x10D30, 0x10D39 },
      { 0x11066, 0x1106F }, { 0x110F0, 0x110F9 }, { 0x11136, 0x1113F },
      { 0x111D0, 0x111D9 }, { 0x112F0, 0x112F9 }, { 0x11450, 0x11459 },
      { 0x114D0, 0x114D9 }, { 0x11650, 0x11659 }, { 0x116C0, 0x116C9 },
      { 0x11730, 0x11739 }, { 0x118E0, 0x118E9 }, { 0x11950, 0x11959 },
      { 0x11C50, 0x11C59 }, { 0x11D50, 0x11D59 }, { 0x11DA0, 0x11DA9 },
      { 0x11F50, 0x11F59 }, { 0x16A60, 0x16A69 }, { 0x16AC0, 0x16AC9 },
      { 0x16B50, 0x16B59 }, { 0x1D7CE, 0x1D7FF }, { 0x1E140, 0x1E149 },
      { 0x1E2F0, 0x1E2F9 }, { 0x1E4F0, 0x1E4F9 }, { 0x1E950, 0x1E959 },
      { 0x1FBF0, 0x1FBF9 },
    }};

    return utils::table_lookup(digit_table, c);
  }
}
