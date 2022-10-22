include config.mk

default: $(BIN)

deps: lib move

################################
# COVERAGE CODE SUPPORT (gcov) #
################################

cov: ccov ldcov

ccov: $(OBJ:=.c) $(MAIN:=.c)
	$(CC) $(CFLAGS) $(COVFLAGS) -c $^

ldcov: $(OBJ:=.o)
	$(CC) $(LDFLAGS) $(COVLDFLAGS) $(OBJ:=.o) $(MAIN:=.o) -o $(BIN)

#####################
# COMPILATION RULES #
#####################
deps: lib move

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ:=.o) $(MAIN:=.o) $(HEADER:=.h)
	$(CC) $(LDFLAGS) $^ -o $@

# Generate static library rule
lib: $(OBJ:=.o)
	$(AR) -rcs $(LIB) $^

# move library and header to destiny directory
move:
	mkdir -p $(LIBDEST)
	mkdir -p $(INCDEST)
	cp -f $(LIB) $(LIBDEST)
	cp -f $(HEADER:=.h) $(INCDEST)

run: default
	@./$(BIN)

install: $(BIN)
	mkdir -p $(DEST)/bin
	cp -f cstatusbar $(DEST)/bin
	chmod 755 $(DEST)/bin/$(BIN)

uninstall:
	rm -f $(DEST)/bin/$(BIN)

clean:
	rm -f $(BIN) modules/*.o *.o *.so *.a

cleanall:
	rm -f $(BIN) modules/*.o *.o *.so *.a *.g* shadow

.PHONY: install uninstall clean run move lib deps
