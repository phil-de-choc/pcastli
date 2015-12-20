/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 19 "pcastli.y" /* yacc.c:339  */

   #ifdef _WIN32
   #pragma warning(disable:4820 4255 4244 4702 4242 4710)
   #endif

   #include <stdio.h>

   #include "node.h"
   #include "tree.h"

#line 77 "pcastli.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "pcastli.tab.h".  */
#ifndef YY_YY_PCASTLI_TAB_H_INCLUDED
# define YY_YY_PCASTLI_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END_OF_INPUT = 258,
    EOL = 259,
    NUM = 260,
    STR = 261,
    VAR = 262,
    FUNCTION = 263,
    WHILE = 264,
    FOR = 265,
    IF = 266,
    ELSE = 267,
    PARENT = 268,
    CHILDSET = 269,
    PLUSPLUS = 270,
    MINUSMINUS = 271,
    STDIN = 272,
    STDOUT = 273,
    STDERR = 274,
    CHAR = 275,
    BYTE = 276,
    INT = 277,
    SIZE_T = 278,
    FLOAT = 279,
    DOUBLE = 280,
    SIGNED = 281,
    UNSIGNED = 282,
    SHORT = 283,
    LONG = 284,
    LOW = 285,
    OR = 286,
    AND = 287,
    EQ = 288,
    NE = 289,
    GT = 290,
    GE = 291,
    LT = 292,
    LE = 293,
    CAST = 294,
    NOT = 295,
    NEG = 296,
    PREPLUSPLUS = 297,
    PREMINUSMINUS = 298,
    POSTPLUSPLUS = 299,
    POSTMINUSMINUS = 300,
    DEREF = 301
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PCASTLI_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 175 "pcastli.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   448

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  173

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    50,    63,
      57,    58,    42,    41,    64,    40,    59,    43,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    56,
       2,    31,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    60,     2,    61,    44,     2,    62,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    54,     2,    55,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    32,    33,    34,    35,
      36,    37,    38,    39,    45,    46,    47,    48,    49,    51,
      52,    53
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    57,    57,    59,    60,    61,    62,    66,    67,    68,
      69,    70,    74,    75,    76,    80,    81,    85,    87,    91,
      92,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   122,   123,   124,   125,
     129,   130,   131,   132,   133,   137,   138,   139,   143,   144,
     145,   146,   150,   151,   152,   153,   154,   155,   159,   163,
     164,   165,   169,   170,   171,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   208,   209
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "END_OF_INPUT", "EOL", "NUM", "STR",
  "VAR", "FUNCTION", "WHILE", "FOR", "IF", "ELSE", "PARENT", "CHILDSET",
  "PLUSPLUS", "MINUSMINUS", "STDIN", "STDOUT", "STDERR", "CHAR", "BYTE",
  "INT", "SIZE_T", "FLOAT", "DOUBLE", "SIGNED", "UNSIGNED", "SHORT",
  "LONG", "LOW", "'='", "OR", "AND", "EQ", "NE", "GT", "GE", "LT", "LE",
  "'-'", "'+'", "'*'", "'/'", "'^'", "CAST", "NOT", "NEG", "PREPLUSPLUS",
  "PREMINUSMINUS", "'&'", "POSTPLUSPLUS", "POSTMINUSMINUS", "DEREF", "'{'",
  "'}'", "';'", "'('", "')'", "'.'", "'['", "']'", "'`'", "'\\''", "','",
  "$accept", "program", "statement", "statement_list",
  "selection_statement", "iteration_statement", "iteration_control", "exp",
  "symbol", "dotlist", "genlist", "objectacces", "codesegment", "formlist",
  "sublist", "type", "starlist", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,    61,   286,   287,   288,   289,   290,   291,   292,   293,
      45,    43,    42,    47,    94,   294,   295,   296,   297,   298,
      38,   299,   300,   301,   123,   125,    59,    40,    41,    46,
      91,    93,    96,    39,    44
};
# endif

#define YYPACT_NINF -45

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-45)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      10,   -45,    27,   -45,   -45,   -45,   -44,   -45,   -18,    18,
      23,    35,   -45,    40,    65,    65,   -45,   -45,   -45,    80,
      65,    80,    65,   222,   146,   222,    44,   -45,   -45,   379,
      -6,   -45,    42,   -42,    50,    80,   112,    80,    80,    80,
      80,   -44,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   204,
     -45,   -45,   -45,   -45,   -45,   -45,    54,    82,   119,    83,
     339,   -45,    71,   -45,   -45,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,   -45,   -45,
     -45,    80,    49,   133,    80,   118,    80,    77,   253,   -45,
      59,    85,   404,    89,   352,   266,   -45,   -45,   -45,   -45,
     124,    84,   -45,   -45,   125,    99,   -45,   -45,   -45,   127,
     -45,   -38,   -45,   282,   324,   140,   140,   157,   157,   157,
     157,   -14,   -14,   111,   111,   111,   404,   -45,    96,   -45,
     404,    75,   -45,    42,   296,    42,   -45,   222,   150,   222,
      80,   222,   -45,   -45,   -45,   136,   -45,   -45,   163,   -45,
     -45,    80,    80,   -45,    80,   -45,   -45,   -45,   -45,   131,
     177,   -45,   -45,   -45,   309,   404,    80,   222,   -45,   132,
     -45,   222,   -45
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     3,     0,     1,     4,    21,    22,    62,     0,     0,
       0,     0,    58,     0,     0,     0,    23,    24,    25,     0,
       0,     0,     0,    12,     0,     0,     0,    10,    11,     8,
      26,    54,    55,    56,    50,     0,    69,    19,    19,     0,
       0,     0,    46,    47,    32,    53,    43,    52,    13,     0,
      75,    78,    85,   101,   102,   103,     0,    87,    79,    89,
       0,   105,     0,     6,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     9,    48,
      49,     0,     0,     0,    72,     0,     0,     0,     0,    70,
       0,     0,    20,     0,     0,     0,     7,    14,    76,    86,
      81,    91,    77,    88,    83,    93,    80,    90,   104,    95,
      34,     0,    68,    42,    41,    37,    38,    40,    39,    35,
      36,    29,    28,    30,    31,    33,    27,    60,     0,    64,
      73,     0,    63,    57,     0,    51,    67,     0,     0,     0,
      19,     0,    59,    82,    92,    97,    84,    94,    99,    96,
     106,     0,     0,    65,     0,    66,    45,    71,    18,     0,
      15,    98,   100,    44,     0,    74,    19,     0,    61,     0,
      16,     0,    17
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -45,   -45,   -23,   -45,   -45,   -45,   -37,   -16,    -8,   -45,
     -21,   -45,   -45,   -45,   -45,   -45,   -45
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    26,    49,    27,    28,    91,    29,    30,    31,
      32,    33,    34,    90,   131,    61,   111
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      48,    93,    62,    44,   150,    46,    42,    43,    60,    79,
      80,    83,    45,     1,    47,    84,    35,    85,    86,    88,
     151,    92,    92,    94,    95,    81,    97,     3,    75,    76,
      77,     4,     5,     6,     7,     8,     9,    10,    11,    36,
      12,    13,    14,    15,    16,    17,    18,    63,    64,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   127,   128,   133,   126,   135,    19,   130,    20,
     134,    41,     7,    21,    98,    37,    99,    22,    12,    13,
      38,    23,   100,   101,    24,     5,     6,     7,     8,    25,
      12,    13,    39,    12,    13,    14,    15,    16,    17,    18,
      40,    82,   102,   159,   103,   107,   144,    20,   108,    87,
     104,   105,   109,   145,   156,    22,   158,   137,   160,    89,
      19,   147,    20,   138,    92,   132,    21,    25,   148,   169,
      22,    12,    13,   153,   112,   163,   164,    24,   165,   154,
     129,   106,    25,   139,   170,   140,   143,   146,   172,   149,
      92,     5,     6,     7,     8,    77,   152,   157,   161,    12,
      13,    14,    15,    16,    17,    18,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    69,    70,    71,    72,
      73,    74,    75,    76,    77,   162,    19,   166,    20,   167,
     171,     0,    21,     0,     0,     0,    22,    73,    74,    75,
      76,    77,     0,    24,     0,     0,     0,     0,    25,     5,
       6,     7,     8,     9,    10,    11,     0,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    11,     0,    12,    13,    14,    15,    16,
      17,    18,     0,     0,    19,     0,    20,     0,     0,     0,
      21,     0,     0,     0,    22,     0,     0,     0,    23,    96,
       0,    24,    19,     0,    20,     0,    25,     0,    21,     0,
       0,     0,    22,     0,     0,     0,    23,     0,     0,    24,
       0,     0,     0,     0,    25,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,   136,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   142,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,   155,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     0,
     168,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     141,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    78,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77
};

static const yytype_int16 yycheck[] =
{
      23,    38,    25,    19,    42,    21,    14,    15,    24,    15,
      16,    53,    20,     3,    22,    57,    60,    59,    60,    35,
      58,    37,    38,    39,    40,    31,    49,     0,    42,    43,
      44,     4,     5,     6,     7,     8,     9,    10,    11,    57,
      13,    14,    15,    16,    17,    18,    19,     3,     4,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    13,    14,    85,    81,    87,    40,    84,    42,
      86,     6,     7,    46,    20,    57,    22,    50,    13,    14,
      57,    54,    28,    29,    57,     5,     6,     7,     8,    62,
      13,    14,    57,    13,    14,    15,    16,    17,    18,    19,
      60,    59,    20,   140,    22,    22,    22,    42,    25,    59,
      28,    29,    29,    29,   137,    50,   139,    58,   141,     7,
      40,    22,    42,    64,   140,     7,    46,    62,    29,   166,
      50,    13,    14,    58,    63,   151,   152,    57,   154,    64,
       7,    22,    62,    58,   167,    56,    22,    22,   171,    22,
     166,     5,     6,     7,     8,    44,    60,     7,    22,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    22,    40,    56,    42,    12,
      58,    -1,    46,    -1,    -1,    -1,    50,    40,    41,    42,
      43,    44,    -1,    57,    -1,    -1,    -1,    -1,    62,     5,
       6,     7,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    40,    -1,    42,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    -1,    54,    55,
      -1,    57,    40,    -1,    42,    -1,    62,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    62,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    61,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    61,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    -1,    -1,    61,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      61,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    66,     0,     4,     5,     6,     7,     8,     9,
      10,    11,    13,    14,    15,    16,    17,    18,    19,    40,
      42,    46,    50,    54,    57,    62,    67,    69,    70,    72,
      73,    74,    75,    76,    77,    60,    57,    57,    57,    57,
      60,     6,    73,    73,    72,    73,    72,    73,    67,    68,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      72,    80,    67,     3,     4,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    56,    15,
      16,    31,    59,    53,    57,    59,    60,    59,    72,     7,
      78,    71,    72,    71,    72,    72,    55,    67,    20,    22,
      28,    29,    20,    22,    28,    29,    22,    22,    25,    29,
      58,    81,    63,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    13,    14,     7,
      72,    79,     7,    75,    72,    75,    61,    58,    64,    58,
      56,    58,    61,    22,    22,    29,    22,    22,    29,    22,
      42,    58,    60,    58,    64,    61,    67,     7,    67,    71,
      67,    22,    22,    72,    72,    72,    56,    12,    61,    71,
      67,    58,    67
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    65,    66,    66,    66,    66,    66,    67,    67,    67,
      67,    67,    68,    68,    68,    69,    69,    70,    70,    71,
      71,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      73,    73,    73,    73,    73,    74,    74,    74,    75,    75,
      75,    75,    76,    76,    76,    76,    76,    76,    77,    78,
      78,    78,    79,    79,    79,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    81,    81
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     2,     3,     3,     3,     1,     2,
       1,     1,     0,     1,     2,     5,     7,     9,     5,     0,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     5,     5,     2,     2,     2,     2,
       1,     3,     2,     2,     1,     1,     1,     3,     1,     4,
       3,     6,     1,     3,     3,     4,     4,     4,     3,     0,
       1,     3,     0,     1,     3,     1,     2,     2,     1,     1,
       2,     2,     3,     2,     3,     1,     2,     1,     2,     1,
       2,     2,     3,     2,     3,     2,     3,     3,     4,     3,
       4,     1,     1,     1,     2,     0,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 59 "pcastli.y" /* yacc.c:1646  */
    { xxsetroot(NULL); YYACCEPT; }
#line 1432 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 60 "pcastli.y" /* yacc.c:1646  */
    { xxsetroot(NULL); YYACCEPT; }
#line 1438 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 61 "pcastli.y" /* yacc.c:1646  */
    { xxsetroot((yyvsp[-1])); YYACCEPT; }
#line 1444 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 62 "pcastli.y" /* yacc.c:1646  */
    { xxsetroot((yyvsp[-1])); YYACCEPT; }
#line 1450 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 66 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 1456 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 67 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1462 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 68 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 1468 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 69 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1474 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 70 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1480 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 74 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxemptylist(); }
#line 1486 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 75 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxfirstitem((yyvsp[0])); }
#line 1492 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 76 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxadditem((yyvsp[-1]),(yyvsp[0])); }
#line 1498 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 80 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xx2childs((yyvsp[-2]),(yyvsp[0]),NT_IF_STMT); }
#line 1504 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 81 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxifelse((yyvsp[-4]),(yyvsp[-2]),(yyvsp[0])); }
#line 1510 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 86 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxfor((yyvsp[-6]),(yyvsp[-4]),(yyvsp[-2]),(yyvsp[0])); }
#line 1516 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 87 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xx2childs((yyvsp[-2]),(yyvsp[0]),NT_WHILE_STMT); }
#line 1522 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 91 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxone(); }
#line 1528 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 92 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1534 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 96 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);                 }
#line 1540 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 97 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);                 }
#line 1546 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 98 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);                 }
#line 1552 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 99 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);                 }
#line 1558 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 100 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);                 }
#line 1564 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 101 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);                 }
#line 1570 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 102 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1576 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 103 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1582 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 104 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1588 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 105 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1594 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 106 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1600 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 107 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));     }
#line 1606 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 108 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1612 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 109 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]);                 }
#line 1618 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 110 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1624 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 111 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1630 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 112 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1636 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 113 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1642 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 114 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1648 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 115 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1654 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 116 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1660 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 117 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxbinary((yyvsp[-1]),(yyvsp[-2]),(yyvsp[0])); }
#line 1666 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 118 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0]));     }
#line 1672 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 119 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxcast((yyvsp[-3]),(yyvsp[-2]),(yyvsp[0]));             }
#line 1678 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 121 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xx2childs((yyvsp[-2]),(yyvsp[0]),NT_FUNC_DEF); }
#line 1684 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 122 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxppmm((yyvsp[0]), PREPLUSPLUS);      }
#line 1690 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 123 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxppmm((yyvsp[0]), PREMINUSMINUS);    }
#line 1696 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 124 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxppmm((yyvsp[-1]), POSTPLUSPLUS);     }
#line 1702 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 125 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxppmm((yyvsp[-1]), POSTMINUSMINUS);   }
#line 1708 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 129 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1714 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 130 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xx2childs((yyvsp[-2]),(yyvsp[0]),NT_VARGENLIST); }
#line 1720 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 131 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0])); }
#line 1726 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 132 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxunary((yyvsp[-1]),(yyvsp[0])); }
#line 1732 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 133 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1738 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 137 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1744 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 138 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1750 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 139 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xx2childs((yyvsp[-2]),(yyvsp[0]),NT_VARGENLIST); }
#line 1756 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 143 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxfirstitem((yyvsp[0])); }
#line 1762 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 144 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxfirstchildset((yyvsp[-3]),(yyvsp[-1])); }
#line 1768 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 145 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxadditem((yyvsp[-2]),(yyvsp[0])); }
#line 1774 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 146 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxaddchildset((yyvsp[-5]),(yyvsp[-3]),(yyvsp[-1])); }
#line 1780 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 150 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1786 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 151 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxaddmember((yyvsp[-2]),(yyvsp[0]),NT_REF); }
#line 1792 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 152 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxaddmember((yyvsp[-2]),(yyvsp[0]),NT_DEREF); }
#line 1798 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 153 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xx2childs((yyvsp[-3]),(yyvsp[-1]),NT_FUNC_CALL); }
#line 1804 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 154 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xx2childs((yyvsp[-3]),(yyvsp[-1]),NT_SUBSCRIPT); }
#line 1810 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 155 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xx2childs((yyvsp[-3]),(yyvsp[-1]),NT_SUBSCRIPT); }
#line 1816 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 159 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxcodesegment((yyvsp[-1]));  }
#line 1822 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 163 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxemptylist(); }
#line 1828 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 164 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxfirstitem((yyvsp[0])); }
#line 1834 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 165 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxadditem((yyvsp[-2]),(yyvsp[0])); }
#line 1840 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 169 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxemptylist(); }
#line 1846 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 170 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxfirstitem((yyvsp[0])); }
#line 1852 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 171 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxadditem((yyvsp[-2]),(yyvsp[0])); }
#line 1858 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 175 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_CHAR);        }
#line 1864 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 176 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_S_CHAR);      }
#line 1870 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 177 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_U_CHAR);      }
#line 1876 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 178 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_BYTE);        }
#line 1882 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 179 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_SHORT);       }
#line 1888 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 180 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_SHORT);       }
#line 1894 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 181 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_SHORT);       }
#line 1900 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 182 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_SHORT);       }
#line 1906 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 183 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_U_SHORT);     }
#line 1912 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 184 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_U_SHORT);     }
#line 1918 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 185 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_INT);         }
#line 1924 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 186 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_INT);         }
#line 1930 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 187 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_U_INT);       }
#line 1936 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 188 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_U_INT);       }
#line 1942 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 189 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_LONG);        }
#line 1948 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 190 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_LONG);        }
#line 1954 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 191 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_LONG);        }
#line 1960 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 192 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_LONG);        }
#line 1966 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 193 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_U_LONG);      }
#line 1972 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 194 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_U_LONG);      }
#line 1978 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 195 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_LONG_LONG);   }
#line 1984 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 196 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_LONG_LONG);   }
#line 1990 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 197 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_LONG_LONG);   }
#line 1996 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 198 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_LONG_LONG);   }
#line 2002 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 199 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_U_LONG_LONG); }
#line 2008 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 200 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_U_LONG_LONG); }
#line 2014 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 201 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_SIZE_T);      }
#line 2020 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 202 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_FLOAT);       }
#line 2026 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 203 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_DOUBLE);      }
#line 2032 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 204 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxtype(DT_LONG_DOUBLE); }
#line 2038 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 208 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxemptylist(); }
#line 2044 "pcastli.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 209 "pcastli.y" /* yacc.c:1646  */
    { (yyval) = xxadditem((yyvsp[-1]),(yyvsp[0])); }
#line 2050 "pcastli.tab.c" /* yacc.c:1646  */
    break;


#line 2054 "pcastli.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 211 "pcastli.y" /* yacc.c:1906  */




/* Called by yyparse on error.  */
void yyerror(char const *s)
{
   fprintf(stderr, "%s\n", s);
}
