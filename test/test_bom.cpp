#include <cassert>

#include <peelo/unicode/bom.hpp>

#if defined(_MSC_VER)
#  pragma warning( disable : 4100 )
#endif

using peelo::unicode::detect_bom;
using peelo::unicode::bom;

static void
test_recognized_bom(
  bom expected_type,
  const char* input,
  std::size_t length
)
{
  // This looks weird but in GitHub CI I get warnings about unused variables if
  // I do this in some other way.
  if (const auto result = detect_bom(input, length))
  {
    assert(*result == expected_type);
  } else {
    assert(false);
  }
}

static void
test_utf8()
{
  test_recognized_bom(bom::utf8, "\xef\xbb\xbf", 3);
}

static void
test_utf16_be()
{
  test_recognized_bom(bom::utf16_be, "\xfe\xff", 2);
}

static void
test_utf16_le()
{
  test_recognized_bom(bom::utf16_le, "\xff\xfe", 2);
}

static void
test_utf32_be()
{
  test_recognized_bom(bom::utf32_be, "\0\0\xfe\xff", 4);
}

static void
test_utf32_le()
{
  test_recognized_bom(bom::utf32_le, "\xff\xfe\0\0", 4);
}

static void
test_utf7()
{
  test_recognized_bom(bom::utf7, "\x2b\x2f\x76", 3);
}

static void
test_utf1()
{
  test_recognized_bom(bom::utf1, "\xf7\x64\x4c", 3);
}

static void
test_utf_ebcdic()
{
  test_recognized_bom(bom::utf_ebcdic, "\xdd\x73\x66\x73", 4);
}

static void
test_scsu()
{
  test_recognized_bom(bom::scsu, "\x0e\xfe\xff", 3);
}

static void
test_bocu_1()
{
  test_recognized_bom(bom::bocu_1, "\xfb\xee\x28", 3);
}

static void
test_gb18030()
{
  test_recognized_bom(bom::gb18030, "\x84\x31\x95\x33", 4);
}

static void
test_unrecognized_bom()
{
  assert(!detect_bom("", 0));
  assert(!detect_bom("a", 1));
  assert(!detect_bom("a\xef\xbb\xbf", 4));
  assert(!detect_bom("\x00\xbb\xbf\xef\xbb\xbf", 6));
}

static void
test_with_string()
{
  assert(!!detect_bom(std::string("\xef\xbb\xbf")));
  assert(!detect_bom(std::string("a")));
}

int
main()
{
  test_utf8();
  test_utf16_be();
  test_utf16_le();
  test_utf32_be();
  test_utf32_le();
  test_utf7();
  test_utf1();
  test_utf_ebcdic();
  test_scsu();
  test_bocu_1();
  test_gb18030();
  test_unrecognized_bom();
  test_with_string();
}
