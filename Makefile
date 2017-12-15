CC=g++
CXXFLAGS=-O1 -Wall -Wextra -MMD -std=c++11
LDLIBS=-lm -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_highgui -laa -lwiringPi
facedetect: facedetect.o ascii.o typewriter.o
clean:
	$(RM) *.o *.d program
-include $(wildcard *.d)
