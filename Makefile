CC=g++
CXXFLAGS=-O2 -Wall -Wextra -MMD -std=c++11
LDLIBS=-lm -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_highgui -lopencv_videoio -laa -lwiringPi
facedetect: facedetect.o ascii.o typewriter.o
clean:
	$(RM) *.o *.d program
-include $(wildcard *.d)
