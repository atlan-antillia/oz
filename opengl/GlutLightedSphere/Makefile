CC = g++
INC = -I./ -I../../include
CFLAG = -O2
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11 -lglut -lGLEW -lGL -lGLU -L/usr/local/lib -lpng -ljpeg

TARGET = GlutLightedSphere

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

