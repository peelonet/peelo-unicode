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

#include <peelo/unicode/ctype.hpp>

static void test_isvalid()
{
  using peelo::unicode::ctype::isvalid;

  assert(isvalid(U'a'));
  assert(isvalid(U' '));

  assert(!isvalid(0x110000));
  assert(!isvalid(0xfdd5));
  assert(!isvalid(0xd801));
}

static void test_isalpha()
{
  using peelo::unicode::ctype::isalpha;

  assert(isalpha(U'a'));
  assert(isalpha(U'A'));
  assert(isalpha(U'ä'));
  assert(isalpha(U'Ä'));

  assert(!isalpha(U'5'));
  assert(!isalpha(U'€'));
  assert(!isalpha(U'-'));
}

static void test_isalnum()
{
  using peelo::unicode::ctype::isalnum;

  assert(isalnum(U'a'));
  assert(isalnum(U'A'));
  assert(isalnum(U'ä'));
  assert(isalnum(U'Ä'));
  assert(isalnum(U'5'));
  assert(isalnum(U'٠'));

  assert(!isalnum(U' '));
  assert(!isalnum(U'€'));
}

static void test_isdigit()
{
  using peelo::unicode::ctype::isdigit;

  for (char32_t c = U'0'; c <= U'9'; ++c)
  {
    assert(isdigit(c));
  }

  assert(!isdigit(U'a'));
  assert(!isdigit(U'F'));
  assert(!isdigit(U'€'));

  assert(isdigit(U'٠'));
  assert(isdigit(U'०'));
  assert(isdigit(U'০'));
  assert(isdigit(U'੦'));
  assert(isdigit(U'൧'));
  assert(isdigit(U'០'));
  assert(isdigit(U'᠐'));
}

static void test_isxdigit()
{
  using peelo::unicode::ctype::isxdigit;

  for (char32_t c = U'a'; c <= U'f'; ++c)
  {
    assert(isxdigit(c));
  }
  for (char32_t c = U'A'; c <= U'F'; ++c)
  {
    assert(isxdigit(c));
  }
  for (char32_t c = U'0'; c <= U'9'; ++c)
  {
    assert(isxdigit(c));
  }

  assert(!isxdigit(U'g'));
  assert(!isxdigit(U'G'));
  assert(!isxdigit(U'€'));
}

static void test_isblank()
{
  using peelo::unicode::ctype::isblank;

  assert(isblank(U' '));
  assert(isblank(U'\t'));
  assert(isblank(U'\u2005'));

  assert(!isblank(U'a'));
  assert(!isblank(U'-'));
  assert(!isblank(U'€'));
}

static void test_iscntrl()
{
  using peelo::unicode::ctype::iscntrl;

  assert(iscntrl(U'\t'));
  assert(iscntrl(U'\n'));

  assert(!iscntrl(U'a'));
  assert(!iscntrl(U' '));
}

static void test_isgraph()
{
  using peelo::unicode::ctype::isgraph;

  assert(isgraph(U'a'));
  assert(isgraph(U'€'));

  assert(!isgraph(U' '));
  assert(!isgraph(U'\u093b'));
}

static void test_isprint()
{
  using peelo::unicode::ctype::isprint;

  assert(isprint(U'a'));
  assert(isprint(U' '));

  assert(!isprint(U'\u0008'));
  assert(!isprint(U'\u074c'));
}

static void test_ispunct()
{
  using peelo::unicode::ctype::ispunct;

  assert(ispunct(U'\u0023'));
  assert(ispunct(U'\u037e'));

  assert(!ispunct(U'a'));
  assert(!ispunct(U' '));
}

static void test_isspace()
{
  using peelo::unicode::ctype::isspace;

  assert(isspace(U' '));
  assert(isspace(U'\t'));
  assert(isspace(U'\u2001'));

  assert(!isspace(U'a'));
  assert(!isspace(U'-'));
}

static void test_islower()
{
  using peelo::unicode::ctype::islower;

  assert(islower(U'a'));
  assert(islower(U'ä'));

  assert(!islower(U'A'));
  assert(!islower(U'Ä'));
  assert(!islower(U'5'));
}

static void test_isupper()
{
  using peelo::unicode::ctype::isupper;

  assert(isupper(U'A'));
  assert(isupper(U'Ä'));

  assert(!isupper(U'a'));
  assert(!isupper(U'ä'));
  assert(!isupper(U'5'));
}

static void test_tolower()
{
  using peelo::unicode::ctype::tolower;

  assert(tolower(U'A') == U'a');
  assert(tolower(U'Ä') == U'ä');
  assert(tolower(U'5') == U'5');
}

static void test_toupper()
{
  using peelo::unicode::ctype::toupper;

  assert(toupper(U'a') == U'A');
  assert(toupper(U'ä') == U'Ä');
  assert(toupper(U'5') == U'5');
}

int main()
{
  test_isvalid();
  test_isalpha();
  test_isalnum();
  test_isdigit();
  test_isxdigit();
  test_isblank();
  test_iscntrl();
  test_isgraph();
  test_isprint();
  test_ispunct();
  test_isspace();
  test_islower();
  test_isupper();
  test_tolower();
  test_toupper();

  return EXIT_SUCCESS;
}
