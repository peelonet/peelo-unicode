#include <cassert>
#include <string>

#include <peelo/unicode/bom.hpp>

#if defined(_MSC_VER)
#  pragma warning( disable : 4100 )
#endif

using namespace peelo::unicode::bom;

static void
test_recognized_bom(
  type expected_type,
  const char* input,
  std::size_t length
)
{
  // This looks weird but in GitHub CI I get warnings about unused variables if
  // I do this in some other way.
  if (const auto result = detect(input, length))
  {
    assert(*result == expected_type);
  } else {
    assert(false);
  }
}

static void
test_codec_roundtrip(type bom_type, const std::u32string& text)
{
  const auto bom = mark(bom_type);
  const auto c = codec_for(bom_type);
  const auto encoded = c.encode(text.c_str(), text.length());
  auto with_bom = std::string(bom);
  std::string validated;
  std::u32string decoded_validated;

  with_bom.append(encoded);

  assert(detect(with_bom) == bom_type);
  assert(c.decode(encoded.c_str(), encoded.length()) == text);
  assert(c.encode_validate(text.c_str(), text.length(), validated));
  assert(validated == encoded);
  assert(
    c.decode_validate(
      encoded.c_str(),
      encoded.length(),
      decoded_validated
    )
  );
  assert(decoded_validated == text);

  // Decode payload after skipping the detected BOM.
  const auto payload = c.decode(
    with_bom.c_str() + bom.size(),
    with_bom.length() - bom.size()
  );

  assert(payload == text);
}

static void
test_utf8()
{
  test_recognized_bom(type::utf8, "\xef\xbb\xbf", 3);
}

static void
test_utf16_be()
{
  test_recognized_bom(type::utf16_be, "\xfe\xff", 2);
}

static void
test_utf16_le()
{
  test_recognized_bom(type::utf16_le, "\xff\xfe", 2);
}

static void
test_utf32_be()
{
  test_recognized_bom(type::utf32_be, "\0\0\xfe\xff", 4);
}

static void
test_utf32_le()
{
  test_recognized_bom(type::utf32_le, "\xff\xfe\0\0", 4);
}

static void
test_utf7()
{
  test_recognized_bom(type::utf7, "\x2b\x2f\x76", 3);
}

static void
test_utf1()
{
  test_recognized_bom(type::utf1, "\xf7\x64\x4c", 3);
}

static void
test_utf_ebcdic()
{
  test_recognized_bom(type::utf_ebcdic, "\xdd\x73\x66\x73", 4);
}

static void
test_scsu()
{
  test_recognized_bom(type::scsu, "\x0e\xfe\xff", 3);
}

static void
test_bocu_1()
{
  test_recognized_bom(type::bocu_1, "\xfb\xee\x28", 3);
}

static void
test_gb18030()
{
  test_recognized_bom(type::gb18030, "\x84\x31\x95\x33", 4);
}

static void
test_unrecognized_bom()
{
  assert(!detect("", 0));
  assert(!detect("a", 1));
  assert(!detect("a\xef\xbb\xbf", 4));
  assert(!detect("\x00\xbb\xbf\xef\xbb\xbf", 6));
}

static void
test_with_string()
{
  assert(!!detect(std::string("\xef\xbb\xbf")));
  assert(!detect(std::string("a")));
}

static void
test_mark()
{
  assert(mark(type::utf8) == std::string_view("\xef\xbb\xbf", 3));
  assert(mark(type::utf16_be) == std::string_view("\xfe\xff", 2));
  assert(mark(type::utf16_le) == std::string_view("\xff\xfe", 2));
  assert(mark(type::utf32_be) == std::string_view("\0\0\xfe\xff", 4));
  assert(mark(type::utf32_le) == std::string_view("\xff\xfe\0\0", 4));
  assert(mark(type::utf7) == std::string_view("\x2b\x2f\x76", 3));
  assert(mark(type::utf1) == std::string_view("\xf7\x64\x4c", 3));
  assert(mark(type::utf_ebcdic) == std::string_view("\xdd\x73\x66\x73", 4));
  assert(mark(type::scsu) == std::string_view("\x0e\xfe\xff", 3));
  assert(mark(type::bocu_1) == std::string_view("\xfb\xee\x28", 3));
  assert(mark(type::gb18030) == std::string_view("\x84\x31\x95\x33", 4));
}

static void
test_codec_for()
{
  const std::u32string text = U"Hello, \u20ac!";

  test_codec_roundtrip(type::utf8, text);
  test_codec_roundtrip(type::utf16_be, text);
  test_codec_roundtrip(type::utf16_le, text);
  test_codec_roundtrip(type::utf32_be, text);
  test_codec_roundtrip(type::utf32_le, text);
  test_codec_roundtrip(type::utf7, text);
  test_codec_roundtrip(type::utf1, text);
  test_codec_roundtrip(type::utf_ebcdic, text);
  test_codec_roundtrip(type::scsu, text);
  test_codec_roundtrip(type::bocu_1, text);
  test_codec_roundtrip(type::gb18030, text);
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
  test_mark();
  test_codec_for();
}
