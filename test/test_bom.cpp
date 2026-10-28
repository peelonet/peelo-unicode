#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/bom.hpp>

using namespace peelo::unicode::bom;

static void
require_recognized_bom(
  type expected_type,
  const char* input,
  std::size_t length
)
{
  const auto result = detect(input, length);

  REQUIRE(result);
  REQUIRE(*result == expected_type);
}

static void
require_codec_roundtrip(type bom_type, const std::u32string& text)
{
  const auto bom = mark(bom_type);
  const auto c = codec_for(bom_type);
  const auto encoded = c.encode(text.c_str(), text.length());
  auto with_bom = std::string(bom);
  std::string validated;
  std::u32string decoded_validated;

  with_bom.append(encoded);

  REQUIRE(detect(with_bom) == bom_type);
  REQUIRE(c.decode(encoded.c_str(), encoded.length()) == text);
  REQUIRE(c.encode_validate(text.c_str(), text.length(), validated));
  REQUIRE(validated == encoded);
  REQUIRE(
    c.decode_validate(
      encoded.c_str(),
      encoded.length(),
      decoded_validated
    )
  );
  REQUIRE(decoded_validated == text);

  const auto payload = c.decode(
    with_bom.c_str() + bom.size(),
    with_bom.length() - bom.size()
  );

  REQUIRE(payload == text);
}

TEST_CASE("bom detect utf8")
{
  require_recognized_bom(type::utf8, "\xef\xbb\xbf", 3);
}

TEST_CASE("bom detect utf16_be")
{
  require_recognized_bom(type::utf16_be, "\xfe\xff", 2);
}

TEST_CASE("bom detect utf16_le")
{
  require_recognized_bom(type::utf16_le, "\xff\xfe", 2);
}

TEST_CASE("bom detect utf32_be")
{
  require_recognized_bom(type::utf32_be, "\0\0\xfe\xff", 4);
}

TEST_CASE("bom detect utf32_le")
{
  require_recognized_bom(type::utf32_le, "\xff\xfe\0\0", 4);
}

TEST_CASE("bom detect utf7")
{
  require_recognized_bom(type::utf7, "\x2b\x2f\x76", 3);
}

TEST_CASE("bom detect utf1")
{
  require_recognized_bom(type::utf1, "\xf7\x64\x4c", 3);
}

TEST_CASE("bom detect utf_ebcdic")
{
  require_recognized_bom(type::utf_ebcdic, "\xdd\x73\x66\x73", 4);
}

TEST_CASE("bom detect scsu")
{
  require_recognized_bom(type::scsu, "\x0e\xfe\xff", 3);
}

TEST_CASE("bom detect bocu_1")
{
  require_recognized_bom(type::bocu_1, "\xfb\xee\x28", 3);
}

TEST_CASE("bom detect gb18030")
{
  require_recognized_bom(type::gb18030, "\x84\x31\x95\x33", 4);
}

TEST_CASE("bom unrecognized")
{
  REQUIRE_FALSE(detect("", 0));
  REQUIRE_FALSE(detect("a", 1));
  REQUIRE_FALSE(detect("a\xef\xbb\xbf", 4));
  REQUIRE_FALSE(detect("\x00\xbb\xbf\xef\xbb\xbf", 6));
}

TEST_CASE("bom detect with string")
{
  REQUIRE(detect(std::string("\xef\xbb\xbf")));
  REQUIRE_FALSE(detect(std::string("a")));
}

TEST_CASE("bom mark")
{
  REQUIRE(mark(type::utf8) == std::string_view("\xef\xbb\xbf", 3));
  REQUIRE(mark(type::utf16_be) == std::string_view("\xfe\xff", 2));
  REQUIRE(mark(type::utf16_le) == std::string_view("\xff\xfe", 2));
  REQUIRE(mark(type::utf32_be) == std::string_view("\0\0\xfe\xff", 4));
  REQUIRE(mark(type::utf32_le) == std::string_view("\xff\xfe\0\0", 4));
  REQUIRE(mark(type::utf7) == std::string_view("\x2b\x2f\x76", 3));
  REQUIRE(mark(type::utf1) == std::string_view("\xf7\x64\x4c", 3));
  REQUIRE(mark(type::utf_ebcdic) == std::string_view("\xdd\x73\x66\x73", 4));
  REQUIRE(mark(type::scsu) == std::string_view("\x0e\xfe\xff", 3));
  REQUIRE(mark(type::bocu_1) == std::string_view("\xfb\xee\x28", 3));
  REQUIRE(mark(type::gb18030) == std::string_view("\x84\x31\x95\x33", 4));
}

TEST_CASE("bom codec_for roundtrip")
{
  const std::u32string text = U"Hello, \u20ac!";

  require_codec_roundtrip(type::utf8, text);
  require_codec_roundtrip(type::utf16_be, text);
  require_codec_roundtrip(type::utf16_le, text);
  require_codec_roundtrip(type::utf32_be, text);
  require_codec_roundtrip(type::utf32_le, text);
  require_codec_roundtrip(type::utf7, text);
  require_codec_roundtrip(type::utf1, text);
  require_codec_roundtrip(type::utf_ebcdic, text);
  require_codec_roundtrip(type::scsu, text);
  require_codec_roundtrip(type::bocu_1, text);
  require_codec_roundtrip(type::gb18030, text);
}
