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
#pragma warning(disable:4996)
#endif

#include <string.h>

#include "eval.h"
#include "util.h"
#include "pcastli.tab.h"

typedef enum
{
   IC_SAME = 0,
   IC_MORE,
   IC_LESS
} indentchange;

typedef struct code_s
{
   struct code_s* next;
   char* str;
   indentchange indent;
   int linestart;
   int preelse;
} code_atom;

typedef struct queue_s
{
   struct queue_s* next;
   node* nodept;
   /* code_atom at the left of point of insertion */
   code_atom* atleft;
} queue_atom;

typedef enum
{
   ASSOC_UNDEF = 0,
   ASSOC_LEFT,
   ASSOC_RIGHT
} assoc;



int get_op_prio(node* nodept)
{
   if (!nodept) return 0;

   switch (nodept->ntype)
   {
   case NT_MATH_OPER:
      if (nodept->opval.math_oper == '=') return 1;
      else if (nodept->opval.math_oper == '+') return 6;
      else if (nodept->opval.math_oper == '-')
      {
         if (nodept->nb_childs == 2) return 6;
         else if (nodept->nb_childs == 1) return 10;
         else fatal_error("Error: Unforeseen \'-\' operator childs number in get_op_prio function.");
      }
      else if (nodept->opval.math_oper == '*')
      {
         if (nodept->nb_childs == 2) return 7;
         else if (nodept->nb_childs == 1) return 10;
         else fatal_error("Error: Unforeseen \'*\' operator childs number in get_op_prio function.");
      }
      else if (nodept->opval.math_oper == '/') return 7;
      else if (nodept->opval.math_oper == '^') return 8;
      else if (nodept->opval.math_oper == '&') return 10;
      break;
   case NT_REL_OPER:
      if (nodept->opval.rel_oper == OR) return 2;
      else if (nodept->opval.rel_oper == AND) return 3;
      else if (nodept->opval.rel_oper == EQ || nodept->opval.rel_oper == NE) return 4;
      else if (nodept->opval.rel_oper == GT || nodept->opval.rel_oper == GE ||
         nodept->opval.rel_oper == LT || nodept->opval.rel_oper == LE) return 5;
      break;
   case NT_INCR_DECR:
      if (nodept->opval.ppmm == PREPLUSPLUS || nodept->opval.ppmm == PREMINUSMINUS) return 10;
      else if (nodept->opval.ppmm == POSTPLUSPLUS || nodept->opval.ppmm == POSTMINUSMINUS) return 11;
      break;
   case NT_CAST:
      return 9;
      break;
   case NT_REF:
   case NT_DEREF:
   case NT_ACCESSLIST:
   case NT_SUBSCRIPT:
   case NT_FUNC_CALL:
      return 12;
   default:
      return 0;
   }

   return 0;
}



assoc get_op_assoc(node* nodept)
{
   if (!nodept) return ASSOC_UNDEF;

   switch (nodept->ntype)
   {
   case NT_MATH_OPER:
      if (nodept->opval.math_oper == '=') return ASSOC_RIGHT;
      else if (nodept->opval.math_oper == '+') return ASSOC_LEFT;
      else if (nodept->opval.math_oper == '-')
      {
         if (nodept->nb_childs == 2) return ASSOC_LEFT;
         else if (nodept->nb_childs == 1) return ASSOC_RIGHT;
         else fatal_error("Error: Unforeseen \'-\' operator childs number in get_op_prio function.");
      }
      else if (nodept->opval.math_oper == '*')
      {
         if (nodept->nb_childs == 2) return ASSOC_LEFT;
         else if (nodept->nb_childs == 1) return ASSOC_RIGHT;
         else fatal_error("Error: Unforeseen \'*\' operator childs number in get_op_prio function.");
      }
      else if (nodept->opval.math_oper == '/') return ASSOC_LEFT;
      else if (nodept->opval.math_oper == '^') return ASSOC_RIGHT;
      break;
   case NT_INCR_DECR:
      if (nodept->opval.ppmm == PREPLUSPLUS || nodept->opval.ppmm == PREMINUSMINUS) return ASSOC_RIGHT;
      else if (nodept->opval.ppmm == POSTPLUSPLUS || nodept->opval.ppmm == POSTMINUSMINUS) return ASSOC_LEFT;
      break;
   case NT_CAST:
      return ASSOC_RIGHT;
      break;
   case NT_REL_OPER:
   case NT_REF:
   case NT_DEREF:
   case NT_ACCESSLIST:
   case NT_SUBSCRIPT:
   case NT_FUNC_CALL:
      return ASSOC_LEFT;
   default:
      return ASSOC_UNDEF;
   }

   return ASSOC_UNDEF;
}

code_atom* enqueue_stmt(
   code_atom* atleft,
   node* currnode,
   code_atom** liststart,
   size_t* listlen,
   queue_atom** queuestart,
   queue_atom** queueend,
   size_t* queuelen)
{
   code_atom * placeholder = NULL, *eolatom = NULL;
   queue_atom * qa = NULL;

   placeholder = malloc(sizeof(code_atom));
   eolatom = malloc(sizeof(code_atom));
   if (!placeholder || !eolatom) fatal_error("Error: Lack of memory in enqueue_stmt function.");
   memset(placeholder, 0, sizeof(code_atom));
   memset(eolatom, 0, sizeof(code_atom));

   eolatom->str = malloc(2);
   if (!eolatom->str) fatal_error("Error: Lack of memory in enqueue_stmt function.");
   eolatom->str[0] = '\n';
   eolatom->str[1] = '\0';

   placeholder->linestart = 1;

   if (atleft)
   {
      eolatom->next = atleft->next;
      atleft->next = placeholder;
   }
   else
   {
      eolatom->next = *liststart;
      *liststart = placeholder;
   }
   placeholder->next = eolatom;

   (*listlen) += 2;

   qa = malloc(sizeof(queue_atom));
   if (!qa) fatal_error("Error: Lack of memory in enqueue_stmt function.");
   memset(qa, 0, sizeof(queue_atom));

   qa->next = NULL;
   qa->nodept = currnode;
   qa->atleft = placeholder;
   if (!*queuelen)
   {
      *queuestart = qa;
   }
   else
   {
      (*queueend)->next = qa;
   }
   (*queueend) = qa;
   (*queuelen)++;

   return eolatom;
}

data eval_treetocode(node* to_eval)
{
   int err = 0, ilastmore = -2;
   data retval, from_eval;
   node* argroot = NULL;
   string tab;
   code_atom* liststart = NULL, * curr = NULL;
   queue_atom* queuestart = NULL, * queueend = NULL;
   size_t listlen = 0, queuelen = 0, i, j, codelen = 0, tabcount = 0, pos;

   memset(&retval, 0, sizeof(data));


   /* Arguments verification. */

   if (to_eval)
   {
      if (to_eval->nb_childs != 2) err = 1;
   }
   else err = 1;

   if (err)
   {
      yyerror("Error: Wrong number of arguments in treetocode.");
      yyerror("       This function has two parameters.");
      return retval;
   }

   from_eval = eval(to_eval->childset[0]);
   if (from_eval.ti.dtype != DT_POINTER || from_eval.ti.nderef != 0 || !from_eval.value.ptr)
   {
      yyerror("Error: First argument of treetocode is not a node pointer.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }
   argroot = from_eval.value.ptr;

   from_eval = eval(to_eval->childset[1]);
   if (from_eval.ti.dtype != DT_STRING || from_eval.ti.nderef != 0)
   {
      yyerror("Error: String expected as second argument of treetocode.");
      abort_called = 1;
      free_data(from_eval);
      return retval;
   }
   tab = from_eval.value.str;


   queuestart = malloc(sizeof(queue_atom));
   if (!queuestart) fatal_error("Error: Lack of memory in treetocode function.");
   queuestart->next = NULL;
   queuestart->nodept = argroot;
   queuestart->atleft = liststart;
   queuelen++;
   queueend = queuestart;

   while (queuelen > 0)
   {
      node* currnode = queuestart->nodept;
      code_atom* atleft = queuestart->atleft, * leftparenth = NULL, * rightparenth = NULL,
         * numconst = NULL, * statement = NULL, * leftbrace = NULL, * rightbrace = NULL,
         * placeholder = NULL, * id = NULL, * comma = NULL, * left = NULL, * right = NULL, 
         * stratom = NULL, * semicol1 = NULL, * semicol2 = NULL, * previous = NULL, 
         * leftquote = NULL, * rightquote = NULL, * accessor = NULL, * leftbracket = NULL, 
         * rightbracket = NULL, * opcode = NULL, * type = NULL, * starlist = NULL;
      int caseone = (queueend == queuestart), bgene = 0;
      queue_atom* qa, * tmp_qa;
      char numstr[128];
      int haslist = 0;
      size_t numstrlen = 0, idstrlen = 0;
      code_atom* tablefts[3] = {NULL, NULL, NULL};

      tmp_qa = queuestart;
      queuestart = queuestart->next;
      free(tmp_qa);
      queuelen--;

      switch (currnode->ntype)
      {
      case NT_NUM_CONST:
         numconst = malloc(sizeof(code_atom));
         if (!numconst) fatal_error("Error: Lack of memory in treetocode function.");
         memset(numconst, 0, sizeof(code_atom));

         if (atleft)
         {
            numconst->next = atleft->next;
            atleft->next = numconst;
         }
         else
         {
            numconst->next = liststart;
            liststart = numconst;
         }
         listlen++;

         sprintf(numstr, "%.10g", currnode->opval.value);
         numstrlen = strlen(numstr);
         numconst->str = malloc(numstrlen + 1);
         if (!numconst->str) fatal_error("Error: Lack of memory in treetocode function.");
         strncpy(numconst->str, numstr, numstrlen + 1);
         break;

      case NT_MATH_OPER:
         if (currnode->nb_childs == 1)
         {
            opcode = malloc(sizeof(code_atom));
            if (!opcode) fatal_error("Error: Lack of memory in treetocode function.");
            memset(opcode, 0, sizeof(code_atom));

            opcode->str = malloc(3);
            if (!opcode) fatal_error("Error: Lack of memory in treetocode function.");

            if (currnode->childset[0]->ntype == NT_MATH_OPER && currnode->childset[0]->nb_childs == 1 &&
               currnode->childset[0]->opval.math_oper == '-')
            {
               /* For not fusing two NEGs in a -- */
               opcode->str[0] = '-';
               opcode->str[1] = ' ';
               opcode->str[2] = '\0';
            }
            else
            {
               opcode->str[0] = currnode->opval.math_oper;
               opcode->str[1] = '\0';
            }

            if (atleft)
            {
               opcode->next = atleft->next;
               atleft->next = opcode;
            }
            else
            {
               opcode->next = liststart;
               liststart = opcode;
            }
            listlen++;

            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));
            qa->next = NULL;
            qa->nodept = currnode->childset[0];
            qa->atleft = opcode;
            if (caseone)
            {
               queuestart = qa;
            }
            else
            {
               queueend->next = qa;
            }
            queueend = qa;
            queuelen++;
         }
         else if (currnode->nb_childs == 2)
         {
            int bpar = 0; /* Boolean for parentheses insertion */
            int bmathrel = 0;
            int priocurr = 0, prioparent = 0;

            priocurr = get_op_prio(currnode);
            prioparent = get_op_prio(currnode->parent);
            if (currnode->parent)
            {
               bmathrel = currnode->parent->ntype == NT_MATH_OPER || currnode->parent->ntype == NT_REL_OPER;
            }
            if (prioparent > priocurr && bmathrel) bpar = 1;
            else if (prioparent == priocurr && priocurr != 0)
            {
               assoc currassoc = get_op_assoc(currnode);
               if (currnode->parent->childset[0] == currnode && currassoc == ASSOC_RIGHT) bpar = 1;
               else if (currnode->parent->childset[1] == currnode && currassoc == ASSOC_LEFT) bpar = 1;
            }

            left = atleft;

            opcode = malloc(sizeof(code_atom));
            if (!opcode) fatal_error("Error: Lack of memory in treetocode function.");
            memset(opcode, 0, sizeof(code_atom));
            opcode->str = malloc(4);
            if (!opcode->str) fatal_error("Error: Lack of memory in treetocode function.");
            opcode->str[0] = ' ';
            opcode->str[1] = currnode->opval.math_oper;
            opcode->str[2] = ' ';
            opcode->str[3] = '\0';

            right = opcode;

            if (bpar)
            {
               leftparenth = malloc(sizeof(code_atom));
               rightparenth = malloc(sizeof(code_atom));
               if (!leftparenth || !rightparenth) 
                  fatal_error("Error: Lack of memory in treetocode function.");
               memset(leftparenth, 0, sizeof(code_atom));
               memset(rightparenth, 0, sizeof(code_atom));

               leftparenth->str = malloc(2);
               rightparenth->str = malloc(2);
               if (!leftparenth->str || !rightparenth->str) 
                  fatal_error("Error: Lack of memory in treetocode function.");

               leftparenth->str[0] = '(';
               leftparenth->str[1] = '\0';
               rightparenth->str[0] = ')';
               rightparenth->str[1] = '\0';
            }

            if (atleft)
            {
               if (bpar)
               {
                  leftparenth->next = opcode;
                  opcode->next = rightparenth;
                  rightparenth->next = atleft->next;
                  atleft->next = leftparenth;
                  left = leftparenth;
                  listlen += 2;
               }
               else
               {
                  opcode->next = atleft->next;
                  atleft->next = opcode;
               }
            }
            else
            {
               if (bpar)
               {
                  leftparenth->next = opcode;
                  opcode->next = rightparenth;
                  rightparenth->next = liststart;
                  left = leftparenth;
                  listlen += 2;
                  liststart = leftparenth;
               }
               else
               {
                  opcode->next = liststart;
                  liststart = opcode;
               }
            }
            listlen++;

            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));
            qa->next = NULL;
            qa->nodept = currnode->childset[0];
            qa->atleft = left;
            if (caseone)
            {
               queuestart = qa;
            }
            else
            {
               queueend->next = qa;
            }
            queueend = qa;
            queuelen++;

            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));
            qa->next = NULL;
            qa->nodept = currnode->childset[1];
            qa->atleft = right;
            queueend->next = qa;
            queueend = qa;
            queuelen++;
         }
         else fatal_error("Error: Unforeseen math operator childs number in treetocode function.");
         break;

      case NT_REL_OPER:
         if (currnode->nb_childs == 1)
         {
            opcode = NULL;

            /* Logical NOT is the only unary relational operator. */
            if (currnode->opval.rel_oper != NOT)
               fatal_error("Error: Unexpected unary relational operator found in treetocode function.");

            opcode = malloc(sizeof(code_atom));
            if (!opcode) fatal_error("Error: Lack of memory in treetocode function.");
            memset(opcode, 0, sizeof(code_atom));

            opcode->str = malloc(2);
            if (!opcode->str) fatal_error("Error: Lack of memory in treetocode function.");
            opcode->str[0] = '!';
            opcode->str[1] = '\0';

            if (atleft)
            {
               opcode->next = atleft->next;
               atleft->next = opcode;
            }
            else
            {
               opcode->next = liststart;
               liststart = opcode;
            }
            listlen++;

            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));
            qa->next = NULL;
            qa->nodept = currnode->childset[0];
            qa->atleft = opcode;
            if (caseone)
            {
               queuestart = qa;
            }
            else
            {
               queueend->next = qa;
            }
            queueend = qa;
            queuelen++;
         }
         else if (currnode->nb_childs == 2)
         {
            opcode = NULL;
            int bpar = 0; /* Boolean for parentheses insertion */
            int bmathrel = 0;
            int priocurr = 0, prioparent = 0;

            priocurr = get_op_prio(currnode);
            prioparent = get_op_prio(currnode->parent);
            if (currnode->parent)
            {
               bmathrel = currnode->parent->ntype == NT_MATH_OPER || currnode->parent->ntype == NT_REL_OPER;
            }
            if (prioparent > priocurr && bmathrel) bpar = 1;
            else if (prioparent == priocurr && priocurr != 0)
            {
               assoc currassoc = get_op_assoc(currnode);
               if (currnode->parent->childset[0] == currnode && currassoc == ASSOC_RIGHT) bpar = 1;
               else if (currnode->parent->childset[1] == currnode && currassoc == ASSOC_LEFT) bpar = 1;
            }

            left = atleft;

            opcode = malloc(sizeof(code_atom));
            if (!opcode) fatal_error("Error: Lack of memory in treetocode function.");
            memset(opcode, 0, sizeof(code_atom));
            opcode->str = malloc(5);
            if (!opcode->str) fatal_error("Error: Lack of memory in treetocode function.");
            switch (currnode->opval.rel_oper)
            {
            case OR:
               strncpy(opcode->str, " || ", 5);
               break;
            case AND:
               strncpy(opcode->str, " && ", 5);
               break;
            case EQ:
               strncpy(opcode->str, " == ", 5);
               break;
            case NE:
               strncpy(opcode->str, " != ", 5);
               break;
            case GT:
               strncpy(opcode->str, " > ", 4);
               break;
            case GE:
               strncpy(opcode->str, " >= ", 5);
               break;
            case LT:
               strncpy(opcode->str, " < ", 4);
               break;
            case LE:
               strncpy(opcode->str, " <= ", 5);
               break;
            default:
               fatal_error("Error: Unexpected binary relational operator found in treetocode function.");
            }

            right = opcode;

            if (bpar)
            {
               leftparenth = malloc(sizeof(code_atom));
               rightparenth = malloc(sizeof(code_atom));
               if (!leftparenth || !rightparenth) fatal_error("Error: Lack of memory in treetocode function.");
               memset(leftparenth, 0, sizeof(code_atom));
               memset(rightparenth, 0, sizeof(code_atom));

               leftparenth->str = malloc(2);
               rightparenth->str = malloc(2);
               if (!leftparenth->str || !rightparenth->str) 
                  fatal_error("Error: Lack of memory in treetocode function.");

               leftparenth->str[0] = '(';
               leftparenth->str[1] = '\0';
               rightparenth->str[0] = ')';
               rightparenth->str[1] = '\0';
            }

            if (atleft)
            {
               if (bpar)
               {
                  leftparenth->next = opcode;
                  opcode->next = rightparenth;
                  rightparenth->next = atleft->next;
                  atleft->next = leftparenth;
                  left = leftparenth;
                  listlen += 2;
               }
               else
               {
                  opcode->next = atleft->next;
                  atleft->next = opcode;
               }
            }
            else
            {
               if (bpar)
               {
                  leftparenth->next = opcode;
                  opcode->next = rightparenth;
                  rightparenth->next = liststart;
                  left = leftparenth;
                  listlen += 2;
                  liststart = leftparenth;
               }
               else
               {
                  opcode->next = liststart;
                  liststart = opcode;
               }
            }
            listlen++;

            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));
            qa->next = NULL;
            qa->nodept = currnode->childset[0];
            qa->atleft = left;
            if (caseone)
            {
               queuestart = qa;
            }
            else
            {
               queueend->next = qa;
            }
            queueend = qa;
            queuelen++;

            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));
            qa->next = NULL;
            qa->nodept = currnode->childset[1];
            qa->atleft = right;
            queueend->next = qa;
            queueend = qa;
            queuelen++;
         }
         else fatal_error("Error: Unforeseen relational operator childs number in treetocode function.");
         break;

      case NT_VARIABLE:
         id = malloc(sizeof(code_atom));
         if (!id) fatal_error("Error: Lack of memory in treetocode function.");
         memset(id, 0, sizeof(code_atom));

         idstrlen = strlen(currnode->opval.name);
         id->str = malloc(idstrlen + 1);
         if (!id->str) fatal_error("Error: Lack of memory in treetocode function.");
         strncpy(id->str, currnode->opval.name, idstrlen + 1);

         if (atleft)
         {
            id->next = atleft->next;
            atleft->next = id;
         }
         else
         {
            id->next = liststart;
            liststart = id;
         }
         listlen++;
         break;

      case NT_STRING:
         stratom = malloc(sizeof(code_atom));
         if (!stratom) fatal_error("Error: Lack of memory in treetocode function.");
         memset(stratom, 0, sizeof(code_atom));

         stratom->str = malloc(currnode->opval.str.length + 2);
         if (!stratom->str) fatal_error("Error: Lack of memory in treetocode function.");
         sprintf(stratom->str, "\"%s\"", currnode->opval.str.tab);

         if (atleft)
         {
            stratom->next = atleft->next;
            atleft->next = stratom;
         }
         else
         {
            stratom->next = liststart;
            liststart = stratom;
         }
         listlen++;
         break;

      case NT_FUNC_DEF:
         haslist = currnode->childset[1]->ntype == NT_LIST;

         if (haslist)
         {
            leftparenth = malloc(sizeof(code_atom));
            rightparenth = malloc(sizeof(code_atom));
            leftbrace = malloc(sizeof(code_atom));
            rightbrace = malloc(sizeof(code_atom));
            if (!leftparenth || !rightparenth || !leftbrace || !rightbrace) 
               fatal_error("Error: Lack of memory in treetocode function.");
            memset(leftparenth, 0, sizeof(code_atom));
            memset(rightparenth, 0, sizeof(code_atom));
            memset(leftbrace, 0, sizeof(code_atom));
            memset(rightbrace, 0, sizeof(code_atom));
            leftparenth->str = malloc(10);
            rightparenth->str = malloc(3);
            leftbrace->str = malloc(3);
            rightbrace->str = malloc(2);
            if (!leftparenth->str || !rightparenth->str || !leftbrace->str || !rightbrace->str) 
               fatal_error("Error: Lack of memory in treetocode function.");

            strncpy(leftparenth->str, "function(", 10);
            strncpy(rightparenth->str, ")\n", 3);
            strncpy(leftbrace->str, "{\n", 3);
            strncpy(rightbrace->str, "}", 2);

            leftbrace->linestart = 1;
            leftbrace->indent = IC_MORE;
            rightbrace->linestart = 1;
            rightbrace->indent = IC_LESS;

            if (atleft)
            {
               rightbrace->next = atleft->next;
               atleft->next = leftparenth;
            }
            else
            {
               rightbrace->next = liststart;
               liststart = leftparenth;
            }
            leftparenth->next = rightparenth;
            rightparenth->next = leftbrace;
            leftbrace->next = rightbrace;
            listlen += 4;
         }
         else
         {
            leftparenth = malloc(sizeof(code_atom));
            rightparenth = malloc(sizeof(code_atom));
            if (!leftparenth || !rightparenth) fatal_error("Error: Lack of memory in treetocode function.");
            memset(leftparenth, 0, sizeof(code_atom));
            memset(rightparenth, 0, sizeof(code_atom));
            leftparenth->str = malloc(10);
            rightparenth->str = malloc(3);
            if (!leftparenth->str || !rightparenth->str) 
               fatal_error("Error: Lack of memory in treetocode function.");

            strncpy(leftparenth->str, "function(", 10);
            strncpy(rightparenth->str, ") ", 3);

            if (atleft)
            {
               rightparenth->next = atleft->next;
               atleft->next = leftparenth;
            }
            else
            {
               rightparenth->next = liststart;
               liststart = leftparenth;
            }
            leftparenth->next = rightparenth;
            listlen += 2;
         }

         left = leftparenth;
         for (i = 0; i < currnode->childset[0]->nb_childs; i++)
         {
            char * varname = currnode->childset[0]->childset[i]->opval.name;

            if (currnode->childset[0]->childset[i]->ntype != NT_VARIABLE)
               fatal_error("Error: Unexpected argument node type in treetocode function.");

            id = malloc(sizeof(code_atom));
            if (!id) fatal_error("Error: Lack of memory in treetocode function.");
            memset(id, 0, sizeof(code_atom));

            idstrlen = strlen(varname);
            id->str = malloc(idstrlen + 3);
            if (!id->str) fatal_error("Error: Lack of memory in treetocode function.");
            if (i == 0) strncpy(id->str, varname, idstrlen + 1);
            else sprintf(id->str, ", %s", varname);

            id->next = left->next;
            left->next = id;
            left = id;

            listlen++;
         }

         if (haslist)
         {
            previous = leftbrace;
            for (i = 0; i < currnode->childset[1]->nb_childs; i++)
            {
               previous = enqueue_stmt(previous, currnode->childset[1]->childset[i],
                  &liststart, &listlen, &queuestart, &queueend, &queuelen);
            }
            previous->next = rightbrace;
         }
         else
         {
            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));

            qa->next = NULL;
            qa->nodept = currnode->childset[1];
            qa->atleft = rightparenth;
            if (caseone)
            {
               queuestart = qa;
            }
            else
            {
               queueend->next = qa;
            }
            queueend = qa;
            queuelen++;
         }
         break;

      case NT_FUNC_CALL:
         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));
         qa->next = NULL;
         qa->nodept = currnode->childset[0];
         qa->atleft = atleft;
         if (caseone)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;

         leftparenth = malloc(sizeof(code_atom));
         rightparenth = malloc(sizeof(code_atom));
         if (!leftparenth || !rightparenth) fatal_error("Error: Lack of memory in treetocode function.");
         memset(leftparenth, 0, sizeof(code_atom));
         memset(rightparenth, 0, sizeof(code_atom));
         leftparenth->str = malloc(2);
         rightparenth->str = malloc(2);
         if (!leftparenth->str || !rightparenth->str) 
            fatal_error("Error: Lack of memory in treetocode function.");
         leftparenth->str[0] = '(';
         leftparenth->str[1] = '\0';
         rightparenth->str[0] = ')';
         rightparenth->str[1] = '\0';

         leftparenth->next = rightparenth;
         if (atleft)
         {
            rightparenth->next = atleft->next;
            atleft->next = leftparenth;
         }
         else
         {
            rightparenth->next = liststart;
            liststart = leftparenth;
         }
         listlen += 2;

         left = leftparenth;
         for (i = 0; i < currnode->childset[1]->nb_childs; i++)
         {
            if (i > 0)
            {
               comma = malloc(sizeof(code_atom));
               if (!comma) fatal_error("Error: Lack of memory in treetocode function.");
               memset(comma, 0, sizeof(code_atom));
               comma->str = malloc(3);
               if (!comma->str) fatal_error("Error: Lack of memory in treetocode function.");
               strncpy(comma->str, ", ", 3);

               comma->next = left->next;
               left->next = comma;
               left = comma;

               listlen++;
            }

            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));
            qa->next = NULL;
            qa->nodept = currnode->childset[1]->childset[i];
            qa->atleft = left;
            queueend->next = qa;
            queueend = qa;
            queuelen++;
         }
         break;

      case NT_IF_STMT:
      case NT_WHILE_STMT:
         haslist = currnode->childset[1]->ntype == NT_LIST;

         statement = malloc(sizeof(code_atom));
         rightparenth = malloc(sizeof(code_atom));
         if (!statement || !rightparenth) fatal_error("Error: Lack of memory in treetocode function.");
         memset(statement, 0, sizeof(code_atom));
         memset(rightparenth, 0, sizeof(code_atom));

         statement->str = malloc(8);
         rightparenth->str = malloc(3);
         if (!statement->str || !rightparenth->str) 
            fatal_error("Error: Lack of memory in treetocode function.");
         if (currnode->ntype == NT_IF_STMT) strncpy(statement->str, "if (", 5);
         else strncpy(statement->str, "while (", 8);

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[0];
         qa->atleft = statement;
         if (caseone)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;

         if (haslist)
         {
            strncpy(rightparenth->str, ")\n", 3);
         }
         else
         {
            strncpy(rightparenth->str, ") ", 3);
         }

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[1];
         qa->atleft = rightparenth;
         queueend->next = qa;
         queueend = qa;
         queuelen++;

         if (atleft)
         {
            rightparenth->next = atleft->next;
            atleft->next = statement;
         }
         else
         {
            rightparenth->next = liststart;
            liststart = statement;
         }
         statement->next = rightparenth;

         listlen += 2;
         break;

      case NT_IFELSE_STMT:
         haslist = currnode->childset[1]->ntype == NT_LIST;

         if (haslist)
         {
            statement = malloc(sizeof(code_atom));
            rightparenth = malloc(sizeof(code_atom));
            leftbrace = malloc(sizeof(code_atom));
            rightbrace = malloc(sizeof(code_atom));
            if (!statement || !rightparenth || !leftbrace || !rightbrace) 
               fatal_error("Error: Lack of memory in treetocode function.");
            memset(statement, 0, sizeof(code_atom));
            memset(rightparenth, 0, sizeof(code_atom));
            memset(leftbrace, 0, sizeof(code_atom));
            memset(rightbrace, 0, sizeof(code_atom));

            statement->str = malloc(5);
            rightparenth->str = malloc(3);
            leftbrace->str = malloc(3);
            rightbrace->str = malloc(3);
            if (!statement->str || !rightparenth->str || !leftbrace->str || !rightbrace->str) 
               fatal_error("Error: Lack of memory in treetocode function.");
            strncpy(statement->str, "if (", 5);
            strncpy(rightparenth->str, ")\n", 3);
            strncpy(leftbrace->str, "{\n", 3);
            strncpy(rightbrace->str, "}\n", 3);

            leftbrace->linestart = 1;
            leftbrace->indent = IC_MORE;
            rightbrace->linestart = 1;
            rightbrace->indent = IC_LESS;
            rightbrace->preelse = 1;

            if (atleft)
            {
               rightbrace->next = atleft->next;
               atleft->next = statement;
            }
            else
            {
               rightbrace->next = liststart;
               liststart = statement;
            }
            statement->next = rightparenth;
            rightparenth->next = leftbrace;
            leftbrace->next = rightbrace;

            listlen += 4;
         }
         else
         {
            statement = malloc(sizeof(code_atom));
            rightparenth = malloc(sizeof(code_atom));
            placeholder = malloc(sizeof(code_atom));
            if (!statement || !rightparenth || !placeholder) 
               fatal_error("Error: Lack of memory in treetocode function.");
            memset(statement, 0, sizeof(code_atom));
            memset(rightparenth, 0, sizeof(code_atom));
            memset(placeholder, 0, sizeof(code_atom));

            statement->str = malloc(5);
            rightparenth->str = malloc(3);
            if (!statement->str || !rightparenth->str) 
               fatal_error("Error: Lack of memory in treetocode function.");
            strncpy(statement->str, "if (", 5);
            strncpy(rightparenth->str, ") ", 3);

            if (atleft)
            {
               placeholder->next = atleft->next;
               atleft->next = statement;
            }
            else
            {
               placeholder->next = liststart;
               liststart = statement;
            }
            statement->next = rightparenth;
            rightparenth->next = placeholder;

            listlen += 3;
         }

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[0];
         qa->atleft = statement;
         if (caseone)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;

         if (haslist)
         {
            previous = leftbrace;
            for (i = 0; i < currnode->childset[1]->nb_childs; i++)
            {
               previous = enqueue_stmt(previous, currnode->childset[1]->childset[i],
                  &liststart, &listlen, &queuestart, &queueend, &queuelen);
            }
            previous->next = rightbrace;
            atleft = rightbrace;
         }
         else
         {
            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));

            qa->next = NULL;
            qa->nodept = currnode->childset[1];
            qa->atleft = rightparenth;
            queueend->next = qa;
            queueend = qa;
            queuelen++;
            atleft = placeholder;
         }

         if (currnode->childset[2]->ntype == NT_LIST)
         {
            previous = NULL;

            statement = malloc(sizeof(code_atom));
            leftbrace = malloc(sizeof(code_atom));
            rightbrace = malloc(sizeof(code_atom));
            if (!statement || !leftbrace || !rightbrace) 
               fatal_error("Error: Lack of memory in treetocode function.");
            memset(statement, 0, sizeof(code_atom));
            memset(leftbrace, 0, sizeof(code_atom));
            memset(rightbrace, 0, sizeof(code_atom));

            statement->str = malloc(7);
            leftbrace->str = malloc(3);
            rightbrace->str = malloc(2);
            if (!statement->str || !leftbrace->str || !rightbrace->str) 
               fatal_error("Error: Lack of memory in treetocode function.");
            if (haslist)
            {
               strncpy(statement->str, "else\n", 6);
               statement->linestart = 1;
            }
            else
            {
               strncpy(statement->str, " else\n", 7);
            }
            strncpy(leftbrace->str, "{\n", 3);
            strncpy(rightbrace->str, "}", 2);

            leftbrace->linestart = 1;
            leftbrace->indent = IC_MORE;
            rightbrace->linestart = 1;
            rightbrace->indent = IC_LESS;

            rightbrace->next = atleft->next;
            atleft->next = statement;
            statement->next = leftbrace;
            leftbrace->next = rightbrace;

            listlen += 3;

            previous = leftbrace;
            for (i = 0; i < currnode->childset[2]->nb_childs; i++)
            {
               previous = enqueue_stmt(previous, currnode->childset[2]->childset[i],
                  &liststart, &listlen, &queuestart, &queueend, &queuelen);
            }
            previous->next = rightbrace;
         }
         else
         {
            statement = malloc(sizeof(code_atom));
            if (!statement) fatal_error("Error: Lack of memory in treetocode function.");
            memset(statement, 0, sizeof(code_atom));

            statement->str = malloc(7);
            if (!statement->str) fatal_error("Error: Lack of memory in treetocode function.");
            if (haslist)
            {
               strncpy(statement->str, "else ", 6);
               statement->linestart = 1;
            }
            else strncpy(statement->str, " else ", 7);

            statement->next = atleft->next;
            atleft->next = statement;

            listlen++;

            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));

            qa->next = NULL;
            qa->nodept = currnode->childset[2];
            qa->atleft = statement;
            queueend->next = qa;
            queueend = qa;
            queuelen++;
         }
         break;

      case NT_FOR_STMT:
         statement = malloc(sizeof(code_atom));
         semicol1 = malloc(sizeof(code_atom));
         semicol2 = malloc(sizeof(code_atom));
         rightparenth = malloc(sizeof(code_atom));
         if (!statement || !semicol1 || !semicol2 || !rightparenth) 
            fatal_error("Error: Lack of memory in treetocode function.");
         memset(statement, 0, sizeof(code_atom));
         memset(semicol1, 0, sizeof(code_atom));
         memset(semicol2, 0, sizeof(code_atom));
         memset(rightparenth, 0, sizeof(code_atom));

         statement->str = malloc(6);
         semicol1->str = malloc(3);
         semicol2->str = malloc(3);
         rightparenth->str = malloc(3);
         if (!statement->str || !semicol1->str || !semicol2->str || !rightparenth->str) 
            fatal_error("Error: Lack of memory in treetocode function.");
         strncpy(statement->str, "for (", 6);
         strncpy(semicol1->str, "; ", 3);
         strncpy(semicol2->str, "; ", 3);
         listlen += 4;

         tablefts[0] = statement;
         tablefts[1] = semicol1;
         tablefts[2] = semicol2;

         for (i = 0; i < 3; i++)
         {
            if (currnode->childset[i]->ntype != NT_NUM_CONST)
            {
               qa = malloc(sizeof(queue_atom));
               if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
               memset(qa, 0, sizeof(queue_atom));

               qa->next = NULL;
               qa->nodept = currnode->childset[i];
               qa->atleft = tablefts[i];
               if (!queuelen)
               {
                  queuestart = qa;
               }
               else
               {
                  queueend->next = qa;
               }
               queueend = qa;
               queuelen++;
            }
         }

         haslist = currnode->childset[3]->ntype == NT_LIST;
         if (haslist)
         {
            strncpy(rightparenth->str, ")\n", 3);
         }
         else
         {
            strncpy(rightparenth->str, ") ", 3);
         }

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[3];
         qa->atleft = rightparenth;
         if (!queuelen)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;

         if (atleft)
         {
            rightparenth->next = atleft->next;
            atleft->next = statement;
         }
         else
         {
            rightparenth->next = liststart;
            liststart = statement;
         }
         semicol2->next = rightparenth;
         semicol1->next = semicol2;
         statement->next = semicol1;
         break;

      case NT_LIST:
         if (currnode->nb_childs > 0)
         {
            if (currnode->childset[0]->ntype == NT_PARENT || 
               currnode->childset[0]->ntype == NT_CHILDSET) bgene = 1;
         }

         if (bgene)
         {
            previous = atleft;
            for (i = 0; i < currnode->nb_childs; i++)
            {
               if (i > 0)
               {
                  accessor = malloc(sizeof(code_atom));
                  if (!accessor) fatal_error("Error: Lack of memory in treetocode function.");
                  memset(accessor, 0, sizeof(code_atom));
                  accessor->str = malloc(2);
                  if (!accessor->str) fatal_error("Error: Lack of memory in treetocode function.");
                  strncpy(accessor->str, ".", 2);

                  listlen++;

                  if (previous)
                  {
                     accessor->next = previous->next;
                     previous->next = accessor;
                  }
                  else
                  {
                     accessor->next = liststart;
                     liststart = accessor;
                  }

                  previous = accessor;
               }

               qa = malloc(sizeof(queue_atom));
               if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
               memset(qa, 0, sizeof(queue_atom));

               qa->next = NULL;
               qa->nodept = currnode->childset[i];
               qa->atleft = previous;
               if (!queuelen)
               {
                  queuestart = qa;
               }
               else
               {
                  queueend->next = qa;
               }
               queueend = qa;
               queuelen++;
            }
         }
         else
         {
            leftbrace = malloc(sizeof(code_atom));
            rightbrace = malloc(sizeof(code_atom));
            if (!leftbrace || !rightbrace) fatal_error("Error: Lack of memory in treetocode function.");
            memset(leftbrace, 0, sizeof(code_atom));
            memset(rightbrace, 0, sizeof(code_atom));

            leftbrace->str = malloc(3);
            rightbrace->str = malloc(2);
            if (!leftbrace->str || !rightbrace->str) 
               fatal_error("Error: Lack of memory in treetocode function.");
            strncpy(leftbrace->str, "{\n", 3);
            strncpy(rightbrace->str, "}", 2);

            leftbrace->linestart = 1;
            leftbrace->indent = IC_MORE;
            rightbrace->linestart = 1;
            rightbrace->indent = IC_LESS;

            if (atleft)
            {
               rightbrace->next = atleft->next;
               atleft->next = leftbrace;
            }
            else
            {
               rightbrace->next = liststart;
               liststart = leftbrace;
            }
            leftbrace->next = rightbrace;

            listlen += 2;

            previous = leftbrace;
            for (i = 0; i < currnode->nb_childs; i++)
            {
               previous = enqueue_stmt(previous, currnode->childset[i],
                  &liststart, &listlen, &queuestart, &queueend, &queuelen);
            }
            previous->next = rightbrace;
         }
         break;

      case NT_CODESEGMENT:
         leftquote = malloc(sizeof(code_atom));
         rightquote = malloc(sizeof(code_atom));
         if (!leftquote || !rightquote) fatal_error("Error: Lack of memory in treetocode function.");
         leftquote->str = malloc(2);
         rightquote->str = malloc(2);
         if (!leftquote->str || !rightquote->str) 
            fatal_error("Error: Lack of memory in treetocode function.");
         strncpy(leftquote->str, "`", 2);
         strncpy(rightquote->str, "\'", 2);

         if (atleft)
         {
            rightquote->next = atleft->next;
            atleft->next = leftquote;
         }
         else
         {
            rightquote->next = liststart;
            liststart = leftquote;
         }
         leftquote->next = rightquote;

         listlen += 2;

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[0];
         qa->atleft = leftquote;
         if (!queuelen)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;
         break;

      case NT_PARENT:
         statement = malloc(sizeof(code_atom));
         if (!statement) fatal_error("Error: Lack of memory in treetocode function.");
         memset(statement, 0, sizeof(code_atom));
         statement->str = malloc(7);
         if (!statement->str) fatal_error("Error: Lack of memory in treetocode function.");
         strncpy(statement->str, "parent", 7);

         if (atleft)
         {
            statement->next = atleft->next;
            atleft->next = statement;
         }
         else
         {
            statement->next = liststart;
            liststart = statement;
         }
         listlen++;
         break;

      case NT_CHILDSET:
         statement = malloc(sizeof(code_atom));
         leftbracket = malloc(sizeof(code_atom));
         rightbracket = malloc(sizeof(code_atom));
         if (!statement || !leftbracket || !rightbracket) 
            fatal_error("Error: Lack of memory in treetocode function.");
         memset(statement, 0, sizeof(code_atom));
         memset(leftbracket, 0, sizeof(code_atom));
         memset(rightbracket, 0, sizeof(code_atom));
         statement->str = malloc(9);
         leftbracket->str = malloc(2);
         rightbracket->str = malloc(2);
         if (!statement->str || !leftbracket->str || !rightbracket->str) 
            fatal_error("Error: Lack of memory in treetocode function.");
         strncpy(statement->str, "childset", 9);
         strncpy(leftbracket->str, "[", 2);
         strncpy(rightbracket->str, "]", 2);

         listlen += 3;

         if (atleft)
         {
            rightbracket->next = atleft->next;
            atleft->next = statement;
         }
         else
         {
            rightbracket->next = liststart;
            liststart = statement;
         }
         leftbracket->next = rightbracket;
         statement->next = leftbracket;

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[0];
         qa->atleft = leftbracket;
         if (!queuelen)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;
         break;

      case NT_VARGENLIST:
         accessor = malloc(sizeof(code_atom));
         if (!accessor) fatal_error("Error: Lack of memory in treetocode function.");
         memset(accessor, 0, sizeof(code_atom));
         accessor->str = malloc(2);
         if (!accessor->str) fatal_error("Error: Lack of memory in treetocode function.");
         strncpy(accessor->str, ".", 2);

         listlen++;

         if (atleft)
         {
            accessor->next = atleft->next;
            atleft->next = accessor;
         }
         else
         {
            accessor->next = liststart;
            liststart = accessor;
         }

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[0];
         qa->atleft = atleft;
         if (!queuelen)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[1];
         qa->atleft = accessor;
         queueend->next = qa;
         queueend = qa;
         queuelen++;
         break;

      case NT_INCR_DECR:
         opcode = malloc(sizeof(code_atom));
         if (!opcode) fatal_error("Error: Lack of memory in treetocode function.");
         memset(opcode, 0, sizeof(code_atom));
         opcode->str = malloc(3);
         if (!opcode->str) fatal_error("Error: Lack of memory in treetocode function.");
         if (currnode->opval.ppmm == PREPLUSPLUS || currnode->opval.ppmm == POSTPLUSPLUS)
            strncpy(opcode->str, "++", 3);
         else if (currnode->opval.ppmm == PREMINUSMINUS || currnode->opval.ppmm == POSTMINUSMINUS)
            strncpy(opcode->str, "--", 3);
         else fatal_error("Error: Unforeseen increment or decrement operator in treetocode function.");

         listlen++;

         if (atleft)
         {
            opcode->next = atleft->next;
            atleft->next = opcode;
         }
         else
         {
            opcode->next = liststart;
            liststart = opcode;
         }

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[0];

         if (currnode->opval.ppmm == PREPLUSPLUS || currnode->opval.ppmm == PREMINUSMINUS)
            qa->atleft = opcode;
         else qa->atleft = atleft;

         if (!queuelen)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;
         break;

      case NT_ACCESSLIST:
         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[0];
         qa->atleft = atleft;
         if (!queuelen)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;

         left = atleft;
         for (i = 1; i < currnode->nb_childs; i += 2)
         {
            opcode = malloc(sizeof(code_atom));
            if (!opcode) fatal_error("Error: Lack of memory in treetocode function.");
            memset(opcode, 0, sizeof(code_atom));
            opcode->str = malloc(3);
            if (!opcode->str) fatal_error("Error: Lack of memory in treetocode function.");
            if (currnode->childset[i]->ntype == NT_REF) strncpy(opcode->str, ".", 2);
            else if (currnode->childset[i]->ntype == NT_DEREF) strncpy(opcode->str, "->", 3);
            else fatal_error("Error: Unexpected operator for access list in treetocode function.");

            if (left)
            {
               opcode->next = left->next;
               left->next = opcode;
            }
            else
            {
               opcode->next = liststart;
               liststart = opcode;
            }
            left = opcode;

            listlen++;

            qa = malloc(sizeof(queue_atom));
            if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
            memset(qa, 0, sizeof(queue_atom));

            qa->next = NULL;
            qa->nodept = currnode->childset[i + 1];
            qa->atleft = opcode;
            queueend->next = qa;
            queueend = qa;
            queuelen++;
         }
         break;

      case NT_SUBSCRIPT:
         leftbracket = malloc(sizeof(code_atom));
         rightbracket = malloc(sizeof(code_atom));
         if (!leftbracket || !rightbracket) fatal_error("Error: Lack of memory in treetocode function.");
         memset(leftbracket, 0, sizeof(code_atom));
         memset(rightbracket, 0, sizeof(code_atom));
         leftbracket->str = malloc(2);
         rightbracket->str = malloc(2);
         if (!leftbracket->str || !rightbracket->str) 
            fatal_error("Error: Lack of memory in treetocode function.");
         strncpy(leftbracket->str, "[", 2);
         strncpy(rightbracket->str, "]", 2);

         listlen += 2;

         if (atleft)
         {
            rightbracket->next = atleft->next;
            atleft->next = leftbracket;
         }
         else
         {
            rightbracket->next = liststart;
            liststart = leftbracket;
         }
         leftbracket->next = rightbracket;

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[0];
         qa->atleft = atleft;
         if (!queuelen)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[1];
         qa->atleft = leftbracket;
         queueend->next = qa;
         queueend = qa;
         queuelen++;
         break;

      case NT_STDIN:
      case NT_STDOUT:
      case NT_STDERR:
         statement = malloc(sizeof(code_atom));
         if (!statement) fatal_error("Error: Lack of memory in treetocode function.");
         memset(statement, 0, sizeof(code_atom));
         statement->str = malloc(7);
         if (!statement->str) fatal_error("Error: Lack of memory in treetocode function.");
         if (currnode->ntype == NT_STDIN) strncpy(statement->str, "stdin", 6);
         else if (currnode->ntype == NT_STDOUT) strncpy(statement->str, "stdout", 7);
         else strncpy(statement->str, "stderr", 7);

         if (atleft)
         {
            statement->next = atleft->next;
            atleft->next = statement;
         }
         else
         {
            statement->next = liststart;
            liststart = statement;
         }
         listlen++;
         break;

      case NT_CAST:
         leftparenth = malloc(sizeof(code_atom));
         rightparenth = malloc(sizeof(code_atom));
         type = malloc(sizeof(code_atom));
         if (!leftparenth || !rightparenth || !type) 
            fatal_error("Error: Lack of memory in treetocode function.");
         memset(leftparenth, 0, sizeof(code_atom));
         memset(rightparenth, 0, sizeof(code_atom));
         memset(type, 0, sizeof(code_atom));
         leftparenth->str = malloc(2);
         rightparenth->str = malloc(3);
         type->str = malloc(19);
         if (!leftparenth->str || !rightparenth->str || !type->str)
            fatal_error("Error: Lack of memory in treetocode function.");
         strncpy(leftparenth->str, "(", 2);
         strncpy(rightparenth->str, ") ", 3);

         switch (currnode->opval.ti.dtype)
         {
         case DT_CHAR:
            strcpy(type->str, "char");
            break;
         case DT_S_CHAR:
            strcpy(type->str, "signed char");
            break;
         case DT_U_CHAR:
            strcpy(type->str, "unsigned char");
            break;
         case DT_BYTE:
            strcpy(type->str, "byte");
            break;
         case DT_SHORT:
            strcpy(type->str, "short");
            break;
         case DT_U_SHORT:
            strcpy(type->str, "unsigned short");
            break;
         case DT_INT:
            strcpy(type->str, "int");
            break;
         case DT_U_INT:
            strcpy(type->str, "unsigned");
            break;
         case DT_LONG:
            strcpy(type->str, "long");
            break;
         case DT_U_LONG:
            strcpy(type->str, "unsigned long");
            break;
         case DT_LONG_LONG:
            strcpy(type->str, "long long");
            break;
         case DT_U_LONG_LONG:
            strcpy(type->str, "unsigned long long");
            break;
         case DT_SIZE_T:
            strcpy(type->str, "size_t");
            break;
         case DT_FLOAT:
            strcpy(type->str, "float");
            break;
         case DT_DOUBLE:
            strcpy(type->str, "double");
            break;
         case DT_LONG_DOUBLE:
            strcpy(type->str, "long double");
            break;
         default:
            fatal_error("Error: Unexpected cast type found in treetocode function.");
         }

         listlen += 3;

         if (atleft)
         {
            rightparenth->next = atleft->next;
            atleft->next = leftparenth;
         }
         else
         {
            rightparenth->next = liststart;
            liststart = leftparenth;
         }
         leftparenth->next = type;

         if (currnode->opval.ti.nderef)
         {
            starlist = malloc(sizeof(code_atom));
            if (!starlist) fatal_error("Error: Lack of memory in treetocode function.");
            memset(starlist, 0, sizeof(code_atom));
            starlist->str = malloc(currnode->opval.ti.nderef + 1);
            if (!starlist->str) fatal_error("Error: Lack of memory in treetocode function.");
            memset(starlist->str, '*', currnode->opval.ti.nderef);
            starlist->str[currnode->opval.ti.nderef] = '\0';

            listlen++;

            starlist->next = rightparenth;
            type->next = starlist;
         }
         else
         {
            type->next = rightparenth;
         }

         qa = malloc(sizeof(queue_atom));
         if (!qa) fatal_error("Error: Lack of memory in treetocode function.");
         memset(qa, 0, sizeof(queue_atom));

         qa->next = NULL;
         qa->nodept = currnode->childset[0];
         qa->atleft = rightparenth;
         if (!queuelen)
         {
            queuestart = qa;
         }
         else
         {
            queueend->next = qa;
         }
         queueend = qa;
         queuelen++;
         break;

      case NT_REF:
      case NT_DEREF:
         opcode = malloc(sizeof(code_atom));
         if (!opcode) fatal_error("Error: Lack of memory in treetocode function.");
         memset(opcode, 0, sizeof(code_atom));
         opcode->str = malloc(3);
         if (!opcode->str) fatal_error("Error: Lack of memory in treetocode function.");
         if (currnode->ntype == NT_REF) strncpy(opcode->str, ".", 2);
         else strncpy(opcode->str, "->", 3);

         listlen++;

         if (atleft)
         {
            opcode->next = atleft->next;
            atleft->next = opcode;
         }
         else
         {
            opcode->next = liststart;
            liststart = opcode;
         }
         break;

      default:
         fatal_error("Error: Unknown node type in treetocode function.");
      }
   }


   curr = liststart;
   for (i = 0; i < listlen; i++)
   {
      if (ilastmore == (int)i - 1) tabcount++;
      if (curr->indent == IC_LESS) tabcount--;
      else if (curr->indent == IC_MORE) ilastmore = (int)i;

      if (curr->linestart) codelen += tabcount * (tab.length - 1);
      if (curr->str) codelen += strlen(curr->str);
      curr = curr->next;
   }
   codelen++;

   retval.ti.dtype = DT_STRING;
   retval.value.str.length = codelen;
   retval.value.str.tab = malloc(codelen);
   if (!retval.value.str.tab) fatal_error("Error: Lack of memory in treetocode function.");
   memset(retval.value.str.tab, 0, codelen);
   curr = liststart;
   pos = 0;
   ilastmore = -2;
   tabcount = 0;
   for (i = 0; i < listlen; i++)
   {
      size_t numadd = 0;
      if (curr->str) numadd = strlen(curr->str);

      if (ilastmore == (int)i - 1) tabcount++;
      if (curr->indent == IC_LESS) tabcount--;
      else if (curr->indent == IC_MORE) ilastmore = (int)i;

      if (curr->linestart)
      {
         if (tab.length > 1)
         {
            for (j = 0; j < tabcount; j++)
            {
               strncpy(&retval.value.str.tab[pos], tab.tab, tab.length - 1);
               pos += tab.length - 1;
            }
         }
         if (curr->preelse)
         {
            if (!tabcount)
            {
               /* from "}\n" to "} " */
               curr->str[1] = ' ';
            }
         }
      }
      if (numadd) strncpy(&retval.value.str.tab[pos], curr->str, numadd);
      pos += numadd;
      curr = curr->next;
   }


   curr = liststart;
   for (i = 0; i < listlen; i++)
   {
      code_atom * prec = curr;
      if (curr->str) free(curr->str);
      curr = curr->next;
      free(prec);
   }

   return retval;
}
