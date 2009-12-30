
CC = gcc
NAME = cssmin
BIN = bin/$(NAME)
DEST = /usr/bin/$(NAME)
CFILES = src/*.c
CFLAGS = -std=c99

all: build 
	
install: $(BIN)
	cp $(BIN) $(DEST) 
	@echo CSSmin `cssmin --version` installed
	
uninstall: $(DEST)
	rm -f $(DEST)
	
build:
	@mkdir -p bin
	@$(CC) $(CFLAGS) $(CFILES) -o $(BIN)
	
test: all
	@./$(BIN) < examples/style.css > examples/style.min.css
	@./$(BIN) < examples/style.css && echo

inspect: test
	@open -a Safari examples/demo.html
	@open -a Safari examples/demo.min.html
	
benchmark: 
	@./$(BIN) < examples/large.css > /dev/null