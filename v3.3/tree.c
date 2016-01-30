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
#pragma warning(disable:4820 4255 4710)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "node.h"
#include "eval.h"
#include "util.h"
#include "gcollection.h"
#include "pcastli.tab.h"


node* xxbinary(node* top, node* left, node* right)
{
   top->childset = malloc(2 * sizeof(node*));
   if (!top->childset)
   {
      yyerror("Error: Lack of memory in xxbinary for top->childset.");
      exit(1);
   }
   top->nb_children = 2;
   top->childset[0] = left;
   if (left) left->parent = top;
   top->childset[1] = right;
   if (right) right->parent = top;

   top->parent = NULL;
   
   return top;
}



node* xxunary(node* top, node* operand)
{
   top->childset = malloc(sizeof(node*));
   if (!top->childset)
   {
      yyerror("Error: Lack of memory in xxunary for top->childset.");
      exit(1);
   }
   top->nb_children = 1;
   top->childset[0] = operand;
   if (operand) operand->parent = top;

   top->parent = NULL;
   
   return top;
}



node* xx2childs(node* child1, node* child2, node_type nt)
{
   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in xx2childs for npt.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));
   
   npt->ntype = nt;

   npt->childset = malloc(2 * sizeof(node*));
   if (!npt->childset)
   {
      yyerror("Error: Lack of memory in xx2childs for npt->childset.");
      exit(1);
   }
   npt->nb_children = 2;
   npt->childset[0] = child1;
   if (child1) child1->parent = npt;
   npt->childset[1] = child2;
   if (child2) child2->parent = npt;

   npt->parent = NULL;
   
   return npt;
}



node* xxfirstitem(node* item)
{
   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in xxfirstitem for npt.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));

   npt->ntype = NT_LIST;
   npt->opval.name = NULL;
   if (item) item->parent = npt;
   npt->nb_children = 1;

   npt->childset = malloc(sizeof(node*));
   if (!npt->childset)
   {
      fprintf(stderr, "Error: Lack of memory in xxfirstitem ");
      fprintf(stderr, "for npt->childset.\n");
      exit(1);
   }
   npt->childset[0] = item;

   npt->parent = NULL;
   
   return npt;
}



node* xxadditem(node* list, node* item)
{
   list->childset = realloc(list->childset, (list->nb_children + 1) * 
      sizeof(node*));
   if (!list->childset)
   {
      yyerror("Error: Lack of memory in xxadditem.");
      exit(1);
   }

   list->childset[list->nb_children] = item;
   if (item) item->parent = list;

   list->nb_children += 1;

   return list;
}



node* xxifelse(node* cond, node* stmt_true, node* stmt_false)
{
   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in xxifelse for npt.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));

   npt->ntype = NT_IFELSE_STMT;

   npt->childset = malloc(3 * sizeof(node*));
   if (!npt->childset)
   {
      yyerror("Error: Lack of memory in xxifelse for npt->childset.");
      exit(1);
   }
   npt->nb_children = 3;
   npt->childset[0] = cond;
   if (cond) cond->parent = npt;
   npt->childset[1] = stmt_true;
   if (stmt_true) stmt_true->parent = npt;
   npt->childset[2] = stmt_false;
   if (stmt_false) stmt_false->parent = npt;

   npt->parent = NULL;
   
   return npt;
}



node* xxfor(node* init, node* cond, node* incr, node* stmt)
{
   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in xxfor for npt.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));

   npt->ntype = NT_FOR_STMT;

   npt->childset = malloc(4 * sizeof(node*));
   if (!npt->childset)
   {
      yyerror("Error: Lack of memory in xxfor for npt->childset.");
      exit(1);
   }
   npt->nb_children = 4;
   npt->childset[0] = init;
   if (init) init->parent = npt;
   npt->childset[1] = cond;
   if (cond) cond->parent = npt;
   npt->childset[2] = incr;
   if (incr) incr->parent = npt;
   npt->childset[3] = stmt;
   if (stmt) stmt->parent = npt;

   npt->parent = NULL;
   
   return npt;
}



node* xxcodesegment(node* segment)
{
   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in xxcodesegment for npt.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));

   /* copy of sizeof(int) instead of sizeof(char) for the
      logical or relational operators */
   npt->opval.name = NULL;
   npt->ntype = NT_CODESEGMENT;

   npt->childset = malloc(sizeof(node*));
   if (!npt->childset)
   {
      yyerror("Error: Lack of memory in xxcodesegment for npt->childset.");
      exit(1);
   }
   npt->nb_children = 1;
   npt->childset[0] = segment;
   if (segment) segment->parent = npt;

   npt->parent = NULL;
   
   return npt;
}



node* xxfirstchildset(node* childset, node* index)
{
   /* List creation. */

   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in xxfirstchildset for npt.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));

   npt->ntype = NT_LIST;
   npt->opval.name = NULL;
   if (childset) childset->parent = npt;
   npt->nb_children = 1;

   npt->childset = malloc(sizeof(node*));
   if (!npt->childset)
   {
      fprintf(stderr, "Error: Lack of memory in xxfirstchildset ");
      fprintf(stderr, "for npt->childset.\n");
      exit(1);
   }
   npt->childset[0] = childset;

   npt->parent = NULL;


   /* Adding of index as a child of childset. */

   childset->childset = malloc(sizeof(node*));
   if(!childset->childset)
   {
      yyerror("Error: Lack of memory for childset in xxfirstchildset.");
      exit(1);
   }
   childset->nb_children = 1;
   childset->childset[0] = index;
   if (index) index->parent = childset;

   return npt;
}



node* xxaddchildset(node* list, node* childset, node* index)
{
   childset->childset = malloc(sizeof(node*));
   if(!childset->childset)
   {
      yyerror("Error: Lack of memory for childset in xxaddchildset.");
      exit(1);
   }
   childset->nb_children = 1;
   childset->childset[0] = index;
   if (index) index->parent = childset;

   list->childset = realloc(list->childset, (list->nb_children + 1) * 
      sizeof(node*));
   if (!list->childset)
   {
      yyerror("Error: Lack of memory for list in xxaddchildset.");
      exit(1);
   }

   list->childset[list->nb_children] = childset;
   if (childset) childset->parent = list;

   list->nb_children += 1;

   return list;
}



node* xxemptylist(void)
{
   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in xxemptylist for npt.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));

   npt->ntype = NT_LIST;
   npt->opval.name = NULL;
   npt->nb_children = 0;
   npt->childset = NULL;
   npt->parent = NULL;

   return npt;
}



node* xxone(void)
{
   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory in xxone for npt.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));

   npt->ntype = NT_NUM_CONST;
   npt->opval.value = 1.0;
   npt->nb_children = 0;
   npt->childset = NULL;
   npt->parent = NULL;

   return npt;
}



node* xxppmm(node* var, int op)
{
   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory for new node in xxppmm.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));

   npt->ntype = NT_INCR_DECR;
   npt->opval.ppmm = op;
   npt->parent = NULL;
   npt->nb_children = 1;

   npt->childset = malloc(sizeof(node*));
   if (!npt->childset)
   {
      yyerror("Error: Lack of memory for child in xxppmm.");
      exit(1);
   }

   npt->childset[0] = var;
   if (var) var->parent = npt;

   return npt;
}



node* xxaddmember(node* list, node* member, node_type nt)
{
   node* ret_lst = NULL;

   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory for new node in xxaddmember.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));
   npt->ntype = nt;

   if (list->ntype == NT_ACCESSLIST)
   {
      xxadditem(list, npt);
      ret_lst = xxadditem(list, member);
   }
   else
   {
      ret_lst = xxemptylist();
      ret_lst->ntype = NT_ACCESSLIST;
      xxadditem(ret_lst, list);
      xxadditem(ret_lst, npt);
      xxadditem(ret_lst, member);
   }

   return ret_lst;
}



node* xxtype(data_type dt)
{
   node* npt = malloc(sizeof(node));
   if (!npt)
   {
      yyerror("Error: Lack of memory for new node in xxtype.");
      exit(1);
   }
   memset(npt, 0, sizeof(node));

   npt->ntype = NT_CAST;
   npt->opval.ti.dtype = dt;
   npt->nb_children = 0;
   npt->childset = NULL;
   npt->parent = NULL;

   return npt;
}



node* xxcast(node* type, node* starlist, node* operand)
{
   type->opval.ti.nderef = starlist->nb_children;
   free_tree(starlist);

   return xxunary(type, operand);
}



void xxsetroot(node* to_eval)
{
   root = to_eval;
}
