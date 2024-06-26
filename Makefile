NAME = woody_woodpacker

IDIR = src/include
CC=gcc
CFLAGS=-I$(IDIR) -g -fsanitize=address

ODIR=obj
SDIR=src

LIBS=-lm

_DEPS = utils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o utils.o inject_code.o rc4.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf $(OBJ)
	rm -rf ./$(ODIR)

re: fclean all

.PHONY: all fclean clean re
