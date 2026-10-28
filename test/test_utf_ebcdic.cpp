#include <cassert>
#include <iostream>
#include <string>

#include <peelo/unicode/encoding/utf_ebcdic.hpp>

using namespace peelo::unicode::encoding;

static std::string
bytes(std::initializer_list<unsigned char> input)
{
  return std::string(input.begin(), input.end());
}

static void
test_single_byte()
{
  // ASCII / controls map through CP1047-style EBCDIC positions
  assert(utf_ebcdic::encode(U"A") == bytes({0xc1}));
  assert(utf_ebcdic::encode(U"0") == bytes({0xf0}));
  assert(utf_ebcdic::encode(U" ") == bytes({0x40}));
  assert(utf_ebcdic::encode(U"\u007f") == bytes({0x07}));
  assert(utf_ebcdic::encode(U"\u0080") == bytes({0x20}));
  assert(utf_ebcdic::encode(U"\u009f") == bytes({0xff}));

  assert(utf_ebcdic::decode(bytes({0xc1})) == U"A");
  assert(utf_ebcdic::decode(bytes({0xf0})) == U"0");
  assert(utf_ebcdic::decode(bytes({0x40})) == U" ");
}

static void
test_bom()
{
  // UTR #16: U+FEFF -> I8 F1 BF B7 BF -> UTF-EBCDIC DD 73 66 73
  assert(utf_ebcdic::encode(U"\ufeff") == bytes({0xdd, 0x73, 0x66, 0x73}));
  assert(utf_ebcdic::decode(bytes({0xdd, 0x73, 0x66, 0x73})) == U"\ufeff");
}

static void
test_multibyte()
{
  assert(utf_ebcdic::encode(U"\u00a0") == bytes({0x80, 0x41}));
  assert(utf_ebcdic::encode(U"\u00ff") == bytes({0x8b, 0x73}));
  assert(utf_ebcdic::encode(U"\u0400") == bytes({0xb8, 0x41, 0x41}));
  assert(utf_ebcdic::encode(U"\u4000") == bytes({0xdc, 0x57, 0x41, 0x41}));
  assert(utf_ebcdic::encode(U"\U00010000") == bytes({0xde, 0x41, 0x41, 0x41}));
  assert(
    utf_ebcdic::encode(U"\U0010fffd")
    == bytes({0xee, 0x42, 0x73, 0x73, 0x71})
  );

  assert(utf_ebcdic::decode(bytes({0x80, 0x41})) == U"\u00a0");
  assert(utf_ebcdic::decode(bytes({0xb8, 0x41, 0x41})) == U"\u0400");
  assert(utf_ebcdic::decode(bytes({0xde, 0x41, 0x41, 0x41})) == U"\U00010000");
  assert(
    utf_ebcdic::decode(bytes({0xee, 0x42, 0x73, 0x73, 0x71}))
    == U"\U0010fffd"
  );
}

static void
test_roundtrip()
{
  const std::u32string input = U"Hello, \u4e16\u754c \U0001f600!";
  const auto encoded = utf_ebcdic::encode(input);
  const auto decoded = utf_ebcdic::decode(encoded);

  assert(decoded == input);
}

static void
test_sequence_length()
{
  assert(utf_ebcdic::sequence_length(0xc1) == 1); // 'A'
  assert(utf_ebcdic::sequence_length(0x40) == 1); // space
  assert(utf_ebcdic::sequence_length(0x80) == 2); // lead of U+00A0
  assert(utf_ebcdic::sequence_length(0xb8) == 3);
  assert(utf_ebcdic::sequence_length(0xdc) == 4);
  assert(utf_ebcdic::sequence_length(0xdd) == 4); // BOM lead
  assert(utf_ebcdic::sequence_length(0xee) == 5);
  assert(utf_ebcdic::sequence_length(0x41) == 0); // trailing byte
}

static void
test_codepoint_length()
{
  assert(utf_ebcdic::codepoint_length(0x0041) == 1);
  assert(utf_ebcdic::codepoint_length(0x009f) == 1);
  assert(utf_ebcdic::codepoint_length(0x00a0) == 2);
  assert(utf_ebcdic::codepoint_length(0x03ff) == 2);
  assert(utf_ebcdic::codepoint_length(0x0400) == 3);
  assert(utf_ebcdic::codepoint_length(0x3fff) == 3);
  assert(utf_ebcdic::codepoint_length(0x4000) == 4);
  assert(utf_ebcdic::codepoint_length(0x10000) == 4);
  assert(utf_ebcdic::codepoint_length(0x10fffd) == 5);
  assert(utf_ebcdic::codepoint_length(0xd800) == 0);
}

static void
test_validate()
{
  std::string encoded;
  std::u32string decoded;

  assert(utf_ebcdic::encode_validate(U"\ufeff", encoded));
  assert(encoded == bytes({0xdd, 0x73, 0x66, 0x73}));

  decoded.clear();
  assert(utf_ebcdic::decode_validate(encoded, decoded));
  assert(decoded == U"\ufeff");

  // Trailing byte alone is invalid
  decoded.clear();
  assert(!utf_ebcdic::decode_validate(bytes({0x41}), decoded));

  // Truncated multi-byte sequence
  decoded.clear();
  assert(!utf_ebcdic::decode_validate(bytes({0xdd, 0x73}), decoded));
}

int
main()
{
  test_single_byte();
  test_bom();
  test_multibyte();
  test_roundtrip();
  test_sequence_length();
  test_codepoint_length();
  test_validate();

  std::cout << "All UTF-EBCDIC tests passed!" << std::endl;

  return 0;
}
