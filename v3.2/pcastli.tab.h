/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
    NO_ELSE = 285,
    LOW = 286,
    OR = 287,
    AND = 288,
    EQ = 289,
    NE = 290,
    GT = 291,
    GE = 292,
    LT = 293,
    LE = 294,
    CAST = 295,
    NOT = 296,
    NEG = 297,
    PREPLUSPLUS = 298,
    PREMINUSMINUS = 299,
    POSTPLUSPLUS = 300,
    POSTMINUSMINUS = 301,
    DEREF = 302
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
