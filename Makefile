CC := g++

CFLAGS := -g -Wall
LDFLAGS := -lm -lsfml-graphics -lsfml-system

EXEC:= a.out
EXPORT := export.tar.gz
DIST = $(wildcard *)

all: src_all
	cp -a $(src_EXEC) $(EXEC)

clean: src_clean
	rm -rf $(EXEC)

distclean: clean src_distclean
	rm -rf $(EXPORT)

dist: distclean
	tar czf $(EXPORT) $(DIST)

DIR:= src
include src/Rules.mk
