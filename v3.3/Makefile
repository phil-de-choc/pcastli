CC=gcc

x86: main.c pcastli.tab.c eval.c init.c lexer.c tree.c util.c gcollection.c slib.c treetocode.c
	$(CC) main.c pcastli.tab.c eval.c init.c lexer.c tree.c util.c gcollection.c slib.c treetocode.c -lm -lreadline -lncurses -rdynamic -ldl -o pcastli -Wall

x64: main.c pcastli.tab.c eval.c init.c lexer.c tree.c util.c gcollection.c slib.c asmcall.c treetocode.c
	$(CC) main.c pcastli.tab.c eval.c init.c lexer.c tree.c util.c gcollection.c slib.c asmcall.c treetocode.c -lm -lreadline -lncurses -rdynamic -ldl -o pcastli -Wall -D_FORTIFY_SOURCE=0

clean:
	rm *.o
