#include <cassert>
#include <iostream>
#include <string>

#include <peelo/unicode/encoding/scsu.hpp>

using namespace peelo::unicode::encoding;

static std::string
bytes(std::initializer_list<unsigned char> input)
{
  return std::string(input.begin(), input.end());
}

static void
test_german()
{
  // UTS #6 §9.1
  const auto encoded = scsu::encode(U"\u00d6l flie\u00dft");

  assert(encoded == bytes({0xd6, 0x6c, 0x20, 0x66, 0x6c, 0x69, 0x65, 0xdf, 0x74}));
  assert(scsu::decode(encoded) == U"\u00d6l flie\u00dft");
}

static void
test_russian()
{
  // UTS #6 §9.2
  const auto encoded = scsu::encode(U"\u041c\u043e\u0441\u043a\u0432\u0430");

  assert(encoded == bytes({0x12, 0x9c, 0xbe, 0xc1, 0xba, 0xb2, 0xb0}));
  assert(scsu::decode(encoded) == U"\u041c\u043e\u0441\u043a\u0432\u0430");
}

static void
test_bom()
{
  assert(scsu::encode(U"\ufeff") == bytes({0x0e, 0xfe, 0xff}));
  assert(scsu::decode(bytes({0x0e, 0xfe, 0xff})) == U"\ufeff");
}

static void
test_ascii()
{
  assert(scsu::encode(U"Hello") == "Hello");
  assert(scsu::decode("Hello") == U"Hello");
}

static void
test_all_features_decode()
{
  // UTS #6 §9.4 compressed sample (decode only — encoder is minimal)
  const auto compressed = bytes({
    0x41, 0xdf, 0x12, 0x81, 0x03, 0x5f, 0x10, 0xdf, 0x1b, 0x03, 0xdf, 0x1c,
    0x88, 0x80, 0x0b, 0xbf, 0xff, 0xff, 0x0d, 0x0a, 0x41, 0x10, 0xdf, 0x12,
    0x81, 0x03, 0x5f, 0x10, 0xdf, 0x13, 0xdf, 0x14, 0x80, 0x15, 0xff
  });
  const auto decoded = scsu::decode(compressed);

  assert(decoded.length() == 18);
  assert(decoded[0] == U'\u0041');
  assert(decoded[1] == U'\u00df');
  assert(decoded[2] == U'\u0401');
  assert(decoded[3] == U'\u015f');
  assert(decoded[6] == U'\uf000');
  assert(decoded[7] == U'\U0010ffff');
  assert(decoded[8] == U'\r');
  assert(decoded[9] == U'\n');
  assert(decoded[17] == U'\U0010ffff');
}

static void
test_roundtrip()
{
  const std::u32string input = U"Hello, \u041c\u043e\u0441\u043a\u0432\u0430 \u30a2\u30c3\u30d7\u30eb!";
  const auto encoded = scsu::encode(input);
  const auto decoded = scsu::decode(encoded);

  assert(decoded == input);
}

static void
test_validate()
{
  std::string encoded;
  std::u32string decoded;

  assert(scsu::encode_validate(U"\ufeff", encoded));
  assert(encoded == bytes({0x0e, 0xfe, 0xff}));

  decoded.clear();
  assert(scsu::decode_validate(encoded, decoded));
  assert(decoded == U"\ufeff");

  // Reserved tag 0x0C
  decoded.clear();
  assert(!scsu::decode_validate(bytes({0x0c}), decoded));

  // Truncated SQU
  decoded.clear();
  assert(!scsu::decode_validate(bytes({0x0e, 0xfe}), decoded));
}

int
main()
{
  test_german();
  test_russian();
  test_bom();
  test_ascii();
  test_all_features_decode();
  test_roundtrip();
  test_validate();

  std::cout << "All SCSU tests passed!" << std::endl;

  return 0;
}
