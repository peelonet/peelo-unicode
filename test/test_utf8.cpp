/*
 * Copyright (c) 2018-2019, peelo.net
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

#include <peelo/unicode/utf8.hpp>

#include <iostream>

static void test_encode()
{
  using peelo::unicode::utf8::encode;

  assert(!encode(U"$").compare("$"));
  assert(!encode(U"¢").compare("\xc2\xa2"));
  assert(!encode(U"€").compare("\xe2\x82\xac"));
  assert(!encode(U"𐍈").compare("\xf0\x90\x8d\x88"));
}

static void test_decode()
{
  using peelo::unicode::utf8::decode;

  assert(!decode("$").compare(U"$"));
  assert(!decode("\xc2\xa2").compare(U"¢"));
  assert(!decode("\xe2\x82\xac").compare(U"€"));
  assert(!decode("\xf0\x90\x8d\x88").compare(U"𐍈"));
}

static void test_sequence_length()
{
  using peelo::unicode::utf8::sequence_length;

  assert(sequence_length(36) == 1);
  assert(sequence_length(194) == 2);
  assert(sequence_length(226) == 3);
  assert(sequence_length(240) == 4);
}

static void test_codepoint_length()
{
  using peelo::unicode::utf8::codepoint_length;

  assert(codepoint_length(0x0024) == 1);
  assert(codepoint_length(0x00a2) == 2);
  assert(codepoint_length(0x20ac) == 3);
  assert(codepoint_length(0x10348) == 4);
}

int main()
{
  test_encode();
  test_decode();
  test_sequence_length();
  test_codepoint_length();

  return EXIT_SUCCESS;
}
