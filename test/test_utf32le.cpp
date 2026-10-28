#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/utf32le.hpp>

TEST_CASE("utf32le encode")
{
  using peelo::unicode::encoding::utf32le::encode;

  REQUIRE(encode(U"$") == std::string("$\x00\x00\x00", 4));
  REQUIRE(encode(U"¢") == std::string("\xa2\x00\x00\x00", 4));
  REQUIRE(encode(U"€") == std::string("\xac \x00\x00", 4));
  REQUIRE(encode(U"𐍈") == std::string("H\x03\x01\x00", 4));
}

TEST_CASE("utf32le decode")
{
  using peelo::unicode::encoding::utf32le::decode;

  REQUIRE(decode("$\x00\x00\x00", 4) == U"$");
  REQUIRE(decode("\xa2\x00\x00\x00", 4) == U"¢");
  REQUIRE(decode("\xac \x00\x00", 4) == U"€");
  REQUIRE(decode("H\x03\x01\x00", 4) == U"𐍈");
}
