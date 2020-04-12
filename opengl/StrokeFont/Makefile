CC = g++
INC = -I./ -I../../include
CFLAG = -O2
LIBDIR = -L../../lib -L/usr/local/lib 
LIB = -lXm -lXt -lX11 -lglut -lGLEW -lGL -lGLU -lpng -ljpeg

TARGET = StrokeFont

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

