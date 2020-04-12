CC = g++
INC = -I./ -I../../include -I/usr/include/freetype2

#2017/12/17
CFLAG = -g2
#CFLAG = -O2
LIBDIR = -L../../lib -L/usr/local/lib 
LIB = -lXft -lXm -lXt -lX11 -lGLEW -lGL -lGLU -lpng -ljpeg -lfontconfig


TARGET = XFTTexture

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

