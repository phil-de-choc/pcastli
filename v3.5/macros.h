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

#ifndef MACROS_H
#define MACROS_H


#define min(a,b)    (((a) < (b)) ? (a) : (b))
#define max(a,b)    (((a) > (b)) ? (a) : (b))


#if defined(BITS32)
#define mac_bin_top(OP) retval.ti.dtype = max(from_eval1.ti.dtype, from_eval2.ti.dtype); \
if (sizes_classes[from_eval1.ti.dtype] > sizes_classes[from_eval2.ti.dtype]) \
{ \
   switch (from_eval1.ti.dtype) \
   { \
   case DT_LONG_DOUBLE: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ushnum; \
         break; \
      case DT_INT: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.inum; \
         break; \
      case DT_U_INT: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.uinum; \
         break; \
      case DT_LONG: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_LONG: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ulnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.stnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.llnum; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ullnum; \
         break; \
      case DT_FLOAT: \
      case DT_DOUBLE: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.num; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.ushnum; \
         break; \
      case DT_INT: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.inum; \
         break; \
      case DT_U_INT: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.uinum; \
         break; \
      case DT_LONG: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.lnum; \
         break; \
      case DT_U_LONG: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.ulnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.stnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.llnum; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.ullnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.ushnum; \
         break; \
      case DT_INT: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.inum; \
         break; \
      case DT_U_INT: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.uinum; \
         break; \
      case DT_LONG: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_LONG: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.ulnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.stnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.ushnum; \
         break; \
      case DT_INT: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.inum; \
         break; \
      case DT_U_INT: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.uinum; \
         break; \
      case DT_LONG: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_LONG: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.ulnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.stnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SIZE_T: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_INT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_INT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_SHORT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SHORT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   default: \
      break; \
   } \
   retval.ti.dtype =  from_eval1.ti.dtype;\
} \
else if (sizes_classes[from_eval1.ti.dtype] < sizes_classes[from_eval2.ti.dtype]) \
{ \
   switch (from_eval2.ti.dtype) \
   { \
   case DT_LONG_DOUBLE: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ldnum = from_eval1.value.cnum OP from_eval2.value.ldnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ldnum = from_eval1.value.scnum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ldnum = from_eval1.value.ucnum OP from_eval2.value.ldnum; \
         break; \
      case DT_SHORT: \
         retval.value.ldnum = from_eval1.value.shnum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ldnum = from_eval1.value.ushnum OP from_eval2.value.ldnum; \
         break; \
      case DT_INT: \
         retval.value.ldnum = from_eval1.value.inum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_INT: \
         retval.value.ldnum = from_eval1.value.uinum OP from_eval2.value.ldnum; \
         break; \
      case DT_LONG: \
         retval.value.ldnum = from_eval1.value.lnum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_LONG: \
         retval.value.ldnum = from_eval1.value.ulnum OP from_eval2.value.ldnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.ldnum = from_eval1.value.stnum OP from_eval2.value.ldnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.ldnum = from_eval1.value.llnum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.ldnum = from_eval1.value.ullnum OP from_eval2.value.ldnum; \
         break; \
      case DT_FLOAT: \
      case DT_DOUBLE: \
         retval.value.ldnum = from_eval1.value.num OP from_eval2.value.ldnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.num = from_eval1.value.cnum OP from_eval2.value.num; \
         break; \
      case DT_S_CHAR: \
         retval.value.num = from_eval1.value.scnum OP from_eval2.value.num; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.num = from_eval1.value.ucnum OP from_eval2.value.num; \
         break; \
      case DT_SHORT: \
         retval.value.num = from_eval1.value.shnum OP from_eval2.value.num; \
         break; \
      case DT_U_SHORT: \
         retval.value.num = from_eval1.value.ushnum OP from_eval2.value.num; \
         break; \
      case DT_INT: \
         retval.value.num = from_eval1.value.inum OP from_eval2.value.num; \
         break; \
      case DT_U_INT: \
         retval.value.num = from_eval1.value.uinum OP from_eval2.value.num; \
         break; \
      case DT_LONG: \
         retval.value.num = from_eval1.value.lnum OP from_eval2.value.num; \
         break; \
      case DT_U_LONG: \
         retval.value.num = from_eval1.value.ulnum OP from_eval2.value.num; \
         break; \
      case DT_SIZE_T: \
         retval.value.num = from_eval1.value.stnum OP from_eval2.value.num; \
         break; \
      case DT_LONG_LONG: \
         retval.value.num = from_eval1.value.llnum OP from_eval2.value.num; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.num = from_eval1.value.ullnum OP from_eval2.value.num; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG_LONG: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ullnum = from_eval1.value.cnum OP from_eval2.value.ullnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ullnum = from_eval1.value.scnum OP from_eval2.value.ullnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ullnum = from_eval1.value.ucnum OP from_eval2.value.ullnum; \
         break; \
      case DT_SHORT: \
         retval.value.ullnum = from_eval1.value.shnum OP from_eval2.value.ullnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ullnum = from_eval1.value.ushnum OP from_eval2.value.ullnum; \
         break; \
      case DT_INT: \
         retval.value.ullnum = from_eval1.value.inum OP from_eval2.value.ullnum; \
         break; \
      case DT_U_INT: \
         retval.value.ullnum = from_eval1.value.uinum OP from_eval2.value.ullnum; \
         break; \
      case DT_LONG: \
         retval.value.ullnum = from_eval1.value.lnum OP from_eval2.value.ullnum; \
         break; \
      case DT_U_LONG: \
         retval.value.ullnum = from_eval1.value.ulnum OP from_eval2.value.ullnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.ullnum = from_eval1.value.stnum OP from_eval2.value.ullnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG_LONG: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.llnum = from_eval1.value.cnum OP from_eval2.value.llnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.llnum = from_eval1.value.scnum OP from_eval2.value.llnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.llnum = from_eval1.value.ucnum OP from_eval2.value.llnum; \
         break; \
      case DT_SHORT: \
         retval.value.llnum = from_eval1.value.shnum OP from_eval2.value.llnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.llnum = from_eval1.value.ushnum OP from_eval2.value.llnum; \
         break; \
      case DT_INT: \
         retval.value.llnum = from_eval1.value.inum OP from_eval2.value.llnum; \
         break; \
      case DT_U_INT: \
         retval.value.llnum = from_eval1.value.uinum OP from_eval2.value.llnum; \
         break; \
      case DT_LONG: \
         retval.value.llnum = from_eval1.value.lnum OP from_eval2.value.llnum; \
         break; \
      case DT_U_LONG: \
         retval.value.llnum = from_eval1.value.ulnum OP from_eval2.value.llnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.llnum = from_eval1.value.stnum OP from_eval2.value.llnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SIZE_T: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.stnum = from_eval1.value.cnum OP from_eval2.value.stnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.stnum = from_eval1.value.scnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.stnum = from_eval1.value.ucnum OP from_eval2.value.stnum; \
         break; \
      case DT_SHORT: \
         retval.value.stnum = from_eval1.value.shnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.stnum = from_eval1.value.ushnum OP from_eval2.value.stnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ulnum = from_eval1.value.cnum OP from_eval2.value.ulnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ulnum = from_eval1.value.scnum OP from_eval2.value.ulnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ulnum = from_eval1.value.ucnum OP from_eval2.value.ulnum; \
         break; \
      case DT_SHORT: \
         retval.value.ulnum = from_eval1.value.shnum OP from_eval2.value.ulnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ulnum = from_eval1.value.ushnum OP from_eval2.value.ulnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.lnum = from_eval1.value.cnum OP from_eval2.value.lnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.lnum = from_eval1.value.scnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.lnum = from_eval1.value.ucnum OP from_eval2.value.lnum; \
         break; \
      case DT_SHORT: \
         retval.value.lnum = from_eval1.value.shnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.lnum = from_eval1.value.ushnum OP from_eval2.value.lnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_INT: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.uinum = from_eval1.value.cnum OP from_eval2.value.uinum; \
         break; \
      case DT_S_CHAR: \
         retval.value.uinum = from_eval1.value.scnum OP from_eval2.value.uinum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.uinum = from_eval1.value.ucnum OP from_eval2.value.uinum; \
         break; \
      case DT_SHORT: \
         retval.value.uinum = from_eval1.value.shnum OP from_eval2.value.uinum; \
         break; \
      case DT_U_SHORT: \
         retval.value.uinum = from_eval1.value.ushnum OP from_eval2.value.uinum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_INT: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.inum = from_eval1.value.cnum OP from_eval2.value.inum; \
         break; \
      case DT_S_CHAR: \
         retval.value.inum = from_eval1.value.scnum OP from_eval2.value.inum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.inum = from_eval1.value.ucnum OP from_eval2.value.inum; \
         break; \
      case DT_SHORT: \
         retval.value.inum = from_eval1.value.shnum OP from_eval2.value.inum; \
         break; \
      case DT_U_SHORT: \
         retval.value.inum = from_eval1.value.ushnum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_SHORT: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ushnum = from_eval1.value.cnum OP from_eval2.value.ushnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ushnum = from_eval1.value.scnum OP from_eval2.value.ushnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ushnum = from_eval1.value.ucnum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SHORT: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.shnum = from_eval1.value.cnum OP from_eval2.value.shnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.shnum = from_eval1.value.scnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.shnum = from_eval1.value.ucnum OP from_eval2.value.shnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   default: \
      break; \
   } \
   retval.ti.dtype =  from_eval2.ti.dtype;\
} \
else \
{ \
   switch (from_eval1.ti.dtype) \
   { \
   case DT_LONG_DOUBLE: \
      retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ldnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.num = from_eval1.value.num OP from_eval2.value.num; \
      break; \
   case DT_U_LONG_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_LONG_LONG: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.ullnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.llnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_LONG_LONG: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.ullnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.llnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SIZE_T: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_SIZE_T: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_LONG: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_INT: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.uinum; \
         break; \
      case DT_INT: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_SIZE_T: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_LONG: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_INT: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.uinum; \
         break; \
      case DT_INT: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_SIZE_T: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_LONG: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_INT: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.uinum; \
         break; \
      case DT_INT: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_INT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_SIZE_T: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.stnum; \
         break; \
      case DT_U_LONG: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.lnum; \
         break; \
      case DT_U_INT: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.uinum; \
         break; \
      case DT_INT: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_INT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_SIZE_T: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.stnum; \
         break; \
      case DT_U_LONG: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.lnum; \
         break; \
      case DT_U_INT: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.uinum; \
         break; \
      case DT_INT: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_SHORT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_SHORT:\
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.ushnum; \
         break; \
      case DT_SHORT:\
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.shnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SHORT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_SHORT:\
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.ushnum; \
         break; \
      case DT_SHORT:\
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.shnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ucnum = from_eval1.value.ucnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ucnum = from_eval1.value.ucnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ucnum = from_eval1.value.ucnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_S_CHAR: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.scnum = from_eval1.value.scnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.scnum = from_eval1.value.scnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.scnum = from_eval1.value.scnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_CHAR: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.cnum = from_eval1.value.cnum OP from_eval2.value.cnum; \
         break;\
      case DT_S_CHAR: \
         retval.value.cnum = from_eval1.value.cnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.cnum = from_eval1.value.cnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   default: \
      break; \
   } \
   retval.ti.dtype =  from_eval1.ti.dtype;\
}
#elif defined(BITS64)
#define mac_bin_top(OP) retval.ti.dtype = max(from_eval1.ti.dtype, from_eval2.ti.dtype); \
if (sizes_classes[from_eval1.ti.dtype] > sizes_classes[from_eval2.ti.dtype]) \
{ \
   switch (from_eval1.ti.dtype) \
   { \
   case DT_LONG_DOUBLE: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ushnum; \
         break; \
      case DT_INT: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.inum; \
         break; \
      case DT_U_INT: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.uinum; \
         break; \
      case DT_LONG: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_LONG: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.llnum; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ullnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.stnum; \
         break; \
      case DT_FLOAT: \
      case DT_DOUBLE: \
         retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.num; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.ushnum; \
         break; \
      case DT_INT: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.inum; \
         break; \
      case DT_U_INT: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.uinum; \
         break; \
      case DT_LONG: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.lnum; \
         break; \
      case DT_U_LONG: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.llnum; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.ullnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.num = from_eval1.value.num OP from_eval2.value.stnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SIZE_T: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.ushnum; \
         break; \
      case DT_INT: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.inum; \
         break; \
      case DT_U_INT: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.uinum; \
         break; \
      case DT_LONG: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_LONG: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.ulnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.ushnum; \
         break; \
      case DT_INT: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.inum; \
         break; \
      case DT_U_INT: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.uinum; \
         break; \
      case DT_LONG: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_LONG: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.ulnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.ushnum; \
         break; \
      case DT_INT: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.inum; \
         break; \
      case DT_U_INT: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.uinum; \
         break; \
      case DT_LONG: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_LONG: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.ulnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_INT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_INT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.ucnum; \
         break; \
      case DT_SHORT: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.shnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_SHORT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SHORT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   default: \
      break; \
   } \
   retval.ti.dtype =  from_eval1.ti.dtype;\
} \
else if (sizes_classes[from_eval1.ti.dtype] < sizes_classes[from_eval2.ti.dtype]) \
{ \
   switch (from_eval2.ti.dtype) \
   { \
   case DT_LONG_DOUBLE: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ldnum = from_eval1.value.cnum OP from_eval2.value.ldnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ldnum = from_eval1.value.scnum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ldnum = from_eval1.value.ucnum OP from_eval2.value.ldnum; \
         break; \
      case DT_SHORT: \
         retval.value.ldnum = from_eval1.value.shnum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ldnum = from_eval1.value.ushnum OP from_eval2.value.ldnum; \
         break; \
      case DT_INT: \
         retval.value.ldnum = from_eval1.value.inum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_INT: \
         retval.value.ldnum = from_eval1.value.uinum OP from_eval2.value.ldnum; \
         break; \
      case DT_LONG: \
         retval.value.ldnum = from_eval1.value.lnum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_LONG: \
         retval.value.ldnum = from_eval1.value.ulnum OP from_eval2.value.ldnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.ldnum = from_eval1.value.llnum OP from_eval2.value.ldnum; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.ldnum = from_eval1.value.ullnum OP from_eval2.value.ldnum; \
         break; \
      case DT_SIZE_T: \
         retval.value.ldnum = from_eval1.value.stnum OP from_eval2.value.ldnum; \
         break; \
      case DT_FLOAT: \
      case DT_DOUBLE: \
         retval.value.ldnum = from_eval1.value.num OP from_eval2.value.ldnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.num = from_eval1.value.cnum OP from_eval2.value.num; \
         break; \
      case DT_S_CHAR: \
         retval.value.num = from_eval1.value.scnum OP from_eval2.value.num; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.num = from_eval1.value.ucnum OP from_eval2.value.num; \
         break; \
      case DT_SHORT: \
         retval.value.num = from_eval1.value.shnum OP from_eval2.value.num; \
         break; \
      case DT_U_SHORT: \
         retval.value.num = from_eval1.value.ushnum OP from_eval2.value.num; \
         break; \
      case DT_INT: \
         retval.value.num = from_eval1.value.inum OP from_eval2.value.num; \
         break; \
      case DT_U_INT: \
         retval.value.num = from_eval1.value.uinum OP from_eval2.value.num; \
         break; \
      case DT_LONG: \
         retval.value.num = from_eval1.value.lnum OP from_eval2.value.num; \
         break; \
      case DT_U_LONG: \
         retval.value.num = from_eval1.value.ulnum OP from_eval2.value.num; \
         break; \
      case DT_LONG_LONG: \
         retval.value.num = from_eval1.value.llnum OP from_eval2.value.num; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.num = from_eval1.value.ullnum OP from_eval2.value.num; \
         break; \
      case DT_SIZE_T: \
         retval.value.num = from_eval1.value.stnum OP from_eval2.value.num; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SIZE_T: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.stnum = from_eval1.value.cnum OP from_eval2.value.stnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.stnum = from_eval1.value.scnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.stnum = from_eval1.value.ucnum OP from_eval2.value.stnum; \
         break; \
      case DT_SHORT: \
         retval.value.stnum = from_eval1.value.shnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.stnum = from_eval1.value.ushnum OP from_eval2.value.stnum; \
         break; \
      case DT_INT: \
         retval.value.stnum = from_eval1.value.inum OP from_eval2.value.stnum; \
         break; \
      case DT_U_INT: \
         retval.value.stnum = from_eval1.value.uinum OP from_eval2.value.stnum; \
         break; \
      case DT_LONG: \
         retval.value.stnum = from_eval1.value.lnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_LONG: \
         retval.value.stnum = from_eval1.value.ulnum OP from_eval2.value.stnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG_LONG: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ullnum = from_eval1.value.cnum OP from_eval2.value.ullnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ullnum = from_eval1.value.scnum OP from_eval2.value.ullnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ullnum = from_eval1.value.ucnum OP from_eval2.value.ullnum; \
         break; \
      case DT_SHORT: \
         retval.value.ullnum = from_eval1.value.shnum OP from_eval2.value.ullnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ullnum = from_eval1.value.ushnum OP from_eval2.value.ullnum; \
         break; \
      case DT_INT: \
         retval.value.ullnum = from_eval1.value.inum OP from_eval2.value.ullnum; \
         break; \
      case DT_U_INT: \
         retval.value.ullnum = from_eval1.value.uinum OP from_eval2.value.ullnum; \
         break; \
      case DT_LONG: \
         retval.value.ullnum = from_eval1.value.lnum OP from_eval2.value.ullnum; \
         break; \
      case DT_U_LONG: \
         retval.value.ullnum = from_eval1.value.ulnum OP from_eval2.value.ullnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG_LONG: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.llnum = from_eval1.value.cnum OP from_eval2.value.llnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.llnum = from_eval1.value.scnum OP from_eval2.value.llnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.llnum = from_eval1.value.ucnum OP from_eval2.value.llnum; \
         break; \
      case DT_SHORT: \
         retval.value.llnum = from_eval1.value.shnum OP from_eval2.value.llnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.llnum = from_eval1.value.ushnum OP from_eval2.value.llnum; \
         break; \
      case DT_INT: \
         retval.value.llnum = from_eval1.value.inum OP from_eval2.value.llnum; \
         break; \
      case DT_U_INT: \
         retval.value.llnum = from_eval1.value.uinum OP from_eval2.value.llnum; \
         break; \
      case DT_LONG: \
         retval.value.llnum = from_eval1.value.lnum OP from_eval2.value.llnum; \
         break; \
      case DT_U_LONG: \
         retval.value.llnum = from_eval1.value.ulnum OP from_eval2.value.llnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ulnum = from_eval1.value.cnum OP from_eval2.value.ulnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ulnum = from_eval1.value.scnum OP from_eval2.value.ulnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ulnum = from_eval1.value.ucnum OP from_eval2.value.ulnum; \
         break; \
      case DT_SHORT: \
         retval.value.ulnum = from_eval1.value.shnum OP from_eval2.value.ulnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.ulnum = from_eval1.value.ushnum OP from_eval2.value.ulnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.lnum = from_eval1.value.cnum OP from_eval2.value.lnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.lnum = from_eval1.value.scnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.lnum = from_eval1.value.ucnum OP from_eval2.value.lnum; \
         break; \
      case DT_SHORT: \
         retval.value.lnum = from_eval1.value.shnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_SHORT: \
         retval.value.lnum = from_eval1.value.ushnum OP from_eval2.value.lnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_INT: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.uinum = from_eval1.value.cnum OP from_eval2.value.uinum; \
         break; \
      case DT_S_CHAR: \
         retval.value.uinum = from_eval1.value.scnum OP from_eval2.value.uinum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.uinum = from_eval1.value.ucnum OP from_eval2.value.uinum; \
         break; \
      case DT_SHORT: \
         retval.value.uinum = from_eval1.value.shnum OP from_eval2.value.uinum; \
         break; \
      case DT_U_SHORT: \
         retval.value.uinum = from_eval1.value.ushnum OP from_eval2.value.uinum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_INT: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.inum = from_eval1.value.cnum OP from_eval2.value.inum; \
         break; \
      case DT_S_CHAR: \
         retval.value.inum = from_eval1.value.scnum OP from_eval2.value.inum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.inum = from_eval1.value.ucnum OP from_eval2.value.inum; \
         break; \
      case DT_SHORT: \
         retval.value.inum = from_eval1.value.shnum OP from_eval2.value.inum; \
         break; \
      case DT_U_SHORT: \
         retval.value.inum = from_eval1.value.ushnum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_SHORT: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ushnum = from_eval1.value.cnum OP from_eval2.value.ushnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ushnum = from_eval1.value.scnum OP from_eval2.value.ushnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ushnum = from_eval1.value.ucnum OP from_eval2.value.ushnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SHORT: \
      switch (from_eval1.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.shnum = from_eval1.value.cnum OP from_eval2.value.shnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.shnum = from_eval1.value.scnum OP from_eval2.value.shnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.shnum = from_eval1.value.ucnum OP from_eval2.value.shnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   default: \
      break; \
   } \
   retval.ti.dtype =  from_eval2.ti.dtype;\
} \
else \
{ \
   switch (from_eval1.ti.dtype) \
   { \
   case DT_LONG_DOUBLE: \
      retval.value.ldnum = from_eval1.value.ldnum OP from_eval2.value.ldnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.num = from_eval1.value.num OP from_eval2.value.num; \
      break; \
   case DT_SIZE_T: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_SIZE_T: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.ullnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.stnum = from_eval1.value.stnum OP from_eval2.value.llnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_SIZE_T: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.ullnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.ullnum = from_eval1.value.ullnum OP from_eval2.value.llnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_SIZE_T: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.stnum; \
         break; \
      case DT_U_LONG_LONG: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.ullnum; \
         break; \
      case DT_LONG_LONG: \
         retval.value.llnum = from_eval1.value.llnum OP from_eval2.value.llnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_LONG: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_INT: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.uinum; \
         break; \
      case DT_INT: \
         retval.value.ulnum = from_eval1.value.ulnum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_LONG: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_LONG: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.lnum; \
         break; \
      case DT_U_INT: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.uinum; \
         break; \
      case DT_INT: \
         retval.value.lnum = from_eval1.value.lnum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_INT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_LONG: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.lnum; \
         break; \
      case DT_U_INT: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.uinum; \
         break; \
      case DT_INT: \
         retval.value.uinum = from_eval1.value.uinum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_INT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_LONG: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.ulnum; \
         break; \
      case DT_LONG: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.lnum; \
         break; \
      case DT_U_INT: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.uinum; \
         break; \
      case DT_INT: \
         retval.value.inum = from_eval1.value.inum OP from_eval2.value.inum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_SHORT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_SHORT:\
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.ushnum; \
         break; \
      case DT_SHORT:\
         retval.value.ushnum = from_eval1.value.ushnum OP from_eval2.value.shnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_SHORT: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_U_SHORT:\
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.ushnum; \
         break; \
      case DT_SHORT:\
         retval.value.shnum = from_eval1.value.shnum OP from_eval2.value.shnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.ucnum = from_eval1.value.ucnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.ucnum = from_eval1.value.ucnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.ucnum = from_eval1.value.ucnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_S_CHAR: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.scnum = from_eval1.value.scnum OP from_eval2.value.cnum; \
         break; \
      case DT_S_CHAR: \
         retval.value.scnum = from_eval1.value.scnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.scnum = from_eval1.value.scnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   case DT_CHAR: \
      switch (from_eval2.ti.dtype) \
      { \
      case DT_CHAR: \
         retval.value.cnum = from_eval1.value.cnum OP from_eval2.value.cnum; \
         break;\
      case DT_S_CHAR: \
         retval.value.cnum = from_eval1.value.cnum OP from_eval2.value.scnum; \
         break; \
      case DT_U_CHAR: \
      case DT_BYTE: \
         retval.value.cnum = from_eval1.value.cnum OP from_eval2.value.ucnum; \
         break; \
      default: \
         break; \
      } \
      break; \
   default: \
      break; \
   } \
   retval.ti.dtype =  from_eval1.ti.dtype;\
}
#endif


#define mac_cast(DEST, TYPE, SRC) switch (SRC.ti.dtype) \
{ \
case DT_CHAR: \
   DEST = (TYPE)SRC.value.cnum; \
   break; \
case DT_S_CHAR: \
   DEST = (TYPE)SRC.value.scnum; \
   break; \
case DT_U_CHAR: \
case DT_BYTE: \
   DEST = (TYPE)SRC.value.ucnum; \
   break; \
case DT_SHORT: \
   DEST = (TYPE)SRC.value.shnum; \
   break; \
case DT_U_SHORT: \
   DEST = (TYPE)SRC.value.ushnum; \
   break; \
case DT_INT: \
   DEST = (TYPE)SRC.value.inum; \
   break; \
case DT_U_INT: \
   DEST = (TYPE)SRC.value.uinum; \
   break; \
case DT_LONG: \
   DEST = (TYPE)SRC.value.lnum; \
   break; \
case DT_U_LONG: \
   DEST = (TYPE)SRC.value.ulnum; \
   break; \
case DT_LONG_LONG: \
   DEST = (TYPE)SRC.value.llnum; \
   break; \
case DT_U_LONG_LONG: \
   DEST = (TYPE)SRC.value.ullnum; \
   break; \
case DT_SIZE_T: \
   DEST = (TYPE)SRC.value.stnum; \
   break; \
case DT_FLOAT: \
case DT_DOUBLE: \
   DEST = (TYPE)SRC.value.num; \
   break; \
case DT_LONG_DOUBLE: \
   DEST = (TYPE)SRC.value.ldnum; \
   break; \
case DT_STRING: \
case DT_MEMORY: \
   DEST = (TYPE)(PVOIDCAST)SRC.value.str.tab; \
   break; \
case DT_POINTER: \
case DT_PFILE: \
case DT_PFPOS_T: \
case DT_HLIB: \
case DT_PFUNC: \
case DT_MEMADR: \
   DEST = (TYPE)(PVOIDCAST)SRC.value.genptr; \
   break; \
default: \
   break; \
}



#define mac_rel(BASE_L, LEFT, OP, BASE_R) switch (BASE_R.ti.dtype) \
{ \
case DT_CHAR: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.cnum; \
   break; \
case DT_S_CHAR: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.scnum; \
   break; \
case DT_U_CHAR: \
case DT_BYTE: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.ucnum; \
   break; \
case DT_SHORT: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.shnum; \
   break; \
case DT_U_SHORT: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.ushnum; \
   break; \
case DT_INT: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.inum; \
   break; \
case DT_U_INT: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.uinum; \
   break; \
case DT_LONG: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.lnum; \
   break; \
case DT_U_LONG: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.ulnum; \
   break; \
case DT_LONG_LONG: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.llnum; \
   break; \
case DT_U_LONG_LONG: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.ullnum; \
   break; \
case DT_SIZE_T: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.stnum; \
   break; \
case DT_FLOAT: \
case DT_DOUBLE: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.num; \
   break; \
case DT_LONG_DOUBLE: \
   retval.value.inum = BASE_L.value.LEFT OP BASE_R.value.ldnum; \
   break; \
default: \
   break; \
}



#define mac_rel_top_stcast(BASE_L, OP, BASE_R, STCAST) switch (BASE_L.ti.dtype) \
{ \
case DT_CHAR: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = BASE_L.value.cnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = BASE_L.value.cnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.cnum OP (char)BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = BASE_L.value.cnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.cnum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = BASE_L.value.cnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.cnum OP (int)BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = BASE_L.value.cnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.cnum OP (long)BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = BASE_L.value.cnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.cnum OP (long long)BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.cnum OP (STCAST)BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.cnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.cnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_S_CHAR: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = BASE_L.value.scnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = BASE_L.value.scnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.scnum OP (char)BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = BASE_L.value.scnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.scnum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = BASE_L.value.scnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.scnum OP (int)BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = BASE_L.value.scnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.scnum OP (long)BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = BASE_L.value.scnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.scnum OP (long long)BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.scnum OP (STCAST)BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.scnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.scnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_U_CHAR: \
case DT_BYTE: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = (char)BASE_L.value.ucnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = (char)BASE_L.value.ucnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = (char)BASE_L.value.ucnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.ucnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_SHORT: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = BASE_L.value.shnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = BASE_L.value.shnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.shnum OP BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = BASE_L.value.shnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.shnum OP (short)BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = BASE_L.value.shnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.shnum OP (int)BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = BASE_L.value.shnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.shnum OP (long)BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = BASE_L.value.shnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.shnum OP (long long)BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.shnum OP (STCAST)BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.shnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.shnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_U_SHORT: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = (short)BASE_L.value.ushnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.ushnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_INT: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = BASE_L.value.inum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = BASE_L.value.inum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.inum OP (char)BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = BASE_L.value.inum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.inum OP (short)BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = BASE_L.value.inum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.inum OP (int)BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = BASE_L.value.inum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.inum OP (long)BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = BASE_L.value.inum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.inum OP (long long)BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.inum OP (STCAST)BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.inum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.inum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_U_INT: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = (int)BASE_L.value.uinum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = (int)BASE_L.value.uinum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.uinum OP BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = (int)BASE_L.value.uinum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.uinum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = (int)BASE_L.value.uinum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.uinum OP BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = (int)BASE_L.value.uinum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.uinum OP BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = (int)BASE_L.value.uinum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.uinum OP BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.uinum OP BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.uinum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.uinum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_LONG: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.lnum OP (int)BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.lnum OP (long)BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.lnum OP (long long)BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.lnum OP (STCAST)BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.lnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_U_LONG: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = (long)BASE_L.value.ulnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = (long)BASE_L.value.ulnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.ulnum OP BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = (long)BASE_L.value.ulnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.ulnum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = (long)BASE_L.value.ulnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.ulnum OP BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = (long)BASE_L.value.ulnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.ulnum OP BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = (long)BASE_L.value.ulnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.ulnum OP BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.ulnum OP BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.ulnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.ulnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_LONG_LONG: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.llnum OP (long long)BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.llnum OP (STCAST)BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.llnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_U_LONG_LONG: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = (long long)BASE_L.value.ullnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = (long long)BASE_L.value.ullnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.ullnum OP BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = (long long)BASE_L.value.ullnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.ullnum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = (long long)BASE_L.value.ullnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.ullnum OP BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = (long long)BASE_L.value.ullnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.ullnum OP BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = (long long)BASE_L.value.ullnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.ullnum OP BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.ullnum OP BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.ullnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.ullnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_SIZE_T: \
   switch (BASE_R.ti.dtype) \
   { \
   case DT_CHAR: \
      retval.value.inum = (STCAST)BASE_L.value.stnum OP BASE_R.value.cnum; \
      break; \
   case DT_S_CHAR: \
      retval.value.inum = (STCAST)BASE_L.value.stnum OP BASE_R.value.scnum; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      retval.value.inum = BASE_L.value.stnum OP BASE_R.value.ucnum; \
      break; \
   case DT_SHORT: \
      retval.value.inum = (STCAST)BASE_L.value.stnum OP BASE_R.value.shnum; \
      break; \
   case DT_U_SHORT: \
      retval.value.inum = BASE_L.value.stnum OP BASE_R.value.ushnum; \
      break; \
   case DT_INT: \
      retval.value.inum = (STCAST)BASE_L.value.stnum OP BASE_R.value.inum; \
      break; \
   case DT_U_INT: \
      retval.value.inum = BASE_L.value.stnum OP BASE_R.value.uinum; \
      break; \
   case DT_LONG: \
      retval.value.inum = (STCAST)BASE_L.value.stnum OP BASE_R.value.lnum; \
      break; \
   case DT_U_LONG: \
      retval.value.inum = BASE_L.value.stnum OP BASE_R.value.ulnum; \
      break; \
   case DT_LONG_LONG: \
      retval.value.inum = (STCAST)BASE_L.value.stnum OP BASE_R.value.llnum; \
      break; \
   case DT_U_LONG_LONG: \
      retval.value.inum = BASE_L.value.stnum OP BASE_R.value.ullnum; \
      break; \
   case DT_SIZE_T: \
      retval.value.inum = BASE_L.value.stnum OP BASE_R.value.stnum; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      retval.value.inum = BASE_L.value.stnum OP BASE_R.value.num; \
      break; \
   case DT_LONG_DOUBLE: \
      retval.value.inum = BASE_L.value.stnum OP BASE_R.value.ldnum; \
      break; \
   default: \
      break; \
   } \
   break; \
case DT_FLOAT: \
case DT_DOUBLE: \
   mac_rel(BASE_L, num, OP, BASE_R) \
   break; \
case DT_LONG_DOUBLE: \
   mac_rel(BASE_L, ldnum, OP, BASE_R) \
   break; \
default: \
   break; \
}


#if defined(BITS32)
#define mac_rel_top(BASE_L, OP, BASE_R) mac_rel_top_stcast(BASE_L, OP, BASE_R, long)
#elif defined(BITS64)
#define mac_rel_top(BASE_L, OP, BASE_R) mac_rel_top_stcast(BASE_L, OP, BASE_R, long long)
#endif


#define mac_rel_left_ptr_lcast(BASE_L, OP, BASE_R, LCAST, LMEMBER) switch (BASE_R.ti.dtype) \
{ \
case DT_CHAR: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP BASE_R.value.cnum; \
   break; \
case DT_S_CHAR: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP BASE_R.value.scnum; \
   break; \
case DT_U_CHAR: \
case DT_BYTE: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP (char)BASE_R.value.ucnum; \
   break; \
case DT_SHORT: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP BASE_R.value.shnum; \
   break; \
case DT_U_SHORT: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP (short)BASE_R.value.ushnum; \
   break; \
case DT_INT: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP BASE_R.value.llnum; \
   break; \
case DT_U_INT: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP (int)BASE_R.value.ullnum; \
   break; \
case DT_LONG: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP BASE_R.value.lnum; \
   break; \
case DT_U_LONG: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP (long)BASE_R.value.ulnum; \
   break; \
case DT_LONG_LONG: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP BASE_R.value.llnum; \
   break; \
case DT_U_LONG_LONG: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP (long long)BASE_R.value.ullnum; \
   break; \
case DT_SIZE_T: \
   retval.value.LMEMBER = (LCAST)BASE_L.value.genptr OP (LCAST)BASE_R.value.stnum; \
   break; \
case DT_FLOAT: \
case DT_DOUBLE: \
   retval.value.LMEMBER = (double)(LCAST)BASE_L.value.genptr OP BASE_R.value.num; \
   break; \
case DT_LONG_DOUBLE: \
   retval.value.LMEMBER = (long double)(LCAST)BASE_L.value.genptr OP BASE_R.value.ldnum; \
   break; \
default: \
   break; \
}



#if defined(BITS32)
   #define mac_rel_left_ptr(BASE_L, OP, BASE_R) mac_rel_left_ptr_lcast(BASE_L, OP, BASE_R, long, lnum)
   #define PVOIDCAST long
#elif defined(BITS64)
   #define mac_rel_left_ptr(BASE_L, OP, BASE_R) mac_rel_left_ptr_lcast(BASE_L, OP, BASE_R, long long, llnum)
   #define PVOIDCAST long long
#endif



#define mac_compare(OP1, OP2) from_eval1 = eval(to_eval->childset[0]); \
from_eval2 = eval(to_eval->childset[1]); \
if (from_eval1.ti.dtype >= DT_CHAR && from_eval1.ti.dtype <= DT_LONG_DOUBLE && \
   from_eval2.ti.dtype >= DT_CHAR && from_eval2.ti.dtype <= DT_LONG_DOUBLE && \
   from_eval1.ti.nderef == 0 && from_eval2.ti.nderef == 0) \
{ \
   retval.ti.dtype = DT_INT; \
   mac_rel_top(from_eval1, OP1, from_eval2) \
} \
else if (from_eval2.ti.dtype >= DT_CHAR && from_eval2.ti.dtype <= DT_LONG_DOUBLE && \
   from_eval1.ti.nderef > 0 && from_eval2.ti.nderef == 0) \
{ \
   retval.ti.dtype = DT_INT; \
   mac_rel_left_ptr(from_eval1, OP1, from_eval2) \
} \
else if (from_eval1.ti.dtype >= DT_CHAR && from_eval1.ti.dtype <= DT_LONG_DOUBLE && \
   from_eval1.ti.nderef == 0 && from_eval2.ti.nderef > 0) \
{ \
   retval.ti.dtype = DT_INT; \
   mac_rel_left_ptr(from_eval2, OP2, from_eval1) \
} \
else if (from_eval1.ti.nderef > 0 && from_eval2.ti.nderef > 0) \
{ \
   retval.value.inum = from_eval1.value.genptr OP1 from_eval2.value.genptr; \
   retval.ti.dtype = DT_INT; \
} \
else \
{ \
   yyerror("Type error for operator \"" #OP1 "\"."); \
   abort_called = 1; \
   free_data(from_eval1); \
   free_data(from_eval2); \
   break; \
}



#define mac_ppmm(OP) switch (to_change->ti.dtype) \
{ \
   case DT_CHAR: \
      to_change->value.cnum OP; \
      break; \
   case DT_S_CHAR: \
      to_change->value.scnum OP; \
      break; \
   case DT_U_CHAR: \
   case DT_BYTE: \
      to_change->value.ucnum OP; \
      break; \
   case DT_SHORT: \
      to_change->value.shnum OP; \
      break; \
   case DT_U_SHORT: \
      to_change->value.ushnum OP; \
      break; \
   case DT_INT: \
      to_change->value.inum OP; \
      break; \
   case DT_U_INT: \
      to_change->value.uinum OP; \
      break; \
   case DT_LONG: \
      to_change->value.lnum OP; \
      break; \
   case DT_U_LONG: \
      to_change->value.ulnum OP; \
      break; \
   case DT_LONG_LONG: \
      to_change->value.llnum OP; \
      break; \
   case DT_U_LONG_LONG: \
      to_change->value.ullnum OP; \
      break; \
   case DT_SIZE_T: \
      to_change->value.stnum OP; \
      break; \
   case DT_FLOAT: \
   case DT_DOUBLE: \
      to_change->value.num OP; \
      break; \
   case DT_LONG_DOUBLE: \
      to_change->value.ldnum OP; \
      break; \
   default: \
      break; \
}



#define mac_gc_switch switch (pdat->ti.dtype) \
{ \
case DT_STRING: \
case DT_MEMORY: \
   if (pt == PT_CHAR_TAB) \
   { \
      if (pdat->value.str.tab == aptr.str_tab) return 1; \
   } \
   break; \
case DT_POINTER: \
   if (pt == PT_NODE) \
   { \
      if (pdat->value.ptr == aptr.ptr) return 1; \
   } \
   break; \
case DT_OBJECT: \
   if (pt_in_object(pdat->value.pObject, aptr, pt)) return 1; \
   break; \
case DT_ARRAY: \
   if (pt_in_array(pdat->value.pArray, aptr, pt)) return 1; \
   break; \
case DT_LIST: \
   if (pt_in_list(pdat->value.pList, aptr, pt)) return 1; \
   break; \
case DT_PFPOS_T: \
   if (pt == PT_PFPOS_T) \
   { \
      if (pdat->value.pfpos == aptr.pfpos) return 1; \
   } \
   break; \
default: \
   break; \
}

#endif
