CC=g++
CXXFLAGS=-O2 -Wall -Wextra -MMD -std=c++11
LDLIBS=-lm -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_highgui -lopencv_videoio -laa -lwiringPi -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util
facedetect: facedetect.o ascii.o typewriter.o
clean:
	$(RM) *.o *.d program
-include $(wildcard *.d)
