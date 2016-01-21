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

#ifndef TREE_H
#define TREE_H

#include "node.h"

node* xxbinary(node* top, node* left, node* right);
node* xxunary(node* top, node* operand);
node* xx2childs(node* child1, node* child2, node_type nt);
node* xxfirstitem(node* item);
node* xxadditem(node* list, node* item);
node* xxifelse(node* cond, node* stmt_true, node* stmt_false);
node* xxfor(node* init, node* cond, node* incr, node* stmt);
node* xxcodesegment(node* segment);
node* xxfirstchildset(node* childset, node* index);
node* xxaddchildset(node* list, node* childset, node* index);
node* xxemptylist(void);
node* xxone(void);
node* xxppmm(node* var, int op);
node* xxobjectaccess(node* first, node* list);
node* xxaddmember(node* list, node* member, node_type nt);
node* xxtype(data_type dt);
node* xxcast(node* type, node* starlist, node* operand);
void xxsetroot(node* to_eval);

#endif
