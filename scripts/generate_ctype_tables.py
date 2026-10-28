#!/usr/bin/env python3
"""
Regenerate peelo-unicode ctype lookup tables from a Unicode Character Database.

Usage:
    python3 generate_ctype_tables.py [/path/to/ucd]

If no path is given, downloads Unicode 18.0.0 UCD.zip into a temporary directory.

Property mappings (matching the existing peelo-unicode semantics):
    isalpha  – Alphabetic (DerivedCoreProperties)
    islower  – Lowercase
    isupper  – Uppercase
    isdigit  – General_Category=Nd
    isspace  – White_Space (PropList)
    isblank  – General_Category=Zs plus U+0009
    ispunct  – General_Category P* or S*
    isgraph  – assigned, excluding Cc, Cs, and Z*
    isprint  – assigned, excluding Cc, Cs, Zl, and Zp
    isemoji  – every range listed in emoji/emoji-data.txt (all properties)
    tolower  – Simple_Lowercase_Mapping for Uppercase characters (A–Z handled in C++)
    toupper  – Simple_Uppercase_Mapping for Lowercase characters that round-trip,
               plus reverse mappings from non-round-trip Uppercase forms (a–z in C++)
"""

from __future__ import annotations

import argparse
import re
import sys
import tempfile
import urllib.request
import zipfile
from pathlib import Path

UNICODE_VERSION = "18.0.0"
UCD_URL = f"https://www.unicode.org/Public/{UNICODE_VERSION}/ucd/UCD.zip"

ROOT = Path(__file__).resolve().parents[1]
CTYPE_DIR = ROOT / "include" / "peelo" / "unicode" / "ctype"

LICENSE = """\
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
"""


def download_ucd(dest: Path) -> Path:
    zip_path = dest / "UCD.zip"
    print(f"Downloading {UCD_URL} ...", file=sys.stderr)
    urllib.request.urlretrieve(UCD_URL, zip_path)
    with zipfile.ZipFile(zip_path) as zf:
        zf.extractall(dest)
    return dest


def parse_property_file(path: Path) -> dict[str, set[int]]:
    props: dict[str, set[int]] = {}
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        fields = [part.strip() for part in line.split(";")]
        cp_field, prop = fields[0], fields[1]
        if ".." in cp_field:
            start_s, end_s = cp_field.split("..")
            start, end = int(start_s, 16), int(end_s, 16)
        else:
            start = end = int(cp_field, 16)
        props.setdefault(prop, set()).update(range(start, end + 1))
    return props


def parse_property_ranges(path: Path) -> list[tuple[int, int]]:
    """Return every code point range from a UCD property file, in file order."""
    ranges: list[tuple[int, int]] = []
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        cp_field = line.split(";", 1)[0].strip()
        if ".." in cp_field:
            start_s, end_s = cp_field.split("..")
            ranges.append((int(start_s, 16), int(end_s, 16)))
        else:
            cp = int(cp_field, 16)
            ranges.append((cp, cp))
    return ranges


def parse_unicode_data(path: Path) -> dict[int, tuple[str, int | None, int | None]]:
    chars: dict[int, tuple[str, int | None, int | None]] = {}
    first: int | None = None
    first_gc: str | None = None
    for line in path.read_text(encoding="utf-8").splitlines():
        fields = line.split(";")
        cp = int(fields[0], 16)
        name, gc = fields[1], fields[2]
        upper = int(fields[12], 16) if fields[12] else None
        lower = int(fields[13], 16) if fields[13] else None
        if name.endswith(", First>"):
            first = cp
            first_gc = gc
            continue
        if name.endswith(", Last>"):
            assert first is not None and first_gc is not None
            for code in range(first, cp + 1):
                chars[code] = (first_gc, None, None)
            first = None
            continue
        chars[cp] = (gc, upper, lower)
    return chars


def set_to_ranges(codepoints: set[int]) -> list[tuple[int, int]]:
    if not codepoints:
        return []
    ordered = sorted(codepoints)
    ranges: list[tuple[int, int]] = []
    start = prev = ordered[0]
    for cp in ordered[1:]:
        if cp == prev + 1:
            prev = cp
            continue
        ranges.append((start, prev))
        start = prev = cp
    ranges.append((start, prev))
    return ranges


def format_ranges(ranges: list[tuple[int, int]], per_line: int = 3) -> str:
    entries = [f"{{ 0x{a:04x}, 0x{b:04x}, }}" for a, b in ranges]
    lines = []
    for i in range(0, len(entries), per_line):
        lines.append("      " + ", ".join(entries[i : i + per_line]) + ",")
    return "\n".join(lines)


def format_case_map(mapping: dict[int, int], per_line: int = 3) -> str:
    # Stable descending key order matches the historical table style.
    items = sorted(mapping.items(), key=lambda item: item[0], reverse=True)
    entries = [f"{{ {src}, {dst} }}" for src, dst in items]
    lines = []
    for i in range(0, len(entries), per_line):
        lines.append("      " + ", ".join(entries[i : i + per_line]) + ",")
    return "\n".join(lines)


def write_range_header(
    filename: str,
    function: str,
    brief: str,
    table_name: str,
    ranges: list[tuple[int, int]],
    extra_comment: str | None = None,
) -> None:
    comment = f"    // {extra_comment}\n" if extra_comment else ""
    content = f"""{LICENSE}#pragma once

#include <peelo/unicode/ctype/_utils.hpp>

namespace peelo::unicode::ctype
{{
  /**
   * {brief}
   */
  inline bool
  {function}(char32_t c)
  {{
{comment}    static const std::array<utils::range, {len(ranges)}> {table_name} =
    {{{{
{format_ranges(ranges)}
    }}}};

    return utils::table_lookup({table_name}, c);
  }}
}}
"""
    (CTYPE_DIR / filename).write_text(content, encoding="utf-8")
    print(f"Wrote {filename} ({len(ranges)} ranges)", file=sys.stderr)


def write_tolower(mapping: dict[int, int]) -> None:
    content = f"""{LICENSE}#pragma once

#include <peelo/unicode/ctype/_utils.hpp>

namespace peelo::unicode::ctype
{{
  /**
   * Converts given Unicode code point into lowercase.
   */
  inline char32_t
  tolower(char32_t c)
  {{
    static const std::unordered_map<char32_t, char32_t> tolower_map =
    {{{{
{format_case_map(mapping)}
    }}}};

    if (c >= 'A' && c <= 'Z')
    {{
      return c + 32;
    }}

    return utils::case_lookup(tolower_map, c);
  }}
}}
"""
    (CTYPE_DIR / "tolower.hpp").write_text(content, encoding="utf-8")
    print(f"Wrote tolower.hpp ({len(mapping)} mappings)", file=sys.stderr)


def write_toupper(mapping: dict[int, int]) -> None:
    content = f"""{LICENSE}#pragma once

#include <peelo/unicode/ctype/_utils.hpp>

namespace peelo::unicode::ctype
{{
  /**
   * Converts given Unicode code point into uppercase.
   */
  inline char32_t
  toupper(char32_t c)
  {{
    static const std::unordered_map<char32_t, char32_t> toupper_map =
    {{{{
{format_case_map(mapping)}
    }}}};

    if (c >= 'a' && c <= 'z')
    {{
      return c - 32;
    }}

    return utils::case_lookup(toupper_map, c);
  }}
}}
"""
    (CTYPE_DIR / "toupper.hpp").write_text(content, encoding="utf-8")
    print(f"Wrote toupper.hpp ({len(mapping)} mappings)", file=sys.stderr)


def build_tolower(ud, uppercase: set[int]) -> dict[int, int]:
    mapping: dict[int, int] = {}
    for cp in uppercase:
        if 0x41 <= cp <= 0x5A:
            continue
        entry = ud.get(cp)
        if entry is None or entry[2] is None:
            continue
        mapping[cp] = entry[2]
    return mapping


def build_toupper(ud, lowercase: set[int], uppercase: set[int]) -> dict[int, int]:
    """
    Round-tripping simple uppercase mappings for Lowercase characters, plus
    reverse mappings from Uppercase characters whose simple lowercase target
    does not round-trip (e.g. İ, K, ẞ).
    """
    mapping: dict[int, int] = {}
    for cp in lowercase:
        if 0x61 <= cp <= 0x7A:
            continue
        entry = ud.get(cp)
        if entry is None or entry[1] is None:
            continue
        upper = entry[1]
        back = ud.get(upper)
        if back is not None and back[2] == cp:
            mapping[cp] = upper

    for cp in uppercase:
        if 0x41 <= cp <= 0x5A:
            continue
        entry = ud.get(cp)
        if entry is None or entry[2] is None:
            continue
        lower = entry[2]
        lower_entry = ud.get(lower)
        if lower_entry is None or lower_entry[1] != cp:
            mapping[lower] = cp
    return mapping


def update_utils_copyright() -> None:
    path = CTYPE_DIR / "_utils.hpp"
    text = path.read_text(encoding="utf-8")
    text = re.sub(
        r"Copyright \(c\) 2018-\d+, peelo\.net",
        "Copyright (c) 2018-2026, peelo.net",
        text,
        count=1,
    )
    path.write_text(text, encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "ucd",
        nargs="?",
        type=Path,
        help="Path to an extracted UCD directory",
    )
    args = parser.parse_args()

    tmp: tempfile.TemporaryDirectory[str] | None = None
    if args.ucd is not None:
        ucd = args.ucd
    else:
        tmp = tempfile.TemporaryDirectory(prefix="peelo-ucd-")
        ucd = download_ucd(Path(tmp.name))

    derived = parse_property_file(ucd / "DerivedCoreProperties.txt")
    proplist = parse_property_file(ucd / "PropList.txt")
    ud = parse_unicode_data(ucd / "UnicodeData.txt")
    emoji_ranges = parse_property_ranges(ucd / "emoji" / "emoji-data.txt")

    assigned = set(ud)
    cc = {cp for cp, (gc, *_) in ud.items() if gc == "Cc"}
    cs = {cp for cp, (gc, *_) in ud.items() if gc == "Cs"}
    z = {cp for cp, (gc, *_) in ud.items() if gc.startswith("Z")}
    zl = {cp for cp, (gc, *_) in ud.items() if gc == "Zl"}
    zp = {cp for cp, (gc, *_) in ud.items() if gc == "Zp"}
    zs = {cp for cp, (gc, *_) in ud.items() if gc == "Zs"}
    nd = {cp for cp, (gc, *_) in ud.items() if gc == "Nd"}
    punct = {cp for cp, (gc, *_) in ud.items() if gc.startswith(("P", "S"))}

    write_range_header(
        "isalpha.hpp",
        "isalpha",
        "Determines whether the given Unicode code point is alphabetic.",
        "alpha_table",
        set_to_ranges(derived["Alphabetic"]),
    )
    write_range_header(
        "islower.hpp",
        "islower",
        "Determines whether the given Unicode code point is a lowercase character.",
        "lower_table",
        set_to_ranges(derived["Lowercase"]),
    )
    write_range_header(
        "isupper.hpp",
        "isupper",
        "Determines whether the given Unicode code point is an uppercase character.",
        "upper_table",
        set_to_ranges(derived["Uppercase"]),
    )
    write_range_header(
        "isdigit.hpp",
        "isdigit",
        "Determines whether the given Unicode code point is a digit.",
        "digit_table",
        set_to_ranges(nd),
    )
    write_range_header(
        "isspace.hpp",
        "isspace",
        "Determines whether the given Unicode code point is a space character.",
        "space_table",
        set_to_ranges(proplist["White_Space"]),
    )
    write_range_header(
        "isblank.hpp",
        "isblank",
        "Determines whether the given Unicode code point is a blank character.",
        "blank_table",
        set_to_ranges(zs | {0x0009}),
    )
    write_range_header(
        "ispunct.hpp",
        "ispunct",
        "Determines whether the given Unicode code point is a punctuation character.",
        "punct_table",
        set_to_ranges(punct),
    )
    write_range_header(
        "isgraph.hpp",
        "isgraph",
        "Determines whether the given Unicode code point is a graphical character.",
        "graph_table",
        set_to_ranges(assigned - cc - cs - z),
    )
    write_range_header(
        "isprint.hpp",
        "isprint",
        "Determines whether the given Unicode code point is a printing character.",
        "print_table",
        set_to_ranges(assigned - cc - cs - zl - zp),
    )
    write_range_header(
        "isemoji.hpp",
        "isemoji",
        "Determines whether the given Unicode character is emoji or not.",
        "emoji_table",
        emoji_ranges,
        extra_comment=(
            f"https://unicode.org/Public/{UNICODE_VERSION}/ucd/emoji/emoji-data.txt"
        ),
    )

    # isemoji uses static inline in the existing header; preserve that.
    emoji_path = CTYPE_DIR / "isemoji.hpp"
    emoji_text = emoji_path.read_text(encoding="utf-8")
    emoji_text = emoji_text.replace(
        "  inline bool\n  isemoji(char32_t c)",
        "  static inline bool\n  isemoji(char32_t c)",
        1,
    )
    emoji_path.write_text(emoji_text, encoding="utf-8")

    write_tolower(build_tolower(ud, derived["Uppercase"]))
    write_toupper(build_toupper(ud, derived["Lowercase"], derived["Uppercase"]))
    update_utils_copyright()

    # Keep iscntrl / isalnum / isvalid / isxdigit as hand-written; only bump years.
    for path in [
        CTYPE_DIR / "iscntrl.hpp",
        CTYPE_DIR / "isalnum.hpp",
        CTYPE_DIR / "isvalid.hpp",
        CTYPE_DIR / "isxdigit.hpp",
        ROOT / "include" / "peelo" / "unicode" / "ctype.hpp",
    ]:
        text = path.read_text(encoding="utf-8")
        text = re.sub(
            r"Copyright \(c\) 2018-\d+, peelo\.net",
            "Copyright (c) 2018-2026, peelo.net",
            text,
            count=1,
        )
        path.write_text(text, encoding="utf-8")

    print(f"Generated ctype tables for Unicode {UNICODE_VERSION}", file=sys.stderr)
    if tmp is not None:
        tmp.cleanup()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
