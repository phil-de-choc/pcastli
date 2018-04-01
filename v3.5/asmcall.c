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

#include <string.h>
#include "node.h"


void asmcall(size_t nchunks, void** raw_args, void (*funcpt)(void), size_t* chunkfloat, void** outregs)
{
   void* args[6];
   double floats[8];
   void** stack;
   void* argpt;
   double argfl;
   int i, iarg = 0, ifloat = 0, istack = 0;
   void* esp_init = 0;
   void* outint;
   double outdouble;
   float outfloat;

   memset(args, 0, 6 * sizeof(void*));
   memset(floats, 0, 8 * sizeof(double));


   stack = malloc(nchunks * sizeof(void*));
   if (!stack)
   {
      yyerror("Error: Lack of memory in asmcall for stack.");
      exit(1);
   }
   memset(stack, 0, nchunks * sizeof(void*));

   i = nchunks-1;
   while (i >= 0)
   {
      if (chunkfloat[i])
      {
         if (chunkfloat[i] <= 8)
         {
            if (ifloat > 7)
            {
               stack[istack++] = raw_args[i];
            }
            else
            {
               memcpy(&floats[ifloat++], &raw_args[i], chunkfloat[i]); 
            }
            i--;
         }
         else
         {
            stack[istack++] = raw_args[i];
            i--;
         }
      }
      else
      {
         if (iarg > 5)
         {
            stack[istack++] = raw_args[i];
         }
         else
         {
            args[iarg++] = raw_args[i];
         }
         i--;
      }
   }

   asm("mov %%rsp, %0" : "=m"(esp_init));

   for (i = istack - 1; i >= 0; i--)
   {
      void* chunk = stack[i];
      asm("push %0" : : "m"(chunk));
   }

   argpt = args[0];
   asm("mov %0, %%rdi"  : : "m"(argpt));
   argpt = args[1];
   asm("mov %0, %%rsi"  : : "m"(argpt));
   argpt = args[2];
   asm("mov %0, %%rdx"  : : "m"(argpt));
   argpt = args[3];
   asm("mov %0, %%rcx"  : : "m"(argpt));
   argpt = args[4];
   asm("mov %0, %%r8"  : : "m"(argpt));
   argpt = args[5];
   asm("mov %0, %%r9"  : : "m"(argpt));

   argfl = floats[1];
   asm("movq %0, %%xmm1"  : : "m"(argfl));
   argfl = floats[2];
   asm("movq %0, %%xmm2"  : : "m"(argfl));
   argfl = floats[3];
   asm("movq %0, %%xmm3"  : : "m"(argfl));
   argfl = floats[4];
   asm("movq %0, %%xmm4"  : : "m"(argfl));
   argfl = floats[5];
   asm("movq %0, %%xmm5"  : : "m"(argfl));
   argfl = floats[6];
   asm("movq %0, %%xmm6"  : : "m"(argfl));
   argfl = floats[7];
   asm("movq %0, %%xmm7"  : : "m"(argfl));
   argfl = floats[0];
   asm("movq %0, %%xmm0"  : : "m"(argfl));

   asm("mov %0, %%eax" : : "m"(ifloat));
   asm("call *%0" : : "m"(funcpt));

   asm("mov %0, %%rsp"  : : "m"(esp_init));

   asm("mov %%rax, %0" : "=m"(outint));
   asm("movsd %%xmm0, %0" : "=m"(outdouble));
   asm("movss %%xmm0, %0" : "=m"(outfloat));

   outregs[0] = (void*)outint;
   memcpy(&outregs[1], &outdouble, 8);
   memcpy(&outregs[2], &outfloat, 4);

   free(stack);
}
