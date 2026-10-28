#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/scsu.hpp>

using namespace peelo::unicode::encoding;

static std::string
bytes(std::initializer_list<unsigned char> input)
{
  return std::string(input.begin(), input.end());
}

TEST_CASE("scsu german")
{
  const auto encoded = scsu::encode(U"\u00d6l flie\u00dft");

  REQUIRE(encoded == bytes({0xd6, 0x6c, 0x20, 0x66, 0x6c, 0x69, 0x65, 0xdf, 0x74}));
  REQUIRE(scsu::decode(encoded) == U"\u00d6l flie\u00dft");
}

TEST_CASE("scsu russian")
{
  const auto encoded = scsu::encode(U"\u041c\u043e\u0441\u043a\u0432\u0430");

  REQUIRE(encoded == bytes({0x12, 0x9c, 0xbe, 0xc1, 0xba, 0xb2, 0xb0}));
  REQUIRE(scsu::decode(encoded) == U"\u041c\u043e\u0441\u043a\u0432\u0430");
}

TEST_CASE("scsu bom")
{
  REQUIRE(scsu::encode(U"\ufeff") == bytes({0x0e, 0xfe, 0xff}));
  REQUIRE(scsu::decode(bytes({0x0e, 0xfe, 0xff})) == U"\ufeff");
}

TEST_CASE("scsu ascii")
{
  REQUIRE(scsu::encode(U"Hello") == "Hello");
  REQUIRE(scsu::decode("Hello") == U"Hello");
}

TEST_CASE("scsu all features decode")
{
  const auto compressed = bytes({
    0x41, 0xdf, 0x12, 0x81, 0x03, 0x5f, 0x10, 0xdf, 0x1b, 0x03, 0xdf, 0x1c,
    0x88, 0x80, 0x0b, 0xbf, 0xff, 0xff, 0x0d, 0x0a, 0x41, 0x10, 0xdf, 0x12,
    0x81, 0x03, 0x5f, 0x10, 0xdf, 0x13, 0xdf, 0x14, 0x80, 0x15, 0xff
  });
  const auto decoded = scsu::decode(compressed);

  REQUIRE(decoded.length() == 18);
  REQUIRE(decoded[0] == U'\u0041');
  REQUIRE(decoded[1] == U'\u00df');
  REQUIRE(decoded[2] == U'\u0401');
  REQUIRE(decoded[3] == U'\u015f');
  REQUIRE(decoded[6] == U'\uf000');
  REQUIRE(decoded[7] == U'\U0010ffff');
  REQUIRE(decoded[8] == U'\r');
  REQUIRE(decoded[9] == U'\n');
  REQUIRE(decoded[17] == U'\U0010ffff');
}

TEST_CASE("scsu roundtrip")
{
  const std::u32string input = U"Hello, \u041c\u043e\u0441\u043a\u0432\u0430 \u30a2\u30c3\u30d7\u30eb!";
  const auto encoded = scsu::encode(input);
  const auto decoded = scsu::decode(encoded);

  REQUIRE(decoded == input);
}

TEST_CASE("scsu validate")
{
  std::string encoded;
  std::u32string decoded;

  REQUIRE(scsu::encode_validate(U"\ufeff", encoded));
  REQUIRE(encoded == bytes({0x0e, 0xfe, 0xff}));

  decoded.clear();
  REQUIRE(scsu::decode_validate(encoded, decoded));
  REQUIRE(decoded == U"\ufeff");

  decoded.clear();
  REQUIRE_FALSE(scsu::decode_validate(bytes({0x0c}), decoded));

  decoded.clear();
  REQUIRE_FALSE(scsu::decode_validate(bytes({0x0e, 0xfe}), decoded));
}
