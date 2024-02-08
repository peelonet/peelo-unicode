#include <cassert>

#include <peelo/unicode/encoding/utf32le.hpp>

static void
test_encode()
{
  using peelo::unicode::encoding::utf32le::encode;

  assert(!encode(U"$").compare(std::string("$\x00\x00\x00", 4)));
  assert(!encode(U"¢").compare(std::string("\xa2\x00\x00\x00", 4)));
  assert(!encode(U"€").compare(std::string("\xac \x00\x00", 4)));
  assert(!encode(U"𐍈").compare(std::string("H\x03\x01\x00", 4)));
}

static void
test_decode()
{
  using peelo::unicode::encoding::utf32le::decode;

  assert(!decode("$\x00\x00\x00", 4).compare(U"$"));
  assert(!decode("\xa2\x00\x00\x00", 4).compare(U"¢"));
  assert(!decode("\xac \x00\x00", 4).compare(U"€"));
  assert(!decode("H\x03\x01\x00", 4).compare(U"𐍈"));
}

int
main()
{
  test_encode();
  test_decode();
}
