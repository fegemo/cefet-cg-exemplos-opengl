#ifndef _TOKSPLIT_H_
#define _TOKSPLIT_H_

#include <stddef.h>

/* copy over the next token from an input string, after
skipping leading blanks (or other whitespace?). The
token is terminated by the first appearance of tokchar,
or by the end of the source string.

The caller must supply sufficient space in token to
receive any token, Otherwise tokens will be truncated.

Returns: a pointer past the terminating tokchar.

This will happily return an infinity of empty tokens if
called with src pointing to the end of a string. Tokens
will never include a copy of tokchar.

released to Public Domain, by C.B. Falconer.
Published 2006-02-20. Attribution appreciated.
*/

const char *toksplit(const char *src, /* Source of tokens */
                     char tokchar, /* token delimiting char */
                     char *token, /* receiver of parsed token */
                     size_t lgh); /* length token can receive */
/* not including final '\0' */

#endif
