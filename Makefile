#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/11/19 14:02:18 by mwelsch           #+#    #+#              #
#    Updated: 2016/04/23 19:48:15 by mwelsch          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = libopt.a
NAME_D = libopt_d.a

NORM_FILE = .last-norm

FT_DIR = ./libft

LIB_CFLAGS = -I$(FT_DIR)/includes/ -I./ -Iinclude/
LIB_LDFLAGS =

INC_DIR = include
SRC_DIR = source
OBJ_DIR = object

COUNT=0

SRCS = $(shell ls $(SRC_DIR) | grep \.c)
INCS_ = $(DIST_INC_PREFIX) $(shell ls $(INC_DIR) | grep \.h | grep -v libopt\.h)
INCS = $(shell echo $(INCS_) | tr ' ' '\n' | cat -n - | sort -uk2 | sort -nk1 | cut -f2-)

OBJ_DEBUG_DIR = $(OBJ_DIR)/debug
OBJ_RELEASE_DIR = $(OBJ_DIR)/release
OBJS = $(patsubst %.c,$(OBJ_RELEASE_DIR)/%.o,$(SRCS))
OBJS_D = $(patsubst %.c,$(OBJ_DEBUG_DIR)/%.o,$(SRCS))
NOBJS = $(shell echo $(OBJS) | wc -w)

VERSION_FILE=.version
VERSION_CHANGES = .changelog

VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = $(shell cat $(VERSION_FILE))
REV=$(shell date +'%l%M')
VERSION_REV = $(shell echo $(REV) | tr -d ' ')

VERSION_TIME = $(shell date +'%H:%M:%S')
VERSION_DATE = $(shell date +'%Y/%m/%d')

VERSION_FLAGS = -D__VERSION_DATE=$(VERSION_DATE)
VERSION_FLAGS += -D__VERSION_BUILD=$(VERSION_BUILD)
VERSION_FLAGS += -D__VERSION_MAJOR=$(VERSION_MINOR)
VERSION_FLAGS += -D__VERSION_MINOR=$(VERSION_MAJOR)
VERSION_FLAGS += -D__VERSION_REV=$(VERSION_REV)

FLAGS = -Wall -Wextra -Werror -std=c89 $(VERSION_FLAGS)
CFLAGS = $(FLAGS) $(LIB_CFLAGS)
CFLAGS_D = $(FLAGS) -g -ggdb -D_DEBUG $(LIB_CFLAGS)

CC = gcc
LD = ar rcs
RM = /bin/rm -f

default: release
all: release debug

debug: $(NAME_D)
release: $(NAME)

$(FT_DIR)/libft.a:
	make -C $(FT_DIR)

$(FT_DIR)/libft_d.a:
	make -C $(FT_DIR)

norme:
	@norminette $(SRC_DIR) $(INC_DIR) Makefile > $(NORM_FILE)
	@echo "$$(grep Error $(NORM_FILE) | wc -l) error(s) detected."
	@echo "$$(grep Warning $(NORM_FILE) | wc -l) warning(s) detected."
	@read -p "View $(NORM_FILE)? [Yn]" ANS; [[ $$ANS == "y" || $$ANS == "Y" || $$ANS == "" ]] && less $(NORM_FILE)

$(NAME): $(OBJS) $(VERSION_FILE)
	@PRCT=$$(echo "scale=1; $(COUNT) / $(NOBJS) * 100" | bc); \
	printf "\r\033[K[$(CC)|%s%%]Linking $@\n" "$$PRCT"; \
	$(LD) $@ $^; \
	printf "\r\033[K[$(CC)|%s%%]Linked $@\n" "$$PRCT"


$(NAME_D): $(OBJS_D) $(VERSION_FILE)
	@PRCT=$$(echo "scale=1; $(COUNT) / $(NOBJS) * 100" | bc); \
	printf "\r\033[K[$(CC)|%s%%]Linking $@\n" "$$PRCT"; \
	$(LD) $@ $^; \
	printf "\r\033[K[$(CC)|%s%%]Linked $@\n" "$$PRCT"

$(OBJ_DEBUG_DIR)/%.o: $(SRC_DIR)/%.c
	@PRCT=$$(echo "scale=1; $(COUNT) / $(NOBJS) * 100" | bc); \
	$$(test -e "$(OBJ_DEBUG_DIR)" || mkdir -p $(OBJ_DEBUG_DIR)); \
	printf "\r\033[K[$(CC)|%s%%]Compiling $< into $@" "$$PRCT"; \
	$(CC) $(CFLAGS_D) -c -I$(INC_DIR) -o $@ $<; \
	$(eval COUNT = $(shell echo "$(COUNT) + 1" | bc) ) \
	printf "\r\033[K[$(CC)|%s%%]Compiled $< into $@" "$$PRCT"

$(OBJ_RELEASE_DIR)/%.o: $(SRC_DIR)/%.c
	@PRCT=$$(echo "scale=1; $(COUNT) / $(NOBJS) * 100" | bc); \
	$$(test -e "$(OBJ_RELEASE_DIR)" || mkdir -p $(OBJ_RELEASE_DIR)); \
	printf "\r\033[K[$(CC)|%s%%]Compiling $< into $@" "$$PRCT"; \
	$(CC) $(CFLAGS) -c -I$(INC_DIR) -o $@ $<; \
	$(eval COUNT = $(shell echo "$(COUNT) + 1" | bc) ) \
	printf "\r\033[K[$(CC)|%s%%]Compiled $< into $@" "$$PRCT"

# build number file
$(VERSION_FILE): $(OBJS)
	@printf "\r\033[KUpdating version ($(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_BUILD).$(VERSION_REV))\n"
	@if ! test -f $@; then echo 0 > $@; fi
	@echo $$(($$(cat $@) + 1)) > $@
	@if ! test -f $(VERSION_CHANGES); then touch $(VERSION_CHANGES); fi
	@echo "Version $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_BUILD).$(VERSION_REV): Library build ($(VERSION_DATE) at $(VERSION_TIME))" | cat - $(VERSION_CHANGES) >> $(VERSION_CHANGES).tmp
	@rm -rf $(VERSION_CHANGES)
	@mv $(VERSION_CHANGES).tmp $(VERSION_CHANGES)

version: $(VERSION_FILE)
	@echo Current $(NAME) version: $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_BUILD).$(VERSION_REV)

test:
	make -C test

clean:
	@N=$$(ls $(OBJ_RELEASE_DIR) | grep '\.o' | wc -w); \
	if [ $$N -gt 0 ]; then \
		echo "Removing $$N object(s)."; \
		$(RM) $(OBJS); \
	fi

fclean : clean
	@if [ -e "$(NAME)" ]; then \
		echo Removing $(NAME); \
		$(RM) $(NAME); \
	fi

clean_debug:
	@N=$$(ls $(OBJ_DEBUG_DIR) | grep '\.o' | wc -w); \
	if [ $$N -gt 0 ]; then \
		echo "Removing $$N object(s)."; \
		$(RM) $(OBJS_D); \
	fi

fclean_debug : clean_debug
	@if [ -e "$(NAME_D)" ]; then \
		echo Removing $(NAME_D); \
		$(RM) $(NAME_D); \
	fi

help:
	@printf "rules:\n"
	@printf "\tall:\t\tsee release.\n"
	@printf "\trelease:\t\tbuilds in release mode.\n"
	@printf "\tdebug:\t\tbuilds in debug mode.\n"
	@printf "\tclean:\t\tremoves all release object(s).\n"
	@printf "\tfclean:\t\tremoves release target\n"
	@printf "\tre:\t\tfclean + release\n"
	@printf "\trelink:\t\trelinks all release object(s) into target.\n"

re: fclean default

relink: $(OBJS)
	@$(RM) $(NAME) && echo "Removed $(NAME)"
	@$(LD) $(LDFLAGS) $(NAME) $^ && echo "Linked $@"

.PHONY: clean fclean all re relink debug release test help default
