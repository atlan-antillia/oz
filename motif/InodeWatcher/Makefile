CC = g++
INC = -I./ -I../../include
CFLAG = -O2
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11 -lpthread
TARGET = InodeWatcher

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

