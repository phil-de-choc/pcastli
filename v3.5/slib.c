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
#pragma warning(disable:4996 4820 4668 4255)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <string.h>

#include "eval.h"
#include "util.h"
#include "gcollection.h"

#if defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
void asmcall(size_t nchunks, void** raw_args, void (*funcpt)(void), size_t* chunkfloat, void** outregs);
#endif


data eval_openlib(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_children != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in openlib.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of openlib has not the string type.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_HLIB;

   #ifdef _WIN32
   retval.value.hlib = LoadLibrary(from_eval.value.str.tab);
   #else
   retval.value.hlib = dlopen(from_eval.value.str.tab, RTLD_LAZY);
   #endif

   return retval;
}



data eval_getfuncpt(node* to_eval)
{
   int err = 0;
   data retval, arg1, arg2;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_children != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in getfuncpt.");
      yyerror("       This function has two parameters.");
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_HLIB || arg1.ti.nderef > 0)
   {
      yyerror("Error: The first argument of getfuncpt has not the library handle type.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_STRING || arg2.ti.nderef > 0)
   {
      yyerror("Error: The second argument of getfuncpt has not the string type.");
      free_data(arg2);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_PFUNC;

   #ifdef _WIN32
   retval.value.pfunc = (void (__cdecl*)(void))GetProcAddress(arg1.value.hlib, arg2.value.str.tab);
   #else
   retval.value.pfunc = dlsym(arg1.value.hlib, arg2.value.str.tab);
   #endif

   return retval;
}



data eval_closelib(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_children != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in closelib.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_HLIB || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of closelib has not the library handle type.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_INT;

   #ifdef _WIN32
   retval.value.inum = FreeLibrary(from_eval.value.hlib);
   #else
   retval.value.inum = dlclose(from_eval.value.hlib);
   #endif

   return retval;
}



data eval_slib_call(data dtpfunc, node* to_eval)
{
   data retval, * argtab = NULL;
   void** raw_args = NULL;
   size_t sznchunks = 0, i = 0;
   void (*pfunc)(void) = NULL;
   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__)) 
   int esp_init = 0;
   int outeax = 0;
   int outedx = 0;
   union
   {
      struct 
      {
         int lowpart;
         int hipart;
      } seq;
      long long ret_int64;
   } cells;
   double outreal = 0.0;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   size_t* chunkfloat = NULL;
   void* outregs[3] = {NULL, NULL, NULL};
   float flnum;
   #endif

   memset(&retval, 0, sizeof(data));

   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))
   if (args_eval(to_eval, 0, &sznchunks, &raw_args, &argtab)) return retval;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   if (args_eval(to_eval, 0, &sznchunks, &raw_args, &argtab, &chunkfloat)) return retval;
   #endif

   pfunc = dtpfunc.value.pfunc;


   #if defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))

   asmcall(sznchunks, raw_args, pfunc, chunkfloat, outregs);
   free(chunkfloat);

   #elif defined _WIN32

   __asm mov esp_init, esp
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      __asm push chunk
   }

   __asm
   {
      ffree st(0)
      ffree st(1)
      ffree st(2)
      ffree st(3)
      ffree st(4)
      ffree st(5)
      ffree st(6)
      ffree st(7)

      call pfunc
      mov outeax, eax
      mov outedx, edx
      fst outreal

      mov esp, esp_init
   }

   #elif defined(__linux__) && defined(__i386__)

   asm("mov %%esp, %0" : "=m"(esp_init));
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      asm("push %0" : : "r"(chunk));
   }

   asm("ffree %st(0)");
   asm("ffree %st(1)");
   asm("ffree %st(2)");
   asm("ffree %st(3)");
   asm("ffree %st(4)");
   asm("ffree %st(5)");
   asm("ffree %st(6)");
   asm("ffree %st(7)");
   asm("call *%0" : : "m"(pfunc) : "%eax", "%edx");
   asm("mov %%eax, %0" : "=m"(outeax));
   asm("mov %%edx, %0" : "=m"(outedx));
   asm("fstl %0" : "=m"(outreal));

   asm("mov %0, %%esp" : : "m"(esp_init));

   #endif


   retval.ti.dtype = DT_OBJECT;
   retval.value.pObject = malloc(sizeof(clos_set));
   if (!retval.value.pObject)
      fatal_error("Error : Lack of memory in eval_slib_call for new object.");
   memset(retval.value.pObject, 0, sizeof(clos_set));
   g_lst_add(retval.value.pObject, PT_OBJECT);

   retval.value.pObject->clos_array = calloc(4, sizeof(closure*));
   if (!retval.value.pObject->clos_array)
      fatal_error("Error : Lack of memory in eval_slib_call for new member array.");

   retval.value.pObject->nb_clos = 4;

   /* ret_int32 */
   retval.value.pObject->clos_array[0] = malloc(sizeof(closure));
   if (!retval.value.pObject->clos_array[0])
      fatal_error("Error : Lack of memory in eval_slib_call for new member.");
   memset(retval.value.pObject->clos_array[0], 0, sizeof(closure));

   retval.value.pObject->clos_array[0]->name = malloc(strlen("ret_int32") + 1);
   if (!retval.value.pObject->clos_array[0]->name)
      fatal_error("Error : Lack of memory in eval_slib_call for ret_int32 name.");

   strcpy(retval.value.pObject->clos_array[0]->name, "ret_int32");

   retval.value.pObject->clos_array[0]->pContainer = retval.value.pObject;


   /* ret_int64 */
   retval.value.pObject->clos_array[1] = malloc(sizeof(closure));
   if (!retval.value.pObject->clos_array[1])
      fatal_error("Error : Lack of memory in eval_slib_call for new member.");
   memset(retval.value.pObject->clos_array[1], 0, sizeof(closure));

   retval.value.pObject->clos_array[1]->name = malloc(strlen("ret_int64") + 1);
   if (!retval.value.pObject->clos_array[1]->name)
      fatal_error("Error : Lack of memory in eval_slib_call for ret_int64 name.");

   strcpy(retval.value.pObject->clos_array[1]->name, "ret_int64");

   retval.value.pObject->clos_array[1]->pContainer = retval.value.pObject;


   /* ret_double */
   retval.value.pObject->clos_array[2] = malloc(sizeof(closure));
   if (!retval.value.pObject->clos_array[2])
      fatal_error("Error : Lack of memory in eval_slib_call for new member.");
   memset(retval.value.pObject->clos_array[2], 0, sizeof(closure));

   retval.value.pObject->clos_array[2]->name = malloc(strlen("ret_double") + 1);
   if (!retval.value.pObject->clos_array[2]->name)
      fatal_error("Error : Lack of memory in eval_slib_call for ret_double name.");
   strcpy(retval.value.pObject->clos_array[2]->name, "ret_double");

   retval.value.pObject->clos_array[2]->pContainer = retval.value.pObject;


   /* ret_float */
   retval.value.pObject->clos_array[3] = malloc(sizeof(closure));
   if (!retval.value.pObject->clos_array[3])
      fatal_error("Error : Lack of memory in eval_slib_call for new member.");
   memset(retval.value.pObject->clos_array[3], 0, sizeof(closure));

   retval.value.pObject->clos_array[3]->name = malloc(strlen("ret_float") + 1);
   if (!retval.value.pObject->clos_array[3]->name)
      fatal_error("Error : Lack of memory in eval_slib_call for ret_float name.");
   strcpy(retval.value.pObject->clos_array[3]->name, "ret_float");

   retval.value.pObject->clos_array[3]->pContainer = retval.value.pObject;

   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))

   retval.value.pObject->clos_array[0]->content.ti.dtype = DT_INT;
   retval.value.pObject->clos_array[0]->content.value.inum = outeax;

   retval.value.pObject->clos_array[1]->content.ti.dtype = DT_LONG_LONG;
   cells.seq.lowpart = outeax;
   cells.seq.hipart = outedx;
   retval.value.pObject->clos_array[1]->content.value.llnum = cells.ret_int64;

   retval.value.pObject->clos_array[2]->content.ti.dtype = DT_DOUBLE;
   retval.value.pObject->clos_array[2]->content.value.num = outreal;

   retval.value.pObject->clos_array[3]->content.ti.dtype = DT_FLOAT;
   retval.value.pObject->clos_array[3]->content.value.num = outreal;

   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))

   retval.value.pObject->clos_array[0]->content.ti.dtype = DT_INT;
   retval.value.pObject->clos_array[0]->content.value.inum = (int)(long long)outregs[0];

   retval.value.pObject->clos_array[1]->content.ti.dtype = DT_LONG_LONG;
   retval.value.pObject->clos_array[1]->content.value.genptr = outregs[0];

   retval.value.pObject->clos_array[2]->content.ti.dtype = DT_DOUBLE;
   retval.value.pObject->clos_array[2]->content.value.genptr = outregs[1];

   memcpy(&flnum, &outregs[2], sizeof(float));
   retval.value.pObject->clos_array[3]->content.ti.dtype = DT_FLOAT;
   retval.value.pObject->clos_array[3]->content.value.num = (double)flnum;

   #endif
 

   for (i = 0; i < to_eval->nb_children; i++)
   {
      free_data(argtab[i]);
   }
   free(argtab);
   free(raw_args);

   return retval;
}
