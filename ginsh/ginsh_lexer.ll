/** @file ginsh_lexer.ll
 *
 *  Lexical analyzer definition for ginsh.
 *  This file must be processed with flex.
 *
 *  GiNaC Copyright (C) 1999 Johannes Gutenberg University Mainz, Germany
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/*
 *  Definitions
 */

%{
#include "config.h"

#include <stdio.h>
extern "C" {
#include <readline/readline.h>
#include <readline/history.h>
}
#include <map>

#include <ginac/ginac.h>
#include "ginsh.h"

#include "ginsh_parser.h"

#define YY_INPUT(buf, result, max_size) (result = ginsh_input(buf, max_size))

// Table of all used symbols
sym_tab syms;

// lex input function
static int ginsh_input(char *buf, int max_size);
%}

	/* Abbreviations */
D	[0-9]
E	[elEL][-+]?{D}+
A	[a-zA-Z_]
AN	[0-9a-zA-Z_]


/*
 *  Lexical rules
 */

%%
[ \t\n]+		/* skip whitespace */
\\$			/* skip line continuations */
"#".*			/* skip comments starting with "#" */
^"!".*			system(yytext + 1);	/* execute shell command */

			/* special values */
Pi			yylval = Pi; return T_LITERAL;
EulerGamma		yylval = EulerGamma; return T_LITERAL;
Catalan			yylval = Catalan; return T_LITERAL;
FAIL			yylval = *new fail(); return T_LITERAL;
I			yylval = I; return T_NUMBER;
Digits			yylval = (long)Digits; return T_DIGITS;

			/* keywords */
quit|exit		return T_QUIT;
print			return T_PRINT;
time			return T_TIME;
xyzzy			return T_XYZZY;
inventory		return T_INVENTORY;
look			return T_LOOK;
score			return T_SCORE;

			/* comparison */
"=="			return T_EQUAL;
"!="			return T_NOTEQ;
"<="			return T_LESSEQ;
">="			return T_GREATEREQ;

			/* last 1..3 expressions */
\"			return T_QUOTE;
\"\"			return T_QUOTE2;
\"\"\"			return T_QUOTE3;

			/* matrix delimiters */
\[\[			return T_MATRIX_BEGIN;
\]\]			return T_MATRIX_END;

			/* numbers */
{D}+			|
{D}+"."{D}*({E})?	|
{D}*"."{D}+({E})?	|
{D}+{E}			yylval = numeric(yytext); return T_NUMBER;

			/* symbols */
{A}{AN}*		{
				if (syms.find(yytext) == syms.end())
					syms[yytext] = *(new symbol(yytext));
				yylval = syms[yytext];
				return T_SYMBOL;
			}

			/* everything else */
.			return *yytext;

%%


/*
 *  Routines
 */

static int line_length = 0;
static char *line_read = NULL;
static char *line_ptr;

// Input function that uses libreadline for interactive input
static int ginsh_input(char *buf, int max_size)
{
	int result;
	if (yy_current_buffer->yy_is_interactive) {
		int actual;

		// Do we need to read a new line?
		if (line_length == 0) {

			// Free old line
			if (line_read)
				free(line_read);

			// Read new line, prompt "> "
			line_read = line_ptr = readline("> ");

			// EOF?
			if (!line_read) {
				line_length = 0;
				return YY_NULL;
			}

			// Add non-empty lines to history
			line_length = strlen(line_read) + 1;
			if (line_length > 1)
				add_history(line_read);

			// Reappend trailing '\n' which is stripped by readline()
			line_read[line_length - 1] = '\n';
		}

		// Copy data to lex buffer
		actual = line_length > max_size ? max_size : line_length;
		memcpy(buf, line_ptr, actual);
		line_length -= actual;
		line_ptr += actual;
		result = actual;

	} else if (((result = fread(buf, 1, max_size, yyin)) == 0) && ferror(yyin))
		YY_FATAL_ERROR("input in flex scanner failed");

	return result;
}

// Scanner terminates on EOF
int yywrap()
{
	return 1;
}