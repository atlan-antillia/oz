CC = g++
INC = -I./ -I../../include
##CFLAG = -O2
CFLAG = -g
LIBDIR = -L../../lib -L/usr/local/lib 
LIB = -lXm -lXt -lX11 -lGLEW -lGL -lGLU -lpng -ljpeg -lpthread

TARGET = PNGTextureRotationByTimerThread

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

