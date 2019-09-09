CC=g++
CXXFLAGS=-O3 -Wall -Wextra -MMD -std=c++14
LDLIBS=-ldl -lpthread -lm -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_highgui -lopencv_videoio -laa -lwiringPi -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util
facedetect: facedetect.o ascii.o typewriter.o detector.o cv_detector.o
clean:
	$(RM) *.o *.d program
-include $(wildcard *.d)
