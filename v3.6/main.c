/*
 * by Parent and Childset Accessible Syntax Tree Language Interpreter
 * Copyright (C) 2008-2018  Philippe Choquette
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef _WIN32
#pragma warning(disable:4820 4710)
#endif


#include <stdio.h>
#include "init.h"
#include "node.h"
#include "util.h"

int yyparse(void);
extern int yynerrs;


int main(void)
{
   printf("by Parent and Childset Accessible Syntax Tree Language ");
   printf("Interpreter v3.6\n");
   printf("Copyright (C) 2008-2018  Philippe Choquette\n\n");
   printf("Type exit() or press Ctrl-c to exit.\n\n> ");
   parse_init();
   clos_stack_init();
   read_params();
   change_prompt("> ");

   while(1) 
   {
      yyparse();
      if (!yynerrs) exec();
      return_called = 0;
      abort_called = 0;
      printf("%s", sPrompt);
   }
   return 0;
}
