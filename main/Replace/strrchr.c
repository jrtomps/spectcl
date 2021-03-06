/* Portable version of strrchr().
   This function is in the public domain. */

/*

@deftypefn Supplemental char* strrchr (const char *@var{s}, int @var{c})

Returns a pointer to the last occurrence of the character @var{c} in
the string @var{s}, or @code{NULL} if not found.  If @var{c} is itself the
null character, the results are undefined.

@end deftypefn

*/

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <ansidecl.h>

char *
strrchr (s, c)
  register const char *s;
  int c;
{
  char *rtnval = 0;

  do {
    if (*s == c)
      rtnval = (char*) s;
  } while (*s++);
  return (rtnval);
}
