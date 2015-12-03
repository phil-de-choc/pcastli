-------------------------------------------------------------------------------
by Parent and Childset Accessible Syntax Tree Language Interpreter source notes
-------------------------------------------------------------------------------

Making Changes
--------------

To make changes to the interpreter, do not try to modify pcastli.tab.c or 
pcastli.tab.h.  These files are produced by GNU Bison.
To get updated pcastli.tab.c and pcastli.tab.h, install and run Bison.
The command:

bison pcastli.y -d

Produces the pcastli.tab.c and pcastli.tab.h files.
I did use Bison v3.0.4 under Cygwin v2.3.0.


32-bit Windows Compilation
--------------------------

I did use cl from Visual Studio Express 2013 with the VS2013 x86 Native Tools Command Prompt.
The command:

cl /O2 main.c pcastli.tab.c eval.c init.c lexer.c tree.c gcollection.c util.c slib.c /Fepcastli32.exe

Builds the interpreter.


64-bit Windows Compilation
--------------------------

I did use cl and ml64 from Visual Studio Express 2013 with the VS2013 x64 Cross Tools Command Prompt.
The commands:

ml64 asmcall.asm /c /Foasmcall.obj
cl /O2 main.c pcastli.tab.c eval.c init.c lexer.c tree.c gcollection.c util.c slib.c asmcall.obj /Fepcastli64.exe

Build the interpreter.


32-bit Linux compilation
------------------------

Make sure you have gcc, libreadline-dev and libncurses-dev installed.

The command:

make x86

Builds the interpreter.


64-bit Linux compilation
------------------------

Make sure you have gcc, libreadline-dev and libncurses-dev installed.

The command:

make x64

Builds the interpreter.


Mac OS X Yosemite compilation
-----------------------------

Make sure you have Xcode with the Command Line Tools installed.

The command:

make x64

Builds the interpreter.
