CC = g++
INC = -I./ -I../../include -I/usr/local/include/opencv4 -I$(HOME)/darknet-master/include -I$(HOME)/darknet-master/3rdparty/stb/include
 
#CFLAG = -O2
#CFLAG = -g -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
CFLAG = -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0

LIBDIR = -L../../lib -L/usr/local/lib 
#LIBDIR = -L../../lib -L/usr/local/lib -L$(HOME)/darknet-master 
LIB = `pkg-config opencv4 --cflags --libs` -lXpm -lXm -lXt -lX11 

LIB_YOLO=-ldarknet

OPENCV_LIBS= \
  -lopencv_core \
  -lopencv_calib3d \
  -lopencv_dnn \
  -lopencv_features2d \
  -lopencv_flann \
  -lopencv_gapi \
  -lopencv_highgui \
  -lopencv_imgcodecs \
  -lopencv_imgproc \
  -lopencv_ml \
  -lopencv_objdetect \
  -lopencv_photo \
  -lopencv_stitching \
  -lopencv_video \
  -lopencv_videoio 

TARGET=CustomYoloObjectDetector

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB) $(OPENCV_LIBS) $(LIB_YOLO) 
#	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB) $(OPENCV_LIBS) $(LIB_YOLO) 

clean:
	rm -f *.o $(TARGET)

