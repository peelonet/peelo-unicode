#include <cassert>

#include <peelo/unicode/encoding/utf16le.hpp>

static void
test_encode()
{
  using peelo::unicode::encoding::utf16le::encode;

  assert(!encode(U"$").compare(std::string("$\x00", 2)));
  assert(!encode(U"¢").compare(std::string("\xa2\x00", 2)));
  assert(!encode(U"€").compare(std::string("\xac ", 2)));
  assert(!encode(U"𐍈").compare(std::string("\x00\xd8H\xdf", 4)));
}

static void
test_decode()
{
  using peelo::unicode::encoding::utf16le::decode;

  assert(!decode(std::string("$\x00", 2)).compare(U"$"));
  assert(!decode(std::string("\xa2\x00", 2)).compare(U"¢"));
  assert(!decode(std::string("\xac ", 2)).compare(U"€"));
  assert(!decode(std::string("\x00\xd8H\xdf", 4)).compare(U"𐍈"));
}

int
main()
{
  test_encode();
  test_decode();
}
