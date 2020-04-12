CC = g++
INC = -I./ -I../../include -I/usr/include/freetype2
CFLAG = -O2
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11
TARGET = FontChooser

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

