#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/utf8.hpp>

TEST_CASE("utf8 encode")
{
  using peelo::unicode::encoding::utf8::encode;

  REQUIRE(encode(U"$") == "$");
  REQUIRE(encode(U"¢") == "\xc2\xa2");
  REQUIRE(encode(U"€") == "\xe2\x82\xac");
  REQUIRE(encode(U"𐍈") == "\xf0\x90\x8d\x88");
}

TEST_CASE("utf8 decode")
{
  using peelo::unicode::encoding::utf8::decode;

  REQUIRE(decode("$") == U"$");
  REQUIRE(decode("\xc2\xa2") == U"¢");
  REQUIRE(decode("\xe2\x82\xac") == U"€");
  REQUIRE(decode("\xf0\x90\x8d\x88") == U"𐍈");
}

TEST_CASE("utf8 sequence_length")
{
  using peelo::unicode::encoding::utf8::sequence_length;

  REQUIRE(sequence_length(36) == 1);
  REQUIRE(sequence_length(194) == 2);
  REQUIRE(sequence_length(226) == 3);
  REQUIRE(sequence_length(240) == 4);
}

TEST_CASE("utf8 codepoint_length")
{
  using peelo::unicode::encoding::utf8::codepoint_length;

  REQUIRE(codepoint_length(0x0024) == 1);
  REQUIRE(codepoint_length(0x00a2) == 2);
  REQUIRE(codepoint_length(0x20ac) == 3);
  REQUIRE(codepoint_length(0x10348) == 4);
}
