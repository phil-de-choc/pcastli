/*
 * by Parent and Childset Accessible Syntax Tree Language Interpreter
 * Copyright (C) 2008-2016  Philippe Choquette
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
#pragma warning(disable:4996 4820 4710)
#endif

#include <stdio.h>
#include <string.h>

#include "init.h"
#include "node.h"
#include "gcollection.h"
#include "util.h"

int strict_access = 1;



void parse_init(void)
{
    contextp = contextstack;
    *contextp = ' ';
    eatlines = 0;
    inputadr.inputfile = stdin;
    inputsrc = IT_FILE;
}



/* Definition of the global closure set. */
void clos_stack_init(void)
{
   clos_set_stack = malloc(50 * sizeof(closure*));
   if (!clos_set_stack) fatal_error("Error: Lack of memory for closure stack.");
   memset(clos_set_stack, 0, 50 * sizeof(closure*));

   clos_set_stack[0] = malloc(sizeof(closure));
   if (!clos_set_stack[0])
      fatal_error("Error: Lack of memory for the first closure.");
   memset(clos_set_stack[0], 0, sizeof(closure));

   set_stack_size = 1;
   set_stack_limit = 50;
}



void read_params(void)
{
   FILE* params;
   char buf[150];

   params = fopen("pcastli.ini", "r");
   if (!params)
   {
      // file creation
      params = fopen("pcastli.ini", "w");
      if (!params) return;

      fprintf(params, "[garbage collector]\n");
      fprintf(params, "; number of executed top statements before launching "
         "garbage collection\n");
      fprintf(params, "nb_executions = 5\n\n");

      fprintf(params, "[context access]\n");
      fprintf(params, "; \"yes\" is context access only for local function and "
         "global context, \"no\" is access to all upward contexts\n");
      fprintf(params, "strict = yes\n");

      fclose(params);
      return;
   }

   fgets(buf, 150, params);
   fgets(buf, 150, params);
   fscanf(params, "nb_executions = %i\n", &max_executions);

   if (max_executions <= 0 || max_executions > MAX_MAX_EXECUTIONS) 
      max_executions = 5;

   fgets(buf, 150, params);
   fgets(buf, 150, params);
   fscanf(params, "strict = %s", buf);

   if (!strncmp(buf, "yes", 3)) strict_access = 1;
   else strict_access = 0;

   fclose(params);
}
