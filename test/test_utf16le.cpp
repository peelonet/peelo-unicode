/*
 * Copyright (c) 2018-2020, peelo.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <cassert>
#include <cstdlib>

#include <peelo/unicode/encoding/utf16le.hpp>

#include <iostream>

static void test_encode()
{
  using peelo::unicode::encoding::utf16le::encode;

  assert(!encode(U"$").compare(std::string("$\x00", 2)));
  assert(!encode(U"¢").compare(std::string("\xa2\x00", 2)));
  assert(!encode(U"€").compare(std::string("\xac ", 2)));
  assert(!encode(U"𐍈").compare(std::string("\x00\xd8H\xdf", 4)));
}

static void test_decode()
{
  using peelo::unicode::encoding::utf16le::decode;

  assert(!decode(std::string("$\x00", 2)).compare(U"$"));
  assert(!decode(std::string("\xa2\x00", 2)).compare(U"¢"));
  assert(!decode(std::string("\xac ", 2)).compare(U"€"));
  assert(!decode(std::string("\x00\xd8H\xdf", 4)).compare(U"𐍈"));
}

int main()
{
  test_encode();
  test_decode();

  return EXIT_SUCCESS;
}
