#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/utf32be.hpp>

TEST_CASE("utf32be encode")
{
  using peelo::unicode::encoding::utf32be::encode;

  REQUIRE(encode(U"$") == std::string("\x00\x00\x00$", 4));
  REQUIRE(encode(U"¢") == std::string("\x00\x00\x00\xa2", 4));
  REQUIRE(encode(U"€") == std::string("\x00\x00 \xac", 4));
  REQUIRE(encode(U"𐍈") == std::string("\x00\x01\x03H", 4));
}

TEST_CASE("utf32be decode")
{
  using peelo::unicode::encoding::utf32be::decode;

  REQUIRE(decode(std::string("\x00\x00\x00$", 4)) == U"$");
  REQUIRE(decode(std::string("\x00\x00\x00\xa2", 4)) == U"¢");
  REQUIRE(decode(std::string("\x00\x00 \xac", 4)) == U"€");
  REQUIRE(decode(std::string("\x00\x01\x03H", 4)) == U"𐍈");
}
