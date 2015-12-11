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

#ifdef _WIN32
#pragma warning(disable:4996 4820 4255 4710)
#else
#include <readline/readline.h>
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "util.h"
#include "pcastli.tab.h"


input_type inputsrc;
input_union inputadr;



void rel_yylval(int op, const char* opname)
{
   yylval = malloc(sizeof(node));
   if (!yylval)
   {
      fprintf(stderr, "Error: Lack of memory in rel_yylval for ");
      fprintf(stderr, "%s\n", opname);
      exit(1);
   }
   memset(yylval, 0, sizeof(node));
   yylval->ntype = NT_REL_OPER;
   yylval->opval.rel_oper = op;
   yylval->parent = NULL;
   yylval->childset = NULL;
   yylval->nb_childs = 0;
}



void math_yylval(char c)
{
   yylval = malloc(sizeof(node));
   if (!yylval)
   {
      fprintf(stderr, "Error: Lack of memory in math_yylval for ");
      fprintf(stderr, "%c\n", c);
      exit(1);
   }
   memset(yylval, 0, sizeof(node));
   yylval->ntype = NT_MATH_OPER;
   yylval->opval.math_oper = c;
   yylval->parent = NULL;
   yylval->childset = NULL;
   yylval->nb_childs = 0;
}


size_t readpos = 0;
size_t maxpos = 0;
char* line = NULL;

char readchar(void)
{
   char retchar = '\0';

   if (inputsrc == IT_FILE)
   {
      #ifdef _WIN32
      retchar = (char)getc(inputadr.inputfile);
      #else
      if (inputadr.inputfile == stdin)
      {
         if (readpos > maxpos || !line)
         {
            free(line);
            readpos = 0;
            line = readline("");
            maxpos = strlen(line);
         }

         retchar = line[readpos++];

         if (retchar == '\0') retchar = '\n';
      }
      else
      {
         retchar = (char)getc(inputadr.inputfile);
      }
      #endif
   }
   else if (inputsrc == IT_STRING)
   {
      retchar = inputadr.str[readpos++];

      if (retchar == '\0') retchar = EOF;
   }
   else fatal_error("Error: Unexpected input type found in readchar function.");

   return retchar;
}

void unreadchar(char c)
{
   if (inputsrc == IT_FILE)
   {
      #ifdef _WIN32
      ungetc(c, inputadr.inputfile);
      #else
      if (inputfile == stdin)
      {
         readpos--;
      }
      else
      {
         ungetc(c, inputadr.inputfile);
      }
      #endif
   }
   else if (inputsrc == IT_STRING)
   {
      readpos--;
   }
   else fatal_error("Error: Unexpected input type found in unreadchar function.");
}

char skip_space(void)
{
   char c;
   while ((c = (char)readchar()) == ' ' || c == '\t' || c == '\f') /* nothing */;
   return c;
}

double readdouble(void)
{
   int founddot = 0;
   int found_e = 0;
   char buf[100];
   int charOK = 1;
   int i = 0;
   char* currbuff = NULL;

   if (inputsrc == IT_FILE) currbuff = line;
   else if (inputsrc == IT_STRING) currbuff = inputadr.str;
   else fatal_error("Error: Unpexpected input type found in readhex function.");

   while (i < 100 && charOK && readpos <= maxpos)
   {
      if (isdigit(currbuff[readpos]))
      {
         buf[i] = currbuff[readpos];
         i++;
         readpos++;
      }
      else if (currbuff[readpos] == '.')
      {
         if (founddot)
         {
            charOK = 0;
         }
         else
         {
            founddot = 1;
            buf[i] = currbuff[readpos];
            i++;
            readpos++;
         }
      }
      else if (currbuff[readpos] == 'e' || currbuff[readpos] == 'E' || currbuff[readpos] ==
         'd' || currbuff[readpos] == 'D')
      {
         if (found_e)
         {
            charOK = 0;
         }
         else
         {
            found_e = 1;
            buf[i] = currbuff[readpos];
            i++;
            readpos++;

            if (currbuff[readpos] == '+' || currbuff[readpos] == '-')
            {
               buf[i] = currbuff[readpos];
               i++;
               readpos++;
            }
         }
      }
      else
      {
         charOK = 0;
      }
   } /* while (i < 100 && charOK && readpos <= maxpos) */

   buf[i] = '\0';

   return atof(buf);
}

int readhex(void)
{
   int i = 2;
   int charOK = 1;
   char buf[11];
   int retval = 0;
   char* currbuff = NULL;

   if (inputsrc == IT_FILE) currbuff = line;
   else if (inputsrc == IT_STRING) currbuff = inputadr.str;
   else fatal_error("Error: Unpexpected input type found in readhex function.");

   readpos += 2;
   buf[0] = '0';
   buf[1] = 'x';

   while (i < 11 && charOK && readpos <= maxpos)
   {
      if ((currbuff[readpos] >= '0' && currbuff[readpos] <= '9') ||
         (currbuff[readpos] >= 'a' && currbuff[readpos] <= 'f') ||
         (currbuff[readpos] >= 'A' && currbuff[readpos] <= 'F'))
      {
         buf[i] = currbuff[readpos];
         i++;
         readpos++;
      }
      else
      {
         charOK = 0;
      }
   }

   buf[i] = '\0';

   sscanf(buf, "%x", &retval);

   return retval;
}



struct
{
   char* rword;
   int num_symbol;
}
reserved_words_table[] = 
{
   { "while",    WHILE },
   { "for",      FOR },
   { "if",       IF },
   { "else",     ELSE },
   { "function", FUNCTION },
   { "parent",   PARENT },
   { "childset", CHILDSET },
   { "stdin",    STDIN },
   { "stdout",   STDOUT },
   { "stderr",   STDERR },
   { "char",     CHAR },
   { "byte",     BYTE },
   { "int",      INT },
   { "float",    FLOAT },
   { "double",   DOUBLE },
   { "signed",   SIGNED },
   { "unsigned", UNSIGNED },
   { "short",    SHORT },
   { "long",     LONG },
   { "size_t",   SIZE_T},
   { NULL, 0}
};



/* Split the input stream into tokens. */
/* This is the lowest of the parsing levels. */

int token(void)
{
   char c;
   static char* symbuf = NULL;
   static int length = 0;

   c = skip_space();

   /* Comments */
   if (c == '#')
   {
      do c = readchar();
      while(c != '\n' && c != EOF && c != '\r');
   }

   /* Hex Numbers */
   if (c == '0')
   {
      char c2 = readchar();
      if (c2 == 'x')
      {
         int hex_nb = 0;

         unreadchar(c2);
         unreadchar(c);

         yylval = malloc(sizeof(node));
         if (!yylval)
         {
            yyerror("Error: Lack of memory in token for a number.");
            exit(1);
         }
         memset(yylval, 0, sizeof(node));
         yylval->ntype = NT_NUM_CONST;

         if (inputsrc == IT_FILE)
         {
            #ifdef _WIN32
            fscanf(inputadr.inputfile, "%x", &hex_nb);
            #else
            if (inputadr.inputfile != stdin)
            {
               fscanf(inputadr.inputfile, "%x", &hex_nb);
            }
            else
            {
               hex_nb = readhex();
            }
            #endif
         }
         else if (inputsrc == IT_STRING)
         {
            hex_nb = readhex();
         }
         else fatal_error("Error: Unexpected input type found in token function.");

         yylval->opval.value = (double)hex_nb;
         yylval->parent = NULL;
         yylval->nb_childs = 0;
         yylval->childset = NULL;

         return NUM;
      }
      else
      {
         unreadchar(c2);
      }
   }

   /* Numbers */
   if (c == '.' || isdigit (c))
   {
      char c2 = readchar();
      if(c == '.' && !isdigit(c2))
      {
         unreadchar(c2);
         return '.';
      }
      unreadchar(c2);
      unreadchar(c);

      yylval = malloc(sizeof(node));
      if (!yylval)
      {
         yyerror("Error: Lack of memory in token for a number.");
         exit(1);
      }
      memset(yylval, 0, sizeof(node));
      yylval->ntype = NT_NUM_CONST;

      if (inputsrc == IT_FILE)
      {
         #ifdef _WIN32
         fscanf(inputadr.inputfile, "%lf", &yylval->opval.value);
         #else
         if (inputadr.inputfile != stdin)
         {
            fscanf(inputadr.inputfile, "%lf", &yylval->opval.value);
         }
         else
         {
            yylval->opval.value = readdouble();
         }
         #endif
      }
      else if (inputsrc == IT_STRING)
      {
         yylval->opval.value = readdouble();
      }
      else fatal_error("Error: Unexpected input type found in token function.");

      yylval->parent = NULL;
      yylval->nb_childs = 0;
      yylval->childset = NULL;

      return NUM;
   }

   /* Strings */
   if (c == '\"')
   {
      int i;

      /* Initially make the buffer long enough
      for a 40-character string. */
      if (length == 0)
         length = 40, symbuf = malloc(length + 1);
      if(!symbuf)
      {
         yyerror("Error: Lack of memory for symbuf in token.");
         exit(1);
      }

      i = 0;
      c = readchar();
      while (c != '\"')
      {
         /* If buffer is full, make it bigger. */
         if (i == length - 1)
         {
            length += 40;
            symbuf = realloc(symbuf, length + 1);
            if(!symbuf)
            {
               yyerror("Error: Lack of memory by realloc");
               yyerror("for symbuf in token.");
               exit(1);
            }
         }

         if (inputsrc ==  IT_FILE)
         {
            if (c == '\n' && inputadr.inputfile == stdin) printf("+ ");
         }

         /* Escape sequences. */
         if (c == '\\')
         {
            c = readchar();
            if (c == 'x')
            {
               int zero_count = 0;
               int j;

               c = readchar();
               while (c == '0')
               {
                  c = readchar();
                  zero_count++;
               }
               if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || 
                  (c >= 'A' && c <= 'F'))
               {
                  char digits[2];
                  int nb_digits = 1, k;
                  int exponent = 0;
                  int sum = 0;

                  digits[0] = c;

                  c = readchar();
                  if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || 
                     (c >= 'a' && c <= 'f'))
                  {
                     digits[1] = c;
                     nb_digits++;
                  }
                  else
                  {
                     unreadchar(c);
                  }
                  for (k = nb_digits - 1; k >= 0; k--)
                  {
                     int charval;
                     if (digits[k] >= '0' && digits[k] <= '9')
                     {
                        charval = digits[k] - 48;
                     }
                     else if (digits[k] >= 'A' && digits[k] <= 'F')
                     {
                        charval = digits[k] - 55;
                     }
                     else
                     {
                        charval = digits[k] - 87;
                     }
                     sum = sum + charval * (int)pow(16, exponent);
                     exponent++;
                  }

                  c = (char) sum;
               }
               else /* c is not hex */
               {
                  unreadchar(c);

                  for (j = 0; j < zero_count; j++)
                  {
                     unreadchar('0');
                  }
                  unreadchar('x');
                  c = '\\';
               }
               goto escape_ok;
            }
            /* Octal number */
            else if (c >= 48 && c <= 55)
            {
               char oct[3];
               int j = 0, k;
               int exponent = 0;
               int sum = 0;
               while (c >= 48 && c <= 55 && j < 3)
               {
                  oct[j] = c;
                  c = readchar();
                  j++;
               }

               unreadchar(c);

               if (j == 1 && oct[0] == '0')
               {
                  c = '\0';
                  goto escape_ok;
               }
               for (k = j - 1; k >= 0; k--)
               {
                  sum = sum + (oct[k] - 48) * (int)pow(8, exponent);
                  exponent++;
               }
               if (sum > 255)
               {
                  for (k = j - 1; k >= 0; k--)
                  {
                     unreadchar(oct[k]);
                  }
                  c = '\\';
               }
               else
               {
                  c = (char) sum;
               }
               goto escape_ok;
            }
            switch(c)
            {
            case 'a':
               c = '\a';
               break;
            case 'b':
               c = '\b';
               break;
            case 'f':
               c = '\f';
               break;
            case 'n':
               c = '\n';
               break;
            case 'r':
               c = '\r';
               break;
            case 't':
               c = '\t';
               break;
            case 'v':
               c = '\v';
               break;
            case '?':
               c = '\?';
               break;
            case '\'':
            case '\"':
            case '\\':
               break;
            default:
               unreadchar(c);
               c = '\\';
            }
         }

escape_ok:

         /* Add this character to the buffer. */
         symbuf[i++] = c;
         /* Get another character. */
         c = readchar();
      }

      symbuf[i] = '\0';

      yylval = malloc(sizeof(node));
      if (!yylval)
      {
         yyerror("Error: Lack of memory in token for a string node.");
         exit(1);
      }
      memset(yylval, 0, sizeof(node));
      yylval->ntype = NT_STRING;
      yylval->opval.str.length = i + 1;
      yylval->opval.str.tab = malloc(yylval->opval.str.length);
      if (!yylval->opval.str.tab)
      {
         yyerror("Error: Lack of memory in token for a string.\n");
         exit(1);
      }
      memcpy(yylval->opval.str.tab, symbuf, yylval->opval.str.length);
      yylval->parent = NULL;
      yylval->nb_childs = 0;
      yylval->childset = NULL;

      return STR;
   }

   /* Identifiers */
   if (isalpha(c) || c == '_')
   {
      int i;

      /* Initially make the buffer long enough
      for a 40-character symbol name.  */
      if (length == 0)
      length = 40, symbuf = malloc(length + 1);
      if(!symbuf)
      {
         yyerror("Error: Lack of memory for symbuf in token.");
         exit(1);
      }

      i = 0;
      do
      {
         /* If buffer is full, make it bigger. */
         if (i == length - 1)
         {
            length += 40;
            symbuf = realloc(symbuf, length + 1);
            if(!symbuf)
            {
               yyerror("Error: Lack of memory by realloc");
               yyerror("for symbuf in token.");
               exit(1);
            }
         }
         /* Add this character to the buffer. */
         symbuf[i++] = c;
         /* Get another character. */
         c = readchar();
      }
      while (isalnum (c) || c == '_');

      unreadchar(c);
      symbuf[i] = '\0';

      /* Search in the reserved words table. */
      i = 0;
      while (reserved_words_table[i].rword)
      {
         if (!strcmp(symbuf, reserved_words_table[i].rword))
         {
            /* word found */

            if (reserved_words_table[i].num_symbol == PARENT || 
               reserved_words_table[i].num_symbol == CHILDSET ||
               reserved_words_table[i].num_symbol == STDIN ||
               reserved_words_table[i].num_symbol == STDOUT ||
               reserved_words_table[i].num_symbol == STDERR)
            {
               yylval = malloc(sizeof(node));
               if (!yylval)
               {
                  fprintf(stderr, "Error: Lack of memory for reserved");
                  fprintf(stderr, "word node in token.\n");
                  exit(1);
               }
               memset(yylval, 0, sizeof(node));
               if (reserved_words_table[i].num_symbol == PARENT)
                  yylval->ntype = NT_PARENT;
               else if (reserved_words_table[i].num_symbol == CHILDSET)
                  yylval->ntype = NT_CHILDSET;
               else if (reserved_words_table[i].num_symbol == STDIN)
                  yylval->ntype = NT_STDIN;
               else if (reserved_words_table[i].num_symbol == STDOUT)
                  yylval->ntype = NT_STDOUT;
               else if (reserved_words_table[i].num_symbol == STDERR)
                  yylval->ntype = NT_STDERR;
               yylval->opval.name = NULL;
               yylval->parent = NULL;
               yylval->childset = NULL;
               yylval->nb_childs = 0;
            }

            return reserved_words_table[i].num_symbol;
         }
         i++;
      }

      /* If the word is not found then creation of a new variable. */
      yylval = malloc(sizeof(node));
      if (!yylval)
      {
         yyerror("Error: Lack of memory in token for a variable.");
         exit(1);
      }
      memset(yylval, 0, sizeof(node));
      yylval->ntype = NT_VARIABLE;
      yylval->opval.name = malloc(strlen(symbuf) + 1);
      if (!yylval->opval.name)
      {
         fprintf(stderr, "Error: Lack of memory in token ");
         fprintf(stderr, "for a variable name.\n");
         exit(1);
      }
      strcpy(yylval->opval.name, symbuf);
      yylval->parent = NULL;
      yylval->nb_childs = 0;
      yylval->childset = NULL;

      return VAR;
   }

   switch (c)
   {
   case '-':
      if ((c = readchar()) == '-')
      {
         return MINUSMINUS;
      }
      else if (c == '>')
      {
         return DEREF;
      }

      unreadchar(c);
      math_yylval('-');
      return '-';
   case '+':
      if ((c = readchar()) == '+')
         return PLUSPLUS;

      unreadchar(c);
      math_yylval('+');
      return '+';
   case '*':
   case '/':
   case '^':
      math_yylval(c);
      return c;

   case '|':
      if ((c = readchar()) == '|')
      {
         rel_yylval(OR, "OR");
         return OR;
      }
      unreadchar(c);
      return '|';

   case '&':
      if ((c = readchar()) == '&')
      {
         rel_yylval(AND, "AND");
         return AND;
      }
      unreadchar(c);
      math_yylval('&');
      return '&';

   case '!':
      if ((c = readchar()) == '=')
      {
         rel_yylval(NE, "NE");
         return NE;
      }
      unreadchar(c);
      rel_yylval(NOT, "NOT");
      return NOT;

   case '>':
      if ((c = readchar()) == '=')
      {
         rel_yylval(GE, "GE");
         return GE;
      }
      unreadchar(c);
      rel_yylval(GT, "GT");
      return GT;

   case '<':
      if ((c = readchar()) == '=')
      {
         rel_yylval(LE, "LE");
         return LE;
      }
      unreadchar(c);
      rel_yylval(LT, "LT");
      return LT;

   case '=':
      if ((c = readchar()) == '=')
      {
         rel_yylval(EQ, "EQ");
         return EQ;
      }
      unreadchar(c);

      yylval = malloc(sizeof(node));
      if (!yylval)
      {
         yyerror("Error: Lack of memory in yylex for \'=\'.");
         exit(1);
      }
      memset(yylval, 0, sizeof(node));
      yylval->ntype = NT_MATH_OPER;
      yylval->opval.math_oper = '=';
      yylval->parent = NULL;
      yylval->childset = NULL;
      yylval->nb_childs = 0;

      return '=';

   case '\r':
      c = (char) getc(inputadr.inputfile);
      if (c != '\n')
      {
         ungetc(c, inputadr.inputfile);
      }
      return EOL;

   case '\n':
      return EOL;

   case EOF:
      return END_OF_INPUT;
   }

   return c;
}


void ifpush(void)
{
   if(contextp - contextstack >= CONTEXTSTACK_SIZE)
   {
      yyerror("Error: Contextstack overflow.");
      exit(1);
   }
   *++contextp = 'i';
}



void ifpop(void)
{
   while (*contextp=='i') *contextp-- = 0;
}



int yylex(void)
{
   int tok;

again:

   tok = token();

   /* Newlines must be handled in a context */
   /* sensitive way.  The following block of */
   /* deals directly with newlines in the */
   /* body of "if" statements. */

   if (tok == EOL) 
   {
      if (eatlines || *contextp != ' ')
      {
         if (inputsrc == IT_FILE)
         {
            if (inputadr.inputfile == stdin) printf("+ ");
         }
         goto again;
      }

      return EOL;
   }

   /* Additional context sensitivities */

   switch(tok) 
   {
   /* Any newlines immediately following the */
   /* the following tokens are discarded. The */
   /* expressions are clearly incomplete. */

   case '+':
   case '-':
   case '*':
   case '/':
   case '^':
   case '&':
   case LT:
   case LE:
   case GE:
   case GT:
   case EQ:
   case NE:
   case OR:
   case AND:
   case '!':
   case '=':
   case CHILDSET:
   case '.':
   case DEREF:
      eatlines = 1;
      break;

   /* Push any "if" statements found and */
   /* discard any immediately following newlines. */

   case IF:
   case WHILE:
   case FOR:
   case FUNCTION:
      ifpush();
      eatlines = 1;
      break;

   /* Terminate any immediately preceding "if" */
   /* statements and discard any immediately */
   /* following newlines. */

   case ELSE:
      ifpop();
      eatlines = 1;
      break;

   /* Any newlines following these tokens can */
   /* indicate the end of an expression. */

   case NUM:
   case STR:
   case STDIN:
   case STDOUT:
   case STDERR:
   case VAR:
   case PARENT:
      ifpop();
      eatlines = 0;
      break;

   /* Handle brackets, braces and parentheses */

   case '{':
   case '(':
   case '[':
   case '`':
      if(contextp - contextstack >= CONTEXTSTACK_SIZE)
      {
         yyerror("Error: contextstack overflow.");
         exit(1);
      }
      *++contextp = tok;
      break;

   case '}':
      *contextp-- = 0;
      ifpop();
      if (*contextp == ' ') eatlines = 0;
      break;

   case ')':
   case ']':
   case '\'':
      *contextp-- = 0;
      if (*contextp == ' ') eatlines = 0;
      break;
   }

   return tok;
}
