#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/utf16le.hpp>

TEST_CASE("utf16le encode")
{
  using peelo::unicode::encoding::utf16le::encode;

  REQUIRE(encode(U"$") == std::string("$\x00", 2));
  REQUIRE(encode(U"¢") == std::string("\xa2\x00", 2));
  REQUIRE(encode(U"€") == std::string("\xac ", 2));
  REQUIRE(encode(U"𐍈") == std::string("\x00\xd8H\xdf", 4));
}

TEST_CASE("utf16le decode")
{
  using peelo::unicode::encoding::utf16le::decode;

  REQUIRE(decode(std::string("$\x00", 2)) == U"$");
  REQUIRE(decode(std::string("\xa2\x00", 2)) == U"¢");
  REQUIRE(decode(std::string("\xac ", 2)) == U"€");
  REQUIRE(decode(std::string("\x00\xd8H\xdf", 4)) == U"𐍈");
}
