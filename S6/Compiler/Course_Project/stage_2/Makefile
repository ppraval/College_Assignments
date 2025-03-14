all: 
	lex bcs22.l
	yacc -d bcs22.y
	gcc lex.yy.c y.tab.c -o compiler.exe

clean: 
	rm -f lex.yy.c y.tab.c y.tab.h compiler.exe