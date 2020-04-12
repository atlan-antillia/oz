CC = g++
INC = -I./ -I../../include -I/usr/local/include/opencv4
#CFLAG = -O2
CFLAG = -g2 -std=c++11
LIBDIR = -L../../lib -L/usr/local/lib 
LIB = -lXpm -lXm -lXt -lX11 
OPENCV_LIBS= \
  -lopencv_calib3d \
  -lopencv_core \
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

TARGET=ImageFlipping

target: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAG) $(INC) -o $(TARGET) $(TARGET).cpp $(LIBDIR) $(LIB) $(OPENCV_LIBS) 

clean:
	rm -f *.o $(TARGET)

