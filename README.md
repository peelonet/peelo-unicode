# peelo-unicode

![Build](https://github.com/peelonet/peelo-unicode/workflows/Build/badge.svg)

Collection of simple to use [Unicode] utilities for C++17. Supports Unicode
15.1.

[Doxygen generated API documentation.][API]

[Unicode]: https://en.wikipedia.org/wiki/Unicode
[API]: https://peelonet.github.io/peelo-unicode/index.html

## Character testing functions

The library ships with Unicode version of [ctype.h] header, containing
following functions inside `peelo::unicode::ctype` namespace:

- `isalnum()`
- `isalpha()`
- `isblank()`
- `iscntrl()`
- `isdigit()`
- `isgraph()`
- `islower()`
- `isprint()`
- `ispunct()`
- `isspace()`
- `isupper()`
- `isxdigit()`
- `tolower()`
- `toupper()`

Additional functions not found in `ctype.h` are:

- `isvalid()` - Tests whether given value is valid Unicode codepoint.
- `isemoji()` - Tests whether given Unicode codepoint is an [emoji].

[ctype.h]: https://en.cppreference.com/w/cpp/header/cctype
[emoji]: https://en.wikipedia.org/wiki/Emoji

### Example

```cpp
#include <iostream>
#include <peelo/unicode/ctype.hpp>

int
main()
{
  using namespace peelo::unicode::ctype;

  std::cout << isalnum(U'Ä') << std::endl;
  std::cout << isdigit(U'൧') << std::endl;
  std::cout << isgraph(U'€') << std::endl;
  std::cout << ispunct(U'\u2001') << std::endl;
  std::cout << std::hex;
  std::cout << tolower(U'Ä') << std::endl;
  std::cout << toupper(U'ä') << std::endl;
}
```

## Character encodings

The library also provides functions for encoding and decoding Unicode character
encodings. Both validating and non-validating (where all encoding/decoding
errors are ignored) functions are provided.

Supported character encodings are:

- [UTF-8]
- [UTF-16BE][UTF-16]
- [UTF-16LE][UTF-16]
- [UTF-32BE][UTF-32]
- [UTF-32LE][UTF-32]

[UTF-8]: https://en.wikipedia.org/wiki/UTF-8
[UTF-16]: https://en.wikipedia.org/wiki/UTF-16
[UTF-32]: https://en.wikipedia.org/wiki/UTF-32

### Example

```cpp
#include <peelo/unicode/encoding.hpp>

int
main()
{
  using namespace peelo::unicode::encoding;

  // Decode UTF-8 input, ignoring any decoding errors.
  std::u32string utf8_decoded = utf8::decode("\xe2\x82\xac");

  // Encode it back to byte string, ignoring any encoding errors.
  std::string utf8_encoded = utf8::encode(utf8_decoded);

  // Decode UTF-32BE input with validation.
  std::u32string utf32be_decoded;
  if (utf32be::decode_validate("\x00\x00 \xac", utf32be_decoded))
  {
    // Given input is valid UTF-32BE.
  } else {
    // Given input is invalid UTF-32BE.
  }

  // Encode it back to byte string, with validation.
  std::string utf32be_encoded;
  if (utf32be::encode_validate(utf32be_decoded, utf32be_encoded))
  {
    // Given input contained only valid Unicode code points.
  } else {
    // Given input contained invalid Unicode code points.
  }
}
```
