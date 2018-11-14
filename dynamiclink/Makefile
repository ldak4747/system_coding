include dynamiclink.mk

TARGET=libdynamiclink.so
TEST_MAIN=main
CFLAGS=-g -fPIC -shared -D_GNU_SOURCE
INCLUDE=
LINK=
LIB=-ldl
OBJS=dynamiclink.o
SRCS=dynamiclink.cc

TEST_SRC=main.cc
TEST_LIB=-ldynamiclink -lpthread
#if link order as follow, will not call hooked systemcall
#TEST_LIB=-lpthread -ldynamiclink
TEST_LIBPATH=-L.

all:$(TEST_MAIN) $(TARGET)
.PHONY: all

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) -c $^

$(TARGET):$(LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LINK) $(LIB)
$(TEST_MAIN):$(TARGET)
	$(CC) -g $(TEST_SRC) $(TEST_LIBPATH) $(TEST_LIB) -o $(TEST_MAIN)

clean:
	rm -f *.o
	rm $(TARGET)
	rm $(TEST_MAIN)
