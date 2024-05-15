CXXFLAGS := -std=c++17 -O3

.PHONY: all clean start lexer parser generate render test

all:
	start
	test

start: lexer parser generate


lexer:
	cd Lexer && g++ $(CXXFLAGS) -o lexer lexer.cpp && ./lexer

parser:
	cd Parser && g++ $(CXXFLAGS) -o parser parser.cpp && ./parser

generate:
	pwd
	python Common/autogenerate.py

clean:
	@echo "Cleaning up..."
	cd Lexer && $(RM) lexer
	cd Parser && $(RM) parser
	cd Common && $(RM) *.gv *.pdf