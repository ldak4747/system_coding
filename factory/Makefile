include include.mk


SOURCE = $(wildcard *.cc)
TARGETS = $(patsubst %.cc, %, $(SOURCE))

CFLAGS = -Wall -g
all:$(TARGETS)

$(TARGETS):%:%.cc
	$(CC) $< $(CFLAGS) -o $@

.PHONY:clean all
clean:
	rm -rf *.o
	rm -rf $(TARGETS)
