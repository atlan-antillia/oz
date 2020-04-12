CC = g++
INC = -I./ -I../../include -I/usr/include/freetype2
CFLAG = 
LIBDIR = -L../../lib
LIB = -lXft -lXm -lXt -lX11 -lfontconfig
TARGET = FTPushButton

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

