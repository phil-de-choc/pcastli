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
#pragma warning(disable:4996 4820 4738 4710)
#endif

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "eval.h"
#include "util.h"
#include "macros.h"
#include "gcollection.h"

char* sPrompt = NULL;
node* root = NULL;
clos_set** clos_set_stack;
size_t set_stack_size;
size_t set_stack_limit;

extern input_type inputsrc;
extern input_union inputadr;
extern int strict_access;



void free_tree(node* npt)
{
   size_t i;

   for (i = 0; i < npt->nb_children; i++)
   {
      free_tree(npt->childset[i]);
   }

   if (npt->ntype == NT_VARIABLE) free(npt->opval.name);
   else if (npt->ntype == NT_STRING) free(npt->opval.str.tab);
   if (npt->nb_children > 0) free(npt->childset);
   free(npt);
}



node* copy_tree(node* to_copy, node* parent)
{
   size_t i;
   node* npt = malloc(sizeof(node));

   if(!npt) fatal_error("Error: Lack of memory for npt in copy_tree.");
   memset(npt, 0, sizeof(node));
   npt->ntype = to_copy->ntype;
   if (to_copy->ntype == NT_VARIABLE)
   {
      /* Name copy. */
      npt->opval.name = malloc(strlen(to_copy->opval.name) + 1);
      if (!npt->opval.name)
         fatal_error("Error: Lack of memory for npt->opval.name in copy_tree.");
      strcpy(npt->opval.name, to_copy->opval.name);
   }
   else if (to_copy->ntype == NT_STRING)
   {
      npt->opval.str.length = to_copy->opval.str.length;
      npt->opval.str.tab = malloc(to_copy->opval.str.length);
      if (!npt->opval.str.tab)
         fatal_error("Error: Lack of memory for npt->opval.str.tab in copy_tree.");
      memcpy(npt->opval.str.tab, to_copy->opval.str.tab, to_copy->opval.str.
         length);
   }
   else npt->opval = to_copy->opval;

   npt->parent = parent;
   npt->nb_children = to_copy->nb_children;

   if (npt->nb_children > 0)
   {
      npt->childset = malloc(npt->nb_children * sizeof(node*));
      if (!npt->childset)
         fatal_error("Error: Lack of memory for npt->childset in copy_tree.");
      memset(npt->childset, 0, npt->nb_children * sizeof(node*));
   }
   else npt->childset = NULL;

   for(i = 0; i < npt->nb_children; i++)
   {
      npt->childset[i] = copy_tree(to_copy->childset[i], npt);
   }
   
   return npt;
}



void create_context(int is_func_cont)
{
   if (++set_stack_size > set_stack_limit)
   {
      set_stack_limit += 50;
      clos_set_stack = realloc(clos_set_stack, set_stack_limit * sizeof(clos_set*));
      if (!clos_set_stack) fatal_error("Error: Lack of memory for closures stack realloc.");
   }

   clos_set_stack[set_stack_size-1] = malloc(sizeof(clos_set));
   if (!clos_set_stack[set_stack_size-1])
      fatal_error("Error: Lack of memory in create_context.");
   memset(clos_set_stack[set_stack_size - 1], 0, sizeof(clos_set));

   clos_set_stack[set_stack_size-1]->is_func_cont = is_func_cont;
}



void free_context(void)
{
   set_stack_size--;
   free_object(clos_set_stack[set_stack_size]);
   clos_set_stack[set_stack_size] = NULL;
}



void add_word(node* listnode, char* word, int wlen)
{
   node* npt;

   listnode->childset = realloc(listnode->childset, (listnode->nb_children+1)*
      sizeof(node*));
   if (!listnode->childset)
      fatal_error("Error: Lack of memory for listnode->childset in add_word.");

   npt = malloc(sizeof(node));
   if (!npt) fatal_error("Error: Lack of memory for npt in add_word.");
   memset(npt, 0, sizeof(node));
   npt->ntype = NT_STRING;
   npt->parent = listnode;
   npt->nb_children = 0;
   npt->childset = NULL;

   npt->opval.str.tab = malloc(wlen + 1);
   npt->opval.str.length = wlen + 1;
   if (!npt->opval.str.tab)
      fatal_error("Error: Lack of memory for the string in add_word.");
   strncpy(npt->opval.str.tab, word, wlen);
   npt->opval.str.tab[wlen] = '\0';

   listnode->childset[listnode->nb_children] = npt;
   listnode->nb_children++;
}



void clear_symbol(const char* name)
{
   long long i;
   size_t j, k;

   /* Symbol name search in closures. */
   for(i = (long long)set_stack_size - 1; i >= 0; i--)
   {
      /* Search in a closure array. */
      for(j = 0; j < clos_set_stack[i]->nb_clos; j++)
      {
         if (!strcmp(name, clos_set_stack[i]->clos_array[j]->name))
         {
            /* Name found. */

            free(clos_set_stack[i]->clos_array[j]->name);
            free_data(clos_set_stack[i]->clos_array[j]->content);
            free(clos_set_stack[i]->clos_array[j]);

            for (k = j; k < clos_set_stack[i]->nb_clos - 1; k++)
            {
               clos_set_stack[i]->clos_array[k] = 
                  clos_set_stack[i]->clos_array[k+1];
            }

            clos_set_stack[i]->nb_clos--;

            clos_set_stack[i]->clos_array = 
               realloc(clos_set_stack[i]->clos_array, 
               clos_set_stack[i]->nb_clos * sizeof(closure*));

            return;
         }
      }
      
      if (clos_set_stack[i]->is_func_cont) i = 1;
   }
}



int to_bool(data d)
{
   if (d.ti.nderef > 0)
   {
      if (d.value.genptr == NULL) return 0;
      else return 1;
   }

   switch(d.ti.dtype)
   {
   case DT_CHAR:
      if (d.value.cnum == 0) return 0;
      else return 1;

   case DT_S_CHAR:
      if (d.value.scnum == 0) return 0;
      else return 1;

   case DT_U_CHAR:
   case DT_BYTE:
      if (d.value.ucnum == 0) return 0;
      else return 1;

   case DT_SHORT:
      if (d.value.shnum == 0) return 0;
      else return 1;

   case DT_U_SHORT:
      if (d.value.ushnum == 0) return 0;
      else return 1;

   case DT_INT:
      if (d.value.inum == 0) return 0;
      else return 1;

   case DT_U_INT:
      if (d.value.uinum == 0) return 0;
      else return 1;

   case DT_LONG:
      if (d.value.lnum == 0) return 0;
      else return 1;

   case DT_U_LONG:
      if (d.value.ulnum == 0) return 0;
      else return 1;

   case DT_LONG_LONG:
      if (d.value.llnum == 0) return 0;
      else return 1;

   case DT_U_LONG_LONG:
      if (d.value.ullnum == 0) return 0;
      else return 1;

   case DT_SIZE_T:
      if (d.value.stnum == 0) return 0;
      else return 1;

   case DT_FLOAT:
   case DT_DOUBLE:
      if (d.value.num == 0.0) return 0;
      else return 1;

   case DT_LONG_DOUBLE:
      if (d.value.ldnum == 0.0) return 0;
      else return 1;

   case DT_POINTER:
      if (d.value.ptr == NULL) return 0;
      else return 1;

   case DT_PFILE:
      if (d.value.pfile == NULL) return 0;
      else return 1;

   case DT_HLIB:
      if (d.value.hlib == NULL) return 0;
      else return 1;

   case DT_PFUNC:
      if (d.value.pfunc == NULL) return 0;
      else return 1;

   default:
      return 0;
   }
}



void change_prompt(char* sNewPrompt)
{
   free(sPrompt);

   sPrompt = (char*)malloc(strlen(sNewPrompt) + 1);

   if (!sPrompt)
   {
      yyerror("Error: Lack of memory in change_prompt.");
      return;
   }

   strcpy(sPrompt, sNewPrompt);
}



void free_object(clos_set* pcs)
{
   size_t i;

   for (i = 0; i < pcs->nb_clos; i++)
   {
      free(pcs->clos_array[i]->name);
      free_data(pcs->clos_array[i]->content);
      free(pcs->clos_array[i]);
   }

   if (pcs->nb_clos > 0) free(pcs->clos_array);

   g_lst_remove(pcs, PT_OBJECT);
   free(pcs);
}



void free_object_incl_str(clos_set* pcs)
{
   size_t i;

   for (i = 0; i < pcs->nb_clos; i++)
   {
      free(pcs->clos_array[i]->name);
      free_data_incl_str(pcs->clos_array[i]->content);
      free(pcs->clos_array[i]);
   }

   if (pcs->nb_clos > 0) free(pcs->clos_array);

   g_lst_remove(pcs, PT_OBJECT);
   free(pcs);
}



void free_array(array* pArray)
{
   size_t i;

   for (i = 0; i < pArray->length; i++) free_data(pArray->dtable[i]);
   free(pArray->dtable);
   g_lst_remove(pArray, PT_ARRAY);
   free(pArray);
}



void free_array_incl_str(array* pArray)
{
   size_t i;

   for (i = 0; i < pArray->length; i++) free_data_incl_str(pArray->dtable[i]);
   free(pArray->dtable);
   g_lst_remove(pArray, PT_ARRAY);
   free(pArray);
}



void free_list(list* pList)
{
   listlink* plink = pList->start, * next;
   size_t i;

   for (i = 0; i < pList->length; i++)
   {
      free_data(plink->content);
      next = plink->next;
      free(plink);
      plink = next;
   }
   g_lst_remove(pList, PT_LIST);
   free(pList);
}



void free_list_incl_str(list* pList)
{
   listlink* plink = pList->start, *next;
   size_t i;

   for (i = 0; i < pList->length; i++)
   {
      free_data_incl_str(plink->content);
      next = plink->next;
      free(plink);
      plink = next;
   }
   g_lst_remove(pList, PT_LIST);
   free(pList);
}



void free_string(char* tab)
{
   g_lst_remove(tab, PT_CHAR_TAB);
   free(tab);
}



void free_data(data dt)
{
   if (dt.ti.nderef > 0) return;

   if (dt.ti.dtype == DT_OBJECT) free_object(dt.value.pObject);
   else if (dt.ti.dtype == DT_ARRAY) free_array(dt.value.pArray);
   else if (dt.ti.dtype == DT_LIST) free_list(dt.value.pList);
}



void free_data_incl_str(data dt)
{
   if (dt.ti.nderef > 0) return;

   if (dt.ti.dtype == DT_OBJECT) free_object(dt.value.pObject);
   else if (dt.ti.dtype == DT_ARRAY) free_array(dt.value.pArray);
   else if (dt.ti.dtype == DT_LIST) free_list(dt.value.pList);
   else if (dt.ti.dtype == DT_STRING) free_string(dt.value.str.tab);
}



void free_closure(closure clos)
{
   free(clos.name);
   free_data(clos.content);
}



int bad_symbol(char* str)
{
   size_t i, len;

   if ((len = strlen(str)) <= 0) return 1;
   if (!isalpha(str[0]) && str[0] != '_') return 1;
   for (i = 0; i < len; i++)
   {
      if (!isalnum(str[i]) && str[i] != '_') return 1;
   }

   return 0;
}



void copy_data(data* pddest, data source)
{
   size_t i;
   listlink** destination, * psrclink;

   memset(pddest, 0, sizeof(data));

   if (source.ti.nderef > 0)
   {
      pddest->value = source.value;
      pddest->ti.nderef = source.ti.nderef;
   }
   else
   {
      switch(source.ti.dtype)
      {
      case DT_CHAR:
      case DT_S_CHAR:
      case DT_U_CHAR:
      case DT_BYTE:
      case DT_SHORT:
      case DT_U_SHORT:
      case DT_INT:
      case DT_U_INT:
      case DT_LONG:
      case DT_U_LONG:
      case DT_LONG_LONG:
      case DT_U_LONG_LONG:
      case DT_SIZE_T:
      case DT_FLOAT:
      case DT_DOUBLE:
      case DT_LONG_DOUBLE:
      case DT_POINTER:
      case DT_PFILE:
      case DT_PFPOS_T:
      case DT_HLIB:
      case DT_PFUNC:
      case DT_MEMORY:
      case DT_MEMADR:
      case DT_STRING:
         pddest->value = source.value;
         break;

      case DT_OBJECT:
         pddest->value.pObject = malloc(sizeof(clos_set));
         if (!pddest->value.pObject)
            fatal_error("Error: Lack of memory in copy_data for new object.");
         memset(pddest->value.pObject, 0, sizeof(clos_set));
         g_lst_add(pddest->value.pObject, PT_OBJECT);
         pddest->value.pObject->nb_clos = source.value.pObject->
            nb_clos;
         pddest->value.pObject->clos_array = malloc(source.value.
            pObject->nb_clos * sizeof(closure*));
         if (!pddest->value.pObject->clos_array)
            fatal_error("Error: Lack of memory in copy_data for member array.");
         memset(pddest->value.pObject->clos_array, 0, source.value.
            pObject->nb_clos * sizeof(closure*));
         for (i = 0; i < source.value.pObject->nb_clos; i++)
         {
            pddest->value.pObject->clos_array[i] = malloc(sizeof(closure));
            if (!pddest->value.pObject->clos_array[i])
               fatal_error("Error: Lack of memory in copy_data for member.");
            memset(pddest->value.pObject->clos_array[i], 0, sizeof(closure));
            copy_clos(pddest->value.pObject->clos_array[i], *source.value.pObject
               ->clos_array[i]);
            pddest->value.pObject->clos_array[i]->pContainer = pddest->value.
               pObject;
         }
         break;

      case DT_ARRAY:
         pddest->value.pArray = malloc(sizeof(array));
         if (!pddest->value.pArray)
            fatal_error("Error: Lack of memory in copy_data for new array.");
         memset(pddest->value.pArray, 0, sizeof(array));
         g_lst_add(pddest->value.pArray, PT_ARRAY);
         pddest->value.pArray->length = source.value.pArray->length;
         pddest->value.pArray->dtable = malloc(source.value.pArray->length * 
            sizeof(data));
         if (!pddest->value.pArray->dtable)
            fatal_error("Error: Lack of memory in copy_data for new array data.");
         memset(pddest->value.pArray->dtable, 0, source.value.pArray->length * 
            sizeof(data));
         for (i = 0; i < source.value.pArray->length; i++)
         {
            copy_data(&pddest->value.pArray->dtable[i], source.value.pArray->
               dtable[i]);
         }
         break;

      case DT_LIST:
         pddest->value.pList = malloc(sizeof(list));
         if (!pddest->value.pList)
            fatal_error("Error: Lack of memory in copy_data for new list.");
         memset(pddest->value.pList, 0, sizeof(list));
         g_lst_add(pddest->value.pList, PT_LIST);
         pddest->value.pList->length = source.value.pList->length;

         destination = &pddest->value.pList->start;
         psrclink = source.value.pList->start;
         for (i = 0; i < source.value.pList->length; i++)
         {
            *destination = malloc(sizeof(listlink));
            if (!*destination)
               fatal_error("Error: Lack of memory in copy_data for link.");
            memset(*destination, 0, sizeof(listlink));
            copy_data(&(*destination)->content, psrclink->content);

            destination = &(*destination)->next;
            psrclink = psrclink->next;
         }
         *destination = NULL;
         break;

      /*case DT_STRING:
         pddest->value.str.tab = malloc(source.value.str.length);
         if (!pddest->value.str.tab)
            fatal_error("Error: Lack of memory in copy_data for new string.");
         g_lst_add(pddest->value.str.tab, PT_CHAR_TAB);
         memcpy(pddest->value.str.tab, source.value.str.tab, source.value.str.length);
         pddest->value.str.length = source.value.str.length;
         break;*/

      default:
         break;
      }
   }

   pddest->ti.dtype = source.ti.dtype;

}



void copy_clos(closure* dest, closure source)
{
   if (source.name)
   {
      dest->name = malloc(strlen(source.name) + 1);
      if (!dest->name)
         fatal_error("Error: Lack of memory in copy_clos for name.");
      strcpy(dest->name, source.name);
   }

   copy_data(&dest->content, source.content);
}



closure* find_symbol(char* name)
{
   long long i, j;

   i = (long long)set_stack_size - 1;
   while(i >= 0)
   {
      /* Search in a closure array. */
      j = (long long)clos_set_stack[i]->nb_clos - 1;
      while(j >= 0)
      {
         if (!strcmp(name, clos_set_stack[i]->clos_array[j]->name))
         {
            return clos_set_stack[i]->clos_array[j];
         }
         j--;
      }
      if (clos_set_stack[i]->is_func_cont && strict_access) i = 0;
      else i--;
   }

   return NULL;
}



/* Resolves the access list in acclist and returns a closure pointer in *ppclos
 * or a data structure in *pdata and the return value tells what was returned. 
 */
cont_type resolve_accesslist(node* acclist, closure** ppclos, data* pdata)
{
   clos_set* p_obj_start;
   closure* pclos_from_find, * pclos_member = NULL;
   data from_eval, base;
   int found, ret_r_value = 0;
   size_t i, j;

   memset(&base, 0, sizeof(data));

   if (acclist->ntype != NT_ACCESSLIST)
      fatal_error("Error: Access list expected in resolve_accesslist.");
   if (acclist->nb_children < 3)
      fatal_error("Error: access list node does not have at least three childs.");

   if (acclist->childset[0]->ntype == NT_VARIABLE)
   {
      pclos_from_find = find_symbol(acclist->childset[0]->opval.name);
      if (!pclos_from_find)
      {
         fprintf(stderr, "Error: Object \"%s\" not found.\n", acclist->childset
            [0]->opval.name);
         abort_called = 1;
         return NEITHER;
      }
      if (pclos_from_find->content.ti.dtype != DT_OBJECT)
      {
         fprintf(stderr, "Error: Symbol \"%s\" is not an object.\n", acclist->
            childset[0]->opval.name);
         abort_called = 1;
         return NEITHER;
      }
      base = pclos_from_find->content;
   }
   else if (acclist->childset[0]->ntype == NT_FUNC_CALL)
   {
      from_eval = eval_func_call(acclist->childset[0]);
      if (from_eval.ti.dtype != DT_OBJECT)
      {
         yyerror("Error: Function did not return an object.");
         abort_called = 1;
         return NEITHER;
      }
      if (from_eval.ti.nderef == 0) ret_r_value = 1;
      base = from_eval;
   }
   else if (acclist->childset[0]->ntype == NT_SUBSCRIPT)
   {
      data* arr_data = resolve_subscript(acclist->childset[0]);
      if (arr_data)
      {
         if (arr_data->ti.dtype != DT_OBJECT)
         {
            yyerror("Error: Subscripting did not return an object.");
            abort_called = 1;
            return NEITHER;
         }
         base = *arr_data;
      }
      else
      {
         if (abort_called) return NEITHER;

         from_eval = eval(acclist->childset[0]);
         if (from_eval.ti.dtype != DT_OBJECT)
         {
            yyerror("Error: Value subscripted is not an object.");
            abort_called = 1;
            free_data(from_eval);
            return NEITHER;
         }
         base = from_eval;
      }
   }
   else if (acclist->childset[0]->ntype == NT_MATH_OPER && acclist->childset[0]->opval.math_oper == '*')
   {
      from_eval = eval(acclist->childset[0]->childset[0]);
      if (!from_eval.ti.nderef)
      {
         yyerror("Error: Insufficient level of indirection of operand.");
         abort_called = 1;
         free_data(from_eval);
         return NEITHER;
      }

      base = *(data*)from_eval.value.genptr;
   }
   else fatal_error("Error: Unexpected first node type in resolve_accesslist.");

   /* At i == 0 there is the object name or the function call. */
   for (i = 1; i < acclist->nb_children; i++)
   {
      found = 0;
      j = 0;

      if (acclist->childset[i]->ntype == NT_REF)
      {
         p_obj_start = base.value.pObject;
         i++;
      }
      else if (acclist->childset[i]->ntype == NT_DEREF)
      {
         if (base.ti.nderef != 1)
         {
            yyerror("Error: Invalid left operand of dereference operator.");
            abort_called = 1;
            return NEITHER;
         }
         p_obj_start = (clos_set*)*(void**)base.value.genptr;
         i++;
      }
      else
      {
         yyerror("Error: Invalid access list format.");
         abort_called = 1;
         return NEITHER;
      }

      if (i >= acclist->nb_children)
      {
         yyerror("Error: Invalid access list format.");
         abort_called = 1;
         return NEITHER;
      }

      while (j < p_obj_start->nb_clos && !found)
      {
         if (acclist->childset[i]->ntype != NT_VARIABLE)
         {
            yyerror("Error: Variable expected in access list.");
            abort_called = 1;
            return NEITHER;
         }
         if (!strcmp(acclist->childset[i]->opval.name, p_obj_start->
            clos_array[j]->name))
         {
            pclos_member = p_obj_start->clos_array[j];
            found = 1;
         }
         j++;
      }

      if (found)
      {
         if (i + 1 < acclist->nb_children)
         {
            if (pclos_member->content.ti.dtype == DT_OBJECT)
            {
               base = pclos_member->content;
            }
            else
            {
               fprintf(stderr, "Error: Member \"%s\" does not contain an object.\n",
                  pclos_member->name);
               abort_called = 1;
               return NEITHER;
            }
         }
      }
      else
      {
         fprintf(stderr, "Error: Member \"%s\" not found.\n", acclist->childset[i]->
            opval.name);
         abort_called = 1;
         return NEITHER;
      }
   }


   if (!ret_r_value)
   {
      *ppclos = pclos_member;
      return CLOSURE;
   }

   /* acclist->childset[0]->ntype == NT_FUNC_CALL */
   copy_data(pdata, pclos_member->content);
   return DATA;
}



data* resolve_subscript(node* to_eval)
{
   data index, * from_resolve = NULL;
   size_t lindex = 0, i = 0;
   array* parr = NULL;
   list* plst = NULL;
   listlink* plink = NULL;

   if (!to_eval->childset)
   {
      yyerror("Error: Subscript node does not have childs.");
      abort_called = 1;
      return NULL;
   }
   if (to_eval->nb_children != 2)
   {
      yyerror("Error: Subscript node does not have two in child count.");
      abort_called = 1;
      return NULL;
   }


   from_resolve = resolve(to_eval->childset[0]);

   if (!from_resolve)
   {
      yyerror("Error: Unresolved subscript left child.");
      abort_called = 1;
      return NULL;
   }

   if ((from_resolve->ti.dtype != DT_ARRAY && from_resolve->ti.dtype != DT_LIST &&
      from_resolve->ti.dtype != DT_STRING && from_resolve->ti.dtype != DT_MEMORY) || 
      from_resolve->ti.nderef)
   {
      yyerror("Error: Variable is not a subscriptable type.");
      abort_called = 1;
      return NULL;
   }

   if (from_resolve->ti.dtype == DT_ARRAY) parr = from_resolve->value.pArray;
   else if (from_resolve->ti.dtype == DT_LIST) plst = from_resolve->value.
      pList;
   else return NULL;


   /* Subscript number evaluation. */
   index = eval(to_eval->childset[1]);
   if (index.ti.dtype < DT_CHAR || index.ti.dtype > DT_LONG_DOUBLE ||
      index.ti.nderef > 0)
   {
      yyerror("Error: Number expected as subscript.");
      abort_called = 1;
      free_data(index);
      return NULL;
   }
   mac_cast(lindex, size_t, index)

   if (parr)
   {
      if (lindex >= parr->length)
      {
         yyerror("Error: Array index out of bound.");
         abort_called = 1;
         return NULL;
      }

      return &parr->dtable[lindex];
   }


   /* pclos->content.ti.dtype has to be DT_LIST */

   if (lindex >= plst->length)
   {
      yyerror("Error: Array index out of bound.");
      abort_called = 1;
      return NULL;
   }

   plink = plst->start;
   for (i = 1; i <= lindex; i++)
   {
      plink = plink->next;
   }

   return &plink->content;
}



void display(char* prefix, data to_display)
{
   size_t i;
   listlink* plink;

   if (to_display.ti.nderef > 0)
   {
      #if defined(BITS32)
      printf("%s\t0x%lx\n", prefix, (unsigned long)to_display.value.genptr);
      #elif defined(BITS64)
      printf("%s\t0x%llx\n", prefix, (unsigned long long)to_display.value.genptr);
      #endif
      return;
   }

   switch(to_display.ti.dtype)
   {
   case DT_CHAR:
      printf("%s\t%c\n", prefix, to_display.value.cnum);
      break;
   case DT_S_CHAR:
      printf("%s\t%c\n", prefix, to_display.value.scnum);
      break;
   case DT_U_CHAR:
      printf("%s\t%u\n", prefix, to_display.value.ucnum);
      break;
   case DT_BYTE:
      printf("%s\t0x%x\n", prefix, to_display.value.byte);
      break;
   case DT_SHORT:
      printf("%s\t%hd\n", prefix, to_display.value.shnum);
      break;
   case DT_U_SHORT:
      printf("%s\t%hu\n", prefix, to_display.value.ushnum);
      break;
   case DT_INT:
      printf("%s\t%d\n", prefix, to_display.value.inum);
      break;
   case DT_U_INT:
      printf("%s\t%u\n", prefix, to_display.value.uinum);
      break;
   case DT_LONG:
      printf("%s\t%ld\n", prefix, to_display.value.lnum);
      break;
   case DT_U_LONG:
      printf("%s\t%lu\n", prefix, to_display.value.ulnum);
      break;
   case DT_LONG_LONG:
      printf("%s\t%lld\n", prefix, to_display.value.llnum);
      break;
   case DT_U_LONG_LONG:
      printf("%s\t%llu\n", prefix, to_display.value.ullnum);
      break;
   case DT_SIZE_T:
      printf("%s\t%zu\n", prefix, to_display.value.stnum);
      break;
   case DT_FLOAT:
   case DT_DOUBLE:
      printf("%s\t%.10g\n", prefix, to_display.value.num);
      break;
    case DT_LONG_DOUBLE:
      printf("%s\t%Lg\n", prefix, to_display.value.ldnum);
      break;
   case DT_STRING:
      printf("%s\t\"%s\"\n", prefix, to_display.value.str.tab);
      break;
   case DT_POINTER:
   case DT_MEMADR:
   case DT_PFILE:
      #if defined(BITS32)
      printf("%s\t0x%lx\n", prefix, (unsigned long)to_display.value.ptr);
      #elif defined(BITS64)
      printf("%s\t0x%llx\n", prefix, (unsigned long long)to_display.value.ptr);
      #endif
      break;
   case DT_OBJECT:
      for (i = 0; i < to_display.value.pObject->nb_clos; i++)
      {
         char* newprefix = strcat4(prefix, "[", to_display.value.pObject->
            clos_array[i]->name, "]");
         
         display(newprefix, to_display.value.pObject->clos_array[i]->content);
         
         free(newprefix);
      }
      break;
   case DT_ARRAY:
      for (i = 0; i < to_display.value.pArray->length; i++)
      {
         char* newprefix, buff[100];

         sprintf(buff, "%zu", i);

         newprefix = strcat4(prefix, "[", buff, "]");
         
         display(newprefix, to_display.value.pArray->dtable[i]);
         
         free(newprefix);
      }
      break;
   case DT_LIST:
      plink = to_display.value.pList->start;
      for (i = 0; i < to_display.value.pList->length; i++)
      {
         char* newprefix, buff[100];

         sprintf(buff, "%zu", i);

         newprefix = strcat4(prefix, "[", buff, "]");

         display(newprefix, plink->content);
         
         free(newprefix);
         plink = plink->next;
      }
      break;
   case DT_HLIB:
      #if defined(BITS64)
      printf("%s\t0x%llx\n", prefix, (unsigned long long)to_display.value.hlib);
      #elif defined(BITS32)
      printf("%s\t0x%lx\n", prefix, (unsigned long)to_display.value.hlib);
      #endif
      break;
   case DT_PFUNC:
      #if defined(BITS64)
      printf("%s\t0x%llx\n", prefix, (unsigned long long)to_display.value.pfunc);
      #elif defined(BITS32)
      printf("%s\t0x%lx\n", prefix, (unsigned long)to_display.value.pfunc);
      #endif
      break;
   case DT_UNDEF:
      if (strlen(prefix) > 0) printf("%s\tundefined\n", prefix);
      break;
   default:
      break;
   }
}



char* strcat4(const char* str1, const char* str2, const char* str3, 
   const char* str4)
{
   char* str;
   size_t len1 = strlen(str1), len2 = strlen(str2), len3 = strlen(str3), len4 = 
      strlen(str4);

   str = malloc(len1 + len2 + len3 + len4 + 1);
   if (!str) fatal_error("Error: Lack of memory in strcat4.");

   strcpy(str, str1);
   strcpy(&str[len1], str2);
   strcpy(&str[len1 + len2], str3);
   strcpy(&str[len1 + len2 + len3], str4);

   return str;
}



data* resolve(node* to_resolve)
{
   data* from_res;

   if (to_resolve->ntype == NT_VARIABLE)
   {
      closure* pclos = find_symbol(to_resolve->opval.name);
      if (!pclos)
      {
         fprintf(stderr,"Error: Symbol \"%s\" not found.\n", to_resolve->opval.
            name);
         abort_called = 1;
         return NULL;
      }
      from_res = &pclos->content;
   }
   else if (to_resolve->ntype == NT_ACCESSLIST)
   {
      closure* pclos;
      data from_eval;
      cont_type ct = resolve_accesslist(to_resolve, &pclos, &from_eval);
      if (ct != CLOSURE)
      {
         if (ct == DATA) free_data(from_eval);
         abort_called = 1;
         return NULL;
      }
      from_res = &pclos->content;
   }
   else if (to_resolve->ntype == NT_SUBSCRIPT)
   {
      from_res = resolve_subscript(to_resolve);
      if (!from_res) abort_called = 1;
   }
   else if (to_resolve->ntype == NT_MATH_OPER && to_resolve->opval.math_oper == '*')
   {
      data from_eval = eval(to_resolve->childset[0]);
      if (!from_eval.ti.nderef)
      {
         yyerror("Error: Insufficient level of indirection of operand.");
         abort_called = 1;
         free_data(from_eval);
         return NULL;
      }

      from_res = (data*)from_eval.value.genptr;
   }
   else
   {
      yyerror("Error: Unexpected node symbol type to resolve.");
      abort_called = 1;
      return NULL;
   }

   return from_res;
}


#if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))

int args_eval(node* to_eval, int nbFirstArgs, size_t* nchunks, void*** raw_args, 
   data** from_evals)
{
   size_t j = 0, size = 0;
   data from_eval;
   int i = 0;

   memset(&from_eval, 0, sizeof(data));
   *nchunks = 0;

   *raw_args = malloc(to_eval->nb_children * sizeof(long double));
   if (!*raw_args)
      fatal_error("Error: Lack of memory in args_eval for arguments table.");
   memset(*raw_args, 0, to_eval->nb_children * sizeof(long double));

   *from_evals = malloc((to_eval->nb_children - nbFirstArgs) * sizeof(data));
   if (!*from_evals)
      fatal_error("Error: Lack of memory in args_eval for arguments table.");
   memset(*from_evals, 0, (to_eval->nb_children - nbFirstArgs) * sizeof(data));

   /* Calls to eval are outside because they alter values in the stack
      in the Linux build. */
   for (i = (int)to_eval->nb_children - 1; i >= nbFirstArgs; i--)
   {
      from_eval = eval(to_eval->childset[i]);
      if (from_eval.ti.dtype <= DT_UNDEF || from_eval.ti.dtype == DT_OBJECT || 
         from_eval.ti.dtype == DT_LIST || from_eval.ti.dtype == DT_ARRAY || 
         from_eval.ti.dtype >= NB_TYPES)
      {
         free(*raw_args);
         free(*from_evals);
         yyerror("Unexpected argument type.");
         abort_called = 1;
         return 1;
      }
      (*from_evals)[i - nbFirstArgs] = from_eval;

      if (from_eval.ti.nderef > 0)
      {
         size = sizeof(void*);
      }
      else
      {
         switch (from_eval.ti.dtype)
         {
         case DT_CHAR:
            size = sizeof(char);
            break;
         case DT_S_CHAR:
            size = sizeof(signed char);
            break;
         case DT_U_CHAR:
         case DT_BYTE:
            size = sizeof(unsigned char);
            break;
         case DT_SHORT:
            size = sizeof(short);
            break;
         case DT_U_SHORT:
            size = sizeof(unsigned short);
            break;
         case DT_INT:
            size = sizeof(int);
            break;
         case DT_U_INT:
            size = sizeof(unsigned int);
            break;
         case DT_LONG:
            size = sizeof(long);
            break;
         case DT_U_LONG:
            size = sizeof(unsigned long);
            break;
         case DT_LONG_LONG:
            size = sizeof(long long);
            break;
         case DT_U_LONG_LONG:
            size = sizeof(unsigned long long);
            break;
         case DT_SIZE_T:
            size = sizeof(size_t);
            break;
         case DT_FLOAT:
            size = sizeof(float);
            break;
         case DT_DOUBLE:
            size = sizeof(double);
            break;
         case DT_LONG_DOUBLE:
            size = sizeof(long double);
            break;
         case DT_STRING:
         case DT_POINTER:
         case DT_PFILE:
         case DT_PFPOS_T:
         case DT_HLIB:
         case DT_PFUNC:
         case DT_MEMORY:
         case DT_MEMADR:
            size = sizeof(void*);
            break;
         default:
            break;
         }
      }

      if (size <= sizeof(void*))
      {
         if (from_eval.ti.dtype == DT_FLOAT && from_eval.ti.nderef == 0)
         {
            float fnum = (float)from_eval.value.num;
            memcpy(&(*raw_args)[j], &fnum, size);
         }
         else
         {
            memcpy(&(*raw_args)[j], &from_eval.value, size);
         }
         j++;
      }
      else
      {
         int remainder = size % sizeof(void*);
         if (remainder > 0)
         {
            size -= remainder;
            memcpy(&(*raw_args)[j], &((void**)&from_eval.value)[size / sizeof(void*)], 
               remainder);
            j++;
         }

         while (size > sizeof(void*))
         {
            size_t hi_pos = size / sizeof(void*) - 1;
            memcpy(&(*raw_args)[j], &((void**)&from_eval.value)[hi_pos], sizeof(void*));
            size -= sizeof(void*);
            j++;
         }

         memcpy(&(*raw_args)[j], &from_eval.value, sizeof(void*));
         j++;
      }
   }

   *nchunks = j;

   return 0;
}

#elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))

int args_eval(node* to_eval, int nbFirstArgs, size_t* nchunks, void*** raw_args, 
   data** from_evals, size_t** chunkfloat)
{
   size_t j = 0, size = 0;
   data from_eval;
   int i = 0;

   memset(&from_eval, 0, sizeof(data));
   *nchunks = 0;

   *raw_args = malloc(to_eval->nb_children * sizeof(long double));
   if (!*raw_args)
      fatal_error("Error: Lack of memory in args_eval for arguments table.");
   memset(*raw_args, 0, to_eval->nb_children * sizeof(long double));

   *from_evals = malloc((to_eval->nb_children - nbFirstArgs) * sizeof(data));
   if (!*from_evals)
      fatal_error("Error: Lack of memory in args_eval for arguments table.");
   memset(*from_evals, 0, (to_eval->nb_children - nbFirstArgs) * sizeof(data));

   *chunkfloat = malloc(to_eval->nb_children * sizeof(long double));
   if (!*chunkfloat)
      fatal_error("Error: Lack of memory in args_eval for arguments table.");
   memset(*chunkfloat, 0, to_eval->nb_children * sizeof(long double));


   /* Calls to eval are outside because they alter values in the stack
      in the Linux build. */
   for (i = (int)to_eval->nb_children - 1; i >= nbFirstArgs; i--)
   {
      size_t argfloat = 0;

      from_eval = eval(to_eval->childset[i]);
      if (from_eval.ti.dtype <= DT_UNDEF || from_eval.ti.dtype == DT_OBJECT || 
         from_eval.ti.dtype == DT_LIST || from_eval.ti.dtype == DT_ARRAY || 
         from_eval.ti.dtype >= NB_TYPES)
      {
         free(*raw_args);
         free(*from_evals);
         yyerror("Unexpected argument type.");
         abort_called = 1;
         return 1;
      }
      (*from_evals)[i - nbFirstArgs] = from_eval;

      if (from_eval.ti.nderef > 0)
      {
         size = sizeof(void*);
      }
      else
      {
         switch (from_eval.ti.dtype)
         {
         case DT_CHAR:
            size = sizeof(char);
            break;
         case DT_S_CHAR:
            size = sizeof(signed char);
            break;
         case DT_U_CHAR:
         case DT_BYTE:
            size = sizeof(unsigned char);
            break;
         case DT_SHORT:
            size = sizeof(short);
            break;
         case DT_U_SHORT:
            size = sizeof(unsigned short);
            break;
         case DT_INT:
            size = sizeof(int);
            break;
         case DT_U_INT:
            size = sizeof(unsigned int);
            break;
         case DT_LONG:
            size = sizeof(long);
            break;
         case DT_U_LONG:
            size = sizeof(unsigned long);
            break;
         case DT_LONG_LONG:
            size = sizeof(long long);
            break;
         case DT_U_LONG_LONG:
            size = sizeof(unsigned long long);
            break;
         case DT_SIZE_T:
            size = sizeof(size_t);
            break;
         case DT_FLOAT:
            size = sizeof(float);
            argfloat = size;
            break;
         case DT_DOUBLE:
            size = sizeof(double);
            argfloat = size;
            break;
         case DT_LONG_DOUBLE:
            size = sizeof(long double);
            argfloat = size;
            break;
         case DT_STRING:
         case DT_POINTER:
         case DT_PFILE:
         case DT_PFPOS_T:
         case DT_HLIB:
         case DT_PFUNC:
         case DT_MEMORY:
         case DT_MEMADR:
            size = sizeof(void*);
            break;
         default:
            break;
         }
      }

      if (size <= sizeof(void*))
      {
         if (from_eval.ti.dtype == DT_FLOAT && from_eval.ti.nderef == 0)
         {
            float fnum = (float)from_eval.value.num;
            memcpy(&(*raw_args)[j], &fnum, size);
         }
         else
         {
            memcpy(&(*raw_args)[j], &from_eval.value, size);
         }
         (*chunkfloat)[j] = argfloat;
         j++;
      }
      else
      {
         int remainder = size % sizeof(void*);
         if (remainder > 0)
         {
            size -= remainder;
            memcpy(&(*raw_args)[j], &((void**)&from_eval.value)[size / sizeof(void*)], 
               remainder);
            (*chunkfloat)[j] = argfloat;
            j++;
         }

         while (size > sizeof(void*))
         {
            size_t hi_pos = size / sizeof(void*) - 1;
            memcpy(&(*raw_args)[j], &((void**)&from_eval.value)[hi_pos], sizeof(void*));
            size -= sizeof(void*);
            (*chunkfloat)[j] = argfloat;
            j++;
         }

         memcpy(&(*raw_args)[j], &from_eval.value, sizeof(void*));
         (*chunkfloat)[j] = argfloat;
         j++;
      }
   }

   *nchunks = j;

   return 0;
}

#endif


void exec(void)
{
   data from_eval;

   if (!root) return;

   from_eval = eval(root);

   g_lst_add(root, PT_NODE);

   if (inputsrc == IT_FILE)
   {
      if (inputadr.inputfile == stdin)
      {
         display("", from_eval);

         abort_called = 0;

         if (nb_executions == max_executions - 1)
         {
            gc();
            nb_executions = 0;
         }
         else
         {
            nb_executions++;
         }
      }
   }

   return_called = 0;
}



void fatal_error(char* msg)
{
   yyerror(msg);
   exit(1);
}
