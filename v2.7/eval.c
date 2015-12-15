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
#pragma warning(disable:4996 4820 4255 4738 4710)
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#include "eval.h"
#include "pcastli.tab.h"
#include "tree.h"
#include "util.h"
#include "macros.h"
#include "gcollection.h"
#include "slib.h"
#include "treetocode.h"

int return_called = 0;
int abort_called = 0;
size_t set_stack_size = 1;

#if defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
void asmcall(size_t nchunks, void** raw_args, void (*funcpt)(void), size_t* chunkfloat, void** outregs);
#endif

int yyparse(void);

typedef enum
{
   FID_NOFUNC = -1,
   FID_PRINT = 0,
   FID_EXIT,
   FID_ABORT,
   FID_INFO,
   FID_MKNODE,
   FID_RMNODE,
   FID_VALUE,
   FID_LENGTH,
   FID_SCAN,
   FID_READ,
   FID_WRITE,
   FID_GETWD,
   FID_SETWD,
   FID_TONODE,
   FID_SUBSEQ,
   FID_CONCAT,
   FID_NTOA,
   FID_ATON,
   FID_RETURN,
   FID_COPYTREE,
   FID_CLEAR,
   FID_EXECUTE,
   FID_PROMPT,
   FID_NAMES,
   FID_ARRAY,
   FID_LIST,
   FID_FILLOBJECT,
   FID_SETLENGTH,
   FID_INSERT,
   FID_REPLACE,
   FID_SOURCE,
   FID_STRLEN,
   FID_AS_ARRAY,
   FID_AS_LIST,
   FID_AS_STATEMENTS,
   FID_GETTYPE,
   FID_UNDEFINE,
   FID_PRINTF,
   FID_SCANF,
   FID_FOPEN,
   FID_FCLOSE,
   FID_FPRINTF,
   FID_FSCANF,
   FID_FEOF,
   FID_FREAD,
   FID_FWRITE,
   FID_CLEARERR,
   FID_FGETPOS,
   FID_FSETPOS,
   FID_FERROR,
   FID_FGETC,
   FID_UNGETC,
   FID_FFLUSH,
   FID_FPUTC,
   FID_FSEEK,
   FID_FTELL,
   FID_FREOPEN,
   FID_REWIND,
   FID_TMPFILE,
   FID_SPRINTF,
   FID_SSCANF,
   FID_REMOVE,
   FID_CLS,
   FID_ATOVAR,
   FID_VARTOA,
   FID_FREETREE,
   FID_STRCLONE,
   FID_STRCPY,
   FID_OPENLIB,
   FID_GETFUNCPT,
   FID_CLOSELIB,
   FID_ALLOC_COPY,
   FID_FREE,
   FID_REPEAT,
   FID_VARTOMEM,
   FID_MEMTOVAR,
   FID_MEMCPY,
   FID_MEMSET,
   FID_MEMCLONE,
   FID_MEMORY,
   FID_TREETOCODE,
   FID_CODETOTREE,
   FID_EXIST,
   FID_CREATENODE,
   FID_APPENDCHILD,
   NB_FUNC
} EFuncID;


typedef struct
{
   char* func_name;
   EFuncID eFnID;
} pair_fn_ID;


pair_fn_ID func_map[] =
{
   {"abort", FID_ABORT},
   {"alloc_copy", FID_ALLOC_COPY},
   {"appendchild", FID_APPENDCHILD},
   {"array", FID_ARRAY},
   {"as_array", FID_AS_ARRAY},
   {"as_list", FID_AS_LIST},
   {"as_statements", FID_AS_STATEMENTS},
   {"aton", FID_ATON},
   {"atovar", FID_ATOVAR},
   {"clear", FID_CLEAR},
   {"clearerr", FID_CLEARERR},
   {"closelib", FID_CLOSELIB},
   {"cls", FID_CLS},
   {"codetotree", FID_CODETOTREE},
   {"concat", FID_CONCAT},
   {"copytree", FID_COPYTREE},
   {"createnode", FID_CREATENODE},
   {"execute", FID_EXECUTE},
   {"exist", FID_EXIST},
   {"exit", FID_EXIT},
   {"fclose", FID_FCLOSE},
   {"feof", FID_FEOF},
   {"ferror", FID_FERROR},
   {"fflush", FID_FFLUSH},
   {"fgetc", FID_FGETC},
   {"fgetpos", FID_FGETPOS},
   {"fillobject", FID_FILLOBJECT},
   {"fopen", FID_FOPEN},
   {"fprintf", FID_FPRINTF},
   {"fputc", FID_FPUTC},
   {"fread", FID_FREAD},
   {"free", FID_FREE},
   {"freetree", FID_FREETREE},
   {"freopen", FID_FREOPEN},
   {"fscanf", FID_FSCANF},
   {"fseek", FID_FSEEK},
   {"fsetpos", FID_FSETPOS},
   {"ftell", FID_FTELL},
   {"fwrite", FID_FWRITE},
   {"getfuncpt", FID_GETFUNCPT},
   {"gettype", FID_GETTYPE},
   {"getwd", FID_GETWD},
   {"info", FID_INFO},
   {"insert", FID_INSERT},
   {"length", FID_LENGTH},
   {"list", FID_LIST},
   {"memclone", FID_MEMCLONE},
   {"memcpy", FID_MEMCPY},
   {"memory", FID_MEMORY},
   {"memset", FID_MEMSET},
   {"memtovar", FID_MEMTOVAR},
   {"mknode", FID_MKNODE},
   {"names", FID_NAMES},
   {"ntoa", FID_NTOA},
   {"openlib", FID_OPENLIB},
   {"print", FID_PRINT},
   {"printf", FID_PRINTF},
   {"prompt", FID_PROMPT},
   {"read", FID_READ},
   {"remove", FID_REMOVE},
   {"repeat", FID_REPEAT},
   {"replace", FID_REPLACE},
   {"return", FID_RETURN},
   {"rewind", FID_REWIND},
   {"rmnode", FID_RMNODE},
   {"scan", FID_SCAN},
   {"scanf", FID_SCANF},
   {"setlength", FID_SETLENGTH},
   {"setwd", FID_SETWD},
   {"source", FID_SOURCE},
   {"sprintf", FID_SPRINTF},
   {"sscanf", FID_SSCANF},
   {"strclone", FID_STRCLONE},
   {"strcpy", FID_STRCPY},
   {"strlen", FID_STRLEN},
   {"subseq", FID_SUBSEQ},
   {"tmpfile", FID_TMPFILE},
   {"tonode", FID_TONODE},
   {"treetocode", FID_TREETOCODE},
   {"undefine", FID_UNDEFINE},
   {"ungetc", FID_UNGETC},
   {"value", FID_VALUE},
   {"vartoa", FID_VARTOA},
   {"vartomem", FID_VARTOMEM},
   {"write", FID_WRITE}
};


typedef struct
{
   char* descr;
   node_type ntype;
} pair_desc_NT;


pair_desc_NT ntype_map[] =
{
   {"->", NT_DEREF},
   {".", NT_REF},
   {"access list", NT_ACCESSLIST},
   {"call",NT_FUNC_CALL},
   {"cast", NT_CAST},
   {"childset", NT_CHILDSET},
   {"childset reserved word", NT_CHILDSET},
   {"code segment", NT_CODESEGMENT},
   {"constant", NT_NUM_CONST},
   {"decr", NT_INCR_DECR},
   {"decrementation", NT_INCR_DECR},
   {"dereference", NT_DEREF},
   {"for", NT_FOR_STMT},
   {"for statement", NT_FOR_STMT},
   {"function call",NT_FUNC_CALL},
   {"function definition",NT_FUNC_DEF},
   {"if", NT_IF_STMT},
   {"if else", NT_IFELSE_STMT},
   {"if else statement", NT_IFELSE_STMT},
   {"if statement", NT_IF_STMT},
   {"incr", NT_INCR_DECR},
   {"incrementation", NT_INCR_DECR},
   {"incrementation or decrementation", NT_INCR_DECR},
   {"list", NT_LIST},
   {"logical operator", NT_REL_OPER},
   {"mathematical operator", NT_MATH_OPER},
   {"numerical constant", NT_NUM_CONST},
   {"object member access list", NT_ACCESSLIST},
   {"parent", NT_PARENT},
   {"parent reserved word", NT_PARENT},
   {"relational operator", NT_REL_OPER},
   {"relational or logical operator", NT_REL_OPER},
   {"stderr", NT_STDERR},
   {"stderr FILE* identifier", NT_STDERR},
   {"stdin", NT_STDIN},
   {"stdin FILE* identifier", NT_STDIN},
   {"stdout", NT_STDOUT},
   {"stdout FILE* identifier", NT_STDOUT},
   {"string", NT_STRING},
   {"structure dereference operator", NT_DEREF},
   {"structure dereference operator (->)", NT_DEREF},
   {"structure reference operator", NT_REF},
   {"structure reference operator (.)", NT_REF},
   {"subscript", NT_SUBSCRIPT},
   {"reference", NT_REF},
   {"vargenlist", NT_VARGENLIST},
   {"variable", NT_VARIABLE},
   {"while", NT_WHILE_STMT},
   {"while statement", NT_WHILE_STMT},
   {"", NT_UNDEF}
};


size_t sizes_classes[] = 
{
   0, /* DT_UNDEF */
   1, /* DT_CHAR */
   1, /* DT_S_CHAR */
   1, /* DT_U_CHAR */
   1, /* DT_BYTE */
   2, /* DT_SHORT */
   2, /* DT_U_SHORT */
   4, /* DT_INT */
   4, /* DT_U_INT */
   4, /* DT_LONG */
   4, /* DT_U_LONG */
   8, /* DT_LONG_LONG */
   8, /* DT_U_LONG_LONG */
   sizeof(size_t), /* DT_SIZE_T */
   9, /* DT_FLOAT */
   9, /* DT_DOUBLE */
   10 /* DT_LONG_DOUBLE */
};



extern size_t maxpos;
extern size_t readpos;
extern int yynerrs;
extern struct
{
   char* rword;
   int num_symbol;
}
reserved_words_table[];

data eval_dotlist(node* start_pt, node* dotlist);


data assign_node_pointers(node* to_eval, data from_eval2)
{
   data from_eval1, retval;
   node* argroot;
   unsigned int found, i;

   memset(&retval, 0, sizeof(data));

   from_eval1 = eval(to_eval->childset[0]);
   if (from_eval1.ti.dtype != DT_POINTER || from_eval2.ti.dtype != DT_POINTER || 
      from_eval1.ti.nderef > 0 || from_eval2.ti.nderef > 0)
   {
      free_data(from_eval1);
      free_data(from_eval2);
      return retval;
   }

   argroot = from_eval1.value.ptr->parent;
   if (!argroot) return from_eval2;

   /* Find from_eval1.value.ptr in argroot->childset. */
   found = 0;
   i = 0;
   while (!found && i < argroot->nb_childs)
   {
      if (from_eval1.value.ptr == argroot->childset[i])
         found = 1;
      i++;
   }

   if (!found)
   {
      yyerror("Error: Node pointer not found in argroot in");
      yyerror("assign_node_pointers.");

      abort_called = 1;

      free_data(from_eval1);
      free_data(from_eval2);
      return retval;
   }

   /* The replaced tree is erased. */
   free_tree(from_eval1.value.ptr);

   /* Tree copy from right node of '='. */
   i--;
   if (from_eval2.value.ptr->ntype == NT_CODESEGMENT)
   {
      argroot->childset[i] = copy_tree(from_eval2.value.ptr->childset[0], 
         argroot);
   }
   else
   {
      argroot->childset[i] = copy_tree(from_eval2.value.ptr, argroot);
   }


   retval.ti.dtype = DT_POINTER;
   retval.value.ptr = argroot->childset[i];

   free_data(from_eval1);
   free_data(from_eval2);
   return retval;
}



data eval_math_oper_assign(node* to_eval)
{
   data retval, from_eval1, from_eval2, dt_ret, * from_resolve;
   size_t nb_clos;
   closure* destination = NULL, source, * pclos_ret;
   char* left_symbol;
   cont_type ct_ret;

   memset(&retval, 0, sizeof(data));

   if (!to_eval) return retval;
   if (to_eval->nb_childs <= 1) return retval;
   if (!to_eval->childset[0] || !to_eval->childset[1]) return retval;

   /* Right child value computation. */
   from_eval2 = eval(to_eval->childset[1]);

   switch(to_eval->childset[0]->ntype)
   {
   case NT_VARIABLE:
      if (same_symbol(to_eval->childset[0], to_eval->childset[1]))
         return from_eval2;

      left_symbol = to_eval->childset[0]->opval.name;
      
      /* Search in all the closures for the variable name. */
      destination = find_symbol(left_symbol);

      if (!destination)
      {
         /* New closure creation. */
         nb_clos = ++clos_set_stack[set_stack_size-1]->nb_clos;
         clos_set_stack[set_stack_size-1]->clos_array = 
            realloc(clos_set_stack[set_stack_size-1]->clos_array,
            nb_clos* sizeof(closure*));
         if (!clos_set_stack[set_stack_size-1]->clos_array)
         {
            yyerror("Error: Lack of memory in eval_math_oper.");
            exit(1);
         }

         clos_set_stack[set_stack_size-1]->clos_array[nb_clos-1] = malloc(sizeof(closure));
         if (!clos_set_stack[set_stack_size-1]->clos_array[nb_clos-1])
         {
            yyerror("Error: Lack of memory in eval_math_oper.");
            exit(1);
         }
         memset(clos_set_stack[set_stack_size-1]->clos_array[nb_clos-1], 0, 
            sizeof(closure));
         clos_set_stack[set_stack_size-1]->clos_array[nb_clos-1]->pContainer = 
            clos_set_stack[set_stack_size-1];

         destination = clos_set_stack[set_stack_size-1]->
            clos_array[nb_clos-1];

         source.name = to_eval->childset[0]->opval.name;
      }
      else
      {
         free_data(destination->content);
         /* Name already exist, do not copy. */
         source.name = NULL;
      }

      destination->content = from_eval2;
      if (source.name)
      {
         destination->name = malloc(strlen(source.name) + 1);
         if (!destination->name)
         {
            yyerror("Error: Lack of memory in eval_math_oper_assign for name.");
            exit(1);
         }
         strcpy(destination->name, source.name);
      }

      return from_eval2;

   case NT_LIST:
   case NT_VARGENLIST:
      return assign_node_pointers(to_eval, from_eval2);

   case NT_MATH_OPER:
      if (to_eval->childset[0]->opval.math_oper != '*') return retval;

      from_eval1 = eval(to_eval->childset[0]->childset[0]);
      if (from_eval1.ti.nderef > 0)
      {
         size_t nderef = from_eval1.ti.nderef;
         void* adr = from_eval1.value.genptr;

         while (nderef > 1)
         {
            adr = (void*)*(void**)adr;
            nderef--;
         }

         *(data*)adr = from_eval2;

         return from_eval2;
      }
      else
      {
         return assign_node_pointers(to_eval, from_eval2);
      }
 
   case NT_ACCESSLIST:
      if (same_symbol(to_eval->childset[0], to_eval->childset[1]))
         return from_eval2;

      ct_ret = resolve_accesslist(to_eval->childset[0], &pclos_ret, &dt_ret);

      if (ct_ret == CLOSURE)
      {
         free_data(pclos_ret->content);
         copy_data(&pclos_ret->content, from_eval2);
         return from_eval2;
      }
      if (ct_ret == DATA)
      {
         yyerror("Error: Trying to assign to a return value.");
      }
      return retval;

   case NT_SUBSCRIPT:
      from_resolve = resolve_subscript(to_eval->childset[0]);

      if (!from_resolve)
      {
         data index;
         size_t llindex = 0;

         from_resolve = resolve(to_eval->childset[0]->childset[0]);

         if (!from_resolve)
         {
            free_data(from_eval2);
            yyerror("Error: Assign failed to subscripted value.");
            return retval;
         }

         if (from_resolve->ti.dtype != DT_STRING && from_resolve->ti.dtype != DT_MEMORY)
         {
            free_data(from_eval2);
            yyerror("Error: Expecting a string or memory before subscript.");
            return retval;
         }

         index = eval(to_eval->childset[0]->childset[1]);
         if (index.ti.dtype < DT_CHAR || index.ti.dtype > DT_LONG_DOUBLE ||
            index.ti.nderef > 0)
         {
            yyerror("Error: Number expected as subscript in assign.");
            abort_called = 1;
            free_data(from_eval2);
            free_data(index);
            return retval;
         }

         mac_cast(llindex, size_t, index)

         if (llindex >= from_resolve->value.str.length)
         {
            yyerror("Error: String or memory subscript out of bound.");
            abort_called = 1;
            free_data(from_eval2);
            return retval;
         }

         if (from_resolve->ti.dtype == DT_STRING)
         {
            if (from_eval2.ti.dtype != DT_STRING)
            {
               yyerror("Error: Can't assign non string to string.");
               abort_called = 1;
               free_data(from_eval2);
               return retval;
            }

            if (from_eval2.value.str.length == 0)
            {
               yyerror("Error: Can't assign from zero length string.");
               abort_called = 1;
               free_data(from_eval2);
               return retval;
            }

            from_resolve->value.str.tab[llindex] = from_eval2.value.
               str.tab[0];

            retval.value.str.length = 2;
            retval.value.str.tab = malloc(2);
            if (!retval.value.str.tab)
            {
               yyerror("Error: Lack of memory in assign for string data.");
               exit(1);
            }
            g_lst_add(retval.value.str.tab, PT_CHAR_TAB);
            retval.value.str.tab[0] = from_eval2.value.str.tab[0];
            retval.value.str.tab[1] = '\0';

            retval.ti.dtype = DT_STRING;

            free_data(from_eval2);
         }
         else /* from_resolve->ti.dtype == DT_MEMORY */
         {
            unsigned char newval = 0;

            if (from_eval2.ti.dtype < DT_CHAR || from_eval2.ti.dtype > DT_LONG_DOUBLE ||
               from_eval2.ti.nderef > 0)
            {
               yyerror("Error: Number expected as rigth operand to assign.");
               abort_called = 1;
               free_data(from_eval2);
               return retval;
            }

            mac_cast(newval, unsigned char, from_eval2)

            from_resolve->value.mem.tab[llindex] = newval;

            retval.value.byte = newval;
            retval.ti.dtype = DT_BYTE;
         }

         return retval;
      }

      free_data(*from_resolve);
      copy_data(from_resolve, from_eval2);

      return from_eval2;

   default:
      yyerror("Error: Can't do assign.");
      return retval;
   }
}



data eval_indir(node* to_eval)
{
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));

   from_eval = eval(to_eval->childset[0]);

   if (from_eval.ti.nderef <= 0)
   {
      if (from_eval.ti.dtype != DT_POINTER)
      {
         yyerror("Type error for unary operator \'*\'.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }
      else
      {
         return from_eval;
      }
   }
   else if (from_eval.ti.nderef == 1)
   {
      copy_data(&retval, *(data*)from_eval.value.genptr);
   }
   else
   {
      retval.ti.dtype = from_eval.ti.dtype;
      retval.ti.nderef = from_eval.ti.nderef - 1;
      retval.value.genptr = *(void**)from_eval.value.genptr;
   }

   return retval;
}



data eval_ref(node* to_eval)
{
   data retval, * pdata;

   memset(&retval, 0, sizeof(data));

   switch (to_eval->childset[0]->ntype)
   {
   case NT_VARIABLE:
   case NT_ACCESSLIST:
   case NT_SUBSCRIPT:
      pdata = resolve(to_eval->childset[0]);
      if (!pdata)
      {
         data index;
         int intindex = 0;
         pdata = resolve(to_eval->childset[0]->childset[0]);
         if (!pdata)
         {
            return retval;
         }
         if (pdata->ti.dtype != DT_MEMORY && pdata->ti.dtype != DT_STRING)
         {
            return retval;
         }

         /* Subscript number evaluation. */
         index = eval(to_eval->childset[0]->childset[1]);
         if (index.ti.dtype < DT_CHAR || index.ti.dtype > DT_LONG_DOUBLE ||
            index.ti.nderef > 0)
         {
            yyerror("Error: Number expected as subscript.");
            free_data(index);
            return retval;
         }

         mac_cast(intindex, int, index)
         abort_called = 0;
         retval.value.memadr = (unsigned char*)&pdata->value.mem.tab[intindex];
         retval.ti.dtype = DT_MEMADR;

         return retval;
      }
      retval.ti.dtype = pdata->ti.dtype;
      retval.ti.nderef = pdata->ti.nderef + 1;
      retval.value.genptr = pdata;
      break;
   case NT_MATH_OPER:
      if (to_eval->childset[0]->opval.math_oper == '*')
      {
         if (to_eval->childset[0]->nb_childs == 1)
         {
            retval = eval_ref(to_eval->childset[0]->childset[0]);
         }
         else
         {
            yyerror("Invalid operand for reference operator.");
         }
      }
      else
      {
         yyerror("Invalid operand for reference operator.");
      }
      break;
   default:
      yyerror("Invalid operand for reference operator.");
   }

   return retval;
}



data eval_math_oper(node* to_eval)
{
   data retval, from_eval1, from_eval2;
   int err = 0;
   data_type dtmaxt;

   memset(&retval, 0, sizeof(data));

   switch (to_eval->opval.math_oper)
   {
   case '-':
      if (to_eval->nb_childs == 1)
      {
         from_eval1 = eval(to_eval->childset[0]);
         if (from_eval1.ti.dtype < DT_CHAR || from_eval1.ti.dtype > DT_LONG_DOUBLE ||
            from_eval1.ti.nderef > 0)
         {
            yyerror("Type error for unary operator \'-\'.");
            abort_called = 1;
            free_data(from_eval1);
            return retval;
         }
         retval.ti.dtype = from_eval1.ti.dtype;
         switch (from_eval1.ti.dtype)
         {
         case DT_CHAR:
            retval.value.cnum = -from_eval1.value.cnum;
            break;
         case DT_S_CHAR:
            retval.value.scnum = -from_eval1.value.scnum;
            break;
         case DT_U_CHAR:
         case DT_BYTE:
            retval.value.ucnum = -(char)from_eval1.value.ucnum;
            break;
         case DT_SHORT:
            retval.value.shnum = -from_eval1.value.shnum;
            break;
         case DT_U_SHORT:
            retval.value.ushnum = -(short)from_eval1.value.ushnum;
            break;
         case DT_INT:
            retval.value.inum = -from_eval1.value.inum;
            break;
         case DT_U_INT:
            retval.value.uinum = -(int)from_eval1.value.uinum;
            break;
         case DT_LONG:
            retval.value.lnum = -from_eval1.value.lnum;
            break;
         case DT_U_LONG:
            retval.value.ulnum = -(long)from_eval1.value.ulnum;
            break;
         case DT_LONG_LONG:
            retval.value.llnum = -from_eval1.value.llnum;
            break;
         case DT_U_LONG_LONG:
            retval.value.ullnum = -(long long)from_eval1.value.ullnum;
            break;
         case DT_SIZE_T:
            #if defined(BITS32)
            retval.value.stnum = -(long)from_eval1.value.stnum;
            #elif defined(BITS64)
            retval.value.stnum = -(long long)from_eval1.value.stnum;
            #endif
            break;
         case DT_FLOAT:
         case DT_DOUBLE:
            retval.value.num = -from_eval1.value.num;
            break;
         case DT_LONG_DOUBLE:
            retval.value.ldnum = -from_eval1.value.ldnum;
            break;
         default:
            break;
         }
         return retval;
      }
      else if (to_eval->nb_childs == 2)
      {
         from_eval1 = eval(to_eval->childset[0]);
         from_eval2 = eval(to_eval->childset[1]);
         if (from_eval1.ti.dtype < DT_CHAR || from_eval1.ti.dtype > DT_LONG_DOUBLE ||
            from_eval2.ti.dtype < DT_CHAR || from_eval2.ti.dtype > DT_LONG_DOUBLE ||
            from_eval1.ti.nderef > 0 || from_eval2.ti.nderef > 0)
         {
            yyerror("Type error for binary operator \'-\'.");
            abort_called = 1;
            free_data(from_eval1);
            free_data(from_eval2);
            return retval;
         }
         mac_bin_top(-)
         return retval;
      }
      else
      {
         fprintf(stderr, "Error: Wrong child number ");
         fprintf(stderr, "for a \'-\' in eval_math_oper.\n");
         exit(1);
      }

   case '+':
      from_eval1 = eval(to_eval->childset[0]);
      from_eval2 = eval(to_eval->childset[1]);
      if (from_eval1.ti.dtype < DT_CHAR || from_eval1.ti.dtype > DT_LONG_DOUBLE ||
         from_eval2.ti.dtype < DT_CHAR || from_eval2.ti.dtype > DT_LONG_DOUBLE ||
         from_eval1.ti.nderef > 0 || from_eval2.ti.nderef > 0)
      {
         yyerror("Type error for operator \'+\'.");
         abort_called = 1;
         free_data(from_eval1);
         free_data(from_eval2);
         return retval;
      }
      mac_bin_top(+)
      return retval;

   case '*':
      if (to_eval->nb_childs == 1)
      {
         return eval_indir(to_eval);
      }
      else if (to_eval->nb_childs == 2)
      {
         from_eval1 = eval(to_eval->childset[0]);
         from_eval2 = eval(to_eval->childset[1]);
         if (from_eval1.ti.dtype < DT_CHAR || from_eval1.ti.dtype > DT_LONG_DOUBLE ||
            from_eval2.ti.dtype < DT_CHAR || from_eval2.ti.dtype > DT_LONG_DOUBLE ||
            from_eval1.ti.nderef > 0 || from_eval2.ti.nderef > 0)
         {
            yyerror("Type error for binary operator \'*\'.");
            abort_called = 1;
            free_data(from_eval1);
            free_data(from_eval2);
           return retval;
         }
         mac_bin_top(*)
         return retval;
      }
      else
      {
         fprintf(stderr, "Error: Wrong child number ");
         fprintf(stderr, "for a \'*\' in eval_math_oper.\n");
         exit(1);
      }

   case '/':
      from_eval1 = eval(to_eval->childset[0]);
      from_eval2 = eval(to_eval->childset[1]);
      if (from_eval1.ti.dtype < DT_CHAR || from_eval1.ti.dtype > DT_LONG_DOUBLE ||
         from_eval2.ti.dtype < DT_CHAR || from_eval2.ti.dtype > DT_LONG_DOUBLE ||
         from_eval1.ti.nderef > 0 || from_eval2.ti.nderef > 0)
      {
         yyerror("Type error for operator \'/\'.");
         abort_called = 1;
         free_data(from_eval1);
         free_data(from_eval2);
         return retval;
      }
      switch (from_eval2.ti.dtype)
      {
      case DT_CHAR:
         if (from_eval2.value.cnum == 0) err = 1;
         break;
      case DT_S_CHAR:
         if (from_eval2.value.scnum == 0) err = 1;
         break;
      case DT_U_CHAR:
      case DT_BYTE:
         if (from_eval2.value.ucnum == 0) err = 1;
         break;
      case DT_SHORT:
         if (from_eval2.value.shnum == 0) err = 1;
         break;
      case DT_U_SHORT:
         if (from_eval2.value.ushnum == 0) err = 1;
         break;
      case DT_INT:
         if (from_eval2.value.inum == 0) err = 1;
         break;
      case DT_U_INT:
         if (from_eval2.value.uinum == 0) err = 1;
         break;
      case DT_LONG:
         if (from_eval2.value.lnum == 0) err = 1;
         break;
      case DT_U_LONG:
         if (from_eval2.value.ulnum == 0) err = 1;
         break;
      case DT_LONG_LONG:
         if (from_eval2.value.llnum == 0) err = 1;
         break;
      case DT_U_LONG_LONG:
         if (from_eval2.value.ullnum == 0) err = 1;
         break;
      case DT_SIZE_T:
         if (from_eval2.value.stnum == 0) err = 1;
         break;
      case DT_FLOAT:
      case DT_DOUBLE:
         if (from_eval2.value.num == 0.0) err = 1;
         break;
      case DT_LONG_DOUBLE:
         if (from_eval2.value.ldnum == 0.0) err = 1;
         break;
      default:
         break;
      }
      if (err == 1)
      {
         yyerror("Error: Zero divide attempt.");
         abort_called = 1;
         free_data(from_eval1);
         free_data(from_eval2);
         return retval;
      }
      mac_bin_top(/)
      return retval;

   case '^':
      from_eval1 = eval(to_eval->childset[0]);
      from_eval2 = eval(to_eval->childset[1]);
      if (from_eval1.ti.dtype < DT_CHAR || from_eval1.ti.dtype > DT_LONG_DOUBLE ||
         from_eval2.ti.dtype < DT_CHAR || from_eval2.ti.dtype > DT_LONG_DOUBLE ||
         from_eval1.ti.nderef > 0 || from_eval2.ti.nderef > 0)
      {
         yyerror("Type error for operator \'^\'.");
         abort_called = 1;
         free_data(from_eval1);
         free_data(from_eval2);
         return retval;
      }
      dtmaxt = max(from_eval1.ti.dtype, from_eval2.ti.dtype);
      if (dtmaxt == DT_LONG_DOUBLE)
      {
         long double ldleft = 0.0, ldright = 0.0;
         mac_cast(ldleft, long double, from_eval1)
         mac_cast(ldright, long double, from_eval2)
         retval.ti.dtype = DT_LONG_DOUBLE;
         retval.value.ldnum = powl(ldleft, ldright);
      }
      else
      {
         double dleft = 0.0, dright = 0.0;
         mac_cast(dleft, double, from_eval1)
         mac_cast(dright, double, from_eval2)
         retval.ti.dtype = DT_DOUBLE;
         retval.value.num = pow(dleft, dright);
      }
      return retval;
      
   case '=':
      return eval_math_oper_assign(to_eval);

   case '&':
      return eval_ref(to_eval);

   default:
      yyerror("Error: Unknown operator in eval_math_oper.");
      exit(1);
   }
}



data eval_rel_oper_eq(data dat1, data dat2)
{
   data retval;

   memset(&retval, 0, sizeof(data));
   retval.ti.dtype = DT_INT;

   if (dat1.ti.nderef == 0 && dat2.ti.nderef == 0)
   {
      if (dat1.ti.dtype >= DT_CHAR && dat1.ti.dtype <= DT_LONG_DOUBLE && 
         dat2.ti.dtype >= DT_CHAR && dat2.ti.dtype <= DT_LONG_DOUBLE)
      {
         mac_rel_top(dat1, ==, dat2)
      }

      else if (dat1.ti.dtype == DT_STRING && dat2.ti.dtype == DT_STRING)
      {
         if (!strcmp(dat1.value.str.tab, dat2.value.str.tab)) 
            retval.value.inum = 1;
      }

      else if (dat1.ti.dtype == DT_POINTER && dat2.ti.dtype == DT_POINTER)
      {
         retval.value.inum = dat1.value.ptr == dat2.value.ptr;
      }

      else if (dat1.ti.dtype == DT_PFILE && dat2.ti.dtype == DT_PFILE)
      {
         retval.value.inum = dat1.value.pfile == dat2.value.pfile;
      }
   }
   else if (dat1.ti.nderef > 0 && dat2.ti.nderef == 0)
   {
      mac_rel_left_ptr(dat1, ==, dat2)
   }
   else if (dat1.ti.nderef == 0 && dat2.ti.nderef > 0)
   {
      mac_rel_left_ptr(dat2, ==, dat1)
   }
   else if (dat1.ti.nderef > 0 && dat2.ti.nderef > 0)
   {
      retval.value.inum = dat1.value.genptr == dat2.value.genptr;
   }
   else
   {
      yyerror("Invalid reference level with == operator.");
      abort_called = 1;
      retval.ti.dtype = DT_UNDEF;
      return retval;
   }

   return retval;
}



data eval_rel_oper(node* to_eval)
{
   data retval, from_eval1, from_eval2;
   int boolres1, boolres2;

   memset(&retval, 0, sizeof(data));

   switch (to_eval->opval.rel_oper)
   {
   case OR:
      from_eval1 = eval(to_eval->childset[0]);
      from_eval2 = eval(to_eval->childset[1]);
      boolres1 = to_bool(from_eval1);
      boolres2 = to_bool(from_eval2);
      free_data(from_eval1);
      free_data(from_eval2);
      retval.ti.dtype = DT_INT;
      retval.value.inum = boolres1 || boolres2;
      break;

   case AND:
      from_eval1 = eval(to_eval->childset[0]);
      from_eval2 = eval(to_eval->childset[1]);
      boolres1 = to_bool(from_eval1);
      boolres2 = to_bool(from_eval2);
      free_data(from_eval1);
      free_data(from_eval2);
      retval.ti.dtype = DT_INT;
      retval.value.inum = boolres1 && boolres2;
      break;

   case NOT:
      from_eval1 = eval(to_eval->childset[0]);
      boolres1 = to_bool(from_eval1);
      free_data(from_eval1);
      retval.ti.dtype = DT_INT;
      retval.value.inum = !boolres1;
      break;

   case GT:
      mac_compare(>, <)
      break;

   case GE:
      mac_compare(>=, <=)
      break;

   case LT:
      mac_compare(<, >)
      break;

   case LE:
      mac_compare(<=, >=)
      break;

case EQ:
      from_eval1 = eval(to_eval->childset[0]);
      from_eval2 = eval(to_eval->childset[1]);
      retval = eval_rel_oper_eq(from_eval1, from_eval2);
      free_data(from_eval1);
      free_data(from_eval2);
      break;

   case NE:
      from_eval1 = eval(to_eval->childset[0]);
      from_eval2 = eval(to_eval->childset[1]);
      retval = eval_rel_oper_eq(from_eval1, from_eval2);
      retval.value.inum = !retval.value.inum;
      free_data(from_eval1);
      free_data(from_eval2);
      break;

   default:
      yyerror("Error: Unknown operator in eval_rel_oper.");
      exit(1);
   }

   return retval;
}



/* Ideally, the argument is a list of one node. */
data eval_print(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in print.");
      yyerror("       This function has one parameter.");
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);

   display("", from_eval);

   free_data(from_eval);
   return retval;   
}



data object_base(size_t nb_members)
{
   data retval;

   memset(&retval, 0, sizeof(data));

   retval.ti.dtype = DT_OBJECT;
   retval.value.pObject = malloc(sizeof(clos_set));
   if (!retval.value.pObject) 
      fatal_error("Error : Lack of memory in object_base for new object.");
   memset(retval.value.pObject, 0, sizeof(clos_set));
   g_lst_add(retval.value.pObject, PT_OBJECT);

   retval.value.pObject->clos_array = calloc(nb_members, sizeof(closure*));
   if (!retval.value.pObject->clos_array) 
      fatal_error("Error : Lack of memory in object_base for new member array.");

   retval.value.pObject->nb_clos = nb_members;

   return retval;
}

void object_member(data * retval, size_t memnum, char * memname)
{
   retval->value.pObject->clos_array[memnum] = malloc(sizeof(closure));
   if (!retval->value.pObject->clos_array[memnum]) 
      fatal_error("Error : Lack of memory in object_member for new member.");
   memset(retval->value.pObject->clos_array[memnum], 0, sizeof(closure));

   retval->value.pObject->clos_array[memnum]->name = malloc(strlen(memname) + 1);
   if (!retval->value.pObject->clos_array[memnum]->name) 
      fatal_error("Error : Lack of memory in object_member for name.");

   strcpy(retval->value.pObject->clos_array[memnum]->name, memname);

   retval->value.pObject->clos_array[memnum]->pContainer = retval->value.pObject;
}

void object_str_val(data * retval, size_t memnum, char * memval)
{
   string* pstr = &retval->value.pObject->clos_array[memnum]->content.value.str;
   pstr->length = strlen(memval) + 1;

   retval->value.pObject->clos_array[memnum]->content.ti.dtype = DT_STRING;
   pstr->tab = malloc(pstr->length);
   if (!pstr->tab) fatal_error("Error : Lack of memory in object_member for name.");

   strcpy(pstr->tab, memval);

   g_lst_add(pstr->tab, PT_CHAR_TAB);
}

data eval_info(node* to_eval)
{
   int err = 0, hasparams = 0;
   data retval, from_eval;
   char* opstr = NULL, * caststr = NULL;
   size_t i;
   string* pstr = NULL;

   memset(&retval, 0, sizeof(data));

   
   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in info.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_POINTER || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of info have not the node pointer type.");
      free_data(from_eval);
      return retval;
   }

   if (!from_eval.value.ptr)
   {
      yyerror("Error: The argument of info is NULL.");
      free_data(from_eval);
      return retval;
   }


   /* to_eval is now the evaluated node pointer value. */
   to_eval = from_eval.value.ptr;

   switch(to_eval->ntype)
   {
   case NT_NUM_CONST:
      retval = object_base(3);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "numerical constant");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      object_member(&retval, 2, "value");
      retval.value.pObject->clos_array[2]->content.ti.dtype = DT_DOUBLE;
      retval.value.pObject->clos_array[2]->content.value.num = to_eval->opval.value;
      break;

   case NT_MATH_OPER:
      retval = object_base(3);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "mathematical operator");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      object_member(&retval, 2, "operator");
      retval.value.pObject->clos_array[2]->content.ti.dtype = DT_CHAR;
      retval.value.pObject->clos_array[2]->content.value.cnum = to_eval->opval.math_oper;
      break;

   case NT_REL_OPER:
      retval = object_base(3);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "relational or logical operator");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;

      switch(to_eval->opval.rel_oper)
      {
      case OR:
         opstr = "||";
         break;
      case AND:
         opstr = "&&";
         break;
      case NOT:
         opstr = "!";
         break;
      case GT:
         opstr = ">";
         break;
      case GE:
         opstr = ">=";
         break;
      case LT:
         opstr = "<";
         break;
      case LE:
         opstr = "<=";
         break;
      case EQ:
         opstr = "==";
         break;
      case NE:
         opstr = "!=";
         break;
      default:
         fatal_error("Error: Unknown operator in info.");
      }
      object_member(&retval, 2, "operator");
      object_str_val(&retval, 2, opstr);
      break;

   case NT_VARIABLE:
      retval = object_base(3);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "variable");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      object_member(&retval, 2, "name");
      object_str_val(&retval, 2, to_eval->opval.name);
      break;

   case NT_STRING:
      retval = object_base(3);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "string");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      object_member(&retval, 2, "value");
      object_str_val(&retval, 2, to_eval->opval.str.tab);
      break;

   case NT_FUNC_DEF:
      if (to_eval->nb_childs >= 1)
      {
         if (to_eval->childset[0]) hasparams = to_eval->childset[0]->nb_childs > 0;
      }
      if (hasparams) retval = object_base(3);
      else retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "function definition");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;

      if (hasparams)
      {
         array* parr = NULL;

         object_member(&retval, 2, "parameters");

         /* array creation with a parameter per item */
         parr = malloc(sizeof(array));
         if (!parr) fatal_error("Error: Lack of memory in info for array struct.");
         memset(parr, 0, sizeof(array));
         g_lst_add(parr, PT_ARRAY);

         retval.value.pObject->clos_array[2]->content.ti.dtype = DT_ARRAY;
         retval.value.pObject->clos_array[2]->content.value.pArray = parr;

         parr->length = to_eval->childset[0]->nb_childs;
         parr->dtable = malloc(parr->length * sizeof(data));
         if (!parr->dtable) fatal_error("Error: Lack of memory in info for array data.");
         memset(parr->dtable, 0, parr->length * sizeof(data));

         for (i = 0; i < to_eval->childset[0]->nb_childs; i++)
         {
            if (to_eval->childset[0]->childset[i]->ntype != NT_VARIABLE)
               fatal_error("Error: Found a non variable function parameter in info.");

            parr->dtable[i].value.str.length = strlen(to_eval->childset[0]->childset[i]->opval.name + 1);
            parr->dtable[i].value.str.tab = malloc(parr->dtable[i].value.str.length);
            if (!parr->dtable[i].value.str.tab) fatal_error("Error: Lack of memory in info for string data.");
            g_lst_add(parr->dtable[i].value.str.tab, PT_CHAR_TAB);
            strcpy(parr->dtable[i].value.str.tab, to_eval->childset[0]->childset[i]->opval.name);

            parr->dtable[i].ti.dtype = DT_STRING;
         }
      }

      break;

   case NT_FUNC_CALL:
      retval = object_base(3);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "function call");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      object_member(&retval, 2, "nb_args");
      retval.value.pObject->clos_array[2]->content.ti.dtype = DT_SIZE_T;
      if (to_eval->nb_childs >= 2) retval.value.pObject->clos_array[2]->content.value.stnum = 
         to_eval->childset[1]->nb_childs;
      else retval.value.pObject->clos_array[2]->content.value.stnum = 0;
      break;

   case NT_IF_STMT:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "if statement");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_IFELSE_STMT:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "if else statement");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_FOR_STMT:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "for statement");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_WHILE_STMT:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "while statement");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_LIST:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "list");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_CODESEGMENT:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "code segment");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_PARENT:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "parent reserved word");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_CHILDSET:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "childset reserved word");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_VARGENLIST:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "vargenlist");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_INCR_DECR:
      retval = object_base(3);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "incrementation or decrementation");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;

      switch(to_eval->opval.ppmm)
      {
      case PREPLUSPLUS:
         opstr = "prefix ++";
         break;
      case PREMINUSMINUS:
         opstr = "prefix --";
         break;
      case POSTPLUSPLUS:
         opstr = "postfix ++";
         break;
      case POSTMINUSMINUS:
         opstr = "postfix --";
         break;
      default:
         fatal_error("Error: Unknown incr. or decr. operator in info.");
      }

      object_member(&retval, 2, "operator");
      object_str_val(&retval, 2, opstr);
      break;

   case NT_ACCESSLIST:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "object member access list");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_SUBSCRIPT:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "subscript");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_STDIN:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "stdin FILE* identifier");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_STDOUT:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "stdout FILE* identifier");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_STDERR:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "stderr FILE* identifier");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_CAST:
      retval = object_base(3);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "cast");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;

      switch (to_eval->opval.ti.dtype)
      {
      case DT_CHAR:
         caststr = "char";
         break;
      case DT_S_CHAR:
         caststr = "signed char";
         break;
      case DT_U_CHAR:
         caststr = "unsigned char";
         break;
      case DT_BYTE:
         caststr = "byte";
         break;
      case DT_SHORT:
         caststr = "short";
         break;
      case DT_U_SHORT:
         caststr = "unsigned short";
         break;
      case DT_INT:
         caststr = "int";
         break;
      case DT_U_INT:
         caststr = "unsigned int";
         break;
      case DT_LONG:
         caststr = "long";
         break;
      case DT_U_LONG:
         caststr = "unsigned long";
         break;
      case DT_LONG_LONG:
         caststr = "long long";
         break;
      case DT_U_LONG_LONG:
         caststr = "unsigned long long";
         break;
      case DT_SIZE_T:
         caststr = "size_t";
         break;
      case DT_FLOAT:
         caststr = "float";
         break;
      case DT_DOUBLE:
         caststr = "double";
         break;
      case DT_LONG_DOUBLE:
         caststr = "long double";
         break;
      default:
         fatal_error("Error: Unknown cast type in info function.");
      }

      object_member(&retval, 2, "cast_type");

      pstr = &retval.value.pObject->clos_array[2]->content.value.str;
      pstr->length = strlen(caststr) + to_eval->opval.ti.nderef + 1;

      retval.value.pObject->clos_array[2]->content.ti.dtype = DT_STRING;
      pstr->tab = malloc(pstr->length);
      if (!pstr->tab) fatal_error("Error : Lack of memory in info for name.");

      strcpy(pstr->tab, caststr);
      if (to_eval->opval.ti.nderef > 0)
      {
         memset(&pstr->tab[strlen(caststr)], '*', to_eval->opval.ti.nderef);
         pstr->tab[pstr->length - 1] = '\0';
      }

      g_lst_add(pstr->tab, PT_CHAR_TAB);
      break;

   case NT_REF:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "structure reference operator (.)");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   case NT_DEREF:
      retval = object_base(2);
      object_member(&retval, 0, "node_type");
      object_str_val(&retval, 0, "structure dereference operator (->)");
      object_member(&retval, 1, "nb_childs");
      retval.value.pObject->clos_array[1]->content.ti.dtype = DT_SIZE_T;
      retval.value.pObject->clos_array[1]->content.value.stnum = to_eval->nb_childs;
      break;

   default:
      fatal_error("Error: Unknown node type in info.");
   }

   free_data(from_eval);
   return retval;
}



data eval_mknode(node* to_eval)
{
   int err = 0;
   data retval, source, dest, index;
   size_t i, lindx = 0;

   memset(&retval, 0, sizeof(data));

   
   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in mknode.");
      yyerror("       This function has three parameters.");
      return retval;
   }

   /* Parameter one: destination. */
   dest = eval(to_eval->childset[0]);
   if (dest.ti.dtype != DT_POINTER || dest.ti.nderef > 0)
   {
      yyerror("Error: Argument one of mknode is not a node pointer.");
      free_data(dest);
      return retval;
   }
   if (!dest.value.ptr)
   {
      yyerror("Error: Argument one of mknode is NULL.");
      return retval;
   }
   if (dest.value.ptr->ntype != NT_LIST)
   {
      yyerror("Error: Argument one of mknode is not a list.");
      return retval;
   }

   /* Parameter two: source. */
   source = eval(to_eval->childset[1]);
   if (source.ti.dtype != DT_POINTER || source.ti.nderef > 0)
   {
      yyerror("Error: Argument two of mknode is not a node pointer.");
      free_data(source);
      return retval;
   }
   if (source.value.ptr)
   {
      if (source.value.ptr->ntype == NT_CODESEGMENT)
         source.value.ptr = source.value.ptr->childset[0];
   }
   else
   {
      yyerror("Error: The argument two of mknode is NULL.");
      return retval;
   }

   /* Parameter three: index. */
   index = eval(to_eval->childset[2]);
   if (index.ti.dtype < DT_CHAR || index.ti.dtype > DT_LONG_DOUBLE ||
      index.ti.nderef > 0)
   {
      yyerror("Error: Argument three in mknode is not numerical.");
      abort_called = 1;
      free_data(index);
      return retval;
   }
   mac_cast(lindx, size_t, index)
   if (lindx > dest.value.ptr->nb_childs)
   {
      yyerror("Array index out of range in mknode.");
      return retval;
   }


   /* List array enlargement. */
   dest.value.ptr->childset = realloc(dest.value.ptr->childset,
      (dest.value.ptr->nb_childs + 1) * sizeof(node*));
   if (!dest.value.ptr->childset)
   {
      yyerror("Lack of memory in mknode.");
      exit(1);
   }

   /* Childs relocation */
   for (i = dest.value.ptr->nb_childs; i > lindx; i--)
   {
      dest.value.ptr->childset[i] = dest.value.ptr->childset[i-1];
   }

   dest.value.ptr->childset[lindx] = copy_tree(source.value.ptr, 
      dest.value.ptr);
   dest.value.ptr->nb_childs++;

   return retval;
}



data eval_rmnode(node* to_eval)
{
   int err = 0;
   size_t lindx = 0, i;
   data retval, list, index;

   memset(&retval, 0, sizeof(data));

   
   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in rmnode.");
      yyerror("       This function has two parameters.");
      return retval;
   }

   /* Parameter one: list. */
   list = eval(to_eval->childset[0]);
   if (list.ti.dtype != DT_POINTER || list.ti.nderef > 0)
   {
      yyerror("Error: Argument one in rmnode is not a node pointer.");
      free_data(list);
      return retval;
   }
   if (!list.value.ptr)
   {
      yyerror("Error: Argument one in rmnode is NULL.");
      return retval;
   }
   if (list.value.ptr->ntype != NT_LIST)
   {
      yyerror("Error: Argument one in rmnode is not a list.");
      return retval;
   }

   /* Parameter two: index. */
   index = eval(to_eval->childset[1]);
   if (index.ti.dtype < DT_CHAR || index.ti.dtype > DT_LONG_DOUBLE ||
      index.ti.nderef > 0)
   {
      yyerror("Error: Argument two in rmnode is not numerical.");
      abort_called = 1;
      free_data(index);
      return retval;
   }
   mac_cast(lindx, size_t, index)
   if (lindx >= list.value.ptr->nb_childs)
   {
      yyerror("Array index out of bound in rmnode.");
      return retval;
   }


   /* Childs relocation */
   for (i = lindx; i < list.value.ptr->nb_childs - 1; i++)
   {
      list.value.ptr->childset[i] = list.value.ptr->childset[i+1];
   }

   /* List array reduction. */
   list.value.ptr->childset = realloc(list.value.ptr->childset,
      --list.value.ptr->nb_childs * sizeof(node*));
   if (!list.value.ptr->childset)
   {
      yyerror("Lack of memory in rmnode.");
      exit(1);
   }

   return retval;
}



/* Returns the numerical value or the string of a node. */
data eval_value(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in value.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);

   if (from_eval.ti.dtype != DT_POINTER || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of value is not a node pointer.");
      free_data(from_eval);
      return retval;
   }

   if (!from_eval.value.ptr)
   {
      yyerror("Error: The argument of value is NULL.");
      return retval;
   }


   if (from_eval.value.ptr->ntype == NT_NUM_CONST)
   {
      retval.value.num = from_eval.value.ptr->opval.value;
      retval.ti.dtype = DT_DOUBLE;
   }
   else if (from_eval.value.ptr->ntype == NT_STRING)
   {
      retval.value.str.length = from_eval.value.ptr->opval.str.length;
      retval.value.str.tab = malloc(from_eval.value.ptr->opval.str.length);
      if (!retval.value.str.tab)
      {
         yyerror("Error: Lack of memory in eval_value.");
         exit(1);
      }
      g_lst_add(retval.value.str.tab, PT_CHAR_TAB);
      memcpy(retval.value.str.tab, from_eval.value.ptr->opval.str.tab,
         from_eval.value.ptr->opval.str.length);
      retval.ti.dtype = DT_STRING;
   }
   else
   {
      yyerror("Error: The node in value is not numeric or a string.");
   }

   return retval;
}



data eval_length(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in length.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of length must not be a pointer.");
      return retval;
   }

   if (from_eval.ti.dtype == DT_POINTER)
   {
      if (from_eval.value.ptr->ntype != NT_LIST)
      {
         yyerror("Error: The node pointer in argument does not reference a "
            "statement list.");
         abort_called = 1;
         return retval;
      }

      retval.value.stnum = from_eval.value.ptr->nb_childs;
   }
   else if (from_eval.ti.dtype == DT_ARRAY)
   {
      retval.value.stnum = from_eval.value.pArray->length;
      free_data(from_eval);
   }
   else if (from_eval.ti.dtype == DT_LIST)
   {
      retval.value.stnum = from_eval.value.pList->length;
      free_data(from_eval);
   }
   else if (from_eval.ti.dtype == DT_STRING || from_eval.ti.dtype == DT_MEMORY)
   {
      retval.value.stnum = from_eval.value.str.length;
   }
   else
   {
      yyerror("Error: Wrong argument type in length.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_SIZE_T;

   return retval;
}



char scanbuf[SCANBUF_LEN];

data eval_scan(void)
{
   int i, wbegin, wlen;
   data retval;
   node* listnode = xxemptylist();

   memset(&retval, 0, sizeof(data));
   retval.ti.dtype = DT_POINTER;
   retval.value.ptr = listnode;

   scanf("%[a-zA-Z0-9.,?! ]", scanbuf);
   /* Eating the '\n' */
   getchar();

   i = wbegin = wlen = 0;
   while (i < SCANBUF_LEN && scanbuf[i] != '\0')
   {
      if (scanbuf[i] != ' ' && scanbuf[i] != '\t')
      {
         if (scanbuf[i] == '.' || scanbuf[i] == ',' || scanbuf[i] == '?'
            || scanbuf[i] == '!')
         {
            if (wlen > 0)
            {
               add_word(listnode, &scanbuf[wbegin], wlen);
               wlen = 0;
            }
            add_word(listnode, &scanbuf[i], 1);
         }
         else
         {
            if (wlen == 0) wbegin = i;
            wlen++;
         }
      }
      else
      {
         if (wlen > 0)
         {
            add_word(listnode, &scanbuf[wbegin], wlen);
            wlen = 0;
         }
      }
      i++;
   }
   if (wlen > 0) add_word(listnode, &scanbuf[wbegin], wlen);

   return retval;
}



data eval_read(node* to_eval)
{
   int err = 0, wlen;
   data retval, from_eval;
   FILE* is;
   node* listnode;
   char c;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in read.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);

   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of read is not a string.");
      free_data(from_eval);
      return retval;
   }


   /* File opening. */
   is = fopen(from_eval.value.str.tab, "r");
   if (!is)
   {
      fprintf(stderr, "Error: Could not open %s.\n", from_eval.value.str.
         tab);
      free_data(from_eval);
      return retval;
   }

   listnode = xxemptylist();
   retval.ti.dtype = DT_POINTER;
   retval.value.ptr = listnode;

   wlen = 0;
   while ((c = (char)fgetc(is))!= EOF && wlen < SCANBUF_LEN)
   {
      if (c != ' ' && c != '\n' && c != '\t')
      {
         if (c == '.' || c == ',' || c == '?' || c == '!')
         {
            if (wlen > 0)
            {
               add_word(listnode, scanbuf, wlen);
               wlen = 0;
            }
            scanbuf[0] = c;
            add_word(listnode, scanbuf, 1);
         }
         else
         {
            scanbuf[wlen] = c;
            wlen++;
         }
      }
      else
      {
         if (wlen > 0)
         {
            add_word(listnode, scanbuf, wlen);
            wlen = 0;
         }
      }
   }
   if (wlen > 0) add_word(listnode, scanbuf, wlen);
   if (wlen == SCANBUF_LEN)
   {  
      fprintf(stderr, "Error: word longer than %i characters detected.\n", 
         SCANBUF_LEN);
   }

   fclose(is);

   free_data(from_eval);

   return retval;
}



data eval_write(node* to_eval)
{
   int err = 0;
   data retval, dest, list, separ;
   FILE* os;
   size_t i;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in write.");
      yyerror("       This function has three parameters.");
      return retval;
   }

   /* Parameter one: destination file name. */
   dest = eval(to_eval->childset[0]);
   if (dest.ti.dtype != DT_STRING || dest.ti.nderef > 0)
   {
      yyerror("Error: Argument one of write is not a string.");
      free_data(dest);
      return retval;
   }

   /* Parameter two: list. */
   list = eval(to_eval->childset[1]);
   if (list.ti.dtype != DT_POINTER || list.ti.nderef > 0)
   {
      yyerror("Error: Argument two of write is not a node pointer.");
      free_data(list);
      return retval;
   }
   if (!list.value.ptr)
   {
      yyerror("Error: Argument two of write is NULL.");
      return retval;
   }
   if (list.value.ptr->ntype != NT_LIST)
   {
      yyerror("Error: Argument two of write is not a list.");
      return retval;
   }

   /* Parameter three: separator. */
   separ = eval(to_eval->childset[2]);
   if (separ.ti.dtype != DT_STRING || separ.ti.nderef > 0)
   {
      yyerror("Error: Argument three of write is not a string.");
      free_data(separ);
      return retval;
   }


   /* File opening. */
   os = fopen(dest.value.str.tab, "w");
   if (!os)
   {
      fprintf(stderr, "Error: Could not open %s.\n", dest.value.str.tab);
      free_data(dest);
      free_data(separ);
      return retval;
   }

   if (list.value.ptr->childset[0]->ntype != NT_STRING)
   {
      yyerror("Error: Node type is not string in write.");
      fclose(os);
      free_data(dest);
      free_data(separ);
      return retval;
   }

   fprintf(os, "%s", list.value.ptr->childset[0]->opval.str.tab);

   for(i = 1; i < list.value.ptr->nb_childs; i++)
   {
      if (list.value.ptr->childset[i]->ntype != NT_STRING)
      {
         yyerror("Error: Node type is not string in write.");
         fclose(os);
         free_data(dest);
         free_data(separ);
         return retval;
      }

      fprintf(os, "%s", separ.value.str.tab);
      fprintf(os, "%s", list.value.ptr->childset[i]->opval.str.tab);
   }

   fclose(os);
   free_data(dest);
   free_data(separ);

   return retval;
}



data eval_getwd(void)
{
   data retval;

   memset(&retval, 0, sizeof(data));

   #ifdef _WIN32
   if (!_getcwd(scanbuf, SCANBUF_LEN))
   {
      yyerror("Error with _getcwd in getwd.");
      return retval;
   }
   #else
   if (!getcwd(scanbuf, SCANBUF_LEN))
   {
      yyerror("Error with getcwd in getwd.");
      return retval;
   }
   #endif

   retval.value.str.length = strlen(scanbuf) + 1;
   retval.value.str.tab = malloc(retval.value.str.length);
   if (!retval.value.str.tab)
   {
      yyerror("Error: Lack ok memory in eval_getwd for string data.");
      exit(1);
   }
   g_lst_add(retval.value.str.tab, PT_CHAR_TAB);
   strcpy(retval.value.str.tab, scanbuf);
   retval.ti.dtype = DT_STRING;

   return retval;
}



data eval_setwd(node* to_eval)
{
   int err = 0;
   data retval, path;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in write.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   path = eval(to_eval->childset[0]);
   if (path.ti.dtype != DT_STRING || path.ti.nderef > 0)
   {
      yyerror("Error: The argument of setwd is not a string.");
      free_data(path);
      return retval;
   }

   #ifdef _WIN32
   if (_chdir(path.value.str.tab) == -1)
      yyerror("Error: Path not found.");
   #else
   if (chdir(path.value.str.tab) == -1)
      yyerror("Error: Path not found.");
   #endif

   free_data(path);

   return retval;
}



data eval_tonode(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   node* npt;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in tonode.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype < DT_CHAR || from_eval.ti.dtype > DT_STRING ||
      from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of tonode is not a string or numeric.");
      free_data(from_eval);
      return retval;
   }

   /* Node creation. */
   npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in eval_tonode for new node.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));
   g_lst_add(npt, PT_NODE);

   if (from_eval.ti.dtype == DT_STRING)
   {
      npt->ntype = NT_STRING;
      npt->opval.str.length = from_eval.value.str.length;
      npt->opval.str.tab = malloc(npt->opval.str.length);
      if (!npt->opval.str.tab)
      {
         yyerror("Error: Lack of memory in eval_tonode for new string.");
         exit(1);
      }
      memcpy(npt->opval.str.tab, from_eval.value.str.tab, 
         from_eval.value.str.length);

      free_data(from_eval);
   }
   else if (from_eval.ti.dtype >= DT_CHAR && from_eval.ti.dtype <= DT_LONG_DOUBLE)
   {
      npt->ntype = NT_NUM_CONST;
      mac_cast(npt->opval.value, double, from_eval)
   }

   npt->parent = NULL;
   npt->nb_childs = 0;
   npt->childset = NULL;

   retval.value.ptr = npt;
   retval.ti.dtype = DT_POINTER;

   return retval;
}



data eval_subseq(node* to_eval)
{
   int err = 0;
   size_t lstart = 0, lend = 0, len, i, j;
   data retval, seq, start, end;
   size_t seqlen = 0;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in subseq.");
      yyerror("       This function has three parameters.");
      abort_called = 1;
      return retval;
   }

   /* Parameter one: sequence. */
   seq = eval(to_eval->childset[0]);
   if (seq.ti.nderef > 0)
   {
      yyerror("Error: Argument one of subseq must not be a pointer.");
      return retval;
   }
   if (seq.ti.dtype == DT_POINTER)
   {
      if (!seq.value.ptr)
      {
         yyerror("Error: Argument one in subseq is NULL.");
         abort_called = 1;
         return retval;
      }
      if (seq.value.ptr->ntype != NT_LIST)
      {
         yyerror("Error: Argument one in subseq is not a list.");
         abort_called = 1;
         return retval;
      }
   }
   else if (seq.ti.dtype != DT_STRING && seq.ti.dtype != DT_ARRAY && seq.ti.dtype != DT_LIST)
   {
      yyerror("Error: Wrong first argument type in subseq.");
      abort_called = 1;
      return retval;
   }

   /* Sequence length. */
   switch(seq.ti.dtype)
   {
   case DT_POINTER:
      seqlen = seq.value.ptr->nb_childs;
      break;
   case DT_STRING:
      seqlen = seq.value.str.length;
      break;
   case DT_ARRAY:
      seqlen = seq.value.pArray->length;
      break;
   case DT_LIST:
      seqlen = seq.value.pList->length;
      break;
   default:
      break;
   }

   /* Parameter two: index of start. */
   start = eval(to_eval->childset[1]);
   if (start.ti.dtype < DT_CHAR || start.ti.dtype > DT_LONG_DOUBLE ||
      start.ti.nderef > 0)
   {
      yyerror("Error: Argument two in subseq is not numerical.");
      abort_called = 1;
      free_data(start);
      free_data(seq);
      return retval;
   }
   mac_cast(lstart, size_t, start)
   if (lstart >= seqlen)
   {
      yyerror("Error: Start array index out of bound in subseq.");
      abort_called = 1;
      free_data(seq);
      return retval;
   }

   /* Parameter three: index of end. */
   end = eval(to_eval->childset[2]);
   if (end.ti.dtype < DT_CHAR || end.ti.dtype > DT_LONG_DOUBLE ||
      end.ti.nderef > 0)
   {
      yyerror("Error: Argument three in subseq is not numerical.");
      abort_called = 1;
      free_data(seq);
      return retval;
   }
   mac_cast(lend, size_t, end)
   if (lend < lstart || lend >= seqlen)
   {
      yyerror("Error: End array index out of bound in subseq.");
      abort_called = 1;
      free_data(seq);
      return retval;
   }


   /* Length of subsequence to allocate. */
   len = lend - lstart + 1;

   if (seq.ti.dtype == DT_POINTER)
   {
      node* npt = malloc(sizeof(node));
      if (!npt)
      {
         yyerror("Error: Lack of memory in eval_subseq for new node.");
         exit(1);
      }
      memset(npt, 0, sizeof(node));
      g_lst_add(npt, PT_NODE);
      npt->ntype = NT_LIST;
      npt->opval.str.length = 0;
      npt->opval.str.tab = NULL;
      npt->parent = NULL;
      npt->nb_childs = len;

      npt->childset = malloc(len * sizeof(node*));
      if (!npt->childset)
      {
         yyerror("Error: Lack of memory in eval_subseq for subnodes.");
         exit(1);
      }
      memset(npt->childset, 0, len * sizeof(node*));

      /* Subtrees copying. */
      j = 0;
      for (i = lstart; i <= lend; i++)
      {
         npt->childset[j] = copy_tree(seq.value.ptr->childset[i], npt);
         j++;
      }

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = npt;
   }
   else if (seq.ti.dtype == DT_STRING)
   {
      char* str = malloc(len + 1);
      if (!str)
      {
         yyerror("Error: Lack of memory in subseq for new string data.");
         exit(1);
      }
      memset(str, 0, len + 1);
      g_lst_add(str, PT_CHAR_TAB);

      j = 0;
      for (i = lstart; i <= lend; i++)
      {
         str[j] = seq.value.str.tab[i];
         j++;
      }
      str[len] = '\0';

      retval.ti.dtype = DT_STRING;
      retval.value.str.length = len + 1;
      retval.value.str.tab = str;
   }
   else if (seq.ti.dtype == DT_ARRAY)
   {
      array* parr = malloc(sizeof(array));
      if (!parr)
      {
         yyerror("Error: Lack of memory in subseq for new array struct.");
         exit(1);
      }
      memset(parr, 0, sizeof(array));
      g_lst_add(parr, PT_ARRAY);
      parr->length = len;
      parr->dtable = malloc(len * sizeof(data));
      if (!parr->dtable)
      {
         yyerror("Error: Lack of memory in subseq for new array data.");
         exit(1);
      }
      memset(parr->dtable, 0, len * sizeof(data));

      j = 0;
      for (i = lstart; i <= lend; i++)
      {
         copy_data(&parr->dtable[j], seq.value.pArray->dtable[i]);
      }

      retval.ti.dtype = DT_ARRAY;
      retval.value.pArray = parr;
   }
   else if (seq.ti.dtype == DT_LIST)
   {
      listlink** destination, * src = seq.value.pList->start;
      list* plst = malloc(sizeof(list));
      if (!plst)
      {
         yyerror("Error: Lack of memory in subseq for new list struct.");
         exit(1);
      }
      memset(plst, 0, sizeof(list));
      g_lst_add(plst, PT_LIST);
      plst->length = len;
      destination = &plst->start;

      for (i = 0; i < lstart; i++)
      {
         src = src->next;
      }
      for (i = lstart; i <= lend; i++)
      {
         *destination = malloc(sizeof(listlink));
         if (!*destination)
         {
            yyerror("Error: Lack of memory in subseq for new link.");
            exit(1);
         }
         memset(*destination, 0, sizeof(listlink));
         copy_data(&(*destination)->content, src->content);

         destination = &(*destination)->next;
         src = src->next;
      }

      retval.ti.dtype = DT_LIST;
      retval.value.pList = plst;
   }

   free_data(seq);

   return retval;
}



data eval_concat(node* to_eval)
{
   int err = 0;
   data retval, * data_tab;
   size_t len = 0, i, j, k;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in concat.");
      yyerror("       This function has at least two parameters.");
      return retval;
   }


   data_tab = malloc(to_eval->nb_childs * sizeof(data));
   if (!data_tab)
   {
      yyerror("Error: Lack of memory in eval_concat for data_tab.");
      exit(1);
   }
   memset(data_tab, 0, to_eval->nb_childs * sizeof(data));

   /* Arguments evaluation */
   for (i = 0; i < to_eval->nb_childs; i++)
   {
      data_tab[i] = eval(to_eval->childset[i]);
      if (data_tab[i].ti.nderef > 0)
      {
         yyerror("Error: At least one argument of concat is a pointer.");
         for (j = 0; j <= i; j++) free_data(data_tab[j]);
         free(data_tab);
         return retval;
      }
   }

   /* Same type verification */
   for (i = 1; i < to_eval->nb_childs; i++)
   {
      if (data_tab[0].ti.dtype != data_tab[i].ti.dtype)
      {
         yyerror("Error: Arguments of concat have not all the same type.");
         for (i = 0; i < to_eval->nb_childs; i++) free_data(data_tab[i]);
         free(data_tab);
         return retval;
      }
   }

   if (data_tab[0].ti.dtype == DT_POINTER)
   {
      node* npt;

      /* Lists verification */
      for (i = 0; i < to_eval->nb_childs; i++)
      {
         if (!data_tab[i].value.ptr) err = 1;
         else
         {
            if (data_tab[i].value.ptr->ntype != NT_LIST)
            {
               err = 1;
            }
         }
         if (err)
         {
            yyerror("Error: Non list found in references passed to concat.");
            for (i = 0; i < to_eval->nb_childs; i++) free_data(data_tab[i]);
            free(data_tab);
            return retval;
         }
      }

      /* Output list length. */
      for (i = 0; i < to_eval->nb_childs; i++)
      {
         len += data_tab[i].value.ptr->nb_childs;
      }

      npt = malloc(sizeof(node));
      if (!npt)
      {
         yyerror("Error: Lack of memory in eval_concat for new node.");
         exit(1);
      }
      memset(npt, 0, sizeof(node));
      g_lst_add(npt, PT_NODE);
      npt->ntype = NT_LIST;
      npt->opval.str.length = 0;
      npt->opval.str.tab = NULL;
      npt->parent = NULL;
      npt->nb_childs = len;

      npt->childset = malloc(len * sizeof(node*));
      if (!npt->childset)
      {
         yyerror("Error: Lack of memory in eval_concat for subnodes.");
         exit(1);
      }
      memset(npt->childset, 0, len * sizeof(node*));

      /* Subtrees copying. */
      k = 0;
      for (i = 0; i < to_eval->nb_childs; i++)
      {
         for (j = 0; j < data_tab[i].value.ptr->nb_childs; j++)
         {
            npt->childset[k] = copy_tree(data_tab[i].value.ptr->childset[j], 
               npt);
            k++;
         }
      }

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = npt;
   }
   else if (data_tab[0].ti.dtype == DT_ARRAY)
   {
      array* parr;

      /* Output array length. */
      for (i = 0; i < to_eval->nb_childs; i++)
      {
         len += data_tab[i].value.pArray->length;
      }

      parr = malloc(sizeof(array));
      if (!parr)
      {
         yyerror("Error: Lack of memory in concat for new array struct.");
         exit(1);
      }
      memset(parr, 0, sizeof(array));
      g_lst_add(parr, PT_ARRAY);
      parr->length = len;
      parr->dtable = malloc(len * sizeof(data));
      if (!parr->dtable)
      {
         yyerror("Error: Lack of memory in concat for new array content.");
         exit(1);
      }
      memset(parr->dtable, 0, len * sizeof(data));

      k = 0;
      for (i = 0; i < to_eval->nb_childs; i++)
      {
         for (j = 0; j < data_tab[i].value.pArray->length; j++)
         {
            copy_data(&parr->dtable[k], data_tab[i].value.pArray->dtable[j]);
            k++;
         }
      }

      retval.ti.dtype = DT_ARRAY;
      retval.value.pArray = parr;
   }
   else if (data_tab[0].ti.dtype == DT_LIST)
   {
      list* plst;
      listlink** destination, * plink_dest, * plink_src;

      /* Output list length. */
      for (i = 0; i < to_eval->nb_childs; i++)
      {
         len += data_tab[i].value.pList->length;
      }

      plst = malloc(sizeof(list));
      if (!plst)
      {
         yyerror("Error: Lack of memory in concat for new list struct.");
         exit(1);
      }
      memset(plst, 0, sizeof(list));
      g_lst_add(plst, PT_LIST);
      plst->length = len;

      /* List creation */
      destination = &plst->start;
      for (i = 0; i < len; i++)
      {
         *destination = malloc(sizeof(list));
         if (!*destination)
         {
            yyerror("Error: Lack of memory in concat for chain link.");
            exit(1);
         }
         destination = &(*destination)->next;
      }
      *destination = NULL;

      /* List filling */
      plink_dest = plst->start;
      for (i = 0; i < to_eval->nb_childs; i++)
      {
         plink_src = data_tab[i].value.pList->start;
         for (j = 0; j < data_tab[i].value.pList->length; j++)
         {
            copy_data(&plink_dest->content, plink_src->content);
            plink_src = plink_src->next;
            plink_dest = plink_dest->next;
         }
      }

      retval.ti.dtype = DT_LIST;
      retval.value.pList = plst;
   }
   else if (data_tab[0].ti.dtype == DT_STRING)
   {
      char* str;

      /* Output string length. */
      for (i = 0; i < to_eval->nb_childs; i++)
      {
         len += strlen(data_tab[i].value.str.tab);
      }

      str = malloc(len + 1);
      if (!str)
      {
         yyerror("Error: Lack of memory in concat for new string data.");
         exit(1);
      }
      memset(str, 0, len + 1);
      g_lst_add(str, PT_CHAR_TAB);

      j = 0;
      for (i = 0; i < to_eval->nb_childs; i++)
      {
         strcpy(&str[j], data_tab[i].value.str.tab);
         j += strlen(data_tab[i].value.str.tab);
      }

      retval.ti.dtype = DT_STRING;
      retval.value.str.length = len + 1;
      retval.value.str.tab = str;
   }
   else
   {
      yyerror("Error: Unexpected argument type in concat.");
      abort_called = 1;
   }

   for (i = 0; i < to_eval->nb_childs; i++) free_data(data_tab[i]);
   free(data_tab);

   return retval;
}



data eval_ntoa(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   char buf[40];

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in ntoa.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype < DT_CHAR || from_eval.ti.dtype > DT_LONG_DOUBLE ||
      from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of ntoa is not numeric.");
      free_data(from_eval);
      return retval;
   }


   /* Conversion from double to string. */
   switch(from_eval.ti.dtype)
   {
   case DT_CHAR:
      sprintf(buf, "%c", from_eval.value.cnum);
      break;
   case DT_S_CHAR:
      sprintf(buf, "%c", from_eval.value.scnum);
      break;
   case DT_U_CHAR:
   case DT_BYTE:
      sprintf(buf, "%u", from_eval.value.ucnum);
      break;
   case DT_SHORT:
      sprintf(buf, "%hd", from_eval.value.shnum);
      break;
   case DT_U_SHORT:
      sprintf(buf, "%hu", from_eval.value.ushnum);
      break;
   case DT_INT:
      sprintf(buf, "%d", from_eval.value.inum);
      break;
   case DT_U_INT:
      sprintf(buf, "%u", from_eval.value.uinum);
      break;
   case DT_LONG:
      sprintf(buf, "%ld", from_eval.value.lnum);
      break;
   case DT_U_LONG:
      sprintf(buf, "%lu", from_eval.value.ulnum);
      break;
   case DT_LONG_LONG:
      sprintf(buf, "%lld", from_eval.value.llnum);
      break;
   case DT_U_LONG_LONG:
      sprintf(buf, "%llu", from_eval.value.ullnum);
      break;
   case DT_SIZE_T:
      sprintf(buf, "%zu", from_eval.value.stnum);
      break;
   case DT_FLOAT:
   case DT_DOUBLE:
      sprintf(buf, "%.10g", from_eval.value.num);
      break;
   case DT_LONG_DOUBLE:
      sprintf(buf, "%Lg", from_eval.value.ldnum);
      break;
    default:
      break;
   }

   retval.value.str.length = strlen(buf) + 1;
   retval.value.str.tab = malloc(retval.value.str.length);
   if (!retval.value.str.tab)
   {
      yyerror("Error: Lack of memory in ntoa.");
      exit(1);
   }
   memset(retval.value.str.tab, 0, retval.value.str.length);
   g_lst_add(retval.value.str.tab, PT_CHAR_TAB);
   strcpy(retval.value.str.tab, buf);
   retval.ti.dtype = DT_STRING;

   return retval;
}



data eval_aton(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in aton.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of aton is not a string.");
      free_data(from_eval);
      return retval;
   }

   retval.value.num = atof(from_eval.value.str.tab);
   retval.ti.dtype = DT_DOUBLE;

   free_data(from_eval);

   return retval;
}



data eval_return(node* to_eval)
{
   data retval;
   memset(&retval, 0, sizeof(data));

   if (!to_eval)
   {
      return_called = 1;
      return retval;
   }
   if (to_eval->nb_childs == 0)
   {
      return_called = 1;
      return retval;
   }
   if (to_eval->nb_childs > 1)
   {
      yyerror("Error: The return function takes no or one argument.");
      abort_called = 1;
      return retval;
   }

   retval = eval(to_eval->childset[0]);

   return_called = 1;

   return retval;
}



data eval_copytree(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in copytree.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_POINTER || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of copytree is not a node pointer.");
      free_data(from_eval);
      return retval;
   }

   retval.ti.dtype = DT_POINTER;
   retval.value.ptr = copy_tree(from_eval.value.ptr, NULL);
   g_lst_add(retval.value.ptr, PT_NODE);

   return retval;
}



data eval_clear(node* to_eval)
{
   data retval;
   size_t i, j, arg;

   memset(&retval, 0, sizeof(data));

   /* to_eval is not NULL, see rules with "subseq". */
   if (to_eval->nb_childs == 0)
   {
       /* Erasing all variables. */
      for (i = 0; i < set_stack_size; i++)
      {
         for (j = 0; j < clos_set_stack[i]->nb_clos; j++)
         {
            free(clos_set_stack[i]->clos_array[j]->name);
            free_data(clos_set_stack[i]->clos_array[j]->content);
            free(clos_set_stack[i]->clos_array[j]);
         }

         free(clos_set_stack[i]->clos_array);

         if (i)
         {
            free(clos_set_stack[i]);
            clos_set_stack[i] = NULL;
         }
         else
         {
            clos_set_stack[0]->nb_clos = 0;
            clos_set_stack[0]->clos_array = NULL;
         }
      }
   }
   else
   {
      /* Erasing selected symbols. */
      for (arg = 0; arg < to_eval->nb_childs; arg++)
      {
         if (to_eval->childset[arg]->ntype == NT_VARIABLE)
         {
            clear_symbol(to_eval->childset[arg]->opval.name);
         }
      }
   }

   return retval;
}



data eval_execute(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in execute.");
      yyerror("       This function has one parameter.");
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);

   if (from_eval.ti.dtype != DT_POINTER || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of execute is not a node pointer.");
      free_data(from_eval);
      return retval;
   }

   return eval(from_eval.value.ptr);
}



data eval_prompt(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in prompt.");
      yyerror("       This function has one parameter.");
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);

   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of prompt is not a string.");
      free_data(from_eval);
      return retval;
   }

   change_prompt(from_eval.value.str.tab);

   free_data(from_eval);

   return retval;
}



data eval_names(node* to_eval)
{
   int err = 0;
   data retval, * from_eval;
   size_t arg, i;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in names.");
      yyerror("       This function has one parameter or more.");
      return retval;
   }

   from_eval = malloc(to_eval->nb_childs * sizeof(data));
   if (!from_eval)
   {
      yyerror("Error : Lack of memory in names for results table.");
      exit(1);
   }
   memset(from_eval, 0, to_eval->nb_childs * sizeof(data));

   arg = 0;
   while (arg < to_eval->nb_childs && !err)
   {
      from_eval[arg] = eval(to_eval->childset[arg]);
      if (from_eval[arg].ti.dtype != DT_STRING || from_eval[arg].ti.nderef > 0) err = 1;
      else if (bad_symbol(from_eval[arg].value.str.tab)) err = 1;
      arg++;
   }

   if (err)
   {
      yyerror("Error: One argument in names call is not a string");
      yyerror("       or an acceptable string.");
      for (i = 0; i < arg; i++) free_data(from_eval[i]);
      free(from_eval);
      return retval;
   }


   /* The clos_set struct is used for an object. */
   /* Each member has a name, a value type and a value. */

   retval.ti.dtype = DT_OBJECT;
   retval.value.pObject = malloc(sizeof(clos_set));
   if (!retval.value.pObject)
   {
      yyerror("Error : Lack of memory in names for new object.");
      exit(1);
   }
   memset(retval.value.pObject, 0, sizeof(clos_set));
   g_lst_add(retval.value.pObject, PT_OBJECT);

   retval.value.pObject->clos_array = malloc(arg * sizeof(closure*));
   if (!retval.value.pObject->clos_array)
   {
      yyerror("Error : Lack of memory in names for new member array.");
      exit(1);
   }
   memset(retval.value.pObject->clos_array, 0, arg * sizeof(closure*));

   retval.value.pObject->nb_clos = to_eval->nb_childs;

   for (arg = 0; arg < to_eval->nb_childs; arg++)
   {
      retval.value.pObject->clos_array[arg] = malloc(sizeof(closure));
      if (!retval.value.pObject->clos_array[arg])
      {
         yyerror("Error : Lack of memory in names for new member.");
         exit(1);
      }
      memset(retval.value.pObject->clos_array[arg], 0, sizeof(closure));

      retval.value.pObject->clos_array[arg]->name = malloc(from_eval[arg].value.
         str.length);
      if (!retval.value.pObject->clos_array[arg]->name)
      {
         yyerror("Error : Lack of memory in names for new member name.");
         exit(1);
      }
      memcpy(retval.value.pObject->clos_array[arg]->name, from_eval[arg].value.
         str.tab, from_eval[arg].value.str.length);
      retval.value.pObject->clos_array[arg]->pContainer = retval.value.pObject;
   }

   for (i = 0; i < arg; i++) free_data(from_eval[i]);
   free(from_eval);

   return retval;
}



data eval_array(node* to_eval)
{
   int zerolen = 0;
   data retval;
   array* pArr;
   size_t arg;

   memset(&retval, 0, sizeof(data));

   if (to_eval)
   {
      if (to_eval->nb_childs == 0) zerolen = 1;
   }
   else zerolen = 1;

   pArr = malloc(sizeof(array));
   if (!pArr)
   {
      yyerror("Error: Lack of memory in eval_array for array.");
      exit(1);
   }
   memset(pArr, 0, sizeof(array));
   g_lst_add(pArr, PT_ARRAY);
   pArr->length = to_eval->nb_childs;

   if (zerolen)
   {
      pArr->dtable = NULL;
      retval.ti.dtype = DT_ARRAY;
      retval.value.pArray = pArr;
      return retval;
   }

   pArr->dtable = malloc(to_eval->nb_childs * sizeof(data));
   if (!pArr->dtable)
   {
      yyerror("Error: Lack of memory in eval_array for data table.");
      exit(1);
   }
   memset(pArr->dtable, 0, to_eval->nb_childs * sizeof(data));

   for (arg = 0; arg < to_eval->nb_childs; arg++)
   {
      pArr->dtable[arg] = eval(to_eval->childset[arg]);
   }

   retval.ti.dtype = DT_ARRAY;
   retval.value.pArray = pArr;

   return retval;
}



data eval_list(node* to_eval)
{
   int zerolen = 0;
   data retval;
   list* plst;
   listlink** destination;
   size_t arg;

   memset(&retval, 0, sizeof(data));

   if (to_eval)
   {
      if (to_eval->nb_childs == 0) zerolen = 1;
   }
   else zerolen = 1;

   plst = malloc(sizeof(list));
   if (!plst)
   {
      yyerror("Error: Lack of memory in eval_list for list.");
      exit(1);
   }
   memset(plst, 0, sizeof(list));
   g_lst_add(plst, PT_LIST);
   plst->length = to_eval->nb_childs;

   if (zerolen)
   {
      plst->start = NULL;
      retval.ti.dtype = DT_LIST;
      retval.value.pList = plst;
      return retval;
   }

   destination = &plst->start;
   for (arg = 0; arg < to_eval->nb_childs; arg++)
   {
      *destination = malloc(sizeof(listlink));
      if (!*destination)
      {
         yyerror("Error: Lack of memory in eval_list for link.");
         exit(1);
      }
      memset(*destination, 0, sizeof(listlink));
      (*destination)->content = eval(to_eval->childset[arg]);
      destination = &(*destination)->next;
   }
   *destination = NULL;
   retval.ti.dtype = DT_LIST;
   retval.value.pList = plst;

   return retval;
}



data eval_fillobject(node* to_eval)
{
   int err = 0;
   data retval, * pdata, from_eval;
   clos_set* pobj;
   closure* pclos;
   size_t nIter, i;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fillobject.");
      yyerror("       This function has two parameters.");
      abort_called = 1;
      return retval;
   }


   /* Findind a DT_POINTER to the destination object. */

   if (to_eval->childset[0]->ntype == NT_VARIABLE)
   {
      pclos = find_symbol(to_eval->childset[0]->opval.name);
      if (!pclos)
      {
         fprintf(stderr, "Error: Symbol \"%s\" not found.\n", to_eval->childset
            [0]->opval.name);
         abort_called = 1;
         return retval;
      }
      if (pclos->content.ti.dtype != DT_OBJECT)
      {
         yyerror("Error: Variable is not an object.");
         abort_called = 1;
         return retval;
      }

      pobj = pclos->content.value.pObject;
   }
   else if (to_eval->childset[0]->ntype == NT_ACCESSLIST)
   {
      cont_type ct;
      ct = resolve_accesslist(to_eval->childset[0], &pclos, &from_eval);

      if (ct != CLOSURE) return retval;

      if (pclos->content.ti.dtype != DT_OBJECT)
      {
         yyerror("Error: Access list does not resolve to an object.");
         abort_called = 1;
         return retval;
      }

      pobj = pclos->content.value.pObject;
   }
   else if (to_eval->childset[0]->ntype == NT_SUBSCRIPT)
   {
      pdata = resolve_subscript(to_eval->childset[0]);
      if (!pdata) return retval;

      if (pdata->ti.dtype != DT_OBJECT)
      {
         yyerror("Error: Subscripting does not resolve to an object.");
         abort_called = 1;
         return retval;
      }

      pobj = pdata->value.pObject;
   }
   else
   {
      yyerror("Error: Wrong first argument type in fillobject.");
      return retval;
   }


   /* Copying data from second argument to first. */

   from_eval = eval(to_eval->childset[1]);
   if (from_eval.ti.nderef > 0)
   {
      yyerror("Error: Second argument of fillobject must not be a pointer.");
      return retval;
   }

   if (from_eval.ti.dtype == DT_ARRAY)
   {
      nIter = min(pobj->nb_clos, from_eval.value.pArray->length);

      for (i = 0; i < nIter; i++)
      {
         free_data(pobj->clos_array[i]->content);
         copy_data(&pobj->clos_array[i]->content, from_eval.value.pArray->
            dtable[i]);
      }
   }
   else if (from_eval.ti.dtype == DT_LIST)
   {
      listlink* plink = from_eval.value.pList->start;
      nIter = min(pobj->nb_clos, from_eval.value.pList->length);

      for (i = 0; i < nIter; i++)
      {
         free_data(pobj->clos_array[i]->content);
         copy_data(&pobj->clos_array[i]->content, plink->content);
         plink = plink->next;
      }
   }
   else if (from_eval.ti.dtype == DT_OBJECT)
   {
      nIter = min(pobj->nb_clos, from_eval.value.pObject->nb_clos);

      for (i = 0; i < nIter; i++)
      {
         free_data(pobj->clos_array[i]->content);
         copy_data(&pobj->clos_array[i]->content, from_eval.value.pObject->
            clos_array[i]->content);
      }
   }
   else
   {
      yyerror("Error: Unsupported data type in fillobject second argument.");
   }

   free_data(from_eval);
   return retval;
}



data eval_setlength(node* to_eval)
{
   int err = 0;
   data retval, length, * pdest;
   size_t oldlen, i, llength = 0;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in setlength.");
      yyerror("       This function has two parameters.");
      abort_called = 1;
      return retval;
   }

   length = eval(to_eval->childset[1]);
   if (length.ti.dtype < DT_CHAR || length.ti.dtype > DT_LONG_DOUBLE ||
      length.ti.nderef > 0)
   {
      yyerror("Error: Second argument type in setlength is not numeric.");
      abort_called = 1;
      free_data(length);
      return retval;
   }
   mac_cast(llength, size_t, length)


   /* Resolving the symbol received as first argument. */
   pdest = resolve(to_eval->childset[0]);
   if (!pdest) return retval;
   if (pdest->ti.nderef > 0)
   {
      yyerror("Error: First argument of setlength must not be a pointer.");
      return retval;
   }

   /* Redimension */
   if (pdest->ti.dtype == DT_ARRAY)
   {
      oldlen = pdest->value.pArray->length;
      pdest->value.pArray->length = llength;

      if (oldlen < llength)
      {
         pdest->value.pArray->dtable = realloc(pdest->value.pArray->dtable, 
            llength * sizeof(data));
         if (!pdest->value.pArray->dtable)
         {
            yyerror("Error: Lack of memory in setlength for greater array.");
            exit(1);
         }
         memset(&pdest->value.pArray->dtable[oldlen], 0, (llength - oldlen) 
            * sizeof(data));
      }
      else if (oldlen == llength)
      {
         return retval;
      }
      else
      {
         for (i = oldlen; i < llength; i++)
         {
            free_data(pdest->value.pArray->dtable[i]);
         }
         if (llength == 0)
         {
            free(pdest->value.pArray->dtable);
            pdest->value.pArray->dtable = NULL;
            return retval;
         }
         pdest->value.pArray->dtable = realloc(pdest->value.pArray->dtable, 
            llength * sizeof(data));
         if (!pdest->value.pArray->dtable)
         {
            yyerror("Error: Lack of memory in setlength for smaller array.");
            exit(1);
         }
         memset(&pdest->value.pArray->dtable[llength-1], 0, sizeof(data));
      }
   }
   else if (pdest->ti.dtype == DT_LIST)
   {
      oldlen = pdest->value.pList->length;
      pdest->value.pList->length = llength;

      if (oldlen < llength)
      {
         listlink** destination = &pdest->value.pList->start;
         for (i = 0; i < oldlen; i++)
         {
            destination = &(*destination)->next;
         }
         for (i = oldlen; i < llength; i++)
         {
            *destination = malloc(sizeof(listlink));
            if (!*destination)
            {
               yyerror("Error: Lack of emmory in setlength for new link.");
               exit(1);
            }
            memset(*destination, 0, sizeof(listlink));
            destination = &(*destination)->next;
         }
      }
      else if (oldlen == llength)
      {
         return retval;
      }
      else
      {
         listlink* plink = pdest->value.pList->start, * next;
         for (i = 0; i < llength; i++)
         {
            plink = plink->next;
         }
         for (i = llength; i < oldlen; i++)
         {
            next = plink->next;
            free_data(plink->content);
            free(plink);
            plink = next;
         }
         if (llength == 0)
         {
            pdest->value.pList->start = NULL;
         }
      }
   }
   else if (pdest->ti.dtype == DT_STRING || pdest->ti.dtype == DT_MEMORY)
   {
      oldlen = pdest->value.str.length;
      pdest->value.str.length = llength;

      if (oldlen < llength)
      {
         g_lst_remove(pdest->value.str.tab, PT_CHAR_TAB);
         pdest->value.str.tab = realloc(pdest->value.str.tab, llength);
         if (!pdest->value.str.tab)
         {
            yyerror("Error: Lack of memory in setlength for longer string.");
            exit(1);
         }
         memset(&pdest->value.str.tab[oldlen - 1], ' ', llength - oldlen);
         g_lst_add(pdest->value.str.tab, PT_CHAR_TAB);
         pdest->value.str.tab[llength - 1] = '\0';
      }
      else if (oldlen == llength)
      {
         return retval;
      }
      else
      {
         g_lst_remove(pdest->value.str.tab, PT_CHAR_TAB);
         pdest->value.str.tab = realloc(pdest->value.str.tab, llength);
         if (!pdest->value.str.tab)
         {
            yyerror("Error: Lack of memory in setlength for shorter string.");
            exit(1);
         }
         g_lst_add(pdest->value.str.tab, PT_CHAR_TAB);
         pdest->value.str.tab[llength - 1] = '\0';
      }
   }
   else if (pdest->ti.dtype == DT_POINTER)
   {
      if (pdest->value.ptr->ntype != NT_LIST)
      {
         yyerror("Error: Node pointer in setlength does not reference a "
            "statement list.");
         abort_called = 1;
         return retval;
      }

      oldlen = pdest->value.ptr->nb_childs;
      pdest->value.ptr->nb_childs = llength;
      if (oldlen < llength)
      {
         pdest->value.ptr->childset = realloc(pdest->value.ptr->childset, 
            llength * sizeof(node*));
         if (!pdest->value.ptr->childset)
         {
            yyerror("Error: Lack of memory in setlength "
               "for new statements list.");
            exit(1);
         }
         for (i = oldlen; i < llength; i++)
         {
            node* npt = malloc(sizeof(node));
            if (!npt)
            {
               yyerror("Error: Lack of memory in setlength for new node.");
               exit(1);
            }
            memset(npt, 0, sizeof(node));
            npt->ntype = NT_NUM_CONST;
            npt->opval.value = 0.0;
            npt->parent = pdest->value.ptr;
            npt->nb_childs = 0;
            npt->childset = NULL;

            pdest->value.ptr->childset[i] = npt;
         }
      }
      else if (oldlen == llength)
      {
         return retval;
      }
      else
      {
         for (i = llength; i < oldlen; i++)
         {  
            free_tree(pdest->value.ptr->childset[i]);
         }
         if (llength == 0)
         {
            free(pdest->value.ptr->childset);
            pdest->value.ptr->childset = NULL;
            return retval;
         }
         pdest->value.ptr->childset = realloc(pdest->value.ptr->childset, 
            llength * sizeof(node*));
         if (!pdest->value.ptr->childset)
         {
            yyerror("Error: Lack of memory in setlength for shorter "
               "statements list.");
            exit(1);
         }
      }
   }
   else
   {
      yyerror("Error: Unexpected data type in first argument in setlength.");
   }

   return retval;
}



data eval_insert(node* to_eval)
{
   int err = 0;
   data retval, pos, * pdest, from_eval;
   size_t oldlen, i, j, addlen, lpos = 0;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in insert.");
      yyerror("       This function has three parameters.");
      abort_called = 1;
      return retval;
   }

   pos = eval(to_eval->childset[2]);
   if (pos.ti.dtype < DT_CHAR || pos.ti.dtype > DT_LONG_DOUBLE ||
      pos.ti.nderef > 0)
   {
      yyerror("Error: Third argument type in insert is not numeric.");
      abort_called = 1;
      free_data(pos);
      return retval;
   }
   mac_cast(lpos, size_t, pos)


   /* Resolving the symbol received as first argument. */
   pdest = resolve(to_eval->childset[0]);
   if (!pdest) return retval;
   if (pdest->ti.nderef > 0)
   {
      yyerror("Error: First argument of insert must not be a pointer.");
      return retval;
   }


   switch(pdest->ti.dtype)
   {
   case DT_STRING:
      oldlen = pdest->value.str.length;
      break;
   case DT_ARRAY:
      oldlen = pdest->value.pArray->length;
      break;
   case DT_LIST:
      oldlen = pdest->value.pList->length;
      break;
   case DT_POINTER:
      if (pdest->value.ptr->ntype != NT_LIST)
      {
         yyerror("Error: Node pointer in first argument in insert does not "
            "reference a statement list.");
         abort_called = 1;
         return retval;
      }
      oldlen = pdest->value.ptr->nb_childs;
      break;
   default:
      yyerror("Error: Unexpected data type in insert first argument.");
      abort_called = 1;
      return retval;
   }

   if (lpos > oldlen)
   {
      yyerror("Error: Index out of bound in insert.");
      abort_called = 1;
      return retval;
   }

   /* Second argument type verification. */
   from_eval = eval(to_eval->childset[1]);
   if (from_eval.ti.dtype != pdest->ti.dtype || from_eval.ti.nderef > 0)
   {
      yyerror("Error: First and second argument in insert are not of the "
         "same type.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }


   /* Insertion. */
   if (pdest->ti.dtype == DT_STRING)
   {
      addlen = strlen(from_eval.value.str.tab);
      if (addlen == 0)
      {
         free_data(from_eval);
         return retval;
      }
      pdest->value.str.length = oldlen + addlen;
      g_lst_remove(pdest->value.str.tab, PT_CHAR_TAB);
      pdest->value.str.tab = realloc(pdest->value.str.tab, oldlen + 
         addlen);
      if (!pdest->value.str.tab)
      {
         yyerror("Error: Lack of memory in insert for new string.");
         exit(1);
      }
      g_lst_add(pdest->value.str.tab, PT_CHAR_TAB);
      for (i = oldlen + addlen - 1; i >= lpos + addlen; i--)
      {
         pdest->value.str.tab[i] = pdest->value.str.tab[i - addlen];
      }
      j = 0;
      for (i = lpos; i < lpos + addlen; i++)
      {
         pdest->value.str.tab[i] = from_eval.value.str.tab[j];
         j++;
      }
      free_data(from_eval);
   }
   else if (pdest->ti.dtype == DT_ARRAY)
   {
      data* alias;
      addlen = from_eval.value.pArray->length;
      if (addlen == 0)
      {
         free_data(from_eval);
         return retval;
      }
      pdest->value.pArray->length = oldlen + addlen;
      pdest->value.pArray->dtable = realloc(pdest->value.pArray->dtable, 
         (oldlen + addlen) * sizeof(data));
      if (!pdest->value.pArray->dtable)
      {
         yyerror("Error: Lack of memory in insert for new array.");
         exit(1);
      }
      alias = pdest->value.pArray->dtable;
      for (i = oldlen + addlen - 1; i >= lpos + addlen; i--)
      {
         alias[i] = alias[i - addlen];
      }
      j = 0;
      for (i = lpos; i < lpos + addlen; i++)
      {
         copy_data(&alias[i], from_eval.value.pArray->dtable[j]);
         j++;
      }
      free_data(from_eval);
   }
   else if (pdest->ti.dtype == DT_LIST)
   {
      listlink** destination = &pdest->value.pList->start, * next;
      addlen = from_eval.value.pList->length;
      if (addlen == 0)
      {
         free_data(from_eval);
         return retval;
      }
      pdest->value.pList->length = oldlen + addlen;
      for (i = 0; i < lpos; i++)
      {
         destination = &(*destination)->next;
      }
      next = *destination;
      *destination = from_eval.value.pList->start;
      destination = &from_eval.value.pList->start;
      for (i = 0; i < addlen; i++)
      {
         destination = &(*destination)->next;
      }
      *destination = next;
      g_lst_remove(from_eval.value.pList, PT_LIST);
      free(from_eval.value.pList);
   }
   else /* pdest->ti.dtype == DT_POINTER */
   {
      node** nptab;
      addlen = from_eval.value.ptr->nb_childs;
      if (addlen == 0)
      {
         free_data(from_eval);
         return retval;
      }
      pdest->value.ptr->nb_childs = oldlen + addlen;
      pdest->value.ptr->childset = realloc(pdest->value.ptr->childset, (oldlen 
         + addlen) * sizeof(node*));
      if (!pdest->value.ptr->childset)
      {
         yyerror("Error: Lack of memory in insert for new list.");
         exit(1);
      }
      nptab = pdest->value.ptr->childset;
      for (i = oldlen + addlen - 1; i >= lpos + addlen; i--)
      {
         nptab[i] = nptab[i - addlen];
      }
      j = 0;
      for (i = lpos; i < lpos + addlen; i++)
      {
         nptab[i] = copy_tree(from_eval.value.ptr->childset[j], pdest->value.
            ptr);
         j++;
      }
   }

   return retval;
}



data eval_replace(node* to_eval)
{
   int err = 0;
   data retval, pos, * pdest, from_eval;
   size_t oldlen, i, j, addlen, end, lpos = 0;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in replace.");
      yyerror("       This function has three parameters.");
      abort_called = 1;
      return retval;
   }

   pos = eval(to_eval->childset[2]);
   if (pos.ti.dtype < DT_CHAR || pos.ti.dtype > DT_LONG_DOUBLE ||
      pos.ti.nderef > 0)
   {
      yyerror("Error: Third argument type in replace is not numeric.");
      abort_called = 1;
      free_data(pos);
      return retval;
   }
   mac_cast(lpos, size_t, pos)


   /* Resolving the symbol received as first argument. */
   pdest = resolve(to_eval->childset[0]);
   if (!pdest) return retval;
   if (pdest->ti.nderef > 0)
   {
      yyerror("Error: First argument of replace must not be a pointer.");
      return retval;
   }

   switch(pdest->ti.dtype)
   {
   case DT_STRING:
      oldlen = pdest->value.str.length;
      break;
   case DT_ARRAY:
      oldlen = pdest->value.pArray->length;
      break;
   case DT_LIST:
      oldlen = pdest->value.pList->length;
      break;
   case DT_POINTER:
      if (pdest->value.ptr->ntype != NT_LIST)
      {
         yyerror("Error: Node pointer in first argument in replace does not "
            "reference a statement list.");
         abort_called = 1;
         return retval;
      }
      oldlen = pdest->value.ptr->nb_childs;
      break;
   default:
      yyerror("Error: Unexpected data type in replace first argument.");
      abort_called = 1;
      return retval;
   }

   if (lpos >= oldlen)
   {
      yyerror("Error: Index out of bound in replace.");
      abort_called = 1;
      return retval;
   }

   /* Second argument type verification. */
   from_eval = eval(to_eval->childset[1]);
   if (from_eval.ti.dtype != pdest->ti.dtype || from_eval.ti.nderef > 0)
   {
      yyerror("Error: First and second argument in replace are not of the "
         "same type.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }


   /* Replacements. */
   if (pdest->ti.dtype == DT_STRING)
   {
      addlen = strlen(from_eval.value.str.tab);
      if (addlen == 0)
      {
         free_data(from_eval);
         return retval;
      }
      end = min(oldlen - 1, lpos + addlen);
      j = 0;
      for (i = lpos; i < end; i++)
      {
         pdest->value.str.tab[i] = from_eval.value.str.tab[j];
         j++;
      }
      free_data(from_eval);
   }
   else if (pdest->ti.dtype == DT_ARRAY)
   {
      addlen = from_eval.value.pArray->length;
      if (addlen == 0)
      {
         free_data(from_eval);
         return retval;
      }
      end = min(oldlen, lpos + addlen);
      j = 0;
      for (i = lpos; i < end; i++)
      {
         free_data(pdest->value.pArray->dtable[i]);
         copy_data(&pdest->value.pArray->dtable[i], from_eval.value.pArray->dtable[j]);
         j++;
      }
      free_data(from_eval);
   }
   else if (pdest->ti.dtype == DT_LIST)
   {
      listlink* plink = pdest->value.pList->start, * srclink;
      addlen = from_eval.value.pList->length;
      if (addlen == 0)
      {
         free_data(from_eval);
         return retval;
      }
      end = min(oldlen, lpos + addlen);
      /* Getting the start link. */
      for (i = 0; i < lpos; i++)
      {
         plink = plink->next;
      }
      srclink = from_eval.value.pList->start;
      /* Copying. */
      for (i = lpos; i < end; i++)
      {
         free_data(plink->content);
         copy_data(&plink->content, srclink->content);

         plink = plink->next;
         srclink = srclink->next;
      }
      free_data(from_eval);
   }
   else /* pdest->ti.dtype == DT_POINTER */
   {
      addlen = from_eval.value.ptr->nb_childs;
      if (addlen == 0)
      {
         free_data(from_eval);
         return retval;
      }
      end = min(oldlen, lpos + addlen);
      j = 0;
      for (i = lpos; i < end; i++)
      {
         free_tree(pdest->value.ptr->childset[i]);
         pdest->value.ptr->childset[i] = copy_tree(from_eval.value.ptr->
            childset[j], pdest->value.ptr);
         j++;
      }
   }

   return retval;
}



data eval_source(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   FILE* fopen_res = NULL;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in source.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of source is not a string.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }

   fopen_res = fopen(from_eval.value.str.tab, "rb");
   if (!fopen_res)
   {
      fprintf(stderr, "Error: Failure to open source file \"%s\".\n", 
         from_eval.value.str.tab);
   }
   else
   {
      input_union previnputadr = inputadr;

      inputadr.inputfile = fopen_res;
      while (!feof(fopen_res) && !abort_called)
      {
         yyparse();
         if (!yynerrs) exec();
      }
      fclose(fopen_res);
      inputadr = previnputadr;
   }

   free_data(from_eval);

   return retval;
}



data eval_strlen(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in strlen.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of strlen is not a string.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }

   retval.ti.dtype = DT_SIZE_T;
   retval.value.stnum = strlen(from_eval.value.str.tab);

   free_data(from_eval);

   return retval;
}



data eval_as_array(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   array* parr;
   size_t i;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in as_array.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of as_array must not be a pointer.");
      return retval;
   }
   if (from_eval.ti.dtype == DT_POINTER)
   {
      if (from_eval.value.ptr)
      {
         if (from_eval.value.ptr->ntype != NT_LIST)
         {
            yyerror("Error: Node pointer in as_array argument does not reference a"
               " statement list.");
            abort_called = 1;
            return retval;
         }
      }
      else
      {
         yyerror("Error: Null node pointer found in as_array argument.");
         abort_called = 1;
         return retval;
      }
      parr = malloc(sizeof(array));
      if (!parr)
      {
         yyerror("Error: Lack of memory in as_array for array struct.");
         exit(1);
      }
      memset(parr, 0, sizeof(array));
      g_lst_add(parr, PT_ARRAY);
      retval.ti.dtype = DT_ARRAY;
      parr->length = from_eval.value.ptr->nb_childs;
      if (parr->length == 0)
      {
         parr->dtable = NULL;
         retval.value.pArray = parr;
         return retval;
      }
      parr->dtable = malloc(parr->length * sizeof(data));
      if (!parr->dtable)
      {
         yyerror("Error: Lack of memory in as_array for array data.");
         exit(1);
      }
      memset(parr->dtable, 0, parr->length * sizeof(data));

      for (i = 0; i < parr->length; i++)
      {
         if (!from_eval.value.ptr->childset[i])
         {
            parr->dtable[i].ti.dtype = DT_UNDEF;
            continue;
         }

         if (from_eval.value.ptr->childset[i]->ntype == NT_STRING)
         {
            parr->dtable[i].value.str.length = from_eval.value.ptr->childset[i]->
               opval.str.length;
            parr->dtable[i].value.str.tab = malloc(parr->dtable[i].value.str.length);
            if (!parr->dtable[i].value.str.tab)
            {
               yyerror("Error: Lack of memory in as_array for string data.");
               exit(1);
            }
            memset(parr->dtable[i].value.str.tab, 0, parr->dtable[i].value.str.length);
            g_lst_add(parr->dtable[i].value.str.tab, PT_CHAR_TAB);
            memcpy(parr->dtable[i].value.str.tab, from_eval.value.ptr->childset[i]->
               opval.str.tab, parr->dtable[i].value.str.length);

            parr->dtable[i].ti.dtype = DT_STRING;
         }
         else if (from_eval.value.ptr->childset[i]->ntype == NT_NUM_CONST)
         {
            parr->dtable[i].ti.dtype = DT_DOUBLE;
            parr->dtable[i].value.num = from_eval.value.ptr->childset[i]->
               opval.value;
         }
         else
         {
            parr->dtable[i].ti.dtype = DT_UNDEF;
         }
      }
      retval.value.pArray = parr;

   } /* if (from_eval.ti.dtype == DT_POINTER) */

   else if (from_eval.ti.dtype == DT_LIST)
   {
      listlink* plink;

      parr = malloc(sizeof(array));
      if (!parr)
      {
         yyerror("Error: Lack of memory in as_array for array struct.");
         exit(1);
      }
      memset(parr, 0, sizeof(array));
      g_lst_add(parr, PT_ARRAY);
      retval.ti.dtype = DT_ARRAY;
      parr->length = from_eval.value.pList->length;
      if (parr->length == 0)
      {
         parr->dtable = NULL;
         retval.value.pArray = parr;
         return retval;
      }
      parr->dtable = malloc(parr->length * sizeof(data));
      if (!parr->dtable)
      {
         yyerror("Error: Lack of memory in as_array for array data.");
         exit(1);
      }
      memset(parr->dtable, 0, sizeof(data));

      plink = from_eval.value.pList->start;
      for (i = 0; i < parr->length; i++)
      {
         copy_data(&parr->dtable[i], plink->content);
         plink = plink->next;
      }

      free_data(from_eval);

      retval.value.pArray = parr;
   }
   else
   {
      yyerror("Error: List or statements list expected in as_array.");
      abort_called = 1;
      free_data(from_eval);
   }

   return retval;
}



data eval_as_list(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   list* plst;
   listlink** destination;
   size_t i;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in as_list.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of as_list must not be a pointer.");
      return retval;
   }
   if (from_eval.ti.dtype == DT_POINTER)
   {
      if (from_eval.value.ptr)
      {
         if (from_eval.value.ptr->ntype != NT_LIST)
         {
            yyerror("Error: Node pointer in as_list argument does not reference a"
               " statement list.");
            abort_called = 1;
            return retval;
         }
      }
      else
      {
         yyerror("Error: Null node pointer found in as_list argument.");
         abort_called = 1;
         return retval;
      }
      plst = malloc(sizeof(list));
      if (!plst)
      {
         yyerror("Error: Lack of memory in as_list for list struct.");
         exit(1);
      }
      memset(plst, 0, sizeof(list));
      g_lst_add(plst, PT_LIST);
      plst->length = from_eval.value.ptr->nb_childs;

      destination = &plst->start;
      for (i = 0; i < plst->length; i++)
      {
         *destination = malloc(sizeof(listlink));
         if (!*destination)
         {
            yyerror("Error: Lack of memory in as_list for list link.");
            exit(1);
         }
         memset(*destination, 0, sizeof(listlink));
         if (!from_eval.value.ptr->childset[i])
         {
            (*destination)->content.ti.dtype = DT_UNDEF;
            continue;
         }
         if (from_eval.value.ptr->childset[i]->ntype == NT_STRING)
         {
            string str;
            str.length = from_eval.value.ptr->childset[i]->opval.str.length;
            str.tab = malloc(str.length);
            if (!str.tab)
            {
               yyerror("Error: Lack of memory is as_list for string data.");
               exit(1);
            }
            memset(str.tab, 0, str.length);
            g_lst_add(str.tab, PT_CHAR_TAB);
            memcpy(str.tab, from_eval.value.ptr->childset[i]->opval.str.tab, 
               str.length);

            (*destination)->content.ti.dtype = DT_STRING;
            (*destination)->content.value.str = str;
         }
         else if (from_eval.value.ptr->childset[i]->ntype == NT_NUM_CONST)
         {
            (*destination)->content.ti.dtype = DT_DOUBLE;
            (*destination)->content.value.num = from_eval.value.ptr->
               childset[i]->opval.value;
         }
         else
         {
            (*destination)->content.ti.dtype = DT_UNDEF;
         }
         destination = &(*destination)->next;
      }
      *destination = NULL;

      retval.ti.dtype = DT_LIST;
      retval.value.pList = plst;

   } /* if (from_eval.ti.dtype == DT_POINTER) */

   else if (from_eval.ti.dtype == DT_ARRAY)
   {
      plst = malloc(sizeof(list));
      if (!plst)
      {
         yyerror("Error: Lack of memory in as_list for list struct.");
         exit(1);
      }
      memset(plst, 0, sizeof(list));
      g_lst_add(plst, PT_LIST);
      plst->length = from_eval.value.pArray->length;

      destination = &plst->start;
      for (i = 0; i < plst->length; i++)
      {
         *destination = malloc(sizeof(listlink));
         if (!*destination)
         {
            yyerror("Error: Lack of memory in as_list for list link.");
            exit(1);
         }
         memset(*destination, 0, sizeof(listlink));
         copy_data(&(*destination)->content, from_eval.value.pArray->dtable[i]);
         destination = &(*destination)->next;
      }
      *destination = NULL;

      free_data(from_eval);

      retval.ti.dtype = DT_LIST;
      retval.value.pList = plst;
   }
   else
   {
      yyerror("Error: Array or statements list expected in as_list.");
      abort_called = 1;
      free_data(from_eval);
   }

   return retval;
}



data eval_as_statements(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   node* npt;
   size_t i;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in as_statements.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of as_statements must not be a pointer.");
      return retval;
   }
   if (from_eval.ti.dtype == DT_ARRAY)
   {
      npt = malloc(sizeof(node));
      if (!npt)
      {
         yyerror("Error: Lack of memory in as_statements for statements list"
            " node.");
         exit(1);
      }
      memset(npt, 0, sizeof(node));
      g_lst_add(npt, PT_NODE);
      npt->ntype = NT_LIST;
      npt->opval.str.length = 0;
      npt->opval.str.tab = NULL;
      npt->parent = NULL;
      npt->nb_childs = from_eval.value.pArray->length;
      npt->childset = NULL;

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = npt;

      if (npt->nb_childs == 0)
      {
         free_data(from_eval);
         return retval;
      }

      npt->childset = malloc(npt->nb_childs * sizeof(node*));
      if (!npt->childset)
      {
         yyerror("Error: Lack of memory in as_statements for node array.");
         exit(1);
      }
      memset(npt->childset, 0, npt->nb_childs * sizeof(node*));

      for (i = 0; i < npt->nb_childs; i++)
      {
         if (from_eval.value.pArray->dtable[i].ti.dtype >= DT_CHAR && 
            from_eval.value.pArray->dtable[i].ti.dtype <= DT_LONG_DOUBLE)
         {
            npt->childset[i] = malloc(sizeof(node));
            if (!npt->childset[i])
            {
               yyerror("Error: Lack of memory in as_statements for data node");
               exit(1);
            }
            memset(npt->childset[i], 0, sizeof(node));
            npt->childset[i]->ntype = NT_NUM_CONST;
            mac_cast(npt->childset[i]->opval.value, double, from_eval.value.pArray->dtable[i])
            npt->childset[i]->parent = npt;
            npt->childset[i]->nb_childs = 0;
            npt->childset[i]->childset = NULL;
         }
         else if (from_eval.value.pArray->dtable[i].ti.dtype == DT_STRING)
         {
            size_t len = from_eval.value.pArray->dtable[i].value.str.length;
            char* str;

            npt->childset[i] = malloc(sizeof(node));
            if (!npt->childset[i])
            {
               yyerror("Error: Lack of memory in as_statements for data node");
               exit(1);
            }
            memset(npt->childset[i], 0, sizeof(node));
            npt->childset[i]->ntype = NT_STRING;
            npt->childset[i]->opval.str.length = len;
            npt->childset[i]->parent = npt;
            npt->childset[i]->nb_childs = 0;
            npt->childset[i]->childset = NULL;

            str = malloc(len);
            if (!str)
            {
               yyerror("Error: Lack of memory in as_statements for string"
                  " data.");
               exit(1);
            }
            memcpy(str, from_eval.value.pArray->dtable[i].value.str.tab, 
               len);

            npt->childset[i]->opval.str.tab = str;
         }
         else
         {
            npt->childset[i] = malloc(sizeof(node));
            if (!npt->childset[i])
            {
               yyerror("Error: Lack of memory in as_statements for zero node");
               exit(1);
            }
            memset(npt->childset[i], 0, sizeof(node));
            npt->childset[i]->ntype = NT_NUM_CONST;
            npt->childset[i]->opval.value = 0.0;
            npt->childset[i]->parent = npt;
            npt->childset[i]->nb_childs = 0;
            npt->childset[i]->childset = NULL;
         }
      }

   } /* if (from_eval.ti.dtype == DT_ARRAY) */

   else if (from_eval.ti.dtype == DT_LIST)
   {
      listlink* plink;

      npt = malloc(sizeof(node));
      if (!npt)
      {
         yyerror("Error: Lack of memory in as_statements for statements list"
            " node.");
         exit(1);
      }
      memset(npt, 0, sizeof(node));
      g_lst_add(npt, PT_NODE);
      npt->ntype = NT_LIST;
      npt->opval.str.length = 0;
      npt->opval.str.tab = NULL;
      npt->parent = NULL;
      npt->nb_childs = from_eval.value.pList->length;
      npt->childset = NULL;

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = npt;

      if (npt->nb_childs == 0)
      {
         free_data(from_eval);
         return retval;
      }

      npt->childset = malloc(npt->nb_childs * sizeof(node*));
      if (!npt->childset)
      {
         yyerror("Error: Lack of memory in as_statements for node array.");
         exit(1);
      }
      memset(npt->childset, 0, npt->nb_childs * sizeof(node*));

      plink = from_eval.value.pList->start;

      for (i = 0; i < npt->nb_childs; i++)
      {
         if (plink->content.ti.dtype >= DT_CHAR && plink->content.ti.dtype <= 
            DT_LONG_DOUBLE)
         {
            npt->childset[i] = malloc(sizeof(node));
            if (!npt->childset[i])
            {
               yyerror("Error: Lack of memory in as_statements for data node");
               exit(1);
            }
            memset(npt->childset[i], 0, sizeof(node));
            npt->childset[i]->ntype = NT_NUM_CONST;
            mac_cast(npt->childset[i]->opval.value, double, plink->content)
            npt->childset[i]->parent = npt;
            npt->childset[i]->nb_childs = 0;
            npt->childset[i]->childset = NULL;
         }
         else if (plink->content.ti.dtype == DT_STRING)
         {
            size_t len = plink->content.value.str.length;
            char* str;

            npt->childset[i] = malloc(sizeof(node));
            if (!npt->childset[i])
            {
               yyerror("Error: Lack of memory in as_statements for data node");
               exit(1);
            }
            memset(npt->childset[i], 0, sizeof(node));
            npt->childset[i]->ntype = NT_STRING;
            npt->childset[i]->opval.str.length = len;
            npt->childset[i]->parent = npt;
            npt->childset[i]->nb_childs = 0;
            npt->childset[i]->childset = NULL;

            str = malloc(len);
            if (!str)
            {
               yyerror("Error: Lack of memory in as_statements for string"
                  " data.");
               exit(1);
            }
            memcpy(str, plink->content.value.str.tab, len);

            npt->childset[i]->opval.str.tab = str;
         }

         plink = plink->next;
      }

   } /* else if (from_eval.ti.dtype == DT_LIST) */

   else
   {
      yyerror("Error: Array or list expected in as_list.");
      abort_called = 1;
   }

   free_data(from_eval);

   return retval;
}



data eval_abort(void)
{
   data retval;

   memset(&retval, 0, sizeof(data));

   abort_called = 1;
   
   return retval;
}



data eval_gettype(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   char* type;
   size_t i, len, len_type;

   memset(&retval, 0, sizeof(data));
   retval.ti.dtype = DT_STRING;


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in gettype.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);

   switch(from_eval.ti.dtype)
   {
   case DT_UNDEF:
      type = "undefined";
      break;
   case DT_CHAR:
      type = "char";
      break;
   case DT_S_CHAR:
      type = "signed char";
      break;
   case DT_U_CHAR:
      type = "unsigned char";
      break;
   case DT_BYTE:
      type = "byte";
      break;
   case DT_SHORT:
      type = "short";
      break;
   case DT_U_SHORT:
      type = "unsigned short";
      break;
   case DT_INT:
      type = "int";
      break;
   case DT_U_INT:
      type = "unsigned int";
      break;
   case DT_LONG:
      type = "long";
      break;
   case DT_U_LONG:
      type = "unsigned long";
      break;
   case DT_LONG_LONG:
      type = "long long";
      break;
   case DT_U_LONG_LONG:
      type = "unsigned long long";
      break;
   case DT_SIZE_T:
      type = "size_t";
      break;
   case DT_FLOAT:
      type = "float";
      break;
   case DT_DOUBLE:
      type = "double";
      break;
   case DT_LONG_DOUBLE:
      type = "long double";
      break;
   case DT_STRING:
      type = "string";
      break;
   case DT_POINTER:
      type = "node pointer";
      break;
   case DT_OBJECT:
      type = "object";
      break;
   case DT_ARRAY:
      type = "array";
      break;
   case DT_LIST:
      type = "list";
      break;
   case DT_PFILE:
      type = "FILE*";
      break;
   case DT_PFPOS_T:
      type = "fpos_t*";
      break;
   case DT_HLIB:
      type = "library handle";
      break;
   case DT_PFUNC:
      type = "function pointer";
      break;
   case DT_MEMORY:
      type = "raw memory";
      break;
   case DT_MEMADR:
      type = "memory address";
      break;
   default:
      yyerror("Error: Unexpected data type in gettype.");
      exit(1);
   }

   len_type = strlen(type);
   len = len_type + from_eval.ti.nderef + 1;

   retval.value.str.length = len;
   retval.value.str.tab = malloc(len);
   if (!retval.value.str.tab)
   {
      yyerror("Error: Lack of memory in gettype for string data.");
      exit(1);
   }
   memset(retval.value.str.tab, 0, len);
   g_lst_add(retval.value.str.tab, PT_CHAR_TAB);
   strcpy(retval.value.str.tab, type);
   for (i = len_type; i < retval.value.str.length - 1; i++)
   {
      retval.value.str.tab[i] = '*';
   }
   retval.value.str.tab[i] = '\0';

   free_data(from_eval);

   return retval;
}



data eval_undefine(node* to_eval)
{
   int err = 0;
   data retval, * resolved;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in undefine.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   resolved = resolve(to_eval->childset[0]);
   if (!resolved) return retval;

   free_data(*resolved);
   resolved->ti.dtype = DT_UNDEF;

   return retval;
}



data eval_printf(node* to_eval)
{
   int err = 0;
   data arg1, retval, * argtab;
   void** raw_args;
   size_t i, sznchunks;
   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__)) 
   int esp_init = 0;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   size_t* chunkfloat;
   void* outregs[3] = {0, 0, 0};
   #endif

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in printf.");
      yyerror("       This function has at least one parameter.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_STRING || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in printf must be a string.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))
   if (args_eval(to_eval, 1, &sznchunks, &raw_args, &argtab)) return retval;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   if (args_eval(to_eval, 1, &sznchunks, &raw_args, &argtab, &chunkfloat)) return retval;
   #endif
   raw_args[sznchunks] = arg1.value.str.tab;
   sznchunks++;


   /* Call to ANSI C printf. */

   #if defined(_WIN32) && !defined(_WIN64)

   __asm mov esp_init, esp
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      __asm push chunk
   }

   __asm call printf

   __asm mov esp, esp_init

   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))

   asmcall(sznchunks, raw_args, (void (*)(void))printf, chunkfloat, outregs);

   #elif defined(__linux__) && defined(__i386__)

   asm("mov %%esp, %0" : "=r"(esp_init));
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      asm("push %0" : : "r"(chunk));
   }

   asm("call printf");

   asm("mov %0, %%esp"  : : "r"(esp_init) : "%esp");

   #endif

   free_data(arg1);
   for (i = 0; i < to_eval->nb_childs - 1; i++)
   {
      free_data(argtab[i]);
   }
   free(argtab);
   free(raw_args);

   return retval;
}



data eval_scanf(node* to_eval)
{
   int err = 0;
   data retval, arg1, * argtab;
   void** raw_args;
   size_t i, sznchunks;
   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__)) 
   int esp_init = 0;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   size_t* chunkfloat;
   void* outregs[3] = {0, 0, 0};
   #endif

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in scanf.");
      yyerror("       This function has at least one parameter.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_STRING || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in scanf must be a string.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))
   if (args_eval(to_eval, 1, &sznchunks, &raw_args, &argtab)) return retval;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   if (args_eval(to_eval, 1, &sznchunks, &raw_args, &argtab, &chunkfloat)) return retval;
   #endif

   raw_args[sznchunks] = arg1.value.str.tab;
   sznchunks++;


   /* Call to ANSI C scanf. */

   #if defined(_WIN32) && !defined(_WIN64)

   __asm mov esp_init, esp

   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      __asm push chunk
   }

   __asm call scanf

   __asm mov esp, esp_init

   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))

   asmcall(sznchunks, raw_args, (void (*)(void))scanf, chunkfloat, outregs);

   #elif defined(__linux__) && defined(__i386__)

   /* Pause before reading. Without this, the scanf call is skipped.  */
   ungetc(getc(stdin), stdin);

   asm("mov %%esp, %0" : "=r"(esp_init));

   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      asm("push %0" : : "r"(chunk));
   }

   asm("call scanf");

   asm("mov %0, %%esp"  : : "r"(esp_init) : "%esp");

   #endif

   getc(stdin);

   /*if (arg1.value.str.tab[arg1.value.str.length - 1] != '\n')
   {
      getc(stdin);
   }*/

   for (i = 0; i < to_eval->nb_childs - 1; i++)
   {
      free_data(argtab[i]);
   }
   free(argtab);
   free(raw_args);

   return retval;
}



data eval_fopen(node* to_eval)
{
   int err = 0;
   data retval, arg1, arg2;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fopen.");
      yyerror("       This function has two parameters.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_STRING || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in fopen is not a string.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_STRING || arg2.ti.nderef > 0)
   {
      yyerror("Error: Argument two in fopen is not a string.");
      free_data(arg1);
      free_data(arg2);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_PFILE;

   retval.value.pfile = fopen(arg1.value.str.tab, arg2.value.str.tab);

   free_data(arg1);
   free_data(arg2);

   return retval;
}



data eval_fclose(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fclose.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_PFILE || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of fclose is not a file pointer.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_INT;
   retval.value.inum = fclose(from_eval.value.pfile);

   return retval;
}



data eval_fprintf(node* to_eval)
{
   int err = 0;
   data retval, arg1, arg2, * argtab;
   void** raw_args;
   size_t i, sznchunks;
   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__)) 
   int esp_init = 0;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   size_t* chunkfloat;
   void* outregs[3] = {0, 0, 0};
   #endif

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fprintf.");
      yyerror("       This function has at least two parameters.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_PFILE || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in fprintf must be a file pointer.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_STRING || arg2.ti.nderef > 0)
   {
      yyerror("Error: Argument two in fprintf must be a string.");
      free_data(arg2);
      abort_called = 1;
      return retval;
   }

   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))
   if (args_eval(to_eval, 2, &sznchunks, &raw_args, &argtab)) return retval;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   if (args_eval(to_eval, 2, &sznchunks, &raw_args, &argtab, &chunkfloat)) return retval;
   #endif
   raw_args[sznchunks] = arg2.value.str.tab;
   sznchunks++;
   raw_args[sznchunks] = arg1.value.pfile;
   sznchunks++;


   /* Call to ANSI C fprintf. */

   #if defined(_WIN32) && !defined(_WIN64)

   __asm mov esp_init, esp
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      __asm push chunk
   }

   __asm call fprintf

   __asm mov esp, esp_init

   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))

   asmcall(sznchunks, raw_args, (void (*)(void))fprintf, chunkfloat, outregs);

   #elif defined(__linux__) && defined(__i386__)

   asm("mov %%esp, %0" : "=r"(esp_init));
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      asm("push %0" : : "r"(chunk));
   }

   asm("call fprintf");

   asm("mov %0, %%esp"  : : "r"(esp_init) : "%esp");

   #endif

   for (i = 0; i < to_eval->nb_childs - 2; i++)
   {
      free_data(argtab[i]);
   }
   free(argtab);
   free(raw_args);

   return retval;
}



data eval_fscanf(node* to_eval)
{
   int err = 0;
   data retval, arg1, arg2, * argtab;
   void** raw_args;
   size_t i, sznchunks;
   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__)) 
   int esp_init = 0;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   size_t* chunkfloat;
   void* outregs[3] = {0, 0, 0};
   #endif

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fscanf.");
      yyerror("       This function has at least two parameters.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_PFILE || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in fscanf must be a file pointer.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_STRING || arg2.ti.nderef > 0)
   {
      yyerror("Error: Argument two in fscanf must be a string.");
      free_data(arg2);
      abort_called = 1;
      return retval;
   }


   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))
   if (args_eval(to_eval, 2, &sznchunks, &raw_args, &argtab)) return retval;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   if (args_eval(to_eval, 2, &sznchunks, &raw_args, &argtab, &chunkfloat)) return retval;
   #endif
   raw_args[sznchunks] = arg2.value.str.tab;
   sznchunks++;
   raw_args[sznchunks] = arg1.value.pfile;
   sznchunks++;


   /* Call to ANSI C fscanf. */

   #if defined(_WIN32) && !defined(_WIN64)

   __asm mov esp_init, esp
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      __asm push chunk
   }

   __asm call fscanf

   __asm mov esp, esp_init

   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))

   asmcall(sznchunks, raw_args, (void (*)(void))fscanf, chunkfloat, outregs);

   #elif defined(__linux__) && defined(__i386__)

   asm("mov %%esp, %0" : "=r"(esp_init));
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      asm("push %0" : : "r"(chunk));
   }

   asm("call fscanf");

   asm("mov %0, %%esp"  : : "r"(esp_init) : "%esp");

   #endif

   for (i = 0; i < to_eval->nb_childs - 2; i++)
   {
      free_data(argtab[i]);
   }
   free(argtab);
   free(raw_args);

   return retval;
}



data eval_feof(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in feof.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_PFILE || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of feof is not a file pointer.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_INT;
   retval.value.inum = feof(from_eval.value.pfile);

   return retval;
}



data eval_fread(node* to_eval)
{
   int err = 0;
   data retval, * buff, size, count, stream;
   long lsize = 0, lcount = 0;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 4) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fread.");
      yyerror("       This function has four parameters.");
      abort_called = 1;
      return retval;
   }

   buff = resolve(to_eval->childset[0]);
   if (!buff)
   {
      abort_called = 1;
      return retval;
   }
   if (buff->ti.dtype != DT_STRING)
   {
      yyerror("Error: Argument one of fread is not a string.");
      abort_called = 1;
      return retval;
   }

   size = eval(to_eval->childset[1]);
   if (size.ti.dtype < DT_CHAR || size.ti.dtype > DT_LONG_DOUBLE ||
      size.ti.nderef > 0)
   {
      yyerror("Error: Argument two of fread is not a number.");
      free_data(size);
      abort_called = 1;
      return retval;
   }
   mac_cast(lsize, long, size)

   count = eval(to_eval->childset[2]);
   if (count.ti.dtype < DT_CHAR || count.ti.dtype > DT_LONG_DOUBLE ||
      count.ti.nderef > 0)
   {
      yyerror("Error: Argument three of fread is not a number.");
      free_data(count);
      abort_called = 1;
      return retval;
   }
   mac_cast(lcount, long, count)

   stream = eval(to_eval->childset[3]);
   if (stream.ti.dtype != DT_PFILE || stream.ti.nderef > 0)
   {
      yyerror("Error: Argument four of fread is not a file pointer.");
      free_data(stream);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_DOUBLE;
   retval.value.num = (double)fread(buff->value.str.tab, lsize, lcount, 
      stream.value.pfile);

   return retval;
}



data eval_fwrite(node* to_eval)
{
   int err = 0;
   data retval, buff, size, count, stream;
   long lsize = 0, lcount = 0;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 4) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fwrite.");
      yyerror("       This function has four parameters.");
      abort_called = 1;
      return retval;
   }

   buff = eval(to_eval->childset[0]);
   if (buff.ti.dtype != DT_STRING)
   {
      yyerror("Error: Argument one of fwrite is not a string.");
      free_data(buff);
      abort_called = 1;
      return retval;
   }

   size = eval(to_eval->childset[1]);
   if (size.ti.dtype < DT_CHAR || size.ti.dtype > DT_LONG_DOUBLE ||
      size.ti.nderef > 0)
   {
      yyerror("Error: Argument two of fwrite is not a number.");
      free_data(size);
      free_data(buff);
      abort_called = 1;
      return retval;
   }
   mac_cast(lsize, long, size)

   count = eval(to_eval->childset[2]);
   if (count.ti.dtype < DT_CHAR || count.ti.dtype > DT_LONG_DOUBLE ||
      count.ti.nderef > 0)
   {
      yyerror("Error: Argument three of fwrite is not a number.");
      free_data(count);
      free_data(buff);
      abort_called = 1;
      return retval;
   }
   mac_cast(lcount, long, count)

   stream = eval(to_eval->childset[3]);
   if (stream.ti.dtype != DT_PFILE || stream.ti.nderef > 0)
   {
      yyerror("Error: Argument four of fwrite is not a file pointer.");
      free_data(stream);
      free_data(buff);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_DOUBLE;
   retval.value.num = (double)fwrite(buff.value.str.tab, lsize, lcount, 
      stream.value.pfile);

   free_data(buff);

   return retval;
}



data eval_clearerr(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in clearerr.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_PFILE || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of clearerr is not a file pointer.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   clearerr(from_eval.value.pfile);

   return retval;
}



data eval_fgetpos(node* to_eval)
{
   int err = 0;
   data retval, arg1, * arg2;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fgetpos.");
      yyerror("       This function has two parameters.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_PFILE || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in fgetpos must be a file pointer.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = resolve(to_eval->childset[1]);
   if (!arg2) return retval;

   if (arg2->ti.dtype != DT_PFPOS_T || arg2->ti.nderef > 0)
   {
      free_data(*arg2);

      arg2->ti.dtype = DT_PFPOS_T;
      arg2->value.pfpos = malloc(sizeof(fpos_t));
      if (!arg2->value.pfpos)
      {
         yyerror("Error: Lack of memory in fgetpos for fpos_t.");
         exit(1);
      }
      memset(arg2->value.pfpos, 0, sizeof(fpos_t));
      g_lst_add(arg2->value.pfpos, PT_PFPOS_T);
   }

   retval.ti.dtype = DT_DOUBLE;
   retval.value.num = fgetpos(arg1.value.pfile, arg2->value.pfpos);

   return retval;
}



data eval_fsetpos(node* to_eval)
{
   int err = 0;
   data retval, arg1, arg2;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fsetpos.");
      yyerror("       This function has two parameters.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_PFILE || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in fsetpos must be a file pointer.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_PFPOS_T || arg2.ti.nderef > 0)
   {
      yyerror("Error: Argument two in fsetpos must be a fpos_t pointer.");
      free_data(arg2);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_DOUBLE;
   retval.value.num = fsetpos(arg1.value.pfile, arg2.value.pfpos);

   return retval;
}



data eval_ferror(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in ferror.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_PFILE || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of ferror is not a file pointer.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_INT;
   retval.value.inum = ferror(from_eval.value.pfile);

   return retval;
}



data eval_fgetc(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fgetc.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_PFILE || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of fgetc is not a file pointer.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_STRING;
   retval.value.str.length = 2;
   retval.value.str.tab = malloc(2 * sizeof(char));
   if (!retval.value.str.tab)
   {
      yyerror("Error: Lack of memory in fgetc for string data.");
      exit(1);
   }
   memset(retval.value.str.tab, 0, 2 * sizeof(char));
   g_lst_add(retval.value.str.tab, PT_CHAR_TAB);
   retval.value.str.tab[0] = (char)fgetc(from_eval.value.pfile);
   retval.value.str.tab[1] = '\0';

   return retval;
}



data eval_ungetc(node* to_eval)
{
   int err = 0, unget_ret;
   data retval, arg1, arg2;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in ungetc.");
      yyerror("       This function has two parameters.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_STRING || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in ungetc must be a string.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   if (arg1.value.str.length != 2)
   {
      yyerror("Error: Argument one in ungetc must have length of two.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_PFILE || arg2.ti.nderef > 0)
   {
      yyerror("Error: Argument two in ungetc must be a file pointer.");
      free_data(arg1);
      free_data(arg2);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_STRING;
   unget_ret = ungetc(arg1.value.str.tab[0], arg2.value.pfile);
   retval.value = arg1.value; /* Memory recycling. */
   retval.value.str.tab[0] = (char)unget_ret;

   return retval;
}



data eval_fflush(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fflush.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_PFILE || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of fflush is not a file pointer.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_INT;
   retval.value.inum = fflush(from_eval.value.pfile);

   return retval;
}



data eval_fputc(node* to_eval)
{
   int err = 0;
   data retval, arg1, arg2;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fputc.");
      yyerror("       This function has two parameters.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_STRING || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in fputc must be a string.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   if (arg1.value.str.length != 2)
   {
      yyerror("Error: Argument one in fputc must have length of two.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_PFILE || arg2.ti.nderef > 0)
   {
      yyerror("Error: Argument two in fputc must be a file pointer.");
      free_data(arg1);
      free_data(arg2);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_STRING;
   retval.value.str.tab = malloc(2);
   if (!retval.value.str.tab)
   {
      yyerror("Error: Lack of memory in eval_fputc for new string.");
      exit(1);
   }
   memset(retval.value.str.tab, 0, 2);
   g_lst_add(retval.value.str.tab, PT_CHAR_TAB);
   retval.value.str.tab[0] = (char)fputc(arg1.value.str.tab[0], arg2.value.pfile);

   return retval;
}



data eval_fseek(node* to_eval)
{
   int err = 0;
   data retval, stream, offset, origin;
   long loffset = -1;
   int iorigin = -1;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in fseek.");
      yyerror("       This function has three parameters.");
      abort_called = 1;
      return retval;
   }

   stream = eval(to_eval->childset[0]);
   if (stream.ti.dtype != DT_PFILE || stream.ti.nderef > 0)
   {
      yyerror("Error: Argument one of fseek must be a file pointer.");
      abort_called = 1;
      free_data(stream);
      return retval;
   }

   offset = eval(to_eval->childset[1]);
   if (offset.ti.dtype < DT_CHAR || offset.ti.dtype > DT_LONG_DOUBLE ||
      offset.ti.nderef > 0)
   {
      yyerror("Error: Argument two of fseek must be a number.");
      abort_called = 1;
      free_data(offset);
      return retval;
   }
   mac_cast(loffset, long, offset)

   origin = eval(to_eval->childset[2]);
   if (origin.ti.dtype < DT_CHAR || origin.ti.dtype > DT_LONG_DOUBLE ||
      origin.ti.nderef > 0)
   {
      yyerror("Error: Argument three of fseek must be a number.");
      abort_called = 1;
      free_data(origin);
      return retval;
   }
   mac_cast(iorigin, int, origin)

   if (iorigin < 0 || iorigin > 2)
   {
      yyerror("Error: Argument three of fseek must be 0, 1 or 2.");
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_DOUBLE;
   retval.value.num = fseek(stream.value.pfile, loffset, iorigin);

   return retval;
}



data eval_ftell(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in ftell.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_PFILE || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of ftell is not a file pointer.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_LONG;
   retval.value.lnum = ftell(from_eval.value.pfile);

   return retval;
}



data eval_freopen(node* to_eval)
{
   int err = 0;
   data retval, path, mode, stream;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in freopen.");
      yyerror("       This function has three parameters.");
      abort_called = 1;
      return retval;
   }

   path = eval(to_eval->childset[0]);
   if (path.ti.dtype != DT_STRING || path.ti.nderef > 0)
   {
      yyerror("Error: Argument one of freopen is not a string.");
      free_data(path);
      abort_called = 1;
      return retval;
   }

   mode = eval(to_eval->childset[1]);
   if (mode.ti.dtype != DT_STRING || mode.ti.nderef > 0)
   {
      yyerror("Error: Argument two of freopen is not a string.");
      free_data(mode);
      free_data(path);
      abort_called = 1;
      return retval;
   }

   stream = eval(to_eval->childset[2]);
   if (stream.ti.dtype != DT_PFILE || stream.ti.nderef > 0)
   {
      yyerror("Error: Argument three of freopen is not a file pointer.");
      free_data(stream);
      free_data(path);
      free_data(mode);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_PFILE;
   retval.value.pfile = freopen(path.value.str.tab, mode.value.str.tab, 
      stream.value.pfile);

   free_data(path);
   free_data(mode);

   return retval;
}



data eval_rewind(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in rewind.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_PFILE || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of rewind is not a file pointer.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   rewind(from_eval.value.pfile);

   return retval;
}



data eval_tmpfile(void)
{
   data retval;

   memset(&retval, 0, sizeof(data));

   retval.ti.dtype = DT_PFILE;
   retval.value.pfile = tmpfile();

   return retval;
}



data eval_sprintf(node* to_eval)
{
   int err = 0;
   data retval, arg1, arg2, * argtab;
   void** raw_args;
   size_t i, sznchunks;
   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__)) 
   int esp_init = 0;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   size_t* chunkfloat;
   void* outregs[3] = {0, 0, 0};
   #endif

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in sprintf.");
      yyerror("       This function has at least two parameters.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_STRING || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in sprintf must be a string.");
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_STRING || arg2.ti.nderef > 0)
   {
      yyerror("Error: Argument two in sprintf must be a string.");
      free_data(arg2);
      abort_called = 1;
      return retval;
   }

   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))
   if (args_eval(to_eval, 2, &sznchunks, &raw_args, &argtab)) return retval;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   if (args_eval(to_eval, 2, &sznchunks, &raw_args, &argtab, &chunkfloat)) return retval;
   #endif
   raw_args[sznchunks] = arg2.value.str.tab;
   sznchunks++;
   raw_args[sznchunks] = arg1.value.str.tab;
   sznchunks++;


   /* Call to ANSI C sprintf. */

   #if defined(_WIN32) && !defined(_WIN64)

   __asm mov esp_init, esp
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      __asm push chunk
   }

   __asm call sprintf

   __asm mov esp, esp_init

   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))

   asmcall(sznchunks, raw_args, (void (*)(void))sprintf, chunkfloat, outregs);

   #elif defined(__linux__) && defined(__i386__)

   asm("mov %%esp, %0" : "=r"(esp_init));
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      asm("push %0" : : "r"(chunk));
   }

   asm("call sprintf");

   asm("mov %0, %%esp"  : : "r"(esp_init) : "%esp");

   #endif

   for (i = 0; i < to_eval->nb_childs - 2; i++)
   {
      free_data(argtab[i]);
   }
   free(argtab);
   free(raw_args);

   return retval;
}



data eval_sscanf(node* to_eval)
{
   int err = 0;
   data retval, arg1, arg2, * argtab;
   void** raw_args;
   size_t i, sznchunks;
   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__)) 
   int esp_init = 0;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   size_t* chunkfloat;
   void* outregs[3] = {0, 0, 0};
   #endif

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in sscanf.");
      yyerror("       This function has at least two parameters.");
      abort_called = 1;
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_STRING || arg1.ti.nderef > 0)
   {
      yyerror("Error: Argument one in sscanf must be a string.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_STRING || arg2.ti.nderef > 0)
   {
      yyerror("Error: Argument two in sscanf must be a string.");
      free_data(arg2);
      abort_called = 1;
      return retval;
   }

   #if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))
   if (args_eval(to_eval, 2, &sznchunks, &raw_args, &argtab)) return retval;
   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   if (args_eval(to_eval, 2, &sznchunks, &raw_args, &argtab, &chunkfloat)) return retval;
   #endif
   raw_args[sznchunks] = arg2.value.str.tab;
   sznchunks++;
   raw_args[sznchunks] = arg1.value.str.tab;
   sznchunks++;


   /* Call to ANSI C sscanf. */

   #if defined(_WIN32) && !defined(_WIN64)

   __asm mov esp_init, esp
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      __asm push chunk
   }

   __asm call sscanf

   __asm mov esp, esp_init

   #elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))

   asmcall(sznchunks, raw_args, (void (*)(void))sscanf, chunkfloat, outregs);

   #elif defined(__linux__) && defined(__i386__)

   asm("mov %%esp, %0" : "=r"(esp_init));
   for (i = 0; i < sznchunks; i++)
   {
      void* chunk = raw_args[i];
      asm("push %0" : : "r"(chunk));
   }

   asm("call sscanf");

   asm("mov %0, %%esp"  : : "r"(esp_init) : "%esp");

   #endif

   for (i = 0; i < to_eval->nb_childs - 2; i++)
   {
      free_data(argtab[i]);
   }
   free(argtab);
   free(raw_args);

   return retval;
}



data eval_remove(node* to_eval)
{
   data retval, * symbol, dataStart, dataEnd;
   int err = 0;
   size_t oldlen, lstart = 0, lend = 0;

   memset(&retval, 0, sizeof(data));


  /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 2 || to_eval->nb_childs > 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in remove.");
      yyerror("       This function has two or three parameters.");
      abort_called = 1;
      return retval;
   }

   symbol = resolve(to_eval->childset[0]);
   if (!symbol) return retval;

   switch(symbol->ti.dtype)
   {
   case DT_STRING:
      oldlen = symbol->value.str.length;
      break;
   case DT_ARRAY:
      oldlen = symbol->value.pArray->length;
      break;
   case DT_LIST:
      oldlen = symbol->value.pList->length;
      break;
   case DT_POINTER:
      if (symbol->value.ptr->ntype != NT_LIST)
      {
         yyerror("Error: Node pointer in first argument in remove does not "
            "reference a statement list.");
         abort_called = 1;
         return retval;
      }
      oldlen = symbol->value.ptr->nb_childs;
      break;
   default:
      yyerror("Error: Unexpected data type in remove first argument.");
      abort_called = 1;
      return retval;
   }

   dataStart = eval(to_eval->childset[1]);
   if (dataStart.ti.dtype < DT_CHAR || dataStart.ti.dtype > DT_LONG_DOUBLE ||
      dataStart.ti.nderef > 0)
   {
      yyerror("Error: Number expected as remove second argument.");
      free_data(dataStart);
      abort_called = 1;
      return retval;
   }
   mac_cast(lstart, size_t, dataStart)

   if (to_eval->nb_childs == 3)
   {
      dataEnd = eval(to_eval->childset[2]);
      if (dataEnd.ti.dtype < DT_CHAR || dataEnd.ti.dtype > DT_LONG_DOUBLE ||
         dataEnd.ti.nderef > 0)
      {
         yyerror("Error: Number expected as remove third argument.");
         free_data(dataEnd);
         abort_called = 1;
         return retval;
      }
      mac_cast(lend, size_t, dataEnd)
   }
   else
   {
      lend = lstart;
   }

   if (lstart >= oldlen|| lend >= oldlen || lstart > lend)
   {
      yyerror("Error: Index out of bound in remove.");
      abort_called = 1;
      return retval;
   }


   /* Deletion */

   if (symbol->ti.dtype == DT_STRING)
   {
      size_t i;
      size_t gap = lend - lstart + 1;
      size_t newlen = oldlen - gap;
      size_t end_copy = newlen + 1;

      if (gap == oldlen)
      {
         g_lst_remove(symbol->value.str.tab, PT_CHAR_TAB);
         free(symbol->value.str.tab);
         symbol->value.str.tab = NULL;
         symbol->value.str.length = 0;
         return retval;
      }

      for (i = lstart; i < end_copy; i++)
      {
         symbol->value.str.tab[i] = symbol->value.str.tab[i+gap];
      }

      g_lst_remove(symbol->value.str.tab, PT_CHAR_TAB);
      symbol->value.str.tab = realloc(symbol->value.str.tab, newlen);
      if (!symbol->value.str.tab)
      {
         yyerror("Error: Lack of memory in remove for new string.");
         exit(1);
      }
      g_lst_add(symbol->value.str.tab, PT_CHAR_TAB);
      symbol->value.str.length = newlen;
   }
   else if (symbol->ti.dtype == DT_ARRAY)
   {
      size_t i;
      size_t gap = lend - lstart + 1;
      size_t newlen = oldlen - gap;
      size_t end_copy = newlen + 1;

      if (gap == oldlen)
      {
         for (i = 0; i < oldlen; i++)
         {
            free_data(symbol->value.pArray->dtable[i]);
         }
         free(symbol->value.pArray->dtable);
         symbol->value.pArray->dtable = NULL;
         symbol->value.pArray->length = 0;
         return retval;
      }

      for (i = lstart; i < end_copy; i++)
      {
         free_data(symbol->value.pArray->dtable[i]);
         symbol->value.pArray->dtable[i] = symbol->value.pArray->dtable[i+gap];
      }

      for(; i <= lend; i++)
      {
         free_data(symbol->value.pArray->dtable[i]);
      }

      symbol->value.pArray->dtable = realloc(symbol->value.pArray->dtable, 
         newlen * sizeof(data));
      if (!symbol->value.pArray->dtable)
      {
         yyerror("Error: Lack of memory in remove for new array.");
         exit(1);
      }
      symbol->value.pArray->length = newlen;
   }
   else if (symbol->ti.dtype == DT_LIST)
   {
      size_t i;
      size_t gap = lend - lstart + 1;
      listlink** ppLink = &symbol->value.pList->start, * pNext;

      for (i = 0; i < lstart; i++)
      {
         ppLink = &(*ppLink)->next;
      }

      pNext = *ppLink;

      for (i = 0; i < gap; i++)
      {
         listlink* pNextNext = pNext->next;
         free_data(pNext->content);
         free(pNext);
         pNext = pNextNext;
      }

      *ppLink = pNext;
      symbol->value.pList->length = oldlen - gap;
   }
   else /* symbol->ti.dtype == DT_POINTER */
   {
      size_t i;
      size_t gap = lend - lstart + 1;
      size_t newlen = oldlen - gap;
      size_t end_copy = newlen + 1;

      if (gap == oldlen)
      {
         free(symbol->value.ptr->childset);
         symbol->value.ptr->childset = NULL;
         symbol->value.ptr->nb_childs = 0;
         return retval;
      }

      for (i = lstart; i < end_copy; i++)
      {
         symbol->value.ptr->childset[i] = symbol->value.ptr->childset[i+gap];
      }

      symbol->value.ptr->childset = realloc(symbol->value.ptr->childset, 
         newlen * sizeof(node*));
      if (!symbol->value.ptr->childset)
      {
         yyerror("Error: Lack of memory in remove for new child set.");
         exit(1);
      }
      symbol->value.ptr->nb_childs = newlen;
   }

   return retval;
}



data eval_cls(void)
{
   data retval;

   memset(&retval, 0, sizeof(data));

   #ifdef _WIN32
      system("cls");
   #else
      system("clear");
   #endif

   return retval;
}



data eval_atovar(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   node* npt;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in atovar.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of atovar is not a string.");
      free_data(from_eval);
      return retval;
   }

   /* Name validation */
   if (!isalpha(from_eval.value.str.tab[0]) && 
      from_eval.value.str.tab[0] != '_')
   {
      yyerror("Error: The string is not a valid identifier.");
      free_data(from_eval);
      return retval;
   }

   /* Node creation. */
   npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in eval_atovar for new node.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));
   g_lst_add(npt, PT_NODE);

   npt->ntype = NT_VARIABLE;
   npt->opval.name = malloc(from_eval.value.str.length);
   if (!npt->opval.name)
   {
      yyerror("Error: Lack of memory in eval_atovar for name string.");
      exit(1);
   }
   strcpy(npt->opval.name, from_eval.value.str.tab);

   npt->parent = NULL;
   npt->nb_childs = 0;
   npt->childset = NULL;

   retval.value.ptr = npt;
   retval.ti.dtype = DT_POINTER;

   free_data(from_eval);

   return retval;
}



data eval_vartoa(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   size_t namelen;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in vartoa.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_POINTER || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of vartoa have not the node pointer type.");
      free_data(from_eval);
      return retval;
   }

   if (!from_eval.value.ptr)
   {
      yyerror("Error: The argument of vartoa is NULL.");
      free_data(from_eval);
      return retval;
   }

   if (from_eval.value.ptr->ntype != NT_VARIABLE)
   {
      yyerror("Error: The argument of vartoa does not "
         "resolve to a variable node.");
      free_data(from_eval);
      return retval;
   }


   namelen = strlen(from_eval.value.ptr->opval.name) + 1;
   retval.value.str.tab = malloc(namelen);
   if (!retval.value.str.tab)
   {
      yyerror("Error: Lack of memory in eval_vartoa for string return value.");
      exit(1);
   }
   memset(retval.value.str.tab, 0, namelen);
   g_lst_add(retval.value.str.tab, PT_CHAR_TAB);

   strcpy(retval.value.str.tab, from_eval.value.ptr->opval.name);
   retval.value.str.length = namelen;
   retval.ti.dtype = DT_STRING;

   free_data(from_eval);

   return retval;
}



data eval_freetree(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in freetree.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_POINTER || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of freetree has not the node pointer type.");
      free_data(from_eval);
      return retval;
   }

   if (!from_eval.value.ptr)
   {
      yyerror("Error: The argument of freetree is NULL.");
      free_data(from_eval);
      return retval;
   }

   free_tree(from_eval.value.ptr);

   return retval;
}



data eval_strclone(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in strclone.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: The argument of strclone has not the string type.");
      free_data(from_eval);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_STRING;
   retval.value.str.length = from_eval.value.str.length;
   retval.value.str.tab = malloc(from_eval.value.str.length);
   if (!retval.value.str.tab)
   {
      yyerror("Error: Lack of memory in eval_strclone for new string.");
      exit(1);
   }
   memcpy(retval.value.str.tab, from_eval.value.str.tab, from_eval.value.str.length);
   g_lst_add(retval.value.str.tab, PT_CHAR_TAB);

   return retval;
}



data eval_strcpy(node* to_eval)
{
   int err = 0;
   data retval, arg1, arg2;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in strcpy.");
      yyerror("       This function has two parameters.");
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype != DT_STRING || arg1.ti.nderef > 0)
   {
      yyerror("Error: The first argument of strcpy has not the string type.");
      free_data(arg1);
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype != DT_STRING || arg2.ti.nderef > 0)
   {
      yyerror("Error: The second argument of strcpy has not the string type.");
      free_data(arg2);
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_STRING;
   retval.value.str.length = arg1.value.str.length;
   retval.value.str.tab = arg1.value.str.tab;

   strcpy(arg1.value.str.tab, arg2.value.str.tab);

   return retval;
}



data eval_alloc_copy(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   data* pdat;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in alloc_copy.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);

   pdat = malloc(sizeof(data));
   if (!pdat)
   {
      yyerror("Error: Lack of memory in alloc_copy for new data.");
      exit(1);
   }
   memset(pdat, 0, sizeof(data));
   copy_data(pdat, from_eval);

   retval.ti.dtype = from_eval.ti.dtype;
   retval.ti.nderef = 1;
   retval.value.genptr = (void*)pdat;

   return retval;
}



data eval_free(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in free.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.nderef != 1)
   {
      yyerror("Error: Wrong reference level of argument of free.");
      return retval;
   }

   switch (from_eval.ti.dtype)
   {
   case DT_OBJECT:
      g_lst_remove(from_eval.value.genptr, PT_OBJECT);
      break;
   case DT_ARRAY:
      g_lst_remove(from_eval.value.genptr, PT_ARRAY);
      break;
   case DT_LIST:
      g_lst_remove(from_eval.value.genptr, PT_LIST);
      break;
   case DT_PFPOS_T:
      g_lst_remove(from_eval.value.genptr, PT_PFPOS_T);
      break;
   default:
      break;
   }

   free_data(*(data*)from_eval.value.genptr);
   free(from_eval.value.genptr);

   return retval;
}



data eval_repeat(node* to_eval)
{
   int err = 0, n = -1, i;
   data retval, arg1, arg2;

   memset(&retval, 0, sizeof(data));

   
   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in repeat.");
      yyerror("       This function has two parameters.");
      return retval;
   }

   arg1 = eval(to_eval->childset[0]);
   if (arg1.ti.dtype <= DT_UNDEF || arg1.ti.dtype >= NB_TYPES)
   {
      yyerror("Error: Invalid type for argument one of repeat.");
      abort_called = 1;
      return retval;
   }

   arg2 = eval(to_eval->childset[1]);
   if (arg2.ti.dtype < DT_CHAR || arg2.ti.dtype > DT_LONG_DOUBLE || arg2.ti.nderef > 0)
   {
      yyerror("Error: Number expected as second argument of repeat.");
      abort_called = 1;
      free_data(arg1);
      return retval;
   }

   mac_cast(n, int, arg2)
   if (n <= 0)
   {
      yyerror("Error: Positive number expected as second argument of repeat.");
      abort_called = 1;
      free_data(arg1);
      return retval;
   }


   retval.value.pArray = malloc(sizeof(array));
   if (!retval.value.pArray)
   {
      yyerror("Error: Lack of memory for array structure in repeat.");
      exit(1);
   }
   memset(retval.value.pArray, 0, sizeof(array));

   g_lst_add(retval.value.pArray, PT_ARRAY);
   retval.ti.dtype = DT_ARRAY;
   retval.value.pArray->length = n;

   retval.value.pArray->dtable = malloc(n * sizeof(data));
   if (!retval.value.pArray->dtable)
   {
      yyerror("Error: Lack of memory for array data in repeat.");
      exit(1);
   }
   memset(retval.value.pArray->dtable, 0, n * sizeof(data));

   for (i = 0; i < n; i++)
   {
      copy_data(&retval.value.pArray->dtable[i], arg1);
   }

   return retval;
}



int sametype(data d1, data d2)
{
   size_t i;

   if (d1.ti.dtype != d2.ti.dtype) return 0;
   if (d1.ti.nderef != d2.ti.nderef) return 0;

   switch (d1.ti.dtype)
   {
      case DT_OBJECT:
         if (d1.value.pObject->nb_clos != d2.value.pObject->nb_clos) return 0;
         for (i = 0; i < d1.value.pObject->nb_clos; i++)
         {
            if (!sametype(d1.value.pObject->clos_array[i]->content, 
               d2.value.pObject->clos_array[i]->content)) return 0;
         }
         break;
      case DT_ARRAY:
         if (d1.value.pArray->length != d2.value.pArray->length) return 0;
         for (i = 0; i < d1.value.pArray->length; i++)
         {
            if (!sametype(d1.value.pArray->dtable[i], 
               d2.value.pArray->dtable[i])) return 0;
         }
         break;
      default:
         break;
   }

   return 1;
}



size_t buffersize(data var, size_t* alignpt)
{
   size_t i, max_align = 1, size, remain, local_offset;

   if (abort_called) return 0;

   if (var.ti.nderef > 0)
   {
      #ifdef _WIN32
      *alignpt = __alignof(void*);
      #else
      *alignpt = __alignof__(void*);
      #endif
      return sizeof(void*);
   }
   else
   {
      switch (var.ti.dtype)
      {
      case DT_UNDEF:
         yyerror("Error: Undefined variable in vartomem or memtovar argument.");
         abort_called = 1;
         return 0;
      case DT_CHAR:
      case DT_S_CHAR:
      case DT_U_CHAR:
      case DT_BYTE:
         #ifdef _WIN32
         *alignpt = __alignof(char);
         #else
         *alignpt = __alignof__(char);
         #endif
         return sizeof(char);
      case DT_SHORT:
      case DT_U_SHORT:
         #ifdef _WIN32
         *alignpt = __alignof(short);
         #else
         *alignpt = __alignof__(short);
         #endif
         return sizeof(short);
      case DT_INT:
      case DT_U_INT:
         #ifdef _WIN32
         *alignpt = __alignof(int);
         #else
         *alignpt = __alignof__(int);
         #endif
         return sizeof(int);
      case DT_LONG:
      case DT_U_LONG:
         #ifdef _WIN32
         *alignpt = __alignof(long);
         #else
         *alignpt = __alignof__(long);
         #endif
         return sizeof(long);
      case DT_LONG_LONG:
      case DT_U_LONG_LONG:
         #ifdef _WIN32
         *alignpt = __alignof(long long);
         #else
         *alignpt = __alignof__(long long);
         #endif
         return sizeof(long long);
      case DT_SIZE_T:
         #ifdef _WIN32
         *alignpt = __alignof(size_t);
         #else
         *alignpt = __alignof__(size_t);
         #endif
         return sizeof(size_t);
      case DT_FLOAT:
         #ifdef _WIN32
         *alignpt = __alignof(float);
         #else
         *alignpt = __alignof__(float);
         #endif
         return sizeof(float);
      case DT_DOUBLE:
         #ifdef _WIN32
         *alignpt = __alignof(double);
         #else
         *alignpt = __alignof__(double);
         #endif
         return sizeof(double);
      case DT_STRING:
      case DT_PFUNC:
      case DT_HLIB:
      case DT_MEMORY:
      case DT_MEMADR:
      case DT_PFILE:
      case DT_PFPOS_T:
         #ifdef _WIN32
         *alignpt = __alignof(void*);
         #else
         *alignpt = __alignof__(void*);
         #endif
         return sizeof(void*);
      case DT_LONG_DOUBLE:
         #ifdef _WIN32
         *alignpt = __alignof(long double);
         #else
         *alignpt = __alignof__(long double);
         #endif
         return sizeof(long double);
      case DT_POINTER:
         yyerror("Error: Node pointer variable type not supported in vartomem or memtovar.");
         abort_called = 1;
         return 0;
      case DT_OBJECT:
         local_offset = 0;
         for (i = 0; i < var.value.pObject->nb_clos; i++)
         {
            size_t align;
            size = buffersize(var.value.pObject->clos_array[i]->content, &align);
            remain = local_offset % align;
            if (remain)
            {
               local_offset += align - remain + size;
            }
            else
            {
               local_offset += size;
            }
            if (max_align < align) max_align = align;
         }
         remain = local_offset % max_align;
         if (remain)
         {
            local_offset += max_align - remain;
         }
         *alignpt = max_align;
         return local_offset;
      case DT_ARRAY:
         for (i = 1; i < var.value.pArray->length; i++)
         {
            if (!sametype(var.value.pArray->dtable[0], var.value.pArray->dtable[i]))
            {
               yyerror("Error: Array elements in vartomem argument must all be of the same type.");
               abort_called = 1;
               return 0;
            }
         }
         return var.value.pArray->length * buffersize(var.value.pArray->dtable[0], alignpt);
      case DT_LIST:
         yyerror("Error: List variable type not supported in vartomem or memtovar.");
         abort_called = 1;
         return 0;
      default:
         yyerror("Error: Unexpected variable type in buffersize.");
         exit(1);
      }
   }
}



void fillbuffer(data var, char* buffer, size_t offset)
{
   size_t i, local_offset, size, align;

   if (var.ti.nderef > 0)
   {
      memcpy(&buffer[offset], &var, sizeof(void*));
      return;
   }
   else
   {
      float fnum;

      switch (var.ti.dtype)
      {
      case DT_CHAR:
      case DT_S_CHAR:
      case DT_U_CHAR:
      case DT_BYTE:
         buffer[offset] = var.value.cnum;
         return;
      case DT_SHORT:
      case DT_U_SHORT:
         memcpy(&buffer[offset], &var, 2);
         return;
      case DT_INT:
      case DT_U_INT:
         memcpy(&buffer[offset], &var, sizeof(int));
         return;
      case DT_LONG:
      case DT_U_LONG:
         memcpy(&buffer[offset], &var, sizeof(long));
         return;
      case DT_FLOAT:
         fnum = (float)var.value.num;
         memcpy(&buffer[offset], &fnum, 4);
         return;
      case DT_LONG_LONG:
      case DT_U_LONG_LONG:
      case DT_DOUBLE:
         memcpy(&buffer[offset], &var, 8);
         return;
      case DT_SIZE_T:
         memcpy(&buffer[offset], &var, sizeof(size_t));
         return;
      case DT_STRING:
      case DT_PFUNC:
      case DT_HLIB:
      case DT_MEMORY:
      case DT_MEMADR:
      case DT_PFILE:
      case DT_PFPOS_T:
         memcpy(&buffer[offset], &var, sizeof(void*));
         return;
      case DT_LONG_DOUBLE:
         memcpy(&buffer[offset], &var, sizeof(long double));
         return;
      case DT_OBJECT:
         local_offset = 0;
         for (i = 0; i < var.value.pObject->nb_clos; i++)
         {
            size_t remain;
            size = buffersize(var.value.pObject->clos_array[i]->content, &align);
            remain = local_offset % align;
            if (remain)
            {
               local_offset += align - remain;
            }
            fillbuffer(var.value.pObject->clos_array[i]->content, buffer, offset + local_offset);
            local_offset += size;
         }
         return;
      case DT_ARRAY:
         size = buffersize(var.value.pArray->dtable[0], &align);
         for (i = 0; i < var.value.pArray->length; i++)
         {
            fillbuffer(var.value.pArray->dtable[i], buffer, offset);
            offset += size;
         }
         return;
      default:
         yyerror("Error: Unexpected variable type in fillbuffer.");
         exit(1);
      }
   }
}



data eval_vartomem(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   size_t align, size;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in vartomem.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);

   size = buffersize(from_eval, &align);
   if (abort_called)
   {
      free_data(from_eval);
      return retval;
   }

   retval.ti.dtype = DT_MEMORY;
   retval.value.mem.length = size;
   retval.value.mem.tab = malloc(size);
   if (!retval.value.mem.tab)
   {
      yyerror("Error: Lack of memory in vartomem for buffer.");
      exit(1);
   }
   memset(retval.value.mem.tab, 0, size);
   g_lst_add(retval.value.mem.tab, PT_CHAR_TAB);

   fillbuffer(from_eval, retval.value.mem.tab, 0);

   free_data(from_eval);
   return retval;
}



void readbuffer(data* var, char* buffer, size_t offset)
{
   size_t i, local_offset, size, align;

   if (var->ti.nderef > 0)
   {
      memcpy(var, &buffer[offset], sizeof(void*));
      return;
   }
   else
   {
      float fnum;

      switch (var->ti.dtype)
      {
      case DT_CHAR:
      case DT_S_CHAR:
      case DT_U_CHAR:
      case DT_BYTE:
         var->value.cnum = buffer[offset];
         return;
      case DT_SHORT:
      case DT_U_SHORT:
         memcpy(var, &buffer[offset], 2);
         return;
      case DT_INT:
      case DT_U_INT:
         memcpy(var, &buffer[offset], sizeof(int));
         return;
      case DT_LONG:
      case DT_U_LONG:
         memcpy(var, &buffer[offset], sizeof(long));
         return;
      case DT_FLOAT:
         memcpy(&fnum, &buffer[offset], 4);
         var->value.num = fnum;
         return;
      case DT_LONG_LONG:
      case DT_U_LONG_LONG:
      case DT_DOUBLE:
         memcpy(var, &buffer[offset], 8);
         return;
      case DT_SIZE_T:
         memcpy(var, &buffer[offset], sizeof(size_t));
         return;
      case DT_STRING:
      case DT_PFUNC:
      case DT_HLIB:
      case DT_MEMORY:
      case DT_MEMADR:
      case DT_PFILE:
      case DT_PFPOS_T:
         memcpy(var, &buffer[offset], sizeof(void*));
         return;
      case DT_LONG_DOUBLE:
         memcpy(var, &buffer[offset], sizeof(long double));
         return;
      case DT_OBJECT:
         local_offset = 0;
         for (i = 0; i < var->value.pObject->nb_clos; i++)
         {
            size_t remain;
            size = buffersize(var->value.pObject->clos_array[i]->content, &align);
            remain = local_offset % align;
            if (remain)
            {
               local_offset += align - remain;
            }
            readbuffer(&var->value.pObject->clos_array[i]->content, buffer, offset + local_offset);
            local_offset += size;
         }
         return;
      case DT_ARRAY:
         size = buffersize(var->value.pArray->dtable[0], &align);
         for (i = 0; i < var->value.pArray->length; i++)
         {
            readbuffer(&var->value.pArray->dtable[i], buffer, offset);
            offset += size;
         }
         return;
      default:
         yyerror("Error: Unexpected variable type in readbuffer.");
         exit(1);
      }
   }
}



data eval_memtovar(node* to_eval)
{
   int err = 0;
   data retval, dmem, * var;
   size_t size, align;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in memtovar.");
      yyerror("       This function has two parameters.");
      return retval;
   }

   var = resolve(to_eval->childset[0]);
   if (!var) return retval;

   dmem = eval(to_eval->childset[1]);
   if (dmem.ti.dtype != DT_MEMORY || dmem.ti.nderef > 0)
   {
      yyerror("Error: Raw memory expected as second argument of memtovar.");
      abort_called = 1;
      free_data(dmem);
      return retval;
   }

   size = buffersize(*var, &align);
   if (size > dmem.value.mem.length)
   {
      yyerror("Error: Variable size is bigger than the length of raw memory in memtovar.");
      abort_called = 1;
      free_data(dmem);
      return retval;
   }
   
   readbuffer(var, dmem.value.mem.tab, 0);

   return retval;
}



data eval_memcpy(node* to_eval)
{
   int err = 0, intlen = 0;
   data retval, from_eval1, from_eval2, len;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in memcpy.");
      yyerror("       This function has three parameters.");
      return retval;
   }

   from_eval1 = eval(to_eval->childset[0]);
   if (from_eval1.ti.dtype != DT_MEMADR && from_eval1.ti.dtype != DT_STRING && 
      from_eval1.ti.dtype != DT_MEMORY && from_eval1.ti.nderef < 1)
   {
      yyerror("Error: First argument of memcpy is not an address.");
      abort_called = 1;
      free_data(from_eval1);
      return retval;
   }

   from_eval2 = eval(to_eval->childset[1]);
   if (from_eval2.ti.dtype != DT_MEMADR && from_eval2.ti.dtype != DT_STRING && 
      from_eval2.ti.dtype != DT_MEMORY && from_eval2.ti.nderef < 1)
   {
      yyerror("Error: Second argument of memcpy is not an address.");
      abort_called = 1;
      free_data(from_eval2);
      return retval;
   }

   len = eval(to_eval->childset[2]);
   if (len.ti.dtype < DT_CHAR || len.ti.dtype > DT_LONG_DOUBLE ||
      len.ti.nderef > 0)
   {
      yyerror("Error: Number expected as third argument of memcpy.");
      abort_called = 1;
      free_data(len);
      return retval;
   }

   mac_cast(intlen, int, len)

   if (intlen <= 0)
   {
      yyerror("Error: Positive number expected as third argument of memcpy.");
      abort_called = 1;
      return retval;
   }

   memcpy(from_eval1.value.genptr, from_eval2.value.genptr, intlen);

   retval.value.memadr = from_eval1.value.genptr;
   retval.ti.dtype = DT_MEMADR;

   return retval;
}



data eval_memset(node* to_eval)
{
   int err = 0, intval = 0, intnum = 0;
   data retval, from_eval, value, num;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 3) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in memset.");
      yyerror("       This function has three parameters.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_MEMADR && from_eval.ti.dtype != DT_STRING && 
      from_eval.ti.dtype != DT_MEMORY && from_eval.ti.nderef < 1)
   {
      yyerror("Error: First argument of memset is not an address.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }

   value = eval(to_eval->childset[1]);
   if (value.ti.dtype < DT_CHAR || value.ti.dtype > DT_LONG_DOUBLE ||
      value.ti.nderef > 0)
   {
      yyerror("Error: Number expected as second argument of memset.");
      abort_called = 1;
      free_data(value);
      return retval;
   }

   num = eval(to_eval->childset[2]);
   if (num.ti.dtype < DT_CHAR || num.ti.dtype > DT_LONG_DOUBLE ||
      num.ti.nderef > 0)
   {
      yyerror("Error: Number expected as third argument of memset.");
      abort_called = 1;
      free_data(num);
      return retval;
   }

   mac_cast(intval, int, value)
   mac_cast(intnum, int, num)

   if (intnum <= 0)
   {
      yyerror("Error: Positive number expected as third argument of memset.");
      abort_called = 1;
      return retval;
   }

   memset(from_eval.value.genptr, intval, intnum);

   retval.value.memadr = from_eval.value.genptr;
   retval.ti.dtype = DT_MEMADR;

   return retval;
}



data eval_memclone(node* to_eval)
{
   int err = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in memclone.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_MEMORY || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Raw memory expected as argument to memclone.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }

   retval.value.mem.tab = malloc(from_eval.value.mem.length);
   if (!retval.value.mem.tab)
   {
      yyerror("Error: Lack of memory in memclone.");
      exit(1);
   }
   memcpy(retval.value.mem.tab, from_eval.value.mem.tab, from_eval.value.mem.length);
   g_lst_add(retval.value.mem.tab, PT_CHAR_TAB);
   retval.value.mem.length = from_eval.value.mem.length;
   retval.ti.dtype = DT_MEMORY;

   return retval;
}



data eval_memory(node* to_eval)
{
   int err = 0, len = 0;
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in memory.");
      yyerror("       This function has one parameter.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype < DT_CHAR || from_eval.ti.dtype > DT_LONG_DOUBLE || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Number expected as argument of memory.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }

   mac_cast(len, int, from_eval)
   if (len <= 0)
   {
      yyerror("Error: Positive number expected argument of repeat.");
      abort_called = 1;
      return retval;
   }

   retval.value.mem.tab = malloc(len);
   if (!retval.value.mem.tab)
   {
      yyerror("Error: Lack of memory in memory function.");
      exit(1);
   }
   memset(retval.value.mem.tab, 0, len);
   g_lst_add(retval.value.mem.tab, PT_CHAR_TAB);
   retval.value.mem.length = len;
   retval.ti.dtype = DT_MEMORY;

   return retval; 
}



data eval_codetotree(node* to_eval)
{
   int err = 0;
   data retval, from_eval;
   input_type previnputsrc;
   input_union previnputadr;
   node* prevroot = NULL;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in codetotree.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }


   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: Argument of codetotree is not a string.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }

   maxpos = retval.value.str.length - 1;
   readpos = 0;

   previnputsrc = inputsrc;
   previnputadr = inputadr;
   prevroot = root;

   inputsrc = IT_STRING;
   inputadr.str = from_eval.value.str.tab;
   yyparse();
   if (!yynerrs)
   {
      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = root;
   }

   inputsrc = previnputsrc;
   inputadr = previnputadr;
   root = prevroot;

   free_data(from_eval);

   return retval;
}



data eval_exist(node* to_eval)
{
   int err = 0, bres = 0;
   data retval, from_eval;
   closure* pclos_ret;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in exist.");
      yyerror("       This function has one parameter.");
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = DT_INT;

   switch (to_eval->childset[0]->ntype)
   {
   case NT_VARIABLE:
      /* Search in all the closures for the variable name. */
      bres = find_symbol(to_eval->childset[0]->opval.name) != NULL;
      break;
   case NT_LIST:
      if (to_eval->childset[0]->childset)
      {
         if (to_eval->childset[0]->childset[0]->ntype == NT_PARENT)
         {
            from_eval = eval_dotlist(to_eval->childset[0], to_eval->childset[0]);
            bres = from_eval.ti.dtype == DT_POINTER;
            if (!bres) free_data(from_eval);
         }
      }
      break;
   case NT_VARGENLIST:
      from_eval = eval(to_eval->childset[0]);
      bres = from_eval.ti.dtype == DT_POINTER;
      if (!bres) free_data(from_eval);
      break;
   case NT_ACCESSLIST:
      bres = resolve_accesslist(to_eval->childset[0], &pclos_ret, &from_eval) == CLOSURE;
      break;
   case NT_SUBSCRIPT:
      bres = resolve_subscript(to_eval->childset[0]) != NULL;
      break;
   default:
      break;
   }

   retval.value.inum = bres;
   abort_called = 0;

   return retval;
}



int valid_id(char* id)
{
   int i = 1;
   char* resword = (char*)0x1;

   if (!isalpha(id[0]) && id[0] != '_') return 0;

   while (id[i] != '\0')
   {
      if (!isalnum(id[i]) && id[i] != '_') return 0;
      i++;
   }

   i = 0;
   resword = reserved_words_table[0].rword;
   while (resword != NULL)
   {
      if (!strcmp(id, resword)) return 0;
      resword = reserved_words_table[++i].rword;
   }

   return 1;
}


int first_valid(char* str)
{
   int i = 0;

   while (str[i] == ' ' && str[i] != '\0') i++;

   if (str[i] == '\0') return -1;
   else return i;
}


int type_token(char* str, int* nextpos)
{
   char buf[9];
   int ibuf = 0, istr = 0, ires = 10;

   istr = first_valid(str);

   if (istr == -1) return -1;
   else if (str[istr] == '*')
   {
      *nextpos = istr + 1;
      return '*';
   }

   memset(buf, 0, 9);

   while (ibuf < 8 && str[istr] != ' ' && str[istr] != '\0' && str[istr] != '*')
   {
      buf[ibuf] = str[istr];
      ibuf++;
      istr++;
   }

   while (reserved_words_table[ires].rword != NULL)
   {
      if (!strcmp(buf, reserved_words_table[ires].rword))
      {
         *nextpos = istr;
         return reserved_words_table[ires].num_symbol;
      }
      ires++;
   }

   return -1;
}


struct
{
   int a, b, c, d;
   data_type dtype;
}
oktoktab[] = 
{
   {CHAR,     0,      0,    0,   DT_CHAR},
   {SIGNED,   CHAR,   0,    0,   DT_S_CHAR},
   {UNSIGNED, CHAR,   0,    0,   DT_U_CHAR},
   {BYTE,     0,      0,    0,   DT_BYTE},
   {SHORT,    0,      0,    0,   DT_SHORT},
   {SHORT,    INT,    0,    0,   DT_SHORT},
   {SIGNED,   SHORT,  0,    0,   DT_SHORT},
   {SIGNED,   SHORT,  INT,  0,   DT_SHORT},
   {UNSIGNED, SHORT,  0,    0,   DT_U_SHORT},
   {UNSIGNED, SHORT,  INT,  0,   DT_U_SHORT},
   {INT,      0,      0,    0,   DT_INT},
   {SIGNED,   INT,    0,    0,   DT_INT},
   {UNSIGNED, 0,      0,    0,   DT_U_INT},
   {UNSIGNED, INT,    0,    0,   DT_U_INT},
   {LONG,     0,      0,    0,   DT_LONG},
   {LONG,     INT,    0,    0,   DT_LONG},
   {SIGNED,   LONG,   0,    0,   DT_LONG},
   {SIGNED,   LONG,   INT,  0,   DT_LONG},
   {UNSIGNED, LONG,   0,    0,   DT_U_LONG},
   {UNSIGNED, LONG,   INT,  0,   DT_U_LONG},
   {LONG,     LONG,   0,    0,   DT_LONG_LONG},
   {LONG,     LONG,   INT,  0,   DT_LONG_LONG},
   {SIGNED,   LONG,   LONG, 0,   DT_LONG_LONG},
   {SIGNED,   LONG,   LONG, INT, DT_LONG_LONG},
   {UNSIGNED, LONG,   LONG, 0,   DT_U_LONG_LONG},
   {UNSIGNED, LONG,   LONG, INT, DT_U_LONG_LONG},
   {SIZE_T,   0,      0,    0,   DT_SIZE_T},
   {FLOAT,    0,      0,    0,   DT_FLOAT},
   {DOUBLE,   0,      0,    0,   DT_DOUBLE},
   {LONG,     DOUBLE, 0,    0,   DT_LONG_DOUBLE},
   {0,        0,      0,    0,   0}
};


type_info parse_cast(char* str)
{
   type_info retinfo;
   int token = 0, addpos = 0, toktab[4] = {0, 0, 0, 0};
   size_t nindirec = 0;
   int itok = 0, istr = 0, found = 0;

   memset(&retinfo, 0, sizeof(type_info));

   while (itok < 4)
   {
      token = type_token(&str[istr], &addpos);

      if (token != -1 && token != '*') toktab[itok] = token;
      else if (token == '*')
      {
         istr += addpos;
         break;
      }
      else break;

      itok++;
      istr += addpos;
   }

   if (token == '*')
   {
      char currchar = str[istr];
      nindirec = 1;

      while (currchar == '*' || currchar == ' ')
      {
         if (currchar == '*') nindirec++;
         istr++;
         currchar = str[istr];
      }

      if (currchar != '\0') return retinfo;
   }

   itok = 0;
   while(!found && oktoktab[itok].a != 0)
   {
      found = !memcmp(toktab, &oktoktab[itok], 4 * sizeof(int));
      itok++;
   }

   if (found)
   {
      retinfo.dtype = oktoktab[itok - 1].dtype;
      retinfo.nderef = nindirec;
   }

   return retinfo;
}


data eval_createnode(node* to_eval)
{
   int err = 0, bcont = 1, i = 0, relop = 0, ppmm = 0;
   data retval, from_eval;
   node_type typetocreate = NT_UNDEF;
   node* newnode;
   type_info castinfo;

   memset(&retval, 0, sizeof(data));


   /* Argument verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs < 1) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in createnode.");
      yyerror("       This function has at least one parameter.");
      abort_called = 1;
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
   {
      yyerror("Error: First argument of createnode is not a string.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }

   while (bcont)
   {
      char c1search = from_eval.value.str.tab[0];
      char c1map = ntype_map[i].descr[0];

      if (c1map == '\0') bcont = 0;
      else
      {
         if (c1search == c1map)
         {
            if (!strcmp(from_eval.value.str.tab, ntype_map[i].descr))
            {
               typetocreate = ntype_map[i].ntype;
               bcont = 0;
            }
         }
         else if (c1map > c1search) bcont = 0;
      }

      i++;
   }


   switch(typetocreate)
   {
   case NT_NUM_CONST:
      if (to_eval->nb_childs < 2)
      {
         yyerror("Error: Second argument expected in createnode. ");
         abort_called = 1;
         return retval;
      }
      from_eval = eval(to_eval->childset[1]);
      if (from_eval.ti.dtype != DT_DOUBLE || from_eval.ti.nderef > 0)
      {
         yyerror("Error: Second argument of createnode is not a double.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }

      newnode = malloc(sizeof(node));
      if (!newnode) fatal_error("Error: Lack of memory for new node in createnode function.");
      memset(newnode, 0, sizeof(node));

      newnode->ntype = typetocreate;
      newnode->opval.value = from_eval.value.num;

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = newnode;
      break;

   case NT_MATH_OPER:
      if (to_eval->nb_childs < 2)
      {
         yyerror("Error: Second argument expected in createnode. ");
         abort_called = 1;
         return retval;
      }
      from_eval = eval(to_eval->childset[1]);
      if (from_eval.ti.dtype != DT_CHAR || from_eval.ti.nderef > 0)
      {
         yyerror("Error: Second argument of createnode is not a char.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }
      if (from_eval.value.cnum != '-' && from_eval.value.cnum != '+' && from_eval.value.cnum != '*'  && 
         from_eval.value.cnum != '/' && from_eval.value.cnum != '^' && from_eval.value.cnum != '=' && 
         from_eval.value.cnum != '&')
      {
         yyerror("Error: Second argument of createnode is not a known operator.");
         abort_called = 1;
         return retval;
      }

      newnode = malloc(sizeof(node));
      if (!newnode) fatal_error("Error: Lack of memory for new node in createnode function.");
      memset(newnode, 0, sizeof(node));

      newnode->ntype = typetocreate;
      newnode->opval.math_oper = from_eval.value.cnum;

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = newnode;
      break;

   case NT_REL_OPER:
      if (to_eval->nb_childs < 2)
      {
         yyerror("Error: Second argument expected in createnode. ");
         abort_called = 1;
         return retval;
      }
      from_eval = eval(to_eval->childset[1]);
      if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
      {
         yyerror("Error: Second argument of createnode is not a string.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }
      if (!strcmp(from_eval.value.str.tab, "||")) relop = OR;
      else if (!strcmp(from_eval.value.str.tab, "&&")) relop = AND;
      else if (!strcmp(from_eval.value.str.tab, "!")) relop = NOT;
      else if (!strcmp(from_eval.value.str.tab, ">")) relop = GT;
      else if (!strcmp(from_eval.value.str.tab, ">=")) relop = GE;
      else if (!strcmp(from_eval.value.str.tab, "<")) relop = LT;
      else if (!strcmp(from_eval.value.str.tab, "<=")) relop = LE;
      else if (!strcmp(from_eval.value.str.tab, "==")) relop = EQ;
      else if (!strcmp(from_eval.value.str.tab, "!=")) relop = NE;
      else
      {
         yyerror("Error: Unknown operator requested in createnode function.");
         abort_called = 1;
         return retval;
      }

      newnode = malloc(sizeof(node));
      if (!newnode) fatal_error("Error: Lack of memory for new node in createnode function.");
      memset(newnode, 0, sizeof(node));

      newnode->ntype = typetocreate;
      newnode->opval.rel_oper = relop;

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = newnode;
      break;

   case NT_VARIABLE:
      if (to_eval->nb_childs < 2)
      {
         yyerror("Error: Second argument expected in createnode. ");
         abort_called = 1;
         return retval;
      }
      from_eval = eval(to_eval->childset[1]);
      if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
      {
         yyerror("Error: Second argument of createnode is not a string.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }
      if (!valid_id(from_eval.value.str.tab))
      {
         yyerror("Error: Second argument of createnode is not a valid identifier.");
         abort_called = 1;
         return retval;
      }

      newnode = malloc(sizeof(node));
      if (!newnode) fatal_error("Error: Lack of memory for new node in createnode function.");
      memset(newnode, 0, sizeof(node));

      newnode->ntype = typetocreate;
      newnode->opval.name = malloc(from_eval.value.str.length);
      if (!newnode->opval.name) 
         fatal_error("Error: Lack of memory for variable name in createnode function.");
      strcpy(newnode->opval.name, from_eval.value.str.tab);

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = newnode;
      break;

   case NT_STRING:
      if (to_eval->nb_childs < 2)
      {
         yyerror("Error: Second argument expected in createnode. ");
         abort_called = 1;
         return retval;
      }
      from_eval = eval(to_eval->childset[1]);
      if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
      {
         yyerror("Error: Second argument of createnode is not a string.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }

      newnode = malloc(sizeof(node));
      if (!newnode) fatal_error("Error: Lack of memory for new node in createnode function.");
      memset(newnode, 0, sizeof(node));

      newnode->ntype = typetocreate;
      newnode->opval.str.length = from_eval.value.str.length;
      newnode->opval.str.tab = malloc(from_eval.value.str.length);
      if (!newnode->opval.str.tab)
         fatal_error("Error: Lack of memory for string in createnode function.");
      strncpy(newnode->opval.str.tab, from_eval.value.str.tab, from_eval.value.str.length);

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = newnode;
      break;

   case NT_FUNC_DEF:
   case NT_FUNC_CALL:
   case NT_IF_STMT:
   case NT_IFELSE_STMT:
   case NT_FOR_STMT:
   case NT_WHILE_STMT:
   case NT_LIST:
   case NT_CODESEGMENT:
   case NT_PARENT:
   case NT_CHILDSET:
   case NT_VARGENLIST:
   case NT_ACCESSLIST:
   case NT_SUBSCRIPT:
   case NT_STDIN:
   case NT_STDOUT:
   case NT_STDERR:
   case NT_REF:
   case NT_DEREF:
      newnode = malloc(sizeof(node));
      if (!newnode) fatal_error("Error: Lack of memory for new node in createnode function.");
      memset(newnode, 0, sizeof(node));

      newnode->ntype = typetocreate;

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = newnode;
      break;

   case NT_INCR_DECR:
      if (to_eval->nb_childs < 2)
      {
         yyerror("Error: Second argument expected in createnode. ");
         abort_called = 1;
         return retval;
      }
      from_eval = eval(to_eval->childset[1]);
      if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
      {
         yyerror("Error: Second argument of createnode is not a string.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }
      if (!strcmp(from_eval.value.str.tab, "prefix ++")) ppmm = PREPLUSPLUS;
      else if (!strcmp(from_eval.value.str.tab, "prefix --")) ppmm = PREMINUSMINUS;
      else if (!strcmp(from_eval.value.str.tab, "postfix ++")) ppmm = POSTPLUSPLUS;
      else if (!strcmp(from_eval.value.str.tab, "postfix --")) ppmm = POSTMINUSMINUS;
      else
      {
         yyerror("Error: Unknown operator requested in createnode function.");
         abort_called = 1;
         return retval;
      }

      newnode = malloc(sizeof(node));
      if (!newnode) fatal_error("Error: Lack of memory for new node in createnode function.");
      memset(newnode, 0, sizeof(node));

      newnode->ntype = typetocreate;
      newnode->opval. ppmm = ppmm;

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = newnode;
      break;

   case NT_CAST:
      if (to_eval->nb_childs < 2)
      {
         yyerror("Error: Second argument expected in createnode. ");
         abort_called = 1;
         return retval;
      }
      from_eval = eval(to_eval->childset[1]);
      if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef > 0)
      {
         yyerror("Error: Second argument of createnode is not a string.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }
      castinfo = parse_cast(from_eval.value.str.tab);
      if (castinfo.dtype == DT_UNDEF)
      {
         yyerror("Error: Second argument of createnode is not a valid cast type.");
         abort_called = 1;
         return retval;
      }
      
      newnode = malloc(sizeof(node));
      if (!newnode) fatal_error("Error: Lack of memory for new node in createnode function.");
      memset(newnode, 0, sizeof(node));

      newnode->ntype = typetocreate;
      newnode->opval.ti = castinfo;

      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = newnode;
      break;

   case NT_UNDEF:
   default:
      yyerror("Error: Specified node type in createnode not found.");
      abort_called = 1;
      return retval;
   }

   return retval;
}



data eval_appendchild(node* to_eval)
{
   int err = 0;
   size_t insertpos = 0;
   data retval, from_eval;
   node* parent = NULL, * toappend = NULL;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in appendchild.");
      yyerror("       This function has two parameters.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_POINTER || from_eval.ti.nderef != 0)
   {
      yyerror("Error: First argument of appendchild is not a pointer to a node.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }
   parent = from_eval.value.ptr;

   from_eval = eval(to_eval->childset[1]);
   if (from_eval.ti.dtype != DT_POINTER || from_eval.ti.nderef != 0)
   {
      yyerror("Error: Second argument of appendchild is not a pointer to a node.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }
   toappend = from_eval.value.ptr;


   if (parent->nb_childs == 0)
   {
      parent->childset = malloc(sizeof(node*));
   }
   else
   {
      parent->childset = realloc(parent->childset, (parent->nb_childs + 1) * sizeof(node*));
      insertpos = parent->nb_childs;
   }
   if (!parent->childset) fatal_error("Error: Lack of memory in appendchild for new child set.");

   parent->nb_childs++;
   parent->childset[insertpos] = toappend;
   toappend->parent = parent;

   return retval;
}



data eval_func_call(node* to_eval)
{
   size_t nb_param = 0, nb_args = 0, nb_members = 0, i;
   node* func_pt = NULL;
   data* data_tab;
   data retval, ret_data;
   closure* cpt, * ret_pclos;
   cont_type ret_ct;
   clos_set* pObject = NULL;

   memset(&retval, 0, sizeof(data));

   if (to_eval->childset[0]->ntype == NT_VARIABLE)
   {
      /* Function's name search in closures. */
      cpt = find_symbol(to_eval->childset[0]->opval.name);
      if (cpt)
      {
         if (cpt->content.ti.dtype == DT_POINTER)
         {
            func_pt = cpt->content.value.ptr;
         }
         else if (cpt->content.ti.dtype == DT_PFUNC)
         {
            return eval_slib_call(cpt->content, to_eval->childset[1]);
         }
         else
         {
            yyerror("Error: Call of a non function.");
            abort_called = 1;
            return retval;
         }
      }
   }
   else if (to_eval->childset[0]->ntype == NT_ACCESSLIST)
   {
      ret_ct = resolve_accesslist(to_eval->childset[0], &ret_pclos, &ret_data);
      if (ret_ct == NEITHER)
      {
         return retval;
      }
      if (ret_ct == CLOSURE)
      {
         if (ret_pclos->content.ti.dtype == DT_POINTER)
         {
            func_pt = ret_pclos->content.value.ptr;
            pObject = ret_pclos->pContainer;
         }
         else if (ret_pclos->content.ti.dtype == DT_PFUNC)
         {
            return eval_slib_call(ret_pclos->content, to_eval->childset[1]);
         }
         else
         {
            yyerror("Error: Node or function pointer expected from access list.");
            abort_called = 1;
            return retval;
         }
      }
      else if (ret_ct == DATA)
      {
         if (ret_data.ti.dtype == DT_POINTER)
         {
            func_pt = ret_data.value.ptr;
         }
         else if (ret_data.ti.dtype == DT_PFUNC)
         {
            return eval_slib_call(ret_data, to_eval->childset[1]);
         }
         else
         {
            yyerror("Error: Node or function pointer expected from access list.");
            abort_called = 1;
            free_data(ret_data);
            return retval;
         }
      }
      else
      {
         yyerror("Error: Unknown cont_type in eval_func_call.");
         exit(1);
      }
   }
   else
   {
      ret_data = eval(to_eval->childset[0]);
      if (ret_data.ti.dtype == DT_POINTER)
      {
         func_pt = ret_data.value.ptr;
      }
      else if (ret_data.ti.dtype == DT_PFUNC)
      {
         return eval_slib_call(ret_data, to_eval->childset[1]);
      }
      else
      {
         yyerror("Error: Node pointer expected from left call evaluation.");
         abort_called = 1;
         free_data(ret_data);
         return retval;
      }
   }


   if (!func_pt)
   {
      /* Internal functions. */

      EFuncID eFnCalled = FID_NOFUNC;

      for (i = 0; i < NB_FUNC; i++)
      {
         char c1search = to_eval->childset[0]->opval.name[0];
         char c1map = func_map[i].func_name[0];

         if (c1search == c1map)
         {
            if (!strcmp(to_eval->childset[0]->opval.name, func_map[i].func_name))
            {
               eFnCalled = func_map[i].eFnID;
               break;
            }
         }
         else if (c1map > c1search) break;
      }

      switch (eFnCalled)
      {
      case FID_PRINT:
         return eval_print(to_eval->childset[1]);
      case FID_EXIT:
         exit(0);
      case FID_ABORT:
         return eval_abort();
      case FID_INFO:
         return eval_info(to_eval->childset[1]);
      case FID_MKNODE:
         return eval_mknode(to_eval->childset[1]);
      case FID_RMNODE:
         return eval_rmnode(to_eval->childset[1]);
      case FID_VALUE:
         return eval_value(to_eval->childset[1]);
      case FID_LENGTH:
         return eval_length(to_eval->childset[1]);
      case FID_SCAN:
         return eval_scan();
      case FID_READ:
         return eval_read(to_eval->childset[1]);
      case FID_WRITE:
         return eval_write(to_eval->childset[1]);
      case FID_GETWD:
         return eval_getwd();
      case FID_SETWD:
         return eval_setwd(to_eval->childset[1]);
      case FID_TONODE:
         return eval_tonode(to_eval->childset[1]);
      case FID_SUBSEQ:
         return eval_subseq(to_eval->childset[1]);
      case FID_CONCAT:
         return eval_concat(to_eval->childset[1]);
      case FID_NTOA:
         return eval_ntoa(to_eval->childset[1]);
      case FID_ATON:
         return eval_aton(to_eval->childset[1]);
      case FID_RETURN:
         return eval_return(to_eval->childset[1]);
      case FID_COPYTREE:
         return eval_copytree(to_eval->childset[1]);
      case FID_CLEAR:
         return eval_clear(to_eval->childset[1]);
      case FID_EXECUTE:
         return eval_execute(to_eval->childset[1]);
      case FID_PROMPT:
         return eval_prompt(to_eval->childset[1]);
      case FID_NAMES:
         return eval_names(to_eval->childset[1]);
      case FID_ARRAY:
         return eval_array(to_eval->childset[1]);
      case FID_LIST:
         return eval_list(to_eval->childset[1]);
      case FID_FILLOBJECT:
         return eval_fillobject(to_eval->childset[1]);
      case FID_SETLENGTH:
         return eval_setlength(to_eval->childset[1]);
      case FID_INSERT:
         return eval_insert(to_eval->childset[1]);
      case FID_REPLACE:
         return eval_replace(to_eval->childset[1]);
      case FID_SOURCE:
         return eval_source(to_eval->childset[1]);
      case FID_STRLEN:
         return eval_strlen(to_eval->childset[1]);
      case FID_AS_ARRAY:
         return eval_as_array(to_eval->childset[1]);
      case FID_AS_LIST:
         return eval_as_list(to_eval->childset[1]);
      case FID_AS_STATEMENTS:
         return eval_as_statements(to_eval->childset[1]);
      case FID_GETTYPE:
         return eval_gettype(to_eval->childset[1]);
      case FID_UNDEFINE:
         return eval_undefine(to_eval->childset[1]);
      case FID_PRINTF:
         return eval_printf(to_eval->childset[1]);
      case FID_SCANF:
         return eval_scanf(to_eval->childset[1]);
      case FID_FOPEN:
         return eval_fopen(to_eval->childset[1]);
      case FID_FCLOSE:
         return eval_fclose(to_eval->childset[1]);
      case FID_FPRINTF:
         return eval_fprintf(to_eval->childset[1]);
      case FID_FSCANF:
         return eval_fscanf(to_eval->childset[1]);
      case FID_FEOF:
         return eval_feof(to_eval->childset[1]);
      case FID_FREAD:
         return eval_fread(to_eval->childset[1]);
      case FID_FWRITE:
         return eval_fwrite(to_eval->childset[1]);
      case FID_CLEARERR:
         return eval_clearerr(to_eval->childset[1]);
      case FID_FGETPOS:
         return eval_fgetpos(to_eval->childset[1]);
      case FID_FSETPOS:
         return eval_fsetpos(to_eval->childset[1]);
      case FID_FERROR:
         return eval_ferror(to_eval->childset[1]);
      case FID_FGETC:
         return eval_fgetc(to_eval->childset[1]);
      case FID_UNGETC:
         return eval_ungetc(to_eval->childset[1]);
      case FID_FFLUSH:
         return eval_fflush(to_eval->childset[1]);
      case FID_FPUTC:
         return eval_fputc(to_eval->childset[1]);
      case FID_FSEEK:
         return eval_fseek(to_eval->childset[1]);
      case FID_FTELL:
         return eval_ftell(to_eval->childset[1]);
      case FID_FREOPEN:
         return eval_freopen(to_eval->childset[1]);
      case FID_REWIND:
         return eval_rewind(to_eval->childset[1]);
      case FID_TMPFILE:
         return eval_tmpfile();
      case FID_SPRINTF:
         return eval_sprintf(to_eval->childset[1]);
      case FID_SSCANF:
         return eval_sscanf(to_eval->childset[1]);
      case FID_REMOVE:
         return eval_remove(to_eval->childset[1]);
      case FID_CLS:
         return eval_cls();
      case FID_ATOVAR:
         return eval_atovar(to_eval->childset[1]);
      case FID_VARTOA:
         return eval_vartoa(to_eval->childset[1]);
      case FID_FREETREE:
         return eval_freetree(to_eval->childset[1]);
      case FID_STRCLONE:
         return eval_strclone(to_eval->childset[1]);
      case FID_STRCPY:
         return eval_strcpy(to_eval->childset[1]);
      case FID_OPENLIB:
         return eval_openlib(to_eval->childset[1]);
      case FID_GETFUNCPT:
         return eval_getfuncpt(to_eval->childset[1]);
      case FID_CLOSELIB:
         return eval_closelib(to_eval->childset[1]);
      case FID_ALLOC_COPY:
         return eval_alloc_copy(to_eval->childset[1]);
      case FID_FREE:
         return eval_free(to_eval->childset[1]);
      case FID_REPEAT:
         return eval_repeat(to_eval->childset[1]);
      case FID_VARTOMEM:
         return eval_vartomem(to_eval->childset[1]);
      case FID_MEMTOVAR:
         return eval_memtovar(to_eval->childset[1]);
      case FID_MEMCPY:
         return eval_memcpy(to_eval->childset[1]);
      case FID_MEMSET:
         return eval_memset(to_eval->childset[1]);
      case FID_MEMCLONE:
         return eval_memclone(to_eval->childset[1]);
      case FID_MEMORY:
         return eval_memory(to_eval->childset[1]);
      case FID_TREETOCODE:
         return eval_treetocode(to_eval->childset[1]);
      case FID_CODETOTREE:
         return eval_codetotree(to_eval->childset[1]);
      case FID_EXIST:
         return eval_exist(to_eval->childset[1]);
      case FID_CREATENODE:
         return eval_createnode(to_eval->childset[1]);
      case FID_APPENDCHILD:
         return eval_appendchild(to_eval->childset[1]);
      default:
         break;
      }


      yyerror("Error: Function not found.");
      abort_called = 1;
      return retval;
   }
   else
   {
      if (func_pt->ntype != NT_FUNC_DEF)
      {
         yyerror("Error: Node pointer does not reference a function definition node in"
            " eval_func_call.");
         abort_called = 1;
         return retval;
      }
   }


   /* Parameters evaluation. */

   if (func_pt->childset[0])
   {
      /* One or more parameters. */
      nb_param = func_pt->childset[0]->nb_childs;
   }

   if (to_eval->childset[1])
   {
      /* One or more arguments. */
      nb_args = to_eval->childset[1]->nb_childs;
   }

   /* Parameter number verification. */
   if (nb_param != nb_args)
   {
      if (to_eval->childset[0]->ntype == NT_VARIABLE)
      {
         fprintf(stderr, "Error: Wrong number of arguments in %s.\n", 
            to_eval->childset[0]->opval.name);
      }
      else
      {
         yyerror("Error: Wrong number of arguments in function call.");
      }
      return retval;
   }

   data_tab = malloc(nb_args * sizeof(data));
   if (!data_tab)
   {
      yyerror("Error: Lack of memory for data_tab in eval_func_call.");
      exit(1);
   }
   memset(data_tab, 0, nb_args * sizeof(data));
   for (i = 0; i < nb_args; i++)
   {
      data_tab[i] = eval(to_eval->childset[1]->childset[i]);
      if (data_tab[i].ti.dtype == DT_UNDEF)
      {
         yyerror("Error: Parameter not evaluated for a function call.");
         abort_called = 1;
         return retval;
      }
   }


   /* Context containing arguments and members creation. */

   create_context();

   if (pObject) nb_members = pObject->nb_clos;

   clos_set_stack[set_stack_size-1]->nb_clos = nb_param + nb_members;
   clos_set_stack[set_stack_size-1]->clos_array = malloc((nb_param + nb_members
      ) * sizeof(closure*));
   if (!clos_set_stack[set_stack_size-1]->clos_array)
   {
      yyerror("Error: Lack of memory for");
      yyerror("clos_array in eval_func_call.");
      exit(1);
   }
   memset(clos_set_stack[set_stack_size-1]->clos_array, 0, (nb_param + nb_members
      ) * sizeof(closure*));
   for (i = 0; i < nb_members; i++)
   {
      /* Copy of the object members. */
      clos_set_stack[set_stack_size-1]->clos_array[i] = malloc(sizeof(closure));
      if (!clos_set_stack[set_stack_size-1]->clos_array[i])
      {
         yyerror("Error: Lack of memory for");
         yyerror("closure in eval_func_call.");
         exit(1);
      }
      memset(clos_set_stack[set_stack_size-1]->clos_array[i], 0, sizeof(closure));
      copy_clos(clos_set_stack[set_stack_size-1]->clos_array[i], *pObject->
         clos_array[i]);
   }
   for (i = 0; i < nb_param; i++)
   {
      size_t len;

      /* Parameter verification. */
      if (func_pt->childset[0]->childset[i]->ntype != NT_VARIABLE)
      {
         yyerror("Error: Encountered a non variable parameter.");
         exit(1);
      }

      /* Copy of the parameter name to the closure. */
      len = strlen(func_pt->childset[0]->childset[i]->opval.name) + 1;
      clos_set_stack[set_stack_size-1]->clos_array[i + nb_members] = malloc(sizeof(closure));
      if (!clos_set_stack[set_stack_size-1]->clos_array[i + nb_members])
      {
         yyerror("Error: Lack of memory for");
         yyerror("closure in eval_func_call.");
         exit(1);
      }
      memset(clos_set_stack[set_stack_size-1]->clos_array[i + nb_members], 0, sizeof(closure));
      clos_set_stack[set_stack_size-1]->clos_array[i + nb_members]->name = malloc(len);
      if(!clos_set_stack[set_stack_size-1]->clos_array[i + nb_members]->name)
      {
         yyerror("Error: Lack of memory for a name in eval_func_call.");
         exit(1);
      }
      strcpy(clos_set_stack[set_stack_size-1]->clos_array[i + nb_members]->name, 
         func_pt->childset[0]->childset[i]->opval.name);

      copy_data(&clos_set_stack[set_stack_size-1]->clos_array[i + nb_members]->
         content, data_tab[i]);
   }


   /* Evaluation of the function body. */

   if (func_pt->childset[1])
   {
      if (func_pt->childset[1]->ntype == NT_LIST)
      {
         i = 0;
         while (i < func_pt->childset[1]->nb_childs && !return_called 
            && !abort_called)
         {
            retval = eval(func_pt->childset[1]->childset[i]);
            i++;
         }
         return_called = 0;
      }
      else
      {
         retval = eval(func_pt->childset[1]);
         return_called = 0;
      }
   }


   /* Members alterations recuperation. */

   for (i = 0; i < nb_members; i++)
   {
      free_data(pObject->clos_array[i]->content);
      copy_data(&pObject->clos_array[i]->content, clos_set_stack[set_stack_size-
         1]->clos_array[i]->content);
   }


   /* Context destruction. */

   free_context();
   for (i = 0; i < nb_args; i++) free_data(data_tab[i]);
   free(data_tab);

   return retval;
}



data eval_dotlist(node* start_pt, node* dotlist)
{
   size_t i = 0;
   node* npt = start_pt;
   data retval;

   memset(&retval, 0, sizeof(data));

   while (i < dotlist->nb_childs)
   {
      if (dotlist->childset[i]->ntype == NT_PARENT)
      {
         if (npt) npt = npt->parent;
         else
         {
            yyerror("Error: NULL parent found in dotlist.");
            abort_called = 1;
            return retval;
         }
      }
      else if (dotlist->childset[i]->ntype == NT_CHILDSET)
      {
         data index = eval(dotlist->childset[i]->childset[0]);
         size_t lindx = 0;

         if (index.ti.dtype < DT_CHAR || index.ti.dtype > DT_LONG_DOUBLE ||
            index.ti.nderef > 0) return retval;
         if (!npt)
         {
            yyerror("Error: NULL childset found in dotlist."); 
            abort_called = 1;
            return retval;
         }

         mac_cast(lindx, size_t, index)
         if(lindx >= npt->nb_childs)
         {
            yyerror("Error: Array index out of bound for childset.");
            abort_called = 1;
            return retval;
         }

         npt = npt->childset[lindx];
      }
      else
      {
         yyerror("Error: Unknown node type in eval_dotlist.");
         exit(1);
      }
      i++;
   }

   retval.ti.dtype = DT_POINTER;
   retval.value.ptr = npt;
   return retval;
}



data eval_incr_decr(node* to_eval)
{
   data retval, * to_change = NULL;

   memset(&retval, 0, sizeof(data));

   to_change = resolve(to_eval->childset[0]);

   if (!to_change) return retval;

   if (to_change->ti.dtype < DT_CHAR || to_change->ti.dtype > DT_LONG_DOUBLE ||
      to_change->ti.nderef > 0)
   {
      yyerror("Variable type error for incrementation or decrementation.");
      abort_called = 1;
      return retval;
   }

   retval.ti.dtype = to_change->ti.dtype;

   switch (to_eval->opval.ppmm)
   {
   case PREPLUSPLUS:
      mac_ppmm(++)
      retval.value = to_change->value;
      break;
   case PREMINUSMINUS:
      mac_ppmm(--)
      retval.value = to_change->value;
      break;
   case POSTPLUSPLUS:
      retval.value = to_change->value;
      mac_ppmm(++)
      break;
   case POSTMINUSMINUS:
      retval.value = to_change->value;
      mac_ppmm(--)
      break;
   }

   return retval;
}



data eval_accesslist(node* to_eval)
{
   data retval, from_eval;
   cont_type ct_ret;
   closure* pclos;

   memset(&retval, 0, sizeof(data));

   ct_ret = resolve_accesslist(to_eval, &pclos, &from_eval);

   if (ct_ret == CLOSURE)
   {
      copy_data(&retval, pclos->content);
      return retval;
   }

   if (ct_ret == DATA)
   {
      return from_eval;
   }

   return retval;
}



data eval_subscript(node* to_eval)
{
   size_t intindex = 0;
   data retval, * from_resolve, from_eval, index;

   memset(&retval, 0, sizeof(data));

   from_resolve = resolve_subscript(to_eval);

   if (abort_called) return retval;

   if (from_resolve)
   {
      copy_data(&retval, *from_resolve);
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_ARRAY && from_eval.ti.dtype != DT_LIST && 
      from_eval.ti.dtype != DT_STRING && from_eval.ti.dtype != DT_MEMORY)
   {
      yyerror("Error: Subscript applied to a non array, list, string or memory.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }

   /* Subscript number evaluation. */
   index = eval(to_eval->childset[1]);
   if (index.ti.dtype < DT_CHAR || index.ti.dtype > DT_LONG_DOUBLE ||
      index.ti.nderef > 0)
   {
      yyerror("Error: Number expected as subscript.");
      abort_called = 1;
      free_data(from_eval);
      free_data(index);
      return retval;
   }

   mac_cast(intindex, size_t, index)

   if (from_eval.ti.dtype == DT_ARRAY)
   {
      if (intindex >= from_eval.value.pArray->length)
      {
         yyerror("Error: Array index out of bound.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }

      copy_data(&retval, from_eval.value.pArray->dtable[intindex]);
   }
   else if (from_eval.ti.dtype == DT_LIST)
   {
      listlink* plink = from_eval.value.pList->start;
      size_t i;

      if (intindex >= from_eval.value.pList->length)
      {
         yyerror("Error: Array index out of bound.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }

      for (i = 1; i <= intindex; i++)
      {
         plink = plink->next;
      }

      copy_data(&retval, plink->content);
   }
   else if (from_eval.ti.dtype == DT_STRING)
   {
      if (intindex >= from_eval.value.str.length)
      {
         yyerror("Error: Array index out of bound.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }

      retval.value.cnum = from_eval.value.str.tab[intindex];
      retval.ti.dtype = DT_CHAR;
   }
   else /* from_eval.ti.dtype == DT_MEMORY */
   {
      if (intindex >= from_eval.value.mem.length)
      {
         yyerror("Error: Memory offset out of bound.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }

      retval.value.byte = (unsigned char)from_eval.value.mem.tab[intindex];
      retval.ti.dtype = DT_BYTE;
   }

   free_data(from_eval);
   return retval;
}



data eval_cast(node* to_eval)
{
   data retval, from_eval;

   memset(&retval, 0, sizeof(data));


   from_eval = eval(to_eval->childset[0]);

   if (to_eval->opval.ti.nderef == 0 && from_eval.ti.nderef == 0)
   {
      if (from_eval.ti.dtype == DT_OBJECT || from_eval.ti.dtype == DT_ARRAY || 
         from_eval.ti.dtype == DT_LIST || from_eval.ti.dtype <= DT_UNDEF ||
         from_eval.ti.dtype >= NB_TYPES)
      {
         yyerror("Error: Cast operand type not supported.");
         free_data(from_eval);
         abort_called = 1;
         return retval;
      }

      switch (to_eval->opval.ti.dtype)
      {
      case DT_CHAR:
         mac_cast(retval.value.cnum, char, from_eval)
         break;
      case DT_S_CHAR:
         mac_cast(retval.value.scnum, signed char, from_eval)
         break;
      case DT_U_CHAR:
      case DT_BYTE:
         mac_cast(retval.value.ucnum, unsigned char, from_eval)
         break;
      case DT_SHORT:
         mac_cast(retval.value.shnum, short, from_eval)
         break;
      case DT_U_SHORT:
         mac_cast(retval.value.ushnum, unsigned short, from_eval)
         break;
      case DT_INT:
         mac_cast(retval.value.inum, int, from_eval)
         break;
      case DT_U_INT:
         mac_cast(retval.value.uinum, unsigned, from_eval)
         break;
      case DT_LONG:
         mac_cast(retval.value.lnum, long, from_eval)
         break;
      case DT_U_LONG:
         mac_cast(retval.value.ulnum, unsigned long, from_eval)
         break;
      case DT_LONG_LONG:
         mac_cast(retval.value.llnum, long long, from_eval)
         break;
      case DT_U_LONG_LONG:
         mac_cast(retval.value.ullnum, unsigned long long, from_eval)
         break;
      case DT_SIZE_T:
         mac_cast(retval.value.stnum, size_t, from_eval)
         break;
      case DT_FLOAT:
      case DT_DOUBLE:
         mac_cast(retval.value.num, double, from_eval)
         break;
      case DT_LONG_DOUBLE:
         mac_cast(retval.value.ldnum, long double, from_eval)
         break;
      default:
         yyerror("Error: Unexpected type in cast.");
         abort_called = 1;
         return retval;
      }
   }
   else if (to_eval->opval.ti.nderef > 0 && from_eval.ti.nderef == 0)
   {
      switch (from_eval.ti.dtype)
      {
      case DT_CHAR:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.cnum;
         break;
      case DT_S_CHAR:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.scnum;
         break;
      case DT_U_CHAR:
      case DT_BYTE:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.ucnum;
         break;
      case DT_SHORT:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.shnum;
         break;
      case DT_U_SHORT:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.ushnum;
         break;
      case DT_INT:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.inum;
         break;
      case DT_U_INT:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.uinum;
         break;
      case DT_LONG:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.lnum;
         break;
      case DT_U_LONG:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.ulnum;
         break;
      case DT_LONG_LONG:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.llnum;
         break;
      case DT_U_LONG_LONG:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.ullnum;
         break;
      case DT_SIZE_T:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.stnum;
         break;
      case DT_FLOAT:
      case DT_DOUBLE:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.num;
         break;
      case DT_LONG_DOUBLE:
         retval.value.genptr = (void*)(PVOIDCAST)from_eval.value.ldnum;
         break;
      case DT_STRING:
      case DT_MEMORY:
         retval.value.genptr = (void*)from_eval.value.str.tab;
         break;
      case DT_POINTER:
      case DT_PFILE:
      case DT_PFPOS_T:
      case DT_HLIB:
      case DT_PFUNC:
      case DT_MEMADR:
         retval.value.genptr = from_eval.value.genptr;
         break;
      default:
         yyerror("Invalid operand data type for cast operator.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }
      retval.ti.nderef = to_eval->opval.ti.nderef;
   }
   else if (to_eval->opval.ti.nderef == 0 && from_eval.ti.nderef > 0)
   {
      switch (to_eval->opval.ti.dtype)
      {
      case DT_CHAR:
         retval.value.cnum = (char)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_S_CHAR:
         retval.value.scnum = (signed char)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_U_CHAR:
      case DT_BYTE:
         retval.value.ucnum = (unsigned char)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_SHORT:
         retval.value.shnum = (short)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_U_SHORT:
         retval.value.ushnum = (unsigned short)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_INT:
         retval.value.inum = (int)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_U_INT:
         retval.value.uinum = (unsigned int)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_LONG:
         retval.value.lnum = (long)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_U_LONG:
         retval.value.ulnum = (unsigned long)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_LONG_LONG:
         retval.value.llnum = (long long)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_U_LONG_LONG:
         retval.value.ullnum = (unsigned long long)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_SIZE_T:
         retval.value.stnum = (size_t)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_FLOAT:
      case DT_DOUBLE:
         retval.value.num = (double)(PVOIDCAST)from_eval.value.genptr;
         break;
      case DT_LONG_DOUBLE:
         retval.value.ldnum = (long double)(PVOIDCAST)from_eval.value.genptr;
         break;
      default:
         break;
      }
   }
   else if (to_eval->opval.ti.nderef > 0 && from_eval.ti.nderef > 0)
   {
      retval.value.genptr = from_eval.value.genptr;
      retval.ti.nderef = to_eval->opval.ti.nderef;
   }

   retval.ti.dtype = to_eval->opval.ti.dtype;

   return retval;
}



data eval(node* to_eval)
{
   int isdotlist = 0, doloop;
   closure* src;
   data retval, from_eval;
   size_t i;

   memset(&retval, 0, sizeof(data));

   if (!to_eval)
   {
      abort_called = 1;
      return retval;
   }

   switch (to_eval->ntype)
   {
   case NT_NUM_CONST:
      retval.ti.dtype = DT_DOUBLE;
      retval.value.num = to_eval->opval.value;
      return retval;

   case NT_MATH_OPER:
      return eval_math_oper(to_eval);

   case NT_REL_OPER:
      return eval_rel_oper(to_eval);

   case NT_VARIABLE:
      /* Search for the variable value in the closure sets. */
      src = find_symbol(to_eval->opval.name);

      if (src)
      {
         copy_data(&retval, src->content);
      }
      else
      {
         fprintf(stderr, "Error: Symbol \"%s\" not found.\n", 
            to_eval->opval.name);

         abort_called = 1;
      }

      return retval;

   case NT_STRING:
      retval.ti.dtype = DT_STRING;
      retval.value.str.length = to_eval->opval.str.length;
      retval.value.str.tab = malloc(to_eval->opval.str.length);
      if (!retval.value.str.tab)
      {
         yyerror("Error: Lack of memory for a string in eval.");
         exit(1);
      }
      memset(retval.value.str.tab, 0, to_eval->opval.str.length);
      g_lst_add(retval.value.str.tab, PT_CHAR_TAB);
      memcpy(retval.value.str.tab, to_eval->opval.str.tab, to_eval->
         opval.str.length);
      return retval;

   case NT_FUNC_DEF:
   case NT_CODESEGMENT:
      retval.ti.dtype = DT_POINTER;
      retval.value.ptr = copy_tree(to_eval, NULL);
      g_lst_add(retval.value.ptr, PT_NODE);
      return retval;

   case NT_FUNC_CALL:
      return eval_func_call(to_eval);

   case NT_IF_STMT:
      if (to_bool(eval(to_eval->childset[0])))
      {
         return eval(to_eval->childset[1]);
      }
      else
      {
         return retval;
      }

   case NT_IFELSE_STMT:
      if (to_bool(eval(to_eval->childset[0])))
      {
         return eval(to_eval->childset[1]);
      }
      else
      {
         return eval(to_eval->childset[2]);
      }

   case NT_WHILE_STMT:
      from_eval = eval(to_eval->childset[0]);
      doloop = to_bool(from_eval);
      free_data(from_eval);
      while(doloop && !return_called && !abort_called)
      {
         retval = eval(to_eval->childset[1]);

         from_eval = eval(to_eval->childset[0]);
         doloop = to_bool(from_eval);
         free_data(from_eval);
      }
      return retval;

   case NT_FOR_STMT:
      create_context();
      from_eval = eval(to_eval->childset[0]);
      free_data(from_eval);
      from_eval = eval(to_eval->childset[1]);
      doloop = to_bool(from_eval);
      free_data(from_eval);
      while(doloop && !return_called && !abort_called)
      {
         retval = eval(to_eval->childset[3]);
         from_eval = eval(to_eval->childset[2]);
         free_data(from_eval);

         from_eval = eval(to_eval->childset[1]);
         doloop = to_bool(from_eval);
         free_data(from_eval);
      }
      free_context();
      return retval;

   case NT_LIST:
      if (to_eval->childset)
      {
         if (to_eval->childset[0]->ntype == NT_PARENT)
         {
            isdotlist = 1;
         }
      }
      if (!isdotlist)
      {
         /* Statement list. */
         /* New context creation. */
         create_context();

         /* Evalutation of each statement. */
         i = 0;
         while(i < to_eval->nb_childs && !return_called &&
            !abort_called)
         {
            retval = eval(to_eval->childset[i]);
            i++;
         }

         /* Context destruction. */
         free_context();
      }
      else
      {
         retval = eval_dotlist(to_eval, to_eval);
      }

      return retval;

   case NT_VARGENLIST:
      from_eval = eval(to_eval->childset[0]);
      if (from_eval.ti.dtype == DT_POINTER)
      {
         return eval_dotlist(from_eval.value.ptr, to_eval->childset[1]);
      }
      else
      {
         yyerror("Error: Trying to apply a genealogical dotted list to a non"
            " node pointer.");
         abort_called = 1;
         free_data(from_eval);
         return retval;
      }

   case NT_INCR_DECR:
      return eval_incr_decr(to_eval);

   case NT_ACCESSLIST:
      return eval_accesslist(to_eval);

   case NT_SUBSCRIPT:
      return eval_subscript(to_eval);

   case NT_STDIN:
      retval.ti.dtype = DT_PFILE;
      retval.value.pfile = stdin;
      return retval;

   case NT_STDOUT:
      retval.ti.dtype = DT_PFILE;
      retval.value.pfile = stdout;
      return retval;

   case NT_STDERR:
      retval.ti.dtype = DT_PFILE;
      retval.value.pfile = stderr;
      return retval;

   case NT_CAST:
      return eval_cast(to_eval);

   default:
      yyerror("Error: Unknown node type in eval.");
      exit(1);
   }
}
