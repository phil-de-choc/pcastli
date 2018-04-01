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

#ifndef UTIL_H
#define UTIL_H

#include "node.h"

void free_tree(node* npt);
node* copy_tree(node* to_copy, node* parent);
void create_context(int is_func_cont);
void free_context(void);
void add_word(node* listnode, char* word, int wlen);
void clear_symbol(const char* name);
int to_bool(data d);
void change_prompt(char* sNewPrompt);
void free_object(clos_set* pcs);
void free_object_incl_str(clos_set* pcs);
void free_array(array* pArray);
void free_array_incl_str(array* pArray);
void free_list(list* pList);
void free_list_incl_str(list* pList);
void free_string(char* tab);
void free_data(data dt);
void free_data_incl_str(data dt);
int bad_symbol(char* str);
void free_closure(closure clos);
void copy_clos(closure* dest, closure source);
void copy_data(data* pddest, data source);
closure* find_symbol(char* name);
cont_type resolve_accesslist(node* acclist, closure** ppclos, data* pdata);
data* resolve_subscript(node* to_eval);
void display(char* prefix, data to_display);
char* strcat4(const char* str1, const char* str2, const char* str3, 
   const char* str4);
data* resolve(node* to_resolve);
void fatal_error(char* msg);

#if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__))
int args_eval(node* to_eval, int nbFirstArgs, size_t* nchunks, void*** raw_args, 
   data** from_evals);
#elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
int args_eval(node* to_eval, int nbFirstArgs, size_t* nchunks, void*** raw_args, 
   data** from_evals, size_t** chunkfloat);
#endif

void exec(void);

#endif
