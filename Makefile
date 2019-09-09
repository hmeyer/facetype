CC=g++
CXXFLAGS=-O3 -Wall -Wextra -MMD -std=c++14 -I/home/pi/tensorflow -I/home/pi/tensorflow/tensorflow/lite/tools/make/downloads/flatbuffers/include/
LDFLAGS=-L/home/pi/tensorflow/tensorflow/lite/tools/make/gen/rpi_armv6/lib
LDLIBS=-ldl -lpthread -lm -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_highgui -lopencv_videoio -laa -lwiringPi -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -ltensorflow-lite
facedetect: facedetect.o ascii.o typewriter.o detector.o cv_detector.o tf_detector.o
clean:
	$(RM) *.o *.d program
-include $(wildcard *.d)
