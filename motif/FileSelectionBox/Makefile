CC = g++
INC = -I./ -I../../include
#CFLAG = -O2
CFLAG = -g
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11
TARGET = FileSelectionBox

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

