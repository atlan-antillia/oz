CC = g++
INC = -I./ -I../../include
CFLAG = -ggdb3  
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11
TARGET = ListView

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

