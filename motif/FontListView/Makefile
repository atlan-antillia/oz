CC = g++
INC = -I./ -I../../include
#CFLAG = -O2
CFLAG = -g2
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11
TARGET = FontListView

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

