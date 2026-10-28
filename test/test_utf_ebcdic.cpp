#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/utf_ebcdic.hpp>

using namespace peelo::unicode::encoding;

static std::string
bytes(std::initializer_list<unsigned char> input)
{
  return std::string(input.begin(), input.end());
}

TEST_CASE("utf_ebcdic single byte")
{
  REQUIRE(utf_ebcdic::encode(U"A") == bytes({0xc1}));
  REQUIRE(utf_ebcdic::encode(U"0") == bytes({0xf0}));
  REQUIRE(utf_ebcdic::encode(U" ") == bytes({0x40}));
  REQUIRE(utf_ebcdic::encode(U"\u007f") == bytes({0x07}));
  REQUIRE(utf_ebcdic::encode(U"\u0080") == bytes({0x20}));
  REQUIRE(utf_ebcdic::encode(U"\u009f") == bytes({0xff}));

  REQUIRE(utf_ebcdic::decode(bytes({0xc1})) == U"A");
  REQUIRE(utf_ebcdic::decode(bytes({0xf0})) == U"0");
  REQUIRE(utf_ebcdic::decode(bytes({0x40})) == U" ");
}

TEST_CASE("utf_ebcdic bom")
{
  REQUIRE(utf_ebcdic::encode(U"\ufeff") == bytes({0xdd, 0x73, 0x66, 0x73}));
  REQUIRE(utf_ebcdic::decode(bytes({0xdd, 0x73, 0x66, 0x73})) == U"\ufeff");
}

TEST_CASE("utf_ebcdic multibyte")
{
  REQUIRE(utf_ebcdic::encode(U"\u00a0") == bytes({0x80, 0x41}));
  REQUIRE(utf_ebcdic::encode(U"\u00ff") == bytes({0x8b, 0x73}));
  REQUIRE(utf_ebcdic::encode(U"\u0400") == bytes({0xb8, 0x41, 0x41}));
  REQUIRE(utf_ebcdic::encode(U"\u4000") == bytes({0xdc, 0x57, 0x41, 0x41}));
  REQUIRE(utf_ebcdic::encode(U"\U00010000") == bytes({0xde, 0x41, 0x41, 0x41}));
  REQUIRE(
    utf_ebcdic::encode(U"\U0010fffd")
    == bytes({0xee, 0x42, 0x73, 0x73, 0x71})
  );

  REQUIRE(utf_ebcdic::decode(bytes({0x80, 0x41})) == U"\u00a0");
  REQUIRE(utf_ebcdic::decode(bytes({0xb8, 0x41, 0x41})) == U"\u0400");
  REQUIRE(utf_ebcdic::decode(bytes({0xde, 0x41, 0x41, 0x41})) == U"\U00010000");
  REQUIRE(
    utf_ebcdic::decode(bytes({0xee, 0x42, 0x73, 0x73, 0x71}))
    == U"\U0010fffd"
  );
}

TEST_CASE("utf_ebcdic roundtrip")
{
  const std::u32string input = U"Hello, \u4e16\u754c \U0001f600!";
  const auto encoded = utf_ebcdic::encode(input);
  const auto decoded = utf_ebcdic::decode(encoded);

  REQUIRE(decoded == input);
}

TEST_CASE("utf_ebcdic sequence_length")
{
  REQUIRE(utf_ebcdic::sequence_length(0xc1) == 1);
  REQUIRE(utf_ebcdic::sequence_length(0x40) == 1);
  REQUIRE(utf_ebcdic::sequence_length(0x80) == 2);
  REQUIRE(utf_ebcdic::sequence_length(0xb8) == 3);
  REQUIRE(utf_ebcdic::sequence_length(0xdc) == 4);
  REQUIRE(utf_ebcdic::sequence_length(0xdd) == 4);
  REQUIRE(utf_ebcdic::sequence_length(0xee) == 5);
  REQUIRE(utf_ebcdic::sequence_length(0x41) == 0);
}

TEST_CASE("utf_ebcdic codepoint_length")
{
  REQUIRE(utf_ebcdic::codepoint_length(0x0041) == 1);
  REQUIRE(utf_ebcdic::codepoint_length(0x009f) == 1);
  REQUIRE(utf_ebcdic::codepoint_length(0x00a0) == 2);
  REQUIRE(utf_ebcdic::codepoint_length(0x03ff) == 2);
  REQUIRE(utf_ebcdic::codepoint_length(0x0400) == 3);
  REQUIRE(utf_ebcdic::codepoint_length(0x3fff) == 3);
  REQUIRE(utf_ebcdic::codepoint_length(0x4000) == 4);
  REQUIRE(utf_ebcdic::codepoint_length(0x10000) == 4);
  REQUIRE(utf_ebcdic::codepoint_length(0x10fffd) == 5);
  REQUIRE(utf_ebcdic::codepoint_length(0xd800) == 0);
}

TEST_CASE("utf_ebcdic validate")
{
  std::string encoded;
  std::u32string decoded;

  REQUIRE(utf_ebcdic::encode_validate(U"\ufeff", encoded));
  REQUIRE(encoded == bytes({0xdd, 0x73, 0x66, 0x73}));

  decoded.clear();
  REQUIRE(utf_ebcdic::decode_validate(encoded, decoded));
  REQUIRE(decoded == U"\ufeff");

  decoded.clear();
  REQUIRE_FALSE(utf_ebcdic::decode_validate(bytes({0x41}), decoded));

  decoded.clear();
  REQUIRE_FALSE(utf_ebcdic::decode_validate(bytes({0xdd, 0x73}), decoded));
}
