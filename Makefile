CC = g++
CFLAGS = -Wall -std=c++11

parser: parser.tab.o lex.yy.o  type.o
	$(CC) $(CFLAGS) -o $@ $^
	
parser.tab.o: parser.tab.cpp parser.tab.hpp
	$(CC) $(CFLAGS) -c -o $@ $<
	
parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d -v $<
	
lex.yy.o: lex.yy.c parser.tab.hpp
	$(CC)  -c -std=c++11 -o $@ $<
	
lex.yy.c: leks.l
	flex $<


type.o: type.cpp type.hpp
	g++ -Wall -c -std=c++11  $< -o $@
	
.PHONY: clean

clean:
	rm -rf *~ *tab.* lex.yy.* *.o parser *.output 