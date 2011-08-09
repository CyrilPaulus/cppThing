.PHONY: src_all src_clean src_distclean
SUBDIRS := $(DIR) $(DIR)/entity $(DIR)/utils $(DIR)/network
src_EXEC := $(DIR)/a.out
src_SRCS := $(foreach D,$(SUBDIRS),$(wildcard $D/*.cpp))
src_OBJS := $(src_SRCS:.cpp=.o)

src_all: $(src_EXEC)

$(src_EXEC): $(src_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(src_OBJS) -o $(src_EXEC)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

src_clean:
	rm -rf $(src_OBJS)

src_distclean:
	rm -rf $(src_EXEC)
