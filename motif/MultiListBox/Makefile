CC = g++
INC = -I./ -I../../include
#CFLAG = -O2
CFLAG = -g -D_GLIBCXX_USE_CXX11_ABI=0
LIBDIR = -L../../lib
LIB = -lXm -lXt -lX11
TARGET = MultiListBox

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB)


clean:
	rm -f *.o $(TARGET)

