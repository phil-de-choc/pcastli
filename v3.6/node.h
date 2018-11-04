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

#ifndef NODE_H
#define NODE_H

#ifdef _WIN32
#pragma warning(disable:4820 4711 4127 4710)
#endif

#if (defined(_WIN32) && !defined(_WIN64)) || (defined(__linux__) && defined(__i386__)) || defined(__GO32__)
   #define BITS32
#elif defined(_WIN64) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && defined(__amd64__))
   #define BITS64
#endif

/* #define _CRTDBG_MAP_ALLOC */
#ifdef _CRTDBG_MAP_ALLOC
   #include <stdlib.h>
   #include <crtdbg.h>
   #include <stdio.h>
#else
   #include <stdlib.h>
   #include <stdio.h>
#endif


int yylex(void);
void yyerror(char const*);


/* Types for syntax trees. */

typedef enum 
{
   NT_UNDEF = 0,
   NT_NUM_CONST,
   NT_MATH_OPER,
   NT_REL_OPER,
   NT_VARIABLE,
   NT_STRING,
   NT_FUNC_DEF,
   NT_FUNC_CALL,
   NT_IF_STMT,
   NT_IFELSE_STMT,
   NT_FOR_STMT,
   NT_WHILE_STMT,
   NT_LIST,
   NT_CODESEGMENT,
   NT_PARENT,
   NT_CHILDSET,
   NT_VARGENLIST,
   NT_INCR_DECR,
   NT_ACCESSLIST,
   NT_SUBSCRIPT,
   NT_STDIN,
   NT_STDOUT,
   NT_STDERR,
   NT_CAST,
   NT_REF,
   NT_DEREF
} node_type;

typedef struct
{
   char* tab;
   size_t length; /* length including NULL termination */
} string;

typedef enum
{
   DT_UNDEF = 0,
   DT_CHAR,
   DT_S_CHAR,
   DT_U_CHAR,
   DT_BYTE,
   DT_SHORT,
   DT_U_SHORT,
   DT_INT,
   DT_U_INT,
   DT_LONG,
   DT_U_LONG,
   DT_LONG_LONG,
   DT_U_LONG_LONG,
   DT_SIZE_T,
   DT_FLOAT,
   DT_DOUBLE,
   DT_LONG_DOUBLE,
   DT_STRING,
   DT_POINTER,
   DT_OBJECT,
   DT_ARRAY,
   DT_LIST,
   DT_PFILE,
   DT_PFPOS_T,
   DT_HLIB,
   DT_PFUNC,
   DT_MEMORY,
   DT_MEMADR,
   NB_TYPES
} data_type;

typedef struct
{
   data_type dtype;
   size_t nderef;
} type_info;

typedef union
{
   char math_oper;
   int rel_oper;
   int ppmm;
   double value;
   char* name;
   string str;
   type_info ti;
} node_data;

typedef struct node_s
{
   node_type ntype;
   struct node_s* parent;
   size_t nb_children;
   struct node_s** childset;
   node_data opval;
} node;


#define YYSTYPE node*


/* Internal lexer / parser state variables */

int eatlines;

#define CONTEXTSTACK_SIZE 50
int contextstack[CONTEXTSTACK_SIZE], * contextp;


/* Types for a syntax tree execution. */

struct clos_set;
struct array;
struct list;

typedef union
{
   char cnum;
   signed char scnum;
   unsigned char ucnum;
   unsigned char byte;
   short shnum;
   unsigned short ushnum;
   int inum;
   unsigned int uinum;
   long lnum;
   unsigned long ulnum;
   long long llnum;
   unsigned long long ullnum;
   size_t stnum;
   double num;
   long double ldnum;
   string str;
   node* ptr;
   void* genptr;
   struct clos_set* pObject;
   struct array* pArray;
   struct list* pList;
   FILE* pfile;
   fpos_t* pfpos;
   void* hlib;
   void (*pfunc)(void);
   string mem;
   unsigned char* memadr;
} data_value;

/* General data type. */
typedef struct
{
   data_value value; /* have to be first for indirection */
   type_info ti;
} data;

typedef struct
{
   char* name;
   struct clos_set* pContainer;
   data content;
} closure;

typedef struct clos_set
{
   size_t nb_clos;
   closure** clos_array;
   int is_func_cont;
} clos_set;

typedef struct array
{
   size_t length;
   data* dtable;
} array;

typedef struct link_s
{
   struct link_s* next;
   data content;
} listlink;

typedef struct list
{
   size_t length;
   listlink* start;
} list;

typedef enum
{
   IT_FILE,
   IT_STRING
} input_type;

typedef union
{
   FILE* inputfile;
   char* str;
} input_union;

extern size_t set_stack_size;
extern clos_set** clos_set_stack;
extern size_t set_stack_limit;


/* Other globals. */

#define SCANBUF_LEN 1000

extern int return_called;
extern int abort_called;
extern char* sPrompt;
extern input_type inputsrc;
extern input_union inputadr;
extern node* root;

typedef enum
{
   NEITHER,
   CLOSURE,
   DATA
} cont_type;

/* input file stack item */
typedef struct f_item
{
   struct f_item* prev;
   FILE* stream;
} file_stack_item;

extern file_stack_item* sourcefiles;

#endif
