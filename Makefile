CXXFLAGS := -std=c++17 -O3

.PHONY: all clean start lexer parser

all:
	start

start: lexer parser

lexer:
	cd Lexer && g++ $(CXXFLAGS) -o lexer lexer.cpp && ./lexer

parser:
	cd Parser && g++ $(CXXFLAGS) -o parser parser.cpp && ./parser

clean:
	@echo "Cleaning up..."
	cd Lexer && $(RM) lexer
	cd Parser && $(RM) parser