; by Parent and Childset Accessible Syntax Tree Language Interpreter
; Copyright (C) 2008-2018  Philippe Choquette

; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU Lesser General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU Lesser General Public License for more details.

; You should have received a copy of the GNU Lesser General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.

.data
rsp_init QWORD 0
raw_args QWORD 0
funcpt QWORD 0
chunkfloat QWORD 0
outregs QWORD 0
arg1 QWORD 0
arg2 QWORD 0
arg3 QWORD 0
arg4 QWORD 0
fl1 QWORD 0
fl2 QWORD 0
fl3 QWORD 0
fl4 QWORD 0
zeroarg QWORD 0
outxmmd QWORD 0
outxmms DWORD 0

.code
; parameters: nchunks(rcx), raw_args(rdx), funcpt(r8), chunkfloat(r9), outregs(rsp+40)
asmcall PROC
  mov funcpt, r8
  mov rsp_init, rsp
  
  mov rax, [rsp+40]
  mov outregs, rax

  cmp rcx, 4
  jg morefour
  sub rsp, 8
  jmp lessfour

morefour:
  mov r8, rcx
  shr r8, 1
  jc endalign
  sub rsp, 8

lessfour:
  cmp rcx, 0
  je tocall

endalign:
  mov chunkfloat, r9
  mov raw_args, rdx

  dec rcx
  mov rax, rcx
  mov r8, 8
  mul r8
  add rax, raw_args
  mov r8, [rax]
  mov arg1, r8
  mov r9, rax

  mov rax, rcx
  mov rdx, 8
  mul rdx
  add rax, chunkfloat
  mov rdx, [rax]
  cmp rdx, 0
  je endif1
  mov fl1, r8
endif1:

  cmp rcx, 0
  je tocall

  dec rcx
  sub r9, 8
  mov r8, [r9]
  mov arg2, r8

  sub rax, 8
  mov rdx, [rax]
  cmp rdx, 0
  je endif2
  mov fl2, r8
endif2:

  jrcxz tocall

  dec rcx
  sub r9, 8
  mov r8, [r9]
  mov arg3, r8

  sub rax, 8
  mov rdx, [rax]
  cmp rdx, 0
  je endif3
  mov fl3, r8
endif3:

  jrcxz tocall

  dec rcx
  sub r9, 8
  mov r8, [r9]
  mov arg4, r8

  sub rax, 8
  mov rdx, [rax]
  cmp rdx, 0
  je endif4
  mov fl4, r8
endif4:

  jrcxz tocall
  
  mov rax, raw_args
@@:
  push [rax]
  add rax, 8
  loop @B

tocall:
  mov rcx, arg1
  mov rdx, arg2
  mov r8,  arg3
  mov r9,  arg4
  movq xmm0, fl1
  movq xmm1, fl2
  movq xmm2, fl3
  movq xmm3, fl4
  sub rsp, 32
  call funcpt
  mov rcx, outregs
  mov [rcx], rax
  add rcx, 8
  movsd outxmmd, xmm0
  mov rdx, outxmmd
  mov [rcx], rdx
  add rcx, 8
  movss outxmms, xmm0
  mov edx, outxmms
  mov [rcx], edx
  mov rsp, rsp_init

  RET
asmcall ENDP

end
