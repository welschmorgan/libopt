COMM_DIR = ../common
LIBFT_DIR = ../../libft
LIBOPT_DIR = ../..

HDRS=$(shell find . -maxdepth 1 -name "*.h") $(shell find common -maxdepth 1 -name "*.h")
SRCS=$(shell find . -maxdepth 1 -name "*.c")
CSRCS=$(shell find $(COMM_DIR) -maxdepth 1 -name "*.c") $(shell find ../.. -mindepth 1 -maxdepth 1 -name "*.c")
OBJS=$(patsubst %.c,%.o,$(SRCS))
COBJS=$(patsubst %.c,%.o,$(CSRCS))
APPS=$(patsubst %.c,%.run,$(SRCS))
DSYMS=$(patsubst %.c,%.run.dSYM,$(SRCS))
NOBJS=$(shell echo "$(APPS) $(OBJS)" | wc -w)

CFLAGS = -Wall -Wextra -Werror -I. -I.. -I../.. -I$(LIBOPT_DIR)/include -I$(LIBFT_DIR)/includes -I$(COMM_DIR) -g
LDFLAGS = -L$(LIBFT_DIR) -L$(LIBOPT_DIR) -lft -lopt_d

CC = gcc
LD = $(CC)

SHELL=/bin/bash
COUNT=1
VOID_STDERR=2>/dev/null

$(COMM_DIR)/%.o: $(COMM_DIR)/%.c
	@PRCT=$$( echo "scale=1; $(COUNT) / $(NOBJS) * 100.0" | bc $(VOID_STDERR) ); \
	printf "\r\033[K[$(CC)|%s%%] compiling %s into %s ... " "$$PRCT" "$<" "$@"; \
	$(CC) $(CFLAGS) -c -o $@ $^; \
	printf "\r\033[K[$(CC)|%s%%] compiled %s into %s ... " "$$PRCT" "$<" "$@"; \

%.o: %.c
	@PRCT=$$( echo "scale=1; $(COUNT) / $(NOBJS) * 100.0" | bc $(VOID_STDERR) ); \
	printf "\r\033[K[$(CC)|%s%%] compiling %s into %s ... " "$$PRCT" "$<" "$@"; \
	$(CC) $(CFLAGS) -c -o $@ $^; \
	$(eval COUNT = $(shell echo "$(COUNT) + 1" | bc) ) \
	printf "\r\033[K[$(CC)|%s%%] compiled %s into %s ... " "$$PRCT" "$<" "$@"; \

%.run: %.o $(COBJS) $(LIBFT_DIR)/libft.a $(LIBOPT_DIR)/libopt_d.a
	@PRCT=$$( echo "scale=1; $(COUNT) / $(NOBJS) * 100.0" | bc $(VOID_STDERR)); \
	printf "\r\033[K[$(CC)|%s%%] linking $@" "$$PRCT" ; \
	$(LD) -o $@ $^ $(LDFLAGS) ; \
	$(eval COUNT = $(shell echo "$(COUNT) + 1" | bc) ) \
	printf "\r\033[K[$(CC)|%s%%] linked $@\n" "$$PRCT";

all: $(COBJS) $(OBJS) $(APPS)

clean:
	@NOBJS=$$(find . -name "*.o" | wc -w | sed 's/ //g'); \
	[ $$NOBJS -gt 0 ] && echo "$$(basename $$(pwd)): removing $$NOBJS object(s)."; \
	rm -f $(OBJS) $(COBJS);

fclean: clean
	@NAPPS=$$(find . -name "*.run" -type f | grep -v "dSYM" | wc -w | sed 's/ //g' ); \
	[ $$NAPPS -gt 0 ] && echo "$$(basename $$(pwd)): removing $$NAPPS target(s)."; \
	rm -f $(APPS)
	@if [ -d "$(DSYMS)" ]; then rm -rf $(DSYMS); fi

re: fclean $(APPS)

.PHONY: clean fclean re all
