.PHONY: src_all src_clean src_distclean
SUBDIRS := $(DIR) $(DIR)/entity $(DIR)/utils $(DIR)/network $(DIR)/ui $(DIR)/menu
src_EXEC := $(DIR)/a.out
src_DS := $(DIR)/ds
src_SRCS := $(foreach D,$(SUBDIRS),$(wildcard $D/*.cpp))
src_OBJS := $(src_SRCS:.cpp=.o)

src_all: $(src_EXEC)

$(src_EXEC): $(src_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(src_OBJS) -o $(src_EXEC)

$(src_DS): $(src_OBJS)
	$(CC) $(CFLAGS) dedi/dedi.cpp config.o server.o ticker.o network/*.o entity/*.o utils/*.o -lsfml-network -lsfml-system -lenet -o $(src_DC)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

src_clean:
	rm -rf $(src_OBJS)

src_distclean:
	rm -rf $(src_EXEC)
