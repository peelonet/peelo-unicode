#include <string>

#include <catch2/catch_test_macros.hpp>

#include <peelo/unicode/encoding/utf7.hpp>

using namespace peelo::unicode::encoding;

TEST_CASE("utf7 ascii")
{
  REQUIRE(utf7::encode(U"Hello") == "Hello");
  REQUIRE(utf7::decode("Hello") == U"Hello");
}

TEST_CASE("utf7 plus")
{
  REQUIRE(utf7::encode(U"+") == "+-");
  REQUIRE(utf7::decode("+-") == U"+");
  REQUIRE(utf7::encode(U"A+B") == "A+-B");
  REQUIRE(utf7::decode("A+-B") == U"A+B");
}

TEST_CASE("utf7 rfc examples")
{
  REQUIRE(utf7::encode(U"A\u2262\u0391.") == "A+ImIDkQ.");
  REQUIRE(utf7::decode("A+ImIDkQ.") == U"A\u2262\u0391.");

  REQUIRE(utf7::encode(U"Hi Mom -\u263a-!") == "Hi Mom -+Jjo--!");
  REQUIRE(utf7::decode("Hi Mom -+Jjo--!") == U"Hi Mom -\u263a-!");

  REQUIRE(utf7::encode(U"\u65e5\u672c\u8a9e") == "+ZeVnLIqe-");
  REQUIRE(utf7::decode("+ZeVnLIqe-") == U"\u65e5\u672c\u8a9e");

  REQUIRE(utf7::encode(U"Item 3 is \u00a31.") == "Item 3 is +AKM-1.");
  REQUIRE(utf7::decode("Item 3 is +AKM-1.") == U"Item 3 is \u00a31.");
}

TEST_CASE("utf7 surrogates")
{
  REQUIRE(utf7::encode(U"a\U0001f600b") == "a+2D3eAA-b");
  REQUIRE(utf7::decode("a+2D3eAA-b") == U"a\U0001f600b");
}

TEST_CASE("utf7 direct and optional")
{
  REQUIRE(utf7::is_direct(U'A'));
  REQUIRE(utf7::is_direct(U'!'));
  REQUIRE(utf7::is_direct(U' '));
  REQUIRE_FALSE(utf7::is_direct(U'+'));
  REQUIRE_FALSE(utf7::is_direct(U'\\'));
  REQUIRE_FALSE(utf7::is_direct(U'~'));
  REQUIRE_FALSE(utf7::is_direct(U'\u00a3'));

  REQUIRE(utf7::encode(U"\\~") == "+AFwAfg-");
  REQUIRE(utf7::decode("+AFwAfg-") == U"\\~");
}

TEST_CASE("utf7 roundtrip")
{
  const std::u32string input = U"Hello, \u4e16\u754c + friends!\n";
  const auto encoded = utf7::encode(input);
  const auto decoded = utf7::decode(encoded);

  REQUIRE(decoded == input);
  REQUIRE(encoded == "Hello, +ThZ1TA +- friends!\n");
}

TEST_CASE("utf7 validate")
{
  std::string encoded;
  std::u32string decoded;

  REQUIRE(utf7::encode_validate(U"Hi Mom -\u263a-!", encoded));
  REQUIRE(encoded == "Hi Mom -+Jjo--!");

  decoded.clear();
  REQUIRE(utf7::decode_validate(encoded, decoded));
  REQUIRE(decoded == U"Hi Mom -\u263a-!");

  decoded.clear();
  REQUIRE_FALSE(utf7::decode_validate("\xff", decoded));

  decoded.clear();
  REQUIRE_FALSE(utf7::decode_validate("+!", decoded));

  decoded.clear();
  REQUIRE_FALSE(utf7::decode_validate("+/v9-", decoded));
}

TEST_CASE("utf7 bom")
{
  REQUIRE(utf7::decode("+/v8-") == U"\ufeff");
  REQUIRE(utf7::encode(U"\ufeff") == "+/v8-");
}
