/*
 * Copyright (c) 2018-2026, peelo.net
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
    static const std::array<utils::range, 72> digit_table =
    {{
      { 0x0030, 0x0039, }, { 0x0660, 0x0669, }, { 0x06f0, 0x06f9, },
      { 0x07c0, 0x07c9, }, { 0x0966, 0x096f, }, { 0x09e6, 0x09ef, },
      { 0x0a66, 0x0a6f, }, { 0x0ae6, 0x0aef, }, { 0x0b66, 0x0b6f, },
      { 0x0be6, 0x0bef, }, { 0x0c66, 0x0c6f, }, { 0x0ce6, 0x0cef, },
      { 0x0d66, 0x0d6f, }, { 0x0de6, 0x0def, }, { 0x0e50, 0x0e59, },
      { 0x0ed0, 0x0ed9, }, { 0x0f20, 0x0f29, }, { 0x1040, 0x1049, },
      { 0x1090, 0x1099, }, { 0x17e0, 0x17e9, }, { 0x1810, 0x1819, },
      { 0x1946, 0x194f, }, { 0x19d0, 0x19d9, }, { 0x1a80, 0x1a89, },
      { 0x1a90, 0x1a99, }, { 0x1b50, 0x1b59, }, { 0x1bb0, 0x1bb9, },
      { 0x1c40, 0x1c49, }, { 0x1c50, 0x1c59, }, { 0xa620, 0xa629, },
      { 0xa8d0, 0xa8d9, }, { 0xa900, 0xa909, }, { 0xa9d0, 0xa9d9, },
      { 0xa9f0, 0xa9f9, }, { 0xaa50, 0xaa59, }, { 0xabf0, 0xabf9, },
      { 0xff10, 0xff19, }, { 0x104a0, 0x104a9, }, { 0x10d30, 0x10d39, },
      { 0x10d40, 0x10d49, }, { 0x11066, 0x1106f, }, { 0x110f0, 0x110f9, },
      { 0x11136, 0x1113f, }, { 0x111d0, 0x111d9, }, { 0x112f0, 0x112f9, },
      { 0x11450, 0x11459, }, { 0x114d0, 0x114d9, }, { 0x11650, 0x11659, },
      { 0x116c0, 0x116c9, }, { 0x116d0, 0x116e3, }, { 0x11730, 0x11739, },
      { 0x118e0, 0x118e9, }, { 0x11950, 0x11959, }, { 0x11bf0, 0x11bf9, },
      { 0x11c50, 0x11c59, }, { 0x11d50, 0x11d59, }, { 0x11da0, 0x11da9, },
      { 0x11de0, 0x11de9, }, { 0x11f50, 0x11f59, }, { 0x16130, 0x16139, },
      { 0x16a60, 0x16a69, }, { 0x16ac0, 0x16ac9, }, { 0x16b50, 0x16b59, },
      { 0x16d70, 0x16d79, }, { 0x1ccf0, 0x1ccf9, }, { 0x1d7ce, 0x1d7ff, },
      { 0x1e140, 0x1e149, }, { 0x1e2f0, 0x1e2f9, }, { 0x1e4f0, 0x1e4f9, },
      { 0x1e5f1, 0x1e5fa, }, { 0x1e950, 0x1e959, }, { 0x1fbf0, 0x1fbf9, },
    }};

    return utils::table_lookup(digit_table, c);
  }
}
