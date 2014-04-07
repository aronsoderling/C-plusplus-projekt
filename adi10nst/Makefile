#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CXXFLAGS =  -g -O2 -Wall -W -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast 
CXXFLAGS += -std=c++11 
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS += -stdlib=libc++

all: libclientserver.a mem_server db_server client

# Create the library; ranlib is for Darwin and maybe other systems.
# Doesn't seem to do any damage on other systems.

libclientserver.a: src/connection.o src/server.o
	ar rv libclientserver.a  src/connection.o src/server.o
	ranlib libclientserver.a

db_server: src/db_server.o src/news_server.o src/message_handler.o src/server.o src/connection.o src/command.o
	$(CC) $^ -o bin/db_server
mem_server: src/mem_server.o src/news_server.o src/message_handler.o src/server.o src/connection.o src/command.o
	$(CC) $^ -o bin/mem_server
client: src/client.o src/connection.o src/message_handler.o src/command.o
	$(CC) $^ -o bin/client

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f src/*.o libclientserver.a src/*.d

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
SRC = $(wildcard src/*.cc)
include $(SRC:.cc=.d)
