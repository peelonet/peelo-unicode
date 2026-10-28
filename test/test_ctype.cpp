#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/ctype.hpp>

TEST_CASE("ctype isvalid")
{
  using peelo::unicode::ctype::isvalid;

  REQUIRE(isvalid(U'a'));
  REQUIRE(isvalid(U' '));

  REQUIRE_FALSE(isvalid(0x110000));
  REQUIRE_FALSE(isvalid(0xfdd5));
  REQUIRE_FALSE(isvalid(0xd801));
}

TEST_CASE("ctype isalpha")
{
  using peelo::unicode::ctype::isalpha;

  REQUIRE(isalpha(U'a'));
  REQUIRE(isalpha(U'A'));
  REQUIRE(isalpha(U'ä'));
  REQUIRE(isalpha(U'Ä'));

  REQUIRE_FALSE(isalpha(U'5'));
  REQUIRE_FALSE(isalpha(U'€'));
  REQUIRE_FALSE(isalpha(U'-'));
}

TEST_CASE("ctype isalnum")
{
  using peelo::unicode::ctype::isalnum;

  REQUIRE(isalnum(U'a'));
  REQUIRE(isalnum(U'A'));
  REQUIRE(isalnum(U'ä'));
  REQUIRE(isalnum(U'Ä'));
  REQUIRE(isalnum(U'5'));
  REQUIRE(isalnum(U'٠'));

  REQUIRE_FALSE(isalnum(U' '));
  REQUIRE_FALSE(isalnum(U'€'));
}

TEST_CASE("ctype isdigit")
{
  using peelo::unicode::ctype::isdigit;

  for (char32_t c = U'0'; c <= U'9'; ++c)
  {
    REQUIRE(isdigit(c));
  }

  REQUIRE_FALSE(isdigit(U'a'));
  REQUIRE_FALSE(isdigit(U'F'));
  REQUIRE_FALSE(isdigit(U'€'));

  REQUIRE(isdigit(U'٠'));
  REQUIRE(isdigit(U'०'));
  REQUIRE(isdigit(U'০'));
  REQUIRE(isdigit(U'੦'));
  REQUIRE(isdigit(U'൧'));
  REQUIRE(isdigit(U'០'));
  REQUIRE(isdigit(U'᠐'));
}

TEST_CASE("ctype isxdigit")
{
  using peelo::unicode::ctype::isxdigit;

  for (char32_t c = U'a'; c <= U'f'; ++c)
  {
    REQUIRE(isxdigit(c));
  }
  for (char32_t c = U'A'; c <= U'F'; ++c)
  {
    REQUIRE(isxdigit(c));
  }
  for (char32_t c = U'0'; c <= U'9'; ++c)
  {
    REQUIRE(isxdigit(c));
  }

  REQUIRE_FALSE(isxdigit(U'g'));
  REQUIRE_FALSE(isxdigit(U'G'));
  REQUIRE_FALSE(isxdigit(U'€'));
}

TEST_CASE("ctype isblank")
{
  using peelo::unicode::ctype::isblank;

  REQUIRE(isblank(U' '));
  REQUIRE(isblank(U'\t'));
  REQUIRE(isblank(U'\u2005'));

  REQUIRE_FALSE(isblank(U'a'));
  REQUIRE_FALSE(isblank(U'-'));
  REQUIRE_FALSE(isblank(U'€'));
}

TEST_CASE("ctype iscntrl")
{
  using peelo::unicode::ctype::iscntrl;

  REQUIRE(iscntrl(U'\t'));
  REQUIRE(iscntrl(U'\n'));

  REQUIRE_FALSE(iscntrl(U'a'));
  REQUIRE_FALSE(iscntrl(U' '));
}

TEST_CASE("ctype isgraph")
{
  using peelo::unicode::ctype::isgraph;

  REQUIRE(isgraph(U'a'));
  REQUIRE(isgraph(U'€'));

  REQUIRE_FALSE(isgraph(U' '));
  REQUIRE_FALSE(isgraph(0x205f));
}

TEST_CASE("ctype isprint")
{
  using peelo::unicode::ctype::isprint;

  REQUIRE(isprint(U'a'));
  REQUIRE(isprint(U' '));

  REQUIRE_FALSE(isprint(U'\u0008'));
  REQUIRE_FALSE(isprint(U'\u074c'));
}

TEST_CASE("ctype ispunct")
{
  using peelo::unicode::ctype::ispunct;

  REQUIRE(ispunct(U'\u0023'));
  REQUIRE(ispunct(U'\u037e'));

  REQUIRE_FALSE(ispunct(U'a'));
  REQUIRE_FALSE(ispunct(U' '));
}

TEST_CASE("ctype isspace")
{
  using peelo::unicode::ctype::isspace;

  REQUIRE(isspace(U' '));
  REQUIRE(isspace(U'\t'));
  REQUIRE(isspace(U'\u2001'));

  REQUIRE_FALSE(isspace(U'a'));
  REQUIRE_FALSE(isspace(U'-'));
}

TEST_CASE("ctype islower")
{
  using peelo::unicode::ctype::islower;

  REQUIRE(islower(U'a'));
  REQUIRE(islower(U'ä'));

  REQUIRE_FALSE(islower(U'A'));
  REQUIRE_FALSE(islower(U'Ä'));
  REQUIRE_FALSE(islower(U'5'));
}

TEST_CASE("ctype isupper")
{
  using peelo::unicode::ctype::isupper;

  REQUIRE(isupper(U'A'));
  REQUIRE(isupper(U'Ä'));

  REQUIRE_FALSE(isupper(U'a'));
  REQUIRE_FALSE(isupper(U'ä'));
  REQUIRE_FALSE(isupper(U'5'));
}

TEST_CASE("ctype isemoji")
{
  using peelo::unicode::ctype::isemoji;

  REQUIRE(isemoji(0x1f642));
  REQUIRE(isemoji(0x1f97b));
  REQUIRE(isemoji(0x1face));

  REQUIRE_FALSE(isemoji(U'A'));
  REQUIRE_FALSE(isemoji(0x20ac));
  REQUIRE_FALSE(isemoji(0x2513));
}

TEST_CASE("ctype tolower")
{
  using peelo::unicode::ctype::tolower;

  REQUIRE(tolower(U'A') == U'a');
  REQUIRE(tolower(U'Ä') == U'ä');
  REQUIRE(tolower(U'5') == U'5');
}

TEST_CASE("ctype toupper")
{
  using peelo::unicode::ctype::toupper;

  REQUIRE(toupper(U'a') == U'A');
  REQUIRE(toupper(U'ä') == U'Ä');
  REQUIRE(toupper(U'5') == U'5');
}
