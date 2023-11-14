/* GNU gettext - internationalization aids
   Copyright (C) 1995-2023 Free Software Foundation, Inc.

   This file was written by Peter Miller <millerp@canb.auug.org.au>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#ifndef _PO_GRAM_H
#define _PO_GRAM_H

#include <stdbool.h>
#include <stdio.h>

#if HAVE_ICONV
# include "unistr.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Multibyte character data type.  */
/* Note this depends on po_lex_charset and po_lex_iconv, which get set
   while the file is being parsed.  */

#define MBCHAR_BUF_SIZE 24

struct mbchar
{
  size_t bytes;         /* number of bytes of current character, > 0 */
#if HAVE_ICONV
  bool uc_valid;        /* true if uc is a valid Unicode character */
  ucs4_t uc;            /* if uc_valid: the current character */
#endif
  char buf[MBCHAR_BUF_SIZE]; /* room for the bytes */
};

/* We want to pass multibyte characters by reference automatically,
   therefore we use an array type.  */
typedef struct mbchar mbchar_t[1];


/* Number of characters that can be pushed back.
   We need 1 for lex_getc, plus 1 for lex_ungetc.  */
#define NPUSHBACK 2

/* Data type of a multibyte character input stream.  */
struct mbfile
{
  FILE *fp;
  bool eof_seen;
  int have_pushback;
  unsigned int bufcount;
  char buf[MBCHAR_BUF_SIZE];
  struct mbchar pushback[NPUSHBACK];
};

/* We want to pass multibyte streams by reference automatically,
   therefore we use an array type.  */
typedef struct mbfile mbfile_t[1];


/* Input, output, and local variables of a PO parser instance.  */
struct po_parser_state
{
  /* ----- Input variables -----  */

  /* Whether the PO file is in the role of a POT file.  */
  bool gram_pot_role;

  /* ----- Output variables -----  */

  /* ----- Local variables of po-lex.c -----  */

  /* Current position within the PO file.  */
  int gram_pos_column;

  /* Whether invalid multibyte sequences in the input shall be signalled
     or silently tolerated.  */
  bool signal_eilseq;

  mbfile_t mbf;
  bool po_lex_obsolete;
  bool po_lex_previous;

  /* ----- Local variables of po-gram-gen.y -----  */
  long plural_counter;
};

extern int po_gram_parse (struct po_parser_state *ps);

#ifdef __cplusplus
}
#endif

#endif
