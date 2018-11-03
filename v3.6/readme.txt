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
I used Bison v3.0.4 under Cygwin v2.3.0.


Windows Compilation
-------------------

I did use Visual Studio Community 2017 with the Windows 10 SDK.
Open VS2017\pcastli.sln and select desired configuration (Debug or Release) and 
(x64 or Win32).


32-bit Linux compilation
------------------------

Make sure you have gcc, libreadline-dev, libncurses5-dev and make installed.

The command:

make x86

Builds the interpreter.


64-bit Linux compilation
------------------------

Make sure you have gcc, libreadline-dev, libncurses5-dev and make installed.

The command:

make x64

Builds the interpreter.


macOS High Sierra compilation
-------------------------------

Make sure you have Xcode with the Command Line Tools installed.

The command:

make x64

Builds the interpreter.
