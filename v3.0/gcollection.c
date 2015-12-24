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
#pragma warning(disable:4820)
#endif

#include <string.h>

#include "gcollection.h"
#include "util.h"
#include "macros.h"

g_item* g_lst = NULL;
g_item* last = NULL;
int nb_executions = 0;
int max_executions = 5;



/* Recursive object search for a pointer. */
int pt_in_object(const clos_set* pobject, alloc_ptr aptr, pointer_type pt)
{
   size_t i = 0;

   if (pt == PT_OBJECT)
   {
      if (pobject == aptr.pObject) return 1;
   }

   for (i = 0; i < pobject->nb_clos; i++)
   {
      if (pobject->clos_array[i]->content.ti.nderef == 0)
      {
         data* pdat = &pobject->clos_array[i]->content;
         mac_gc_switch
      }
   }

   return 0;
}



int pt_in_array(const array* parr, alloc_ptr aptr, pointer_type pt)
{
   size_t i = 0;

   if (pt == PT_ARRAY)
   {
      if (parr == aptr.pArray) return 1;
   }

   for (i = 0; i < parr->length; i++)
   {
      if (parr->dtable[i].ti.nderef == 0)
      {
         data* pdat = &parr->dtable[i];
         mac_gc_switch
      }
   }

   return 0;
}



int pt_in_list(const list* plst, alloc_ptr aptr, pointer_type pt)
{
   size_t i = 0;
   listlink * plink = plst->start;

   if (pt == PT_LIST)
   {
      if (plst == aptr.pList) return 1;
   }

   for (i = 0; i < plst->length; i++)
   {
      if (plink->content.ti.nderef == 0)
      {
         data* pdat = &plink->content;
         mac_gc_switch
      }
      plink = plink->next;
   }

   return 0;
}



/* Search a pointer in closures. */
int search(alloc_ptr aptr, pointer_type pt)
{
   size_t j = 0;

   /* No "i" because gc is only called between top statements. */
   for (j = 0; j < clos_set_stack[0]->nb_clos; j++)
   {
      if (clos_set_stack[0]->clos_array[j]->content.ti.nderef == 0)
      {
         data* pdat = &clos_set_stack[0]->clos_array[j]->content;
         mac_gc_switch
      }
   }

   return 0;
}



/* Garbage collect. */
void gc(void)
{
   g_item* i_pt = g_lst;
   int found = 0;

   while (i_pt)
   {
      s_item* stack_top = NULL, * last_top = NULL, * new_top = NULL;

      found = 0;

      if (i_pt->pt == PT_NODE)
      {
         /* Pushing the first node on the stack. */
         stack_top = malloc(sizeof(s_item));
         if (!stack_top)
         {
            yyerror("Error: Lack of memory in gc for stack_top.");
            exit(1);
         }
         memset(stack_top, 0, sizeof(s_item));
         stack_top->aptr = i_pt->aptr;

         while (stack_top && !found)
         {
            size_t i_child = 0;
            alloc_ptr searched_aptr;

            /* Popping a node. */
            searched_aptr = stack_top->aptr;

            last_top = stack_top;
            stack_top = stack_top->prev;
            free(last_top);

            /* Searching the popped value in closures. */
            found = search(searched_aptr, PT_NODE);

            /* Pushing all the childs of the popped node. */
            for (i_child = 0; i_child < searched_aptr.ptr->nb_childs; i_child++)
            {
               new_top = malloc(sizeof(s_item));
               if (!new_top)
               {
                  yyerror("Error: Lack of memory in gc for new_top.");
                  exit(1);
               }
               memset(new_top, 0, sizeof(s_item));
               new_top->prev = stack_top;
               new_top->aptr.ptr = searched_aptr.ptr->childset[i_child];

               stack_top = new_top;
            }
         }
      }
      else
      {
         found = search(i_pt->aptr, i_pt->pt);
      }

      if (!found)
      {
         g_item* i_pt_next = i_pt->next;

         if (i_pt->prev) i_pt->prev->next = i_pt->next;
         else g_lst = i_pt->next;
         if (i_pt->next) i_pt->next->prev = i_pt->prev;
         else last = i_pt->prev;

         switch(i_pt->pt)
         {
         case PT_NODE:
            free_tree(i_pt->aptr.ptr);
            break;
         case PT_CHAR_TAB:
            free(i_pt->aptr.str_tab);
            break;
         case PT_OBJECT:
            g_free_object(i_pt->aptr.pObject);
            break;
         case PT_ARRAY:
            g_free_array(i_pt->aptr.pArray);
            break;
         case PT_LIST:
            g_free_list(i_pt->aptr.pList);
            break;
         case PT_PFPOS_T:
            free(i_pt->aptr.pfpos);
            break;
         default:
            break;
         }

         free(i_pt);
         i_pt = i_pt_next;
      }
      else
      {
         /* If the stack is not empty, emptying it. */
         while (stack_top)
         {
            new_top = stack_top->prev;
            free(stack_top);
            stack_top = new_top;
         }
         i_pt = i_pt->next;
      }
   }
}



/* Adding an item to the garbage list. */
void g_lst_add(void* vptr, pointer_type pt)
{
   alloc_ptr aptr;

   memset(&aptr, 0, sizeof(alloc_ptr));

   if (!vptr) return;

   aptr.vptr = vptr;

   if (!g_lst)
   {
      g_lst = malloc(sizeof(g_item));
      if (!g_lst)
      {
         yyerror("Error: Lack of memory in g_lst_add for g_lst.");
         exit(1);
      }
      memset(g_lst, 0, sizeof(g_item));

      g_lst->aptr = aptr;
      g_lst->pt = pt;

      last = g_lst;
   }
   else
   {
      last->next = malloc(sizeof(g_item));
      if (!last->next)
      {
         yyerror("Error: Lack of memory in g_lst_add for last->next.");
         exit(1);
      }
      memset(last->next, 0, sizeof(g_item));

      last->next->prev = last;
      last->next->aptr = aptr;
      last->next->pt = pt;

      last = last->next;
   }
}



void g_lst_remove(void* vptr, pointer_type pt)
{
   g_item* i_pt = g_lst;
   while (i_pt)
   {
      if (i_pt->aptr.vptr == vptr && i_pt->pt == pt)
      {
         if (i_pt->prev) i_pt->prev->next = i_pt->next;
         else g_lst = i_pt->next;
         if (i_pt->next) i_pt->next->prev = i_pt->prev;
         else last = i_pt->prev;
         free(i_pt);
         break;
      }
      i_pt = i_pt->next;
   }
}


void g_free_object(clos_set* pcs)
{
   size_t i = 0;

   for (i = 0; i < pcs->nb_clos; i++)
   {
      free(pcs->clos_array[i]->name);
      free(pcs->clos_array[i]);
   }

   if (pcs->nb_clos > 0) free(pcs->clos_array);

   free(pcs);
}



void g_free_array(array* pArray)
{
   free(pArray->dtable);
   free(pArray);
}



void g_free_list(list* pList)
{
   listlink* plink = pList->start, * next = NULL;
   size_t i = 0;;

   for (i = 0; i < pList->length; i++)
   {
      next = plink->next;
      free(plink);
      plink = next;
   }
   free(pList);
}
