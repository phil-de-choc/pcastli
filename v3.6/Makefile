CC=gcc

x86: main.c pcastli.tab.c eval.c init.c lexer.c tree.c util.c gcollection.c slib.c treetocode.c
	$(CC) main.c pcastli.tab.c eval.c init.c lexer.c tree.c util.c gcollection.c slib.c treetocode.c -lm -lreadline -lncurses -rdynamic -ldl -o pcastli -Wall

x64: main.c pcastli.tab.c eval.c init.c lexer.c tree.c util.c gcollection.c slib.c asmcall.c treetocode.c
	$(CC) main.c pcastli.tab.c eval.c init.c lexer.c tree.c util.c gcollection.c slib.c asmcall.c treetocode.c -lm -lreadline -lncurses -rdynamic -ldl -o pcastli -Wall -D_FORTIFY_SOURCE=0

dos: main.c pcastl~1.c eval.c init.c lexer.c tree.c util.c gcolle~1.c slib.c treeto~1.c
	$(CC) main.c pcastl~1.c eval.c init.c lexer.c tree.c util.c gcolle~1.c slib.c treeto~1.c -lm -s -o pcastli.exe -Wall

clean:
	rm *.o
