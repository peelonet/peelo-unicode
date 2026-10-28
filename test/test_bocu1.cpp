#include <cassert>
#include <iostream>
#include <string>

#include <peelo/unicode/encoding/bocu1.hpp>

using namespace peelo::unicode::encoding;

static std::string
bytes(std::initializer_list<unsigned char> input)
{
  return std::string(input.begin(), input.end());
}

static void
test_bom()
{
  // UTS #40: U+FEFF -> FB EE 28
  assert(bocu1::encode(U"\ufeff") == bytes({0xfb, 0xee, 0x28}));
  assert(bocu1::decode(bytes({0xfb, 0xee, 0x28})) == U"\ufeff");
}

static void
test_ascii_and_controls()
{
  assert(bocu1::encode(U"ABC") == bytes({0x91, 0x92, 0x93}));
  assert(bocu1::decode(bytes({0x91, 0x92, 0x93})) == U"ABC");

  assert(bocu1::encode(U" A") == bytes({0x20, 0x91}));
  assert(bocu1::encode(U"\nA") == bytes({0x0a, 0x91}));
  assert(bocu1::decode(bytes({0x0a, 0x91})) == U"\nA");
}

static void
test_scripts()
{
  assert(bocu1::encode(U"\u4e2d\u6587") == bytes({0xfb, 0x33, 0xd7, 0x3d, 0xcc}));
  assert(bocu1::decode(bytes({0xfb, 0x33, 0xd7, 0x3d, 0xcc})) == U"\u4e2d\u6587");

  assert(
    bocu1::encode(U"\u3042\u3044\u3046")
    == bytes({0xfb, 0x11, 0x59, 0x64, 0x66})
  );
  assert(
    bocu1::decode(bytes({0xfb, 0x11, 0x59, 0x64, 0x66}))
    == U"\u3042\u3044\u3046"
  );

  assert(
    bocu1::encode(U"\uac00\uac01")
    == bytes({0xfb, 0x96, 0xb1, 0x39, 0x52})
  );
}

static void
test_roundtrip()
{
  const std::u32string input =
    U"Hello, world!\n\u4e2d\u6587 \u3042\u3044\u30a2 \uac00\U0001f600";
  const auto encoded = bocu1::encode(input);
  const auto decoded = bocu1::decode(encoded);

  assert(decoded == input);
}

static void
test_reset_byte()
{
  // FF resets state without emitting a code point
  const auto decoded = bocu1::decode(bytes({0x91, 0xff, 0x91}));

  assert(decoded == U"AA");
}

static void
test_validate()
{
  std::string encoded;
  std::u32string decoded;

  assert(bocu1::encode_validate(U"\ufeff", encoded));
  assert(encoded == bytes({0xfb, 0xee, 0x28}));

  decoded.clear();
  assert(bocu1::decode_validate(encoded, decoded));
  assert(decoded == U"\ufeff");

  // Illegal trail byte (NUL) after a multi-byte lead
  decoded.clear();
  assert(!bocu1::decode_validate(bytes({0xd0, 0x00}), decoded));

  // Truncated multi-byte sequence
  decoded.clear();
  assert(!bocu1::decode_validate(bytes({0xfb}), decoded));
}

int
main()
{
  test_bom();
  test_ascii_and_controls();
  test_scripts();
  test_roundtrip();
  test_reset_byte();
  test_validate();

  std::cout << "All BOCU-1 tests passed!" << std::endl;

  return 0;
}
