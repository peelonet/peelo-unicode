#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/utf1.hpp>

using namespace peelo::unicode::encoding;

static std::string
bytes(std::initializer_list<unsigned char> input)
{
  return std::string(input.begin(), input.end());
}

TEST_CASE("utf1 encode vectors")
{
  REQUIRE(utf1::encode(U"\u007f") == bytes({0x7f}));
  REQUIRE(utf1::encode(U"\u0080") == bytes({0x80}));
  REQUIRE(utf1::encode(U"\u009f") == bytes({0x9f}));
  REQUIRE(utf1::encode(U"\u00a0") == bytes({0xa0, 0xa0}));
  REQUIRE(utf1::encode(U"\u00bf") == bytes({0xa0, 0xbf}));
  REQUIRE(utf1::encode(U"\u00c0") == bytes({0xa0, 0xc0}));
  REQUIRE(utf1::encode(U"\u00ff") == bytes({0xa0, 0xff}));
  REQUIRE(utf1::encode(U"\u0100") == bytes({0xa1, 0x21}));
  REQUIRE(utf1::encode(U"\u015d") == bytes({0xa1, 0x7e}));
  REQUIRE(utf1::encode(U"\u015e") == bytes({0xa1, 0xa0}));
  REQUIRE(utf1::encode(U"\u01bd") == bytes({0xa1, 0xff}));
  REQUIRE(utf1::encode(U"\u01be") == bytes({0xa2, 0x21}));
  REQUIRE(utf1::encode(U"\u07ff") == bytes({0xaa, 0x72}));
  REQUIRE(utf1::encode(U"\u0800") == bytes({0xaa, 0x73}));
  REQUIRE(utf1::encode(U"\u0fff") == bytes({0xb5, 0x48}));
  REQUIRE(utf1::encode(U"\u1000") == bytes({0xb5, 0x49}));
  REQUIRE(utf1::encode(U"\u4015") == bytes({0xf5, 0xff}));
  REQUIRE(utf1::encode(U"\u4016") == bytes({0xf6, 0x21, 0x21}));
  REQUIRE(utf1::encode(U"\ud7ff") == bytes({0xf7, 0x2f, 0xc3}));
  REQUIRE(utf1::encode(U"\ue000") == bytes({0xf7, 0x3a, 0x79}));
  REQUIRE(utf1::encode(U"\uf8ff") == bytes({0xf7, 0x5c, 0x3c}));
  REQUIRE(utf1::encode(U"\ufeff") == bytes({0xf7, 0x64, 0x4c}));
  REQUIRE(utf1::encode(U"\ufffd") == bytes({0xf7, 0x65, 0xad}));
  REQUIRE(utf1::encode(U"\U00010000") == bytes({0xf7, 0x65, 0xb0}));
  REQUIRE(utf1::encode(U"\U00038e2d") == bytes({0xfb, 0xff, 0xff}));
  REQUIRE(utf1::encode(U"\U00038e2e") == bytes({0xfc, 0x21, 0x21, 0x21, 0x21}));
  REQUIRE(utf1::encode(U"\U00100000") == bytes({0xfc, 0x21, 0x37, 0xb2, 0x7b}));
  REQUIRE(utf1::encode(U"\U0010fffd") == bytes({0xfc, 0x21, 0x39, 0x6e, 0x6a}));
}

TEST_CASE("utf1 decode vectors")
{
  REQUIRE(utf1::decode(bytes({0x7f})) == U"\u007f");
  REQUIRE(utf1::decode(bytes({0x80})) == U"\u0080");
  REQUIRE(utf1::decode(bytes({0xa0, 0xa0})) == U"\u00a0");
  REQUIRE(utf1::decode(bytes({0xa1, 0x21})) == U"\u0100");
  REQUIRE(utf1::decode(bytes({0xf6, 0x21, 0x21})) == U"\u4016");
  REQUIRE(utf1::decode(bytes({0xf7, 0x64, 0x4c})) == U"\ufeff");
  REQUIRE(utf1::decode(bytes({0xf7, 0x65, 0xb0})) == U"\U00010000");
  REQUIRE(utf1::decode(bytes({0xfb, 0xff, 0xff})) == U"\U00038e2d");
  REQUIRE(utf1::decode(bytes({0xfc, 0x21, 0x21, 0x21, 0x21})) == U"\U00038e2e");
  REQUIRE(utf1::decode(bytes({0xfc, 0x21, 0x39, 0x6e, 0x6a})) == U"\U0010fffd");
}

TEST_CASE("utf1 roundtrip")
{
  const std::u32string input = U"Hello, \u4e16\u754c \U0001f600!";
  const auto encoded = utf1::encode(input);
  const auto decoded = utf1::decode(encoded);

  REQUIRE(decoded == input);
}

TEST_CASE("utf1 sequence_length")
{
  REQUIRE(utf1::sequence_length(0x41) == 1);
  REQUIRE(utf1::sequence_length(0x9f) == 1);
  REQUIRE(utf1::sequence_length(0xa0) == 2);
  REQUIRE(utf1::sequence_length(0xa1) == 2);
  REQUIRE(utf1::sequence_length(0xf5) == 2);
  REQUIRE(utf1::sequence_length(0xf6) == 3);
  REQUIRE(utf1::sequence_length(0xfb) == 3);
  REQUIRE(utf1::sequence_length(0xfc) == 5);
  REQUIRE(utf1::sequence_length(0xff) == 5);
}

TEST_CASE("utf1 codepoint_length")
{
  REQUIRE(utf1::codepoint_length(0x007f) == 1);
  REQUIRE(utf1::codepoint_length(0x00a0) == 2);
  REQUIRE(utf1::codepoint_length(0x0100) == 2);
  REQUIRE(utf1::codepoint_length(0x4015) == 2);
  REQUIRE(utf1::codepoint_length(0x4016) == 3);
  REQUIRE(utf1::codepoint_length(0x38e2d) == 3);
  REQUIRE(utf1::codepoint_length(0x38e2e) == 5);
  REQUIRE(utf1::codepoint_length(0x10fffd) == 5);
  REQUIRE(utf1::codepoint_length(0x10ffff) == 0);
  REQUIRE(utf1::codepoint_length(0xd800) == 0);
}

TEST_CASE("utf1 validate")
{
  std::string encoded;
  std::u32string decoded;

  REQUIRE(utf1::encode_validate(U"\ufeff", encoded));
  REQUIRE(encoded == bytes({0xf7, 0x64, 0x4c}));

  decoded.clear();
  REQUIRE(utf1::decode_validate(encoded, decoded));
  REQUIRE(decoded == U"\ufeff");

  decoded.clear();
  REQUIRE_FALSE(utf1::decode_validate(bytes({0xa0, 0x20}), decoded));

  decoded.clear();
  REQUIRE_FALSE(utf1::decode_validate(bytes({0xa1, 0x20}), decoded));
}
