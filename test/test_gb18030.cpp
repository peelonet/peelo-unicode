#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/gb18030.hpp>

using namespace peelo::unicode::encoding;

TEST_CASE("gb18030 ascii")
{
  const std::u32string input = U"Hello";
  const auto encoded = gb18030::encode(input);

  REQUIRE(encoded == "Hello");

  const auto decoded = gb18030::decode(encoded);
  REQUIRE(decoded == input);
}

TEST_CASE("gb18030 decode_validate")
{
  std::u32string output;
  REQUIRE(gb18030::decode_validate("Hello", output));
  REQUIRE(output == U"Hello");
}

TEST_CASE("gb18030 byte validation")
{
  REQUIRE(gb18030::is_first_byte(0x00));
  REQUIRE(gb18030::is_first_byte(0x7F));
  REQUIRE(gb18030::is_first_byte(0x81));
  REQUIRE(gb18030::is_first_byte(0xFE));
  REQUIRE_FALSE(gb18030::is_first_byte(0x80));
  REQUIRE_FALSE(gb18030::is_first_byte(0xFF));

  REQUIRE(gb18030::is_two_byte_second(0x40));
  REQUIRE(gb18030::is_two_byte_second(0x7E));
  REQUIRE(gb18030::is_two_byte_second(0x80));
  REQUIRE(gb18030::is_two_byte_second(0xFE));
  REQUIRE_FALSE(gb18030::is_two_byte_second(0x7F));

  REQUIRE(gb18030::is_four_byte_second_fourth(0x30));
  REQUIRE(gb18030::is_four_byte_second_fourth(0x39));
  REQUIRE_FALSE(gb18030::is_four_byte_second_fourth(0x2F));
  REQUIRE_FALSE(gb18030::is_four_byte_second_fourth(0x3A));
}

TEST_CASE("gb18030 two-byte chinese")
{
  const char gb_zhongwen[] = "\xD6\xD0\xCE\xC4";
  const auto decoded = gb18030::decode(gb_zhongwen, 4);
  REQUIRE(decoded.length() == 2);
  REQUIRE(decoded[0] == U'\u4E2D');
  REQUIRE(decoded[1] == U'\u6587');

  const std::u32string unicode_zhongwen = U"\u4E2D\u6587";
  const auto encoded = gb18030::encode(unicode_zhongwen);
  REQUIRE(encoded.length() == 4);
  REQUIRE(static_cast<unsigned char>(encoded[0]) == 0xD6);
  REQUIRE(static_cast<unsigned char>(encoded[1]) == 0xD0);
  REQUIRE(static_cast<unsigned char>(encoded[2]) == 0xCE);
  REQUIRE(static_cast<unsigned char>(encoded[3]) == 0xC4);
}

TEST_CASE("gb18030 four-byte encoding")
{
  const std::u32string input = U"\U00020000";
  const auto encoded = gb18030::encode(input);

  REQUIRE(encoded.length() == 4);

  REQUIRE(static_cast<unsigned char>(encoded[0]) >= 0x81);
  REQUIRE(static_cast<unsigned char>(encoded[0]) <= 0xFE);
  REQUIRE(static_cast<unsigned char>(encoded[1]) >= 0x30);
  REQUIRE(static_cast<unsigned char>(encoded[1]) <= 0x39);
  REQUIRE(static_cast<unsigned char>(encoded[2]) >= 0x81);
  REQUIRE(static_cast<unsigned char>(encoded[2]) <= 0xFE);
  REQUIRE(static_cast<unsigned char>(encoded[3]) >= 0x30);
  REQUIRE(static_cast<unsigned char>(encoded[3]) <= 0x39);

  const auto decoded = gb18030::decode(encoded);
  REQUIRE(decoded == input);
}

TEST_CASE("gb18030 mixed content")
{
  const std::u32string input = U"Hello\u4E2D\u6587World";
  const auto encoded = gb18030::encode(input);
  const auto decoded = gb18030::decode(encoded);

  REQUIRE(decoded == input);
}
