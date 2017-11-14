CC=g++
CXXFLAGS=-g -Wall -Wextra -MMD -std=c++11
LDLIBS=-lm -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_highgui -laa
facedetect: facedetect.o ascii.o
clean:
	$(RM) *.o *.d program
-include $(wildcard *.d)
