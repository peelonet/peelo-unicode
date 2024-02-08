#include <cassert>

#include <peelo/unicode/encoding/utf32be.hpp>

static void
test_encode()
{
  using peelo::unicode::encoding::utf32be::encode;

  assert(!encode(U"$").compare(std::string("\x00\x00\x00$", 4)));
  assert(!encode(U"¢").compare(std::string("\x00\x00\x00\xa2", 4)));
  assert(!encode(U"€").compare(std::string("\x00\x00 \xac", 4)));
  assert(!encode(U"𐍈").compare(std::string("\x00\x01\x03H", 4)));
}

static void
test_decode()
{
  using peelo::unicode::encoding::utf32be::decode;

  assert(!decode(std::string("\x00\x00\x00$", 4)).compare(U"$"));
  assert(!decode(std::string("\x00\x00\x00\xa2", 4)).compare(U"¢"));
  assert(!decode(std::string("\x00\x00 \xac", 4)).compare(U"€"));
  assert(!decode(std::string("\x00\x01\x03H", 4)).compare(U"𐍈"));
}

int
main()
{
  test_encode();
  test_decode();
}
