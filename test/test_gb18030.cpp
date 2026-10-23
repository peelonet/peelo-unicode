#include <cassert>
#include <cstring>
#include <iostream>

#include <peelo/unicode/encoding/gb18030.hpp>

using namespace peelo::unicode::encoding;

static void
test_ascii()
{
  // ASCII should be passed through unchanged
  const std::u32string input = U"Hello";
  const auto encoded = gb18030::encode(input);

  assert(encoded == "Hello");

  const auto decoded = gb18030::decode(encoded);
  assert(decoded == input);

  std::cout << "ASCII test passed" << std::endl;
}

static void
test_decode_validate()
{
  // Valid ASCII
  std::u32string output;
  assert(gb18030::decode_validate("Hello", output));
  assert(output == U"Hello");

  std::cout << "decode_validate test passed" << std::endl;
}

static void
test_byte_validation()
{
  // Test byte validation functions
  assert(gb18030::is_first_byte(0x00));  // ASCII
  assert(gb18030::is_first_byte(0x7F));  // ASCII
  assert(gb18030::is_first_byte(0x81));  // Multi-byte start
  assert(gb18030::is_first_byte(0xFE));  // Multi-byte start
  assert(!gb18030::is_first_byte(0x80)); // Invalid
  assert(!gb18030::is_first_byte(0xFF)); // Invalid

  assert(gb18030::is_two_byte_second(0x40));
  assert(gb18030::is_two_byte_second(0x7E));
  assert(gb18030::is_two_byte_second(0x80));
  assert(gb18030::is_two_byte_second(0xFE));
  assert(!gb18030::is_two_byte_second(0x7F)); // Gap

  assert(gb18030::is_four_byte_second_fourth(0x30));
  assert(gb18030::is_four_byte_second_fourth(0x39));
  assert(!gb18030::is_four_byte_second_fourth(0x2F));
  assert(!gb18030::is_four_byte_second_fourth(0x3A));

  std::cout << "Byte validation test passed" << std::endl;
}

static void
test_two_byte_chinese()
{
  // Test common Chinese characters (2-byte encoding)
  // U+4E2D (中) = GB18030 0xD6D0
  // U+6587 (文) = GB18030 0xCEC4

  // Decode test: 中文 in GB18030
  const char gb_zhongwen[] = "\xD6\xD0\xCE\xC4";
  const auto decoded = gb18030::decode(gb_zhongwen, 4);
  assert(decoded.length() == 2);
  assert(decoded[0] == U'\u4E2D'); // 中
  assert(decoded[1] == U'\u6587'); // 文

  // Encode test
  const std::u32string unicode_zhongwen = U"\u4E2D\u6587";
  const auto encoded = gb18030::encode(unicode_zhongwen);
  assert(encoded.length() == 4);
  assert(static_cast<unsigned char>(encoded[0]) == 0xD6);
  assert(static_cast<unsigned char>(encoded[1]) == 0xD0);
  assert(static_cast<unsigned char>(encoded[2]) == 0xCE);
  assert(static_cast<unsigned char>(encoded[3]) == 0xC4);

  std::cout << "Two-byte Chinese test passed" << std::endl;
}

static void
test_four_byte_encoding()
{
  // Test 4-byte encoding for characters outside GBK range
  // U+20000 (first CJK Extension B character) should use 4-byte encoding

  const std::u32string input = U"\U00020000";
  const auto encoded = gb18030::encode(input);

  // Should be 4 bytes
  assert(encoded.length() == 4);

  // Verify structure: [0x81-0xFE][0x30-0x39][0x81-0xFE][0x30-0x39]
  assert(static_cast<unsigned char>(encoded[0]) >= 0x81);
  assert(static_cast<unsigned char>(encoded[0]) <= 0xFE);
  assert(static_cast<unsigned char>(encoded[1]) >= 0x30);
  assert(static_cast<unsigned char>(encoded[1]) <= 0x39);
  assert(static_cast<unsigned char>(encoded[2]) >= 0x81);
  assert(static_cast<unsigned char>(encoded[2]) <= 0xFE);
  assert(static_cast<unsigned char>(encoded[3]) >= 0x30);
  assert(static_cast<unsigned char>(encoded[3]) <= 0x39);

  // Round-trip test
  const auto decoded = gb18030::decode(encoded);
  assert(decoded == input);

  std::cout << "Four-byte encoding test passed" << std::endl;
}

static void
test_mixed_content()
{
  // Test mixed ASCII and Chinese
  const std::u32string input = U"Hello\u4E2D\u6587World";
  const auto encoded = gb18030::encode(input);
  const auto decoded = gb18030::decode(encoded);

  assert(decoded == input);

  std::cout << "Mixed content test passed" << std::endl;
}

int
main()
{
  test_ascii();
  test_decode_validate();
  test_byte_validation();
  test_two_byte_chinese();
  test_four_byte_encoding();
  test_mixed_content();

  std::cout << "All GB18030 tests passed!" << std::endl;

  return 0;
}
