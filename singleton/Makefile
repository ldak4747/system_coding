include singleton.mk

TARGET=main
CFLAGS=-g
INCLUDEDIR=-I $(GTEST)/include
LINKDIR=-L$(GTEST)
LIB=-lpthread -lgtest

OBJS=main.o
SRCS=main.cc

all: $(TARGET)
.PHONY: all

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDEDIR) $(SRCS) -c $^

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LINKDIR) $(LIB)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
