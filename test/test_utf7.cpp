#include <cassert>
#include <iostream>

#include <peelo/unicode/encoding/utf7.hpp>

using namespace peelo::unicode::encoding;

static void
test_ascii()
{
  assert(utf7::encode(U"Hello") == "Hello");
  assert(utf7::decode("Hello") == U"Hello");
}

static void
test_plus()
{
  assert(utf7::encode(U"+") == "+-");
  assert(utf7::decode("+-") == U"+");
  assert(utf7::encode(U"A+B") == "A+-B");
  assert(utf7::decode("A+-B") == U"A+B");
}

static void
test_rfc_examples()
{
  // RFC 2152 examples
  assert(utf7::encode(U"A\u2262\u0391.") == "A+ImIDkQ.");
  assert(utf7::decode("A+ImIDkQ.") == U"A\u2262\u0391.");

  assert(utf7::encode(U"Hi Mom -\u263a-!") == "Hi Mom -+Jjo--!");
  assert(utf7::decode("Hi Mom -+Jjo--!") == U"Hi Mom -\u263a-!");

  assert(utf7::encode(U"\u65e5\u672c\u8a9e") == "+ZeVnLIqe-");
  assert(utf7::decode("+ZeVnLIqe-") == U"\u65e5\u672c\u8a9e");

  assert(utf7::encode(U"Item 3 is \u00a31.") == "Item 3 is +AKM-1.");
  assert(utf7::decode("Item 3 is +AKM-1.") == U"Item 3 is \u00a31.");
}

static void
test_surrogates()
{
  // U+1F600 (😀) encodes as a UTF-16 surrogate pair in the shifted sequence.
  assert(utf7::encode(U"a\U0001f600b") == "a+2D3eAA-b");
  assert(utf7::decode("a+2D3eAA-b") == U"a\U0001f600b");
}

static void
test_direct_and_optional()
{
  assert(utf7::is_direct(U'A'));
  assert(utf7::is_direct(U'!'));
  assert(utf7::is_direct(U' '));
  assert(!utf7::is_direct(U'+'));
  assert(!utf7::is_direct(U'\\'));
  assert(!utf7::is_direct(U'~'));
  assert(!utf7::is_direct(U'\u00a3'));

  // Backslash and tilde must use shifted encoding.
  assert(utf7::encode(U"\\~") == "+AFwAfg-");
  assert(utf7::decode("+AFwAfg-") == U"\\~");
}

static void
test_roundtrip()
{
  const std::u32string input = U"Hello, \u4e16\u754c + friends!\n";
  const auto encoded = utf7::encode(input);
  const auto decoded = utf7::decode(encoded);

  assert(decoded == input);
  assert(encoded == "Hello, +ThZ1TA +- friends!\n");
}

static void
test_validate()
{
  std::string encoded;
  std::u32string decoded;

  assert(utf7::encode_validate(U"Hi Mom -\u263a-!", encoded));
  assert(encoded == "Hi Mom -+Jjo--!");

  decoded.clear();
  assert(utf7::decode_validate(encoded, decoded));
  assert(decoded == U"Hi Mom -\u263a-!");

  decoded.clear();
  assert(!utf7::decode_validate("\xff", decoded));

  decoded.clear();
  assert(!utf7::decode_validate("+!", decoded));

  decoded.clear();
  assert(!utf7::decode_validate("+/v9-", decoded));
}

static void
test_bom()
{
  assert(utf7::decode("+/v8-") == U"\ufeff");
  assert(utf7::encode(U"\ufeff") == "+/v8-");
}

int
main()
{
  test_ascii();
  test_plus();
  test_rfc_examples();
  test_surrogates();
  test_direct_and_optional();
  test_roundtrip();
  test_validate();
  test_bom();

  std::cout << "All UTF-7 tests passed!" << std::endl;

  return 0;
}
