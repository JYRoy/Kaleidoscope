SOURCES = $(shell find ast lexer logger parser -name '*.cpp')
HEADERS = $(shell find ast lexer logger parser -name '*.h')
OBJ = ${SOURCES:.cpp=.o}

CC = clang++
CFLAGS = -g -O3 -I llvm/include -I llvm/build/include -I ./
LLVMFLAGS = `llvm-config --cxxflags --ldflags --system-libs --libs all`

.PHONY: main

main: main.cpp ${OBJ}
	${CC} ${CFLAGS} ${LLVMFLAGS} ${OBJ} $< -o $@

clean:
	rm -r ${OBJ}

%.o: %.cpp ${HEADERS}
	${CC} ${CFLAGS} ${LLVMFLAGS} -c $< -o $@
