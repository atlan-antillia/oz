CC = g++
INC = -I./ -I../../include
#CFLAG = -g3
CFLAG = -O2
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11 -lGLEW -lGL -lGLU -L/usr/local/lib -lpng -ljpeg

TARGET =CubeWithColorEyeLightPositioner

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

