STD=-std=c++11
CC=g++
TARGET=argparser
OBJS=example.o

%.o: %.cc
	$(CC) -c $< $(STD)

.PHONY: all

$(TARGET):  $(OBJS)
	$(CC) -o $(TARGET) $(STD) $(OBJS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)
