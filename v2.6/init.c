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
#pragma warning(disable:4996 4820)
#endif

#include <stdio.h>

#include "init.h"
#include "node.h"
#include "gcollection.h"


void parse_init(void)
{
    contextp = contextstack;
    *contextp = ' ';
    eatlines = 0;
    inputfile = stdin;
}



/* Definition of the global closure set. */
void clos_stack_init(void)
{
   clos_set_stack[0] = malloc(sizeof(closure));
   if (!clos_set_stack[0])
   {
      yyerror("Error: Lack of memory for the first closure.");
      exit(1);
   }
   clos_set_stack[0]->nb_clos = 0;
   clos_set_stack[0]->clos_array = NULL;
}



void read_params(void)
{
   FILE* params;
   char buf[100];

   params = fopen("pcastli.ini", "r");
   if (!params)
   {
      // file creation
      params = fopen("pcastli.ini", "w");
      if (!params) return;

      fprintf(params, "[garbage collector]\n");
      fprintf(params, "; number of executed top statements before launching "
         "garbage collection\n");
      fprintf(params, "nb_executions = 5\n");

      fclose(params);
      return;
   }

   fgets(buf, 100, params);
   fgets(buf, 100, params);
   fscanf(params, "nb_executions = %i\n", &max_executions);

   if (max_executions <= 0 || max_executions > MAX_MAX_EXECUTIONS) 
      max_executions = 5;

   fclose(params);
}
