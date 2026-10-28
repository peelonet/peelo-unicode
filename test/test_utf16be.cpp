#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/utf16be.hpp>

TEST_CASE("utf16be encode")
{
  using peelo::unicode::encoding::utf16be::encode;

  REQUIRE(encode(U"$") == std::string("\x00$", 2));
  REQUIRE(encode(U"¢") == std::string("\x00\xa2", 2));
  REQUIRE(encode(U"€") == std::string(" \xac", 2));
  REQUIRE(encode(U"𐍈") == std::string("\xd8\x00\xdfH", 4));
}

TEST_CASE("utf16be decode")
{
  using peelo::unicode::encoding::utf16be::decode;

  REQUIRE(decode(std::string("\x00$", 2)) == U"$");
  REQUIRE(decode(std::string("\x00\xa2", 2)) == U"¢");
  REQUIRE(decode(std::string(" \xac", 2)) == U"€");
  REQUIRE(decode(std::string("\xd8\x00\xdfH", 4)) == U"𐍈");
}
