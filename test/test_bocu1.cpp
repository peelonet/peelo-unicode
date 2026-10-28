#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/bocu1.hpp>

using namespace peelo::unicode::encoding;

static std::string
bytes(std::initializer_list<unsigned char> input)
{
  return std::string(input.begin(), input.end());
}

TEST_CASE("bocu1 bom")
{
  REQUIRE(bocu1::encode(U"\ufeff") == bytes({0xfb, 0xee, 0x28}));
  REQUIRE(bocu1::decode(bytes({0xfb, 0xee, 0x28})) == U"\ufeff");
}

TEST_CASE("bocu1 ascii and controls")
{
  REQUIRE(bocu1::encode(U"ABC") == bytes({0x91, 0x92, 0x93}));
  REQUIRE(bocu1::decode(bytes({0x91, 0x92, 0x93})) == U"ABC");

  REQUIRE(bocu1::encode(U" A") == bytes({0x20, 0x91}));
  REQUIRE(bocu1::encode(U"\nA") == bytes({0x0a, 0x91}));
  REQUIRE(bocu1::decode(bytes({0x0a, 0x91})) == U"\nA");
}

TEST_CASE("bocu1 scripts")
{
  REQUIRE(bocu1::encode(U"\u4e2d\u6587") == bytes({0xfb, 0x33, 0xd7, 0x3d, 0xcc}));
  REQUIRE(bocu1::decode(bytes({0xfb, 0x33, 0xd7, 0x3d, 0xcc})) == U"\u4e2d\u6587");

  REQUIRE(
    bocu1::encode(U"\u3042\u3044\u3046")
    == bytes({0xfb, 0x11, 0x59, 0x64, 0x66})
  );
  REQUIRE(
    bocu1::decode(bytes({0xfb, 0x11, 0x59, 0x64, 0x66}))
    == U"\u3042\u3044\u3046"
  );

  REQUIRE(
    bocu1::encode(U"\uac00\uac01")
    == bytes({0xfb, 0x96, 0xb1, 0x39, 0x52})
  );
}

TEST_CASE("bocu1 roundtrip")
{
  const std::u32string input =
    U"Hello, world!\n\u4e2d\u6587 \u3042\u3044\u30a2 \uac00\U0001f600";
  const auto encoded = bocu1::encode(input);
  const auto decoded = bocu1::decode(encoded);

  REQUIRE(decoded == input);
}

TEST_CASE("bocu1 reset byte")
{
  const auto decoded = bocu1::decode(bytes({0x91, 0xff, 0x91}));

  REQUIRE(decoded == U"AA");
}

TEST_CASE("bocu1 validate")
{
  std::string encoded;
  std::u32string decoded;

  REQUIRE(bocu1::encode_validate(U"\ufeff", encoded));
  REQUIRE(encoded == bytes({0xfb, 0xee, 0x28}));

  decoded.clear();
  REQUIRE(bocu1::decode_validate(encoded, decoded));
  REQUIRE(decoded == U"\ufeff");

  decoded.clear();
  REQUIRE_FALSE(bocu1::decode_validate(bytes({0xd0, 0x00}), decoded));

  decoded.clear();
  REQUIRE_FALSE(bocu1::decode_validate(bytes({0xfb}), decoded));
}
