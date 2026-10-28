#include <cassert>
#include <iostream>
#include <string>

#include <peelo/unicode/encoding/utf1.hpp>

using namespace peelo::unicode::encoding;

static std::string
bytes(std::initializer_list<unsigned char> input)
{
  return std::string(input.begin(), input.end());
}

static void
test_encode_vectors()
{
  // Wikipedia / ISO 10646 Annex G reference vectors
  assert(utf1::encode(U"\u007f") == bytes({0x7f}));
  assert(utf1::encode(U"\u0080") == bytes({0x80}));
  assert(utf1::encode(U"\u009f") == bytes({0x9f}));
  assert(utf1::encode(U"\u00a0") == bytes({0xa0, 0xa0}));
  assert(utf1::encode(U"\u00bf") == bytes({0xa0, 0xbf}));
  assert(utf1::encode(U"\u00c0") == bytes({0xa0, 0xc0}));
  assert(utf1::encode(U"\u00ff") == bytes({0xa0, 0xff}));
  assert(utf1::encode(U"\u0100") == bytes({0xa1, 0x21}));
  assert(utf1::encode(U"\u015d") == bytes({0xa1, 0x7e}));
  assert(utf1::encode(U"\u015e") == bytes({0xa1, 0xa0}));
  assert(utf1::encode(U"\u01bd") == bytes({0xa1, 0xff}));
  assert(utf1::encode(U"\u01be") == bytes({0xa2, 0x21}));
  assert(utf1::encode(U"\u07ff") == bytes({0xaa, 0x72}));
  assert(utf1::encode(U"\u0800") == bytes({0xaa, 0x73}));
  assert(utf1::encode(U"\u0fff") == bytes({0xb5, 0x48}));
  assert(utf1::encode(U"\u1000") == bytes({0xb5, 0x49}));
  assert(utf1::encode(U"\u4015") == bytes({0xf5, 0xff}));
  assert(utf1::encode(U"\u4016") == bytes({0xf6, 0x21, 0x21}));
  assert(utf1::encode(U"\ud7ff") == bytes({0xf7, 0x2f, 0xc3}));
  assert(utf1::encode(U"\ue000") == bytes({0xf7, 0x3a, 0x79}));
  assert(utf1::encode(U"\uf8ff") == bytes({0xf7, 0x5c, 0x3c}));
  assert(utf1::encode(U"\ufeff") == bytes({0xf7, 0x64, 0x4c}));
  assert(utf1::encode(U"\ufffd") == bytes({0xf7, 0x65, 0xad}));
  assert(utf1::encode(U"\U00010000") == bytes({0xf7, 0x65, 0xb0}));
  assert(utf1::encode(U"\U00038e2d") == bytes({0xfb, 0xff, 0xff}));
  assert(utf1::encode(U"\U00038e2e") == bytes({0xfc, 0x21, 0x21, 0x21, 0x21}));
  assert(utf1::encode(U"\U00100000") == bytes({0xfc, 0x21, 0x37, 0xb2, 0x7b}));
  assert(utf1::encode(U"\U0010fffd") == bytes({0xfc, 0x21, 0x39, 0x6e, 0x6a}));
}

static void
test_decode_vectors()
{
  assert(utf1::decode(bytes({0x7f})) == U"\u007f");
  assert(utf1::decode(bytes({0x80})) == U"\u0080");
  assert(utf1::decode(bytes({0xa0, 0xa0})) == U"\u00a0");
  assert(utf1::decode(bytes({0xa1, 0x21})) == U"\u0100");
  assert(utf1::decode(bytes({0xf6, 0x21, 0x21})) == U"\u4016");
  assert(utf1::decode(bytes({0xf7, 0x64, 0x4c})) == U"\ufeff");
  assert(utf1::decode(bytes({0xf7, 0x65, 0xb0})) == U"\U00010000");
  assert(utf1::decode(bytes({0xfb, 0xff, 0xff})) == U"\U00038e2d");
  assert(utf1::decode(bytes({0xfc, 0x21, 0x21, 0x21, 0x21})) == U"\U00038e2e");
  assert(utf1::decode(bytes({0xfc, 0x21, 0x39, 0x6e, 0x6a})) == U"\U0010fffd");
}

static void
test_roundtrip()
{
  const std::u32string input = U"Hello, \u4e16\u754c \U0001f600!";
  const auto encoded = utf1::encode(input);
  const auto decoded = utf1::decode(encoded);

  assert(decoded == input);
}

static void
test_sequence_length()
{
  assert(utf1::sequence_length(0x41) == 1);
  assert(utf1::sequence_length(0x9f) == 1);
  assert(utf1::sequence_length(0xa0) == 2);
  assert(utf1::sequence_length(0xa1) == 2);
  assert(utf1::sequence_length(0xf5) == 2);
  assert(utf1::sequence_length(0xf6) == 3);
  assert(utf1::sequence_length(0xfb) == 3);
  assert(utf1::sequence_length(0xfc) == 5);
  assert(utf1::sequence_length(0xff) == 5);
}

static void
test_codepoint_length()
{
  assert(utf1::codepoint_length(0x007f) == 1);
  assert(utf1::codepoint_length(0x00a0) == 2);
  assert(utf1::codepoint_length(0x0100) == 2);
  assert(utf1::codepoint_length(0x4015) == 2);
  assert(utf1::codepoint_length(0x4016) == 3);
  assert(utf1::codepoint_length(0x38e2d) == 3);
  assert(utf1::codepoint_length(0x38e2e) == 5);
  assert(utf1::codepoint_length(0x10fffd) == 5);
  assert(utf1::codepoint_length(0x10ffff) == 0);
  assert(utf1::codepoint_length(0xd800) == 0);
}

static void
test_validate()
{
  std::string encoded;
  std::u32string decoded;

  assert(utf1::encode_validate(U"\ufeff", encoded));
  assert(encoded == bytes({0xf7, 0x64, 0x4c}));

  decoded.clear();
  assert(utf1::decode_validate(encoded, decoded));
  assert(decoded == U"\ufeff");

  decoded.clear();
  assert(!utf1::decode_validate(bytes({0xa0, 0x20}), decoded));

  decoded.clear();
  assert(!utf1::decode_validate(bytes({0xa1, 0x20}), decoded));
}

int
main()
{
  test_encode_vectors();
  test_decode_vectors();
  test_roundtrip();
  test_sequence_length();
  test_codepoint_length();
  test_validate();

  std::cout << "All UTF-1 tests passed!" << std::endl;

  return 0;
}
