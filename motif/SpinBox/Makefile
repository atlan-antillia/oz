CC = g++
INC = -I./ -I../../include
CFLAG = -O2
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11
TARGET = SpinBox

target: $(TARGET)

$(TARGET):
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

