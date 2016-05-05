COMM_DIR=common
TPL_DIR=template

TSTS=$(shell find . -maxdepth 1 -mindepth 1 -type d | grep -v "$(COMM_DIR)\|$(TPL_DIR)" )
HDRS=$(shell find . -maxdepth 1 -name "*.h") $(shell find common -maxdepth 1 -name "*.h")
SRCS=$(shell find . -maxdepth 1 -name "*.c")
CSRCS=$(shell find common -maxdepth 1 -name "*.c")
OBJS=$(patsubst %.c,%.o,$(SRCS))
COBJS=$(patsubst %.c,%.o,$(CSRCS))
APPS=$(patsubst %.c,%.run,$(SRCS))
NOBJS=$(shell echo $(OBJS) $(COBJS) | wc -w)

LIBFT_DIR=../libft
LIBOPT_DIR = ..


CFLAGS = -Wall -Wextra -Werror -I. -I.. -I$(LIBOPT_DIR)/include -I$(LIBFT_DIR)/includes -I$(COMM_DIR) -g
LDFLAGS = -L$(LIBFT_DIR) -lft -L$(LIBOPT_DIR) -lopt_d

CC = gcc
LD = $(CC)

SHELL=/bin/bash
COUNT=0
VOID_STDERR=2>/dev/null

$(COMM_DIR)/%.o: $(COMM_DIR)/%.c
	@PRCT=$$( echo "scale=1; $(COUNT) / $(NOBJS) * 100.0" | bc $(VOID_STDERR) ); \
	printf "\r\033[K[$(CC)|%s%%] compiling %s into %s ... " "$$PRCT" "$<" "$@"; \
	$(CC) $(CFLAGS) -c -o $@ $^; \
	printf "\r\033[K[$(CC)|%s%%] compiled %s into %s ... " "$$PRCT" "$<" "$@"; \

all: $(COBJS)
	@for TEST in $(TSTS); do make -C $$TEST -f ../$(TPL_DIR)/Makefile; done

common:
	make -C common

clean:
	@NOBJS=$$(find . -mindepth 1 -maxdepth 1 -name "*.o" | wc -w | sed 's/ //g' ); \
	[ $$NOBJS -gt 0 ] && echo "common: removing $$NOBJS object(s): $(OBJS) $(COBJS)."; \
	rm -f $(OBJS) $(COBJS)
	@for TEST in $(TSTS); do make clean -C $$TEST -f ../$(TPL_DIR)/Makefile; done

fclean: clean
	@NAPPS=$$(find . -mindepth 1 -maxdepth 1 -name "*.run" | grep -v "dSYM" | wc -w | sed 's/ //g' ); \
	[ $$NAPPS -gt 0 ] && echo "common: removing $$NAPPS target(s)."; \
	rm -f $(APPS)
	@for TEST in $(TSTS); do make fclean -C $$TEST -f ../$(TPL_DIR)/Makefile; done

re: fclean all

.PHONY: clean fclean re all common
