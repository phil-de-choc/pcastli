/*
 * by Parent and Childset Accessible Syntax Tree Language Interpreter
 * Copyright (C) 2008-2015  Philippe Choquette
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

%{
   #ifdef _WIN32
   #pragma warning(disable:4820 4255 4244 4702 4242 4710)
   #endif

   #include <stdio.h>

   #include "node.h"
   #include "tree.h"
%}


%token END_OF_INPUT EOL
%token NUM STR VAR FUNCTION
%token WHILE FOR IF ELSE
%token PARENT CHILDSET
%token PLUSPLUS MINUSMINUS
%token STDIN STDOUT STDERR
%token CHAR BYTE INT SIZE_T FLOAT DOUBLE
%token SIGNED UNSIGNED SHORT LONG

%left  LOW
%right '='
%left  OR
%left  AND
%left  EQ NE
%left  GT GE LT LE
%left  '-' '+'
%left  '*' '/' '%'
%right '^' /* exponentiation */
%right CAST
%right NOT /* negation */ NEG /* unary minus */ PREPLUSPLUS PREMINUSMINUS '&'
%left  POSTPLUSPLUS POSTMINUSMINUS
%left  DEREF


%%

program
   : 
   | END_OF_INPUT                   { xxsetroot(NULL); YYACCEPT; }
   | program EOL                    { xxsetroot(NULL); YYACCEPT; }
   | program statement EOL          { xxsetroot($2); YYACCEPT; }
   | program statement END_OF_INPUT { xxsetroot($2); YYACCEPT; }
   ;

statement
   : '{' statement_list '}' { $$ = $2; }
   | exp                    { $$ = $1; }
   | exp ';'                { $$ = $1; }
   | selection_statement    { $$ = $1; }
   | iteration_statement    { $$ = $1; }
   ;

statement_list
   :                              { $$ = xxemptylist(); }
   | statement                    { $$ = xxfirstitem($1); }
   | statement_list statement     { $$ = xxadditem($1,$2); }
   ;

selection_statement
   : IF '(' exp ')' statement                { $$ = xx2childs($3,$5,NT_IF_STMT); }
   | IF '(' exp ')' statement ELSE statement { $$ = xxifelse($3,$5,$7); }
   ;

iteration_statement
   : FOR '(' iteration_control ';' iteration_control ';' iteration_control ')' statement 
                                               { $$ = xxfor($3,$5,$7,$9); }
   | WHILE '(' iteration_control ')' statement { $$ = xx2childs($3,$5,NT_WHILE_STMT); }
   ;

iteration_control
   :     { $$ = xxone(); }
   | exp { $$ = $1; }
   ;

exp     
   : NUM               { $$ = $1;                 }
   | STR               { $$ = $1;                 }
   | STDIN             { $$ = $1;                 }
   | STDOUT            { $$ = $1;                 }
   | STDERR            { $$ = $1;                 }
   | symbol            { $$ = $1;                 }
   | symbol '=' exp    { $$ = xxbinary($2,$1,$3); }
   | exp '+' exp       { $$ = xxbinary($2,$1,$3); }
   | exp '-' exp       { $$ = xxbinary($2,$1,$3); }
   | exp '*' exp       { $$ = xxbinary($2,$1,$3); }
   | exp '/' exp       { $$ = xxbinary($2,$1,$3); }
   | exp '%' exp       { $$ = xxbinary($2,$1,$3); }
   | '-' exp %prec NEG { $$ = xxunary($1,$2);     }
   | exp '^' exp       { $$ = xxbinary($2,$1,$3); }
   | '(' exp ')'       { $$ = $2;                 }
   | exp LT exp        { $$ = xxbinary($2,$1,$3); }
   | exp LE exp        { $$ = xxbinary($2,$1,$3); }
   | exp EQ exp        { $$ = xxbinary($2,$1,$3); }
   | exp NE exp        { $$ = xxbinary($2,$1,$3); }
   | exp GE exp        { $$ = xxbinary($2,$1,$3); }
   | exp GT exp        { $$ = xxbinary($2,$1,$3); }
   | exp AND exp       { $$ = xxbinary($2,$1,$3); }
   | exp OR exp        { $$ = xxbinary($2,$1,$3); }
   | NOT exp           { $$ = xxunary($1,$2);     }
   | '(' type  starlist ')' exp %prec CAST  { $$ = xxcast($2,$3,$5);             }
   | FUNCTION '(' formlist ')' statement %prec LOW 
                                            { $$ = xx2childs($3,$5,NT_FUNC_DEF); }
   | PLUSPLUS symbol %prec PREPLUSPLUS      { $$ = xxppmm($2, PREPLUSPLUS);      }
   | MINUSMINUS symbol %prec PREMINUSMINUS  { $$ = xxppmm($2, PREMINUSMINUS);    }
   | symbol PLUSPLUS %prec POSTPLUSPLUS     { $$ = xxppmm($1, POSTPLUSPLUS);     }
   | symbol MINUSMINUS %prec POSTMINUSMINUS { $$ = xxppmm($1, POSTMINUSMINUS);   }
   ;

symbol
   : codesegment             { $$ = $1; }
   | codesegment '.' genlist { $$ = xx2childs($1,$3,NT_VARGENLIST); }
   | '&' symbol %prec NEG    { $$ = xxunary($1,$2); }
   | '*' symbol %prec NEG    { $$ = xxunary($1,$2); }
   | dotlist                 { $$ = $1; }
   ;

dotlist
   : genlist                 { $$ = $1; }
   | objectacces             { $$ = $1; }
   | objectacces '.' genlist { $$ = xx2childs($1,$3,NT_VARGENLIST); }
   ;

genlist
   : PARENT                           { $$ = xxfirstitem($1); }
   | CHILDSET '[' exp ']'             { $$ = xxfirstchildset($1,$3); }
   | genlist '.' PARENT               { $$ = xxadditem($1,$3); }
   | genlist '.' CHILDSET '[' exp ']' { $$ = xxaddchildset($1,$3,$5); }
   ;

objectacces
   : VAR                             { $$ = $1; }
   | objectacces '.' VAR %prec DEREF { $$ = xxaddmember($1,$3,NT_REF); }
   | objectacces DEREF VAR           { $$ = xxaddmember($1,$3,NT_DEREF); }
   | objectacces '(' sublist ')'     { $$ = xx2childs($1,$3,NT_FUNC_CALL); }
   | objectacces '[' exp ']'         { $$ = xx2childs($1,$3,NT_SUBSCRIPT); }
   | STR '[' exp ']'                 { $$ = xx2childs($1,$3,NT_SUBSCRIPT); }
   ;

codesegment
   : '`' statement '\'' { $$ = xxcodesegment($2);  }
   ;

formlist
   :                  { $$ = xxemptylist(); }
   | VAR              { $$ = xxfirstitem($1); }
   | formlist ',' VAR { $$ = xxadditem($1,$3); }
   ;

sublist
   :                 { $$ = xxemptylist(); }
   | exp             { $$ = xxfirstitem($1); }
   | sublist ',' exp { $$ = xxadditem($1,$3); }
   ;

type
   : CHAR                   { $$ = xxtype(DT_CHAR);        }
   | SIGNED CHAR            { $$ = xxtype(DT_S_CHAR);      }
   | UNSIGNED CHAR          { $$ = xxtype(DT_U_CHAR);      }
   | BYTE                   { $$ = xxtype(DT_BYTE);        }
   | SHORT                  { $$ = xxtype(DT_SHORT);       }
   | SHORT INT              { $$ = xxtype(DT_SHORT);       }
   | SIGNED SHORT           { $$ = xxtype(DT_SHORT);       }
   | SIGNED SHORT INT       { $$ = xxtype(DT_SHORT);       }
   | UNSIGNED SHORT         { $$ = xxtype(DT_U_SHORT);     }
   | UNSIGNED SHORT INT     { $$ = xxtype(DT_U_SHORT);     }
   | INT                    { $$ = xxtype(DT_INT);         }
   | SIGNED INT             { $$ = xxtype(DT_INT);         }
   | UNSIGNED               { $$ = xxtype(DT_U_INT);       }
   | UNSIGNED INT           { $$ = xxtype(DT_U_INT);       }
   | LONG                   { $$ = xxtype(DT_LONG);        }
   | LONG INT               { $$ = xxtype(DT_LONG);        }
   | SIGNED LONG            { $$ = xxtype(DT_LONG);        }
   | SIGNED LONG INT        { $$ = xxtype(DT_LONG);        }
   | UNSIGNED LONG          { $$ = xxtype(DT_U_LONG);      }
   | UNSIGNED LONG INT      { $$ = xxtype(DT_U_LONG);      }
   | LONG LONG              { $$ = xxtype(DT_LONG_LONG);   }
   | LONG LONG INT          { $$ = xxtype(DT_LONG_LONG);   }
   | SIGNED LONG LONG       { $$ = xxtype(DT_LONG_LONG);   }
   | SIGNED LONG LONG INT   { $$ = xxtype(DT_LONG_LONG);   }
   | UNSIGNED LONG LONG     { $$ = xxtype(DT_U_LONG_LONG); }
   | UNSIGNED LONG LONG INT { $$ = xxtype(DT_U_LONG_LONG); }
   | SIZE_T                 { $$ = xxtype(DT_SIZE_T);      }
   | FLOAT                  { $$ = xxtype(DT_FLOAT);       }
   | DOUBLE                 { $$ = xxtype(DT_DOUBLE);      }
   | LONG DOUBLE            { $$ = xxtype(DT_LONG_DOUBLE); }
   ;

starlist
   :              { $$ = xxemptylist(); }
   | starlist '*' { $$ = xxadditem($1,$2); }
   ;
%%



/* Called by yyparse on error.  */
void yyerror(char const *s)
{
   fprintf(stderr, "%s\n", s);
}
