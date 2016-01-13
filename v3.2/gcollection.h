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

#ifndef GCOLLECTION_H
#define GCOLLECTION_H

#include <stdlib.h>

#include "node.h"


typedef enum
{
   PT_NODE,
   PT_CHAR_TAB,
   PT_OBJECT,
   PT_ARRAY,
   PT_LIST,
   PT_PFPOS_T
} pointer_type;


typedef union
{
   void* vptr;
   node* ptr;
   char* str_tab;
   struct clos_set* pObject;
   struct array* pArray;
   struct list* pList;
   fpos_t* pfpos;
} alloc_ptr;

typedef struct g_item_s
{
   struct g_item_s* prev;
   struct g_item_s* next;
   alloc_ptr aptr;
   pointer_type pt;
} g_item;

extern g_item* g_lst;
extern int nb_executions;
extern int max_executions;
extern g_item* last;

#define MAX_MAX_EXECUTIONS 1000

void g_lst_add(void* vptr, pointer_type pt);
void g_lst_remove(void* vptr, pointer_type pt);
int pt_in_object(const clos_set* pobject, alloc_ptr aptr, pointer_type pt);
int pt_in_array(const array* parr, alloc_ptr aptr, pointer_type pt);
int pt_in_list(const list* plst, alloc_ptr aptr, pointer_type pt);
void g_free_object(clos_set* pcs);
void g_free_array(array* pArray);
void g_free_list(list* pList);

typedef struct s_item_s
{
   struct s_item_s* prev;
   alloc_ptr aptr;
} s_item;


void gc(void);

#endif
