#include <cassert>

#include <peelo/unicode/encoding/utf16be.hpp>

static void
test_encode()
{
  using peelo::unicode::encoding::utf16be::encode;

  assert(!encode(U"$").compare(std::string("\x00$", 2)));
  assert(!encode(U"¢").compare(std::string("\x00\xa2", 2)));
  assert(!encode(U"€").compare(std::string(" \xac", 2)));
  assert(!encode(U"𐍈").compare(std::string("\xd8\x00\xdfH", 4)));
}

static void
test_decode()
{
  using peelo::unicode::encoding::utf16be::decode;

  assert(!decode(std::string("\x00$", 2)).compare(U"$"));
  assert(!decode(std::string("\x00\xa2", 2)).compare(U"¢"));
  assert(!decode(std::string(" \xac", 2)).compare(U"€"));
  assert(!decode(std::string("\xd8\x00\xdfH", 4)).compare(U"𐍈"));
}

int
main()
{
  test_encode();
  test_decode();
}
