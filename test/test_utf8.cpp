#include <cassert>

#include <peelo/unicode/encoding/utf8.hpp>

static void
test_encode()
{
  using peelo::unicode::encoding::utf8::encode;

  assert(!encode(U"$").compare("$"));
  assert(!encode(U"¢").compare("\xc2\xa2"));
  assert(!encode(U"€").compare("\xe2\x82\xac"));
  assert(!encode(U"𐍈").compare("\xf0\x90\x8d\x88"));
}

static void
test_decode()
{
  using peelo::unicode::encoding::utf8::decode;

  assert(!decode("$").compare(U"$"));
  assert(!decode("\xc2\xa2").compare(U"¢"));
  assert(!decode("\xe2\x82\xac").compare(U"€"));
  assert(!decode("\xf0\x90\x8d\x88").compare(U"𐍈"));
}

static void
test_sequence_length()
{
  using peelo::unicode::encoding::utf8::sequence_length;

  assert(sequence_length(36) == 1);
  assert(sequence_length(194) == 2);
  assert(sequence_length(226) == 3);
  assert(sequence_length(240) == 4);
}

static void
test_codepoint_length()
{
  using peelo::unicode::encoding::utf8::codepoint_length;

  assert(codepoint_length(0x0024) == 1);
  assert(codepoint_length(0x00a2) == 2);
  assert(codepoint_length(0x20ac) == 3);
  assert(codepoint_length(0x10348) == 4);
}

int
main()
{
  test_encode();
  test_decode();
  test_sequence_length();
  test_codepoint_length();
}
