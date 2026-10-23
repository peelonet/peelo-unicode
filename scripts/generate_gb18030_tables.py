#!/usr/bin/env python3
"""
Generate GB18030 mapping tables for peelo-unicode library.

Downloads the WHATWG GB18030 index and generates C++ header with mapping tables.

Usage:
    python3 generate_gb18030_tables.py > ../include/peelo/unicode/encoding/gb18030_tables.hpp

Data source:
    https://encoding.spec.whatwg.org/index-gb18030.txt
    https://encoding.spec.whatwg.org/index-gb18030-ranges.txt
"""

import urllib.request
import sys
from datetime import datetime

GB18030_INDEX_URL = "https://encoding.spec.whatwg.org/index-gb18030.txt"
GB18030_RANGES_URL = "https://encoding.spec.whatwg.org/index-gb18030-ranges.txt"


def download_file(url: str) -> str:
    """Download a file and return its contents."""
    print(f"// Downloading {url}...", file=sys.stderr)
    with urllib.request.urlopen(url, timeout=30) as response:
        return response.read().decode('utf-8')


def parse_index_file(content: str) -> list[tuple[int, int]]:
    """
    Parse WHATWG index file.
    Format: "  index\t0xHHHH\tcharacter (name)"
    Returns list of (index, unicode_codepoint) tuples.
    """
    mappings = []
    for line in content.splitlines():
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        parts = line.split('\t')
        if len(parts) >= 2:
            try:
                index = int(parts[0].strip())
                # Codepoint may have 0x prefix
                cp_str = parts[1].strip()
                if cp_str.startswith('0x') or cp_str.startswith('0X'):
                    codepoint = int(cp_str, 16)
                else:
                    codepoint = int(cp_str)
                mappings.append((index, codepoint))
            except ValueError:
                continue
    return mappings


def parse_ranges_file(content: str) -> list[tuple[int, int]]:
    """
    Parse WHATWG GB18030 ranges file.
    Format: "  offset\t0xHHHH"
    Returns list of (gb18030_offset, unicode_codepoint) tuples for 4-byte ranges.
    """
    ranges = []
    for line in content.splitlines():
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        parts = line.split('\t')
        if len(parts) >= 2:
            try:
                offset = int(parts[0].strip())
                # Codepoint may have 0x prefix
                cp_str = parts[1].strip()
                if cp_str.startswith('0x') or cp_str.startswith('0X'):
                    codepoint = int(cp_str, 16)
                else:
                    codepoint = int(cp_str)
                ranges.append((offset, codepoint))
            except ValueError:
                continue
    return ranges


def index_to_gb_bytes(index: int) -> tuple[int, int]:
    """
    Convert WHATWG linear index to GB18030 two-byte sequence.

    The index maps linearly to the GB18030 two-byte space:
    - First byte: 0x81-0xFE (126 values)
    - Second byte: 0x40-0x7E (63 values) or 0x80-0xFE (127 values) = 190 values total

    index = (first_byte - 0x81) * 190 + second_byte_offset
    where second_byte_offset accounts for the gap at 0x7F
    """
    first_byte = (index // 190) + 0x81
    second_offset = index % 190

    if second_offset < 0x3F:  # 0-62 -> 0x40-0x7E
        second_byte = second_offset + 0x40
    else:  # 63-189 -> 0x80-0xFE
        second_byte = second_offset + 0x41  # 0x80 - 0x3F = 0x41

    return (first_byte, second_byte)


def gb_bytes_to_uint16(b1: int, b2: int) -> int:
    """Convert two bytes to a uint16_t value."""
    return (b1 << 8) | b2


def format_entries(entries: list[tuple[int, int]], per_line: int, fmt: str) -> str:
    """Format (a, b) pairs as C++ brace initializers."""
    lines = []
    for i in range(0, len(entries), per_line):
        chunk = entries[i:i + per_line]
        lines.append("    " + "".join(fmt.format(a=a, b=b) for a, b in chunk))
    return "\n".join(lines)


def generate_header(two_byte_mappings: list[tuple[int, int]],
                    four_byte_ranges: list[tuple[int, int]]) -> str:
    """Generate the C++ header file content."""

    # Convert index mappings to (gb_code, unicode) pairs
    gb_to_unicode = []
    for index, unicode_cp in two_byte_mappings:
        b1, b2 = index_to_gb_bytes(index)
        gb_code = gb_bytes_to_uint16(b1, b2)
        gb_to_unicode.append((gb_code, unicode_cp))

    # Sort by GB code for decoding lookup
    gb_to_unicode_sorted = sorted(gb_to_unicode, key=lambda x: x[0])

    # Create reverse mapping sorted by Unicode for encoding lookup
    unicode_to_gb_sorted = sorted(gb_to_unicode, key=lambda x: x[1])

    gb_to_unicode_body = format_entries(
        gb_to_unicode_sorted, 4, "{{0x{a:04X},0x{b:04X}}},"
    )
    unicode_to_gb_body = format_entries(
        [(uni, gb) for gb, uni in unicode_to_gb_sorted],
        4,
        "{{0x{a:04X},0x{b:04X}}},",
    )
    ranges_body = format_entries(
        four_byte_ranges, 3, "{{0x{a:X},0x{b:04X}}},"
    )

    return f"""/*
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

/*
 * AUTO-GENERATED FILE - DO NOT EDIT
 * Generated on: {datetime.now().strftime("%Y-%m-%d")}
 * Source: WHATWG Encoding Standard
 *   - {GB18030_INDEX_URL}
 *   - {GB18030_RANGES_URL}
 */
#pragma once

#include <array>
#include <cstdint>

namespace peelo::unicode::encoding::gb18030::tables
{{
  // Two-byte GB18030 to Unicode mapping
  // Sorted by GB18030 code for binary search during decoding
  // Total entries: {len(gb_to_unicode_sorted)}
  struct gb_to_unicode_entry
  {{
    std::uint16_t gb;
    char32_t unicode;
  }};

  inline constexpr std::array<gb_to_unicode_entry, {len(gb_to_unicode_sorted)}> gb_to_unicode =
  {{{{
{gb_to_unicode_body}
  }}}};

  // Unicode to GB18030 two-byte mapping
  // Sorted by Unicode codepoint for binary search during encoding
  struct unicode_to_gb_entry
  {{
    char32_t unicode;
    std::uint16_t gb;
  }};

  inline constexpr std::array<unicode_to_gb_entry, {len(unicode_to_gb_sorted)}> unicode_to_gb =
  {{{{
{unicode_to_gb_body}
  }}}};

  // Four-byte GB18030 ranges for Unicode mapping
  // These define the algorithmic mapping between 4-byte codes and Unicode
  // Format: (gb_offset, unicode_codepoint) - linear mapping starts from each entry
  struct four_byte_range
  {{
    std::uint32_t gb_offset;
    char32_t unicode_start;
  }};

  inline constexpr std::array<four_byte_range, {len(four_byte_ranges)}> four_byte_ranges =
  {{{{
{ranges_body}
  }}}};

}} // namespace peelo::unicode::encoding::gb18030::tables
"""


def main():
    try:
        # Download the data files
        index_content = download_file(GB18030_INDEX_URL)
        ranges_content = download_file(GB18030_RANGES_URL)

        # Parse the files
        two_byte_mappings = parse_index_file(index_content)
        four_byte_ranges = parse_ranges_file(ranges_content)

        print(f"// Parsed {len(two_byte_mappings)} two-byte mappings", file=sys.stderr)
        print(f"// Parsed {len(four_byte_ranges)} four-byte ranges", file=sys.stderr)

        # Generate the header
        header = generate_header(two_byte_mappings, four_byte_ranges)

        # Output to stdout
        print(header)

        print("// Done!", file=sys.stderr)

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
