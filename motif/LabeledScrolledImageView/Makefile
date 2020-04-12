CC = g++
INC = -I./ -I../../include
CFLAG = 
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11
TARGET = LabeledScrolledImageView

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

