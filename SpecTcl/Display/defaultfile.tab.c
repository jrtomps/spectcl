
/* A Bison parser, made from defaultfile.y
   by GNU bison 1.30.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse defaultfileparse
#define yylex defaultfilelex
#define yyerror defaultfileerror
#define yylval defaultfilelval
#define yychar defaultfilechar
#define yydebug defaultfiledebug
#define yynerrs defaultfilenerrs
# define	NAME	257
# define	NUMBER	258
# define	DESCRIPTION	259
# define	PEAK	260
# define	UPDATE	261
# define	OBJECTS	262
# define	FLIPPED	263
# define	NOFLIPPED	264
# define	TITLE	265
# define	GEOMETRY	266
# define	ZOOMED	267
# define	WINDOW	268
# define	AXES	269
# define	NOAXES	270
# define	TICKS	271
# define	LABELS	272
# define	LABEL	273
# define	NOLABEL	274
# define	REDUCTION	275
# define	SAMPLED	276
# define	SUMMED	277
# define	AVERAGED	278
# define	SCALE	279
# define	AUTO	280
# define	MANUAL	281
# define	COUNTSAXIS	282
# define	LOG	283
# define	LINEAR	284
# define	FLOOR	285
# define	CEILING	286
# define	EXPANDED	287
# define	RENDITION	288
# define	SMOOTHED	289
# define	HISTOGRAM	290
# define	POINTS	291
# define	LINE	292
# define	SCATTER	293
# define	BOX	294
# define	COLOR	295
# define	CONTOUR	296
# define	LEGO	297
# define	REFRESH	298
# define	INTEGER	299
# define	COMMA	300
# define	ENDWINDOW	301
# define	ENDLINE	302
# define	ATTRIBUTES	303
# define	ENDATTRIBUTES	304
# define	RENDITION_1D	305
# define	RENDITION_2D	306
# define	SUPERIMPOSE	307
# define	GROBJFONT	308
# define	UNMATCHED	309
# define	DEFAULTFILE	310
# define	QSTRING	311

#line 1 "defaultfile.y"

/*
**  Facility:
**    NSCL display system window file I/O subsystem.
**  Abstract:
**    winfile.y   - This file contains a YACC/BISON description of the
**                  grammar of the context free language which is used to
**                  describe window save files.
** Author:
**   Ron Fox
**   NSCL
**   Michigan State University
**   East Lansing, MI 48824-1321
** Revision level:
**   @(#)defaultfile.y	2.1 12/22/93 
*/
static char *yaccrevlevel = "@(#)defaultfile.y	2.1 12/22/93  - windows file parser\n";
extern void windfileerror(char *text);
/* #define SHARED extern */
#include <assert.h>
#ifdef unix
#include <memory.h>
#endif
#ifdef VMS
#include <stdlib.h>
#endif
#include <stdio.h>

#include "windio.h"
#include "dispwind.h"
#include "dispshare.h"
#include "dfltmgr.h"
#include "grobjdisplay.h"
#include "panemgr.h"
#define defaultfileerror(text) windfileerror((text))
#define yylex         defaultfilelex
#define windfilelex() defaultfilelex
#define yyleng dfyyleng
#define yytext dfyytext
#define getlexrev() dfgetlexrev()
#define get_title() dfget_title()
#define win_startlex() def_startlex()
#define yynerrs defaultfilenerrs
static int ticks = 0, 
    labels= 0, 
    axes  = 0;	/* Axis label attribute flags. */
static int name        = 0, 
    number      = 0, 
    description = 0, 
    peak        = 0, 
    update      = 0, 
    objects     = 0; /* Title attr. flags */
static struct limit { int low;
		int high;
	      } xlim, ylim, limits;	/* Limits structures. */


extern int windfilelex_line;
static win_attributed *current = Xamine_GetDefaultGenericAttributes();


#line 62 "defaultfile.y"
typedef union {
   int integer;
   char string[80];
 } YYSTYPE;
#include <stdio.h>



#define	YYFINAL		109
#define	YYFLAG		-32768
#define	YYNTBASE	58

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 311 ? yytranslate[x] : 86)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57
};

#if YYDEBUG != 0
static const short yyprhs[] =
{
       0,     0,     2,     5,     7,    11,    16,    21,    27,    30,
      33,    35,    38,    40,    42,    44,    46,    48,    50,    52,
      54,    56,    60,    64,    67,    70,    72,    75,    77,    79,
      81,    84,    87,    91,    95,    97,   100,   102,   104,   106,
     108,   110,   112,   115,   118,   122,   124,   126,   128,   132,
     134,   137,   141,   143,   145,   149,   153,   157,   164,   166,
     168,   170,   172,   174,   176,   178,   180,   182,   186,   188
};
static const short yyrhs[] =
{
      59,     0,    85,    59,     0,    60,     0,    61,    62,    81,
       0,    61,    63,    62,    81,     0,    61,    62,    81,    84,
       0,    61,    63,    62,    81,    84,     0,    49,    85,     0,
      50,    85,     0,    64,     0,    63,    64,     0,    65,     0,
      68,     0,    71,     0,    72,     0,    74,     0,    76,     0,
      78,     0,    79,     0,    80,     0,    15,    66,    85,     0,
      16,    66,    85,     0,    15,    85,     0,    16,    85,     0,
      67,     0,    66,    67,     0,    17,     0,    18,     0,    15,
       0,    19,    85,     0,    20,    85,     0,    19,    69,    85,
       0,    20,    69,    85,     0,    70,     0,    69,    70,     0,
       3,     0,     4,     0,     5,     0,     6,     0,     7,     0,
       8,     0,     9,    85,     0,    10,    85,     0,    21,    73,
      85,     0,    22,     0,    23,     0,    24,     0,    25,    75,
      85,     0,    26,     0,    27,    45,     0,    28,    77,    85,
       0,    29,     0,    30,     0,    31,    45,    85,     0,    32,
      45,    85,     0,    44,    45,    85,     0,    51,    82,    85,
      52,    83,    85,     0,    35,     0,    36,     0,    37,     0,
      38,     0,    39,     0,    40,     0,    41,     0,    42,     0,
      43,     0,    54,    45,    85,     0,    48,     0,    48,    85,
       0
};

#endif

#if YYDEBUG != 0
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   125,   125,   128,   131,   132,   134,   135,   140,   143,
     146,   146,   149,   149,   149,   150,   150,   150,   151,   151,
     152,   155,   163,   170,   175,   183,   184,   187,   191,   195,
     201,   206,   212,   223,   236,   236,   239,   243,   247,   250,
     253,   256,   261,   267,   274,   277,   281,   285,   291,   294,
     298,   305,   308,   312,   318,   325,   332,   340,   343,   347,
     351,   355,   361,   365,   369,   373,   377,   383,   391,   391
};
#endif


#if YYDEBUG != 0 || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "NAME", "NUMBER", "DESCRIPTION", "PEAK", 
  "UPDATE", "OBJECTS", "FLIPPED", "NOFLIPPED", "TITLE", "GEOMETRY", 
  "ZOOMED", "WINDOW", "AXES", "NOAXES", "TICKS", "LABELS", "LABEL", 
  "NOLABEL", "REDUCTION", "SAMPLED", "SUMMED", "AVERAGED", "SCALE", 
  "AUTO", "MANUAL", "COUNTSAXIS", "LOG", "LINEAR", "FLOOR", "CEILING", 
  "EXPANDED", "RENDITION", "SMOOTHED", "HISTOGRAM", "POINTS", "LINE", 
  "SCATTER", "BOX", "COLOR", "CONTOUR", "LEGO", "REFRESH", "INTEGER", 
  "COMMA", "ENDWINDOW", "ENDLINE", "ATTRIBUTES", "ENDATTRIBUTES", 
  "RENDITION_1D", "RENDITION_2D", "SUPERIMPOSE", "GROBJFONT", "UNMATCHED", 
  "DEFAULTFILE", "QSTRING", "setup_file", "setup_filel", "description", 
  "window_clause", "endwindow_clause", "attribute_clauses", 
  "attribute_clause", "axes_clause", "axis_attributes", "axis_attribute", 
  "labels_clause", "label_attributes", "label_attribute", 
  "flipped_clause", "reduction_clause", "reduction_attribute", 
  "scale_clause", "scale_attribute", "countsaxis_clause", 
  "countsaxis_attributes", "floor_clause", "ceiling_clause", 
  "refresh_clause", "rendition_clauses", "rend1_attribute", 
  "rend2_attribute", "font_clauses", "blankline", NULL
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    58,    58,    59,    60,    60,    60,    60,    61,    62,
      63,    63,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    65,    65,    65,    65,    66,    66,    67,    67,    67,
      68,    68,    68,    68,    69,    69,    70,    70,    70,    70,
      70,    70,    71,    71,    72,    73,    73,    73,    74,    75,
      75,    76,    77,    77,    78,    79,    80,    81,    82,    82,
      82,    82,    83,    83,    83,    83,    83,    84,    85,    85
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     2,     1,     3,     4,     4,     5,     2,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     2,     1,     2,     1,     1,     1,
       2,     2,     3,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     1,     1,     1,     3,     1,
       2,     3,     1,     1,     3,     3,     3,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     2
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,    68,     0,     1,     3,     0,     0,    69,     8,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     2,    42,    43,    29,    27,    28,
       0,    25,    23,     0,    24,    36,    37,    38,    39,    40,
      41,     0,    34,    30,     0,    31,    45,    46,    47,     0,
      49,     0,     0,    52,    53,     0,     0,     0,     0,     9,
       0,     4,     0,    11,    26,    21,    22,    35,    32,    33,
      44,    50,    48,    51,    54,    55,    56,    58,    59,    60,
      61,     0,     0,     6,     5,     0,     0,     7,     0,    67,
      62,    63,    64,    65,    66,     0,    57,     0,     0,     0
};

static const short yydefgoto[] =
{
     107,     3,     4,     5,    22,    23,    24,    25,    40,    41,
      26,    51,    52,    27,    28,    59,    29,    62,    30,    65,
      31,    32,    33,    71,    91,   105,    93,     6
};

static const short yypact[] =
{
     -19,   -41,   -41,-32768,-32768,    12,   -15,-32768,-32768,   -41,
     -41,   -13,   -13,    11,    11,    46,    45,    44,    -9,    -7,
      -4,   -41,     1,    12,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     -13,-32768,-32768,   -13,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,    11,-32768,-32768,    11,-32768,-32768,-32768,-32768,   -41,
  -32768,    10,   -41,-32768,-32768,   -41,   -41,   -41,   -41,-32768,
     -12,     3,     1,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   -41,    15,-32768,     3,    23,   -41,-32768,     6,-32768,
  -32768,-32768,-32768,-32768,-32768,   -41,-32768,    63,    76,-32768
};

static const short yypgoto[] =
{
  -32768,    71,-32768,-32768,    55,-32768,    56,-32768,    68,   -37,
  -32768,    67,     0,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,    13,-32768,-32768,   -11,    -1
};


#define	YYLAST		104


static const short yytable[] =
{
       7,     8,    37,    74,    38,    39,    74,     1,    35,    36,
      42,    44,    53,    55,    45,    46,    47,    48,    49,    50,
      69,     9,    10,    87,    88,    89,    90,    11,    12,     1,
       2,    13,    14,    15,     2,     1,    66,    16,    67,    75,
      17,    68,    76,    18,    19,   100,   101,   102,   103,   104,
      78,    77,    70,    79,    77,    81,    20,    92,    80,     1,
      96,    82,    21,   108,    83,    84,    85,    86,    56,    57,
      58,    60,    61,    63,    64,    98,   109,    34,    72,    73,
      43,    54,     0,    97,     0,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,   106
};

static const short yycheck[] =
{
       1,     2,    15,    40,    17,    18,    43,    48,     9,    10,
      11,    12,    13,    14,     3,     4,     5,     6,     7,     8,
      21,     9,    10,    35,    36,    37,    38,    15,    16,    48,
      49,    19,    20,    21,    49,    48,    45,    25,    45,    40,
      28,    45,    43,    31,    32,    39,    40,    41,    42,    43,
      51,    51,    51,    54,    54,    45,    44,    54,    59,    48,
      45,    62,    50,     0,    65,    66,    67,    68,    22,    23,
      24,    26,    27,    29,    30,    52,     0,     6,    23,    23,
      12,    14,    -1,    94,    -1,    72,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/soft/alpha/bison/share/bison/bison.simple"

/* Skeleton output parser for bison,
   Copyright 1984, 1989, 1990, 2000, 2001 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
# ifdef alloca
#  define YYSTACK_USE_ALLOCA 1
# else /* alloca not defined */
#  ifdef __GNUC__
#   define YYSTACK_USE_ALLOCA 1
#   define alloca __builtin_alloca
#  else /* not GNU C.  */
#   if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#    define YYSTACK_USE_ALLOCA 1
#    include <alloca.h>
#   else /* not sparc */
     /* We think this test detects Watcom and Microsoft C.  */
     /* This used to test MSDOS, but that is a bad idea since that
	symbol is in the user namespace.  */
#    if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#     if 0
       /* No need for malloc.h, which pollutes the namespace; instead,
	  just don't use alloca.  */
#      include <malloc.h>
#     endif
#    else /* not MSDOS, or __TURBOC__ */
#     if defined(_AIX)
       /* I don't know what this was needed for, but it pollutes the
	  namespace.  So I turned it off.  rms, 2 May 1997.  */
       /* #include <malloc.h>  */
 #pragma alloca
#      define YYSTACK_USE_ALLOCA 1
#     else /* not MSDOS, or __TURBOC__, or _AIX */
#      if 0
	/* haible@ilog.fr says this works for HPUX 9.05 and up, and on
	   HPUX 10.  Eventually we can turn this on.  */
#       ifdef __hpux
#        define YYSTACK_USE_ALLOCA 1
#        define alloca __builtin_alloca
#  	endif /* __hpux */
#      endif
#     endif /* not _AIX */
#    endif /* not MSDOS, or __TURBOC__ */
#   endif /* not sparc */
#  endif /* not GNU C */
# endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifndef YYSTACK_USE_ALLOCA
# define YYSTACK_USE_ALLOCA 0
#endif

#if YYSTACK_USE_ALLOCA
# define YYSTACK_ALLOC alloca
#else
# define YYSTACK_ALLOC malloc
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG
# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    fprintf Args;				\
} while (0)
/* Nonzero means print parse trace. [The following comment makes no
   sense to me.  Could someone clarify it?  --akim] Since this is
   uninitialized, it does not stop multiple parsers from coexisting.
   */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).  */
#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
# define __yy_memcpy(To, From, Count)	__builtin_memcpy (To, From, Count)
#else				/* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
# ifndef __cplusplus
__yy_memcpy (to, from, count)
     char *to;
     const char *from;
     unsigned int count;
# else /* __cplusplus */
__yy_memcpy (char *to, const char *from, unsigned int count)
# endif
{
  register const char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif

#line 216 "/soft/alpha/bison/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# ifdef __cplusplus
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else /* !__cplusplus */
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif /* !__cplusplus */
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define _YY_DECL_VARIABLES				\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
_YY_DECL_VARIABLES				\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
_YY_DECL_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yysv': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
# if YYLSP_NEEDED
  YYLTYPE yyloc;
# endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack. Use copies of
	 these so that the &'s don't force the real ones into memory.
	 */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#if YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of the
	 data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow ("parser stack overflow",
		  &yyss1, size * sizeof (*yyssp),
		  &yyvs1, size * sizeof (*yyvsp),
		  &yyls1, size * sizeof (*yylsp),
		  &yystacksize);
# else
      yyoverflow ("parser stack overflow",
		  &yyss1, size * sizeof (*yyssp),
		  &yyvs1, size * sizeof (*yyvsp),
		  &yystacksize);
# endif

      yyss = yyss1; yyvs = yyvs1;
# if YYLSP_NEEDED
      yyls = yyls1;
# endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror ("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
# if YYLSP_NEEDED
	      free (yyls);
# endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
# if !YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
# endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
# if YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
# endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#if YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %d\n", yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 21:
#line 156 "defaultfile.y"
{
		    assert(current != NULL);
		    if(ticks)  current->tickson();
		    if(labels) current->axis_labelson();
		    if(axes)   current->axeson();
		    ticks = labels = axes = 0;
		  ;
    break;}
case 22:
#line 164 "defaultfile.y"
{
		    if(ticks) current->ticksoff();
		    if(labels)current->axis_labelsoff();
		    if(axes)  current->axesoff();
		    ticks = labels = axes = 0;
		  ;
    break;}
case 23:
#line 171 "defaultfile.y"
{
		   assert(current != NULL);
		   current->set_axes(TRUE, TRUE, TRUE);
		 ;
    break;}
case 24:
#line 176 "defaultfile.y"
{
		   assert(current != NULL);
		   current->set_axes(FALSE,FALSE,FALSE);
		 ;
    break;}
case 27:
#line 188 "defaultfile.y"
{
		     ticks = TRUE;
		   ;
    break;}
case 28:
#line 192 "defaultfile.y"
{
		     labels = TRUE;
		   ;
    break;}
case 29:
#line 196 "defaultfile.y"
{
		     axes = TRUE;
		   ;
    break;}
case 30:
#line 202 "defaultfile.y"
{
		   assert(current != NULL);
		   current->set_titles(TRUE, TRUE, TRUE, TRUE, TRUE, TRUE);
		 ;
    break;}
case 31:
#line 207 "defaultfile.y"
{
		   assert(current != NULL);
		   current->set_titles(FALSE, FALSE, FALSE,
					 FALSE, FALSE, FALSE);
		 ;
    break;}
case 32:
#line 213 "defaultfile.y"
{
		    assert(current != NULL);
		    if(name)        current->dispname();
		    if(number)      current->dispid();
		    if(description) current->dispid();
		    if(peak)        current->disppeak();
		    if(update)      current->dispupd();
		    if(objects)     current->labelobj();
		    name = number = description = peak = update = objects = 0;
		  ;
    break;}
case 33:
#line 224 "defaultfile.y"
{
		    assert(current != NULL);
		    if(name)        current->hidename();
		    if(number)      current->hideid();
		    if(description) current->hidedescr();
		    if(peak)        current->hidepeak();
		    if(update)      current->hideupd();
		    if(objects)     current->unlabelobj();
		    name = number = description = peak = update = objects = 0;
		  ;
    break;}
case 36:
#line 240 "defaultfile.y"
{
		     name = TRUE;
		   ;
    break;}
case 37:
#line 244 "defaultfile.y"
{
		     number = TRUE;
		   ;
    break;}
case 38:
#line 247 "defaultfile.y"
{
		  description = TRUE;
		;
    break;}
case 39:
#line 250 "defaultfile.y"
{
		  peak  = TRUE;
		;
    break;}
case 40:
#line 253 "defaultfile.y"
{
		  update = TRUE;
		;
    break;}
case 41:
#line 256 "defaultfile.y"
{
		  objects = TRUE;
		;
    break;}
case 42:
#line 262 "defaultfile.y"
{ 
		  assert(current != NULL);
		  current->sideways();
		;
    break;}
case 43:
#line 268 "defaultfile.y"
{
		  assert(current != NULL);
		  current->normal();
		;
    break;}
case 45:
#line 277 "defaultfile.y"
{
                        assert(current != NULL);
			current->sample();
		      ;
    break;}
case 46:
#line 281 "defaultfile.y"
{
		      assert(current != NULL);
		      current->sum();
		    ;
    break;}
case 47:
#line 285 "defaultfile.y"
{
		      assert(current != NULL);
		      current->average();
		    ;
    break;}
case 49:
#line 294 "defaultfile.y"
{
                       assert(current != NULL);
		       current->autoscale();
		     ;
    break;}
case 50:
#line 299 "defaultfile.y"
{
		   assert(current != NULL);
		   current->setfs(yyvsp[0].integer);
		 ;
    break;}
case 52:
#line 308 "defaultfile.y"
{
			     assert(current != NULL);
			     current->log();
			   ;
    break;}
case 53:
#line 312 "defaultfile.y"
{
			 assert(current != NULL);
			 current->linear();
		       ;
    break;}
case 54:
#line 319 "defaultfile.y"
{
		   assert(current != NULL);
		   current->setfloor(yyvsp[-1].integer);
		 ;
    break;}
case 55:
#line 326 "defaultfile.y"
{
		  assert(current != NULL);
		  current->setceiling(yyvsp[-1].integer);
		;
    break;}
case 56:
#line 333 "defaultfile.y"
{
		  assert(current != NULL);
		  current->update_interval(yyvsp[-1].integer);	/* Set the update interval. */
                ;
    break;}
case 58:
#line 344 "defaultfile.y"
{ 
		    Xamine_SetDefault1DRendition(smoothed);
		  ;
    break;}
case 59:
#line 348 "defaultfile.y"
{
		    Xamine_SetDefault1DRendition(histogram);
		  ;
    break;}
case 60:
#line 352 "defaultfile.y"
{ 
		    Xamine_SetDefault1DRendition(points);
		  ;
    break;}
case 61:
#line 356 "defaultfile.y"
{
		    Xamine_SetDefault1DRendition(lines);
		  ;
    break;}
case 62:
#line 362 "defaultfile.y"
{ 
		    Xamine_SetDefault2DRendition(scatter);
		  ;
    break;}
case 63:
#line 366 "defaultfile.y"
{ 
		    Xamine_SetDefault2DRendition(boxes);
		  ;
    break;}
case 64:
#line 370 "defaultfile.y"
{ 
		    Xamine_SetDefault2DRendition(color);
		  ;
    break;}
case 65:
#line 374 "defaultfile.y"
{
		    Xamine_SetDefault2DRendition(contour);
		  ;
    break;}
case 66:
#line 378 "defaultfile.y"
{
		    Xamine_SetDefault2DRendition(lego);
		  ;
    break;}
case 67:
#line 384 "defaultfile.y"
{
		 Display *d;
		 d = XtDisplay(Xamine_Getpanemgr()->getid());
		 Xamine_SetObjectLabelIndex(d, yyvsp[-1].integer);
               ;
    break;}
}

#line 610 "/soft/alpha/bison/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (int) (sizeof (yytname) / sizeof (char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen (yytname[x]) + 15, count++;
	  size += strlen ("parse error, unexpected `") + 1;
	  size += strlen (yytname[YYTRANSLATE (yychar)]);
	  msg = (char *) malloc (size);
	  if (msg != 0)
	    {
	      strcpy (msg, "parse error, unexpected `");
	      strcat (msg, yytname[YYTRANSLATE (yychar)]);
	      strcat (msg, "'");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (int) (sizeof (yytname) / sizeof (char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat (msg, count == 0 ? ", expecting `" : " or `");
			strcat (msg, yytname[x]);
			strcat (msg, "'");
			count++;
		      }
		}
	      yyerror (msg);
	      free (msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#if YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#if YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 393 "defaultfile.y"

