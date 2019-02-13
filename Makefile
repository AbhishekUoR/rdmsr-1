EXE=rdmsr
CC	:= gcc
CFLAGS = -c -g  -Wall -Werror -fPIC -std=gnu11
CEXTRA = -nostdlib


OBJS = rdmsr.o
CLEAN = rm -f $(EXE) *.o *.a  *.so *.S *.map *.asm

.PHONY: all
all: $(EXE) mapfile

#*.o: *.c
#	$(CC) $(CFLAGS) $<

.PHONY: $(EXE)
$(EXE): $(OBJS)
	$(CC) $(LFLAGS) -o $(EXE) $(OBJS)
	$(shell ./dis.sh *.o)


.PHONY: mapfile
mapfile: $(EXE)
	$(shell ld -M=$(EXE).map -o /dev/null $(EXE) &> /dev/null)

.PHONY: clean
clean:
	$(shell $(CLEAN) &> /dev/null)
	@echo "repo is clean"

.PHONY: superclean
superclean: clean
	$(shell rm *~ &> /dev/null)
	@echo "cleaned unwanted backup files"

.PHONY: test
test: FORCE
	./$(EXE)

FORCE:
