CC = g++
INC = -I./ -I../../include
CFLAG = -O2
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11 -lGLEW -lGL -lGLU -ljpeg -L/usr/local/lib -lpng

TARGET = Disk

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

