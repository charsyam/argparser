STD=-std=c++11
CC=g++
TARGET=unittest
OBJS=unittest.o
LIBS=gtest/src/gtest-all.cc gtest/src/gtest_main.cc
INCS=-Igtest

%.o: %.cc
	$(CC) -c $< $(STD)

.PHONY: all

$(TARGET):  $(OBJS)
	$(CC) -o $(TARGET) $(STD) $(OBJS) $(INCS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)
