CC=g++
CXXFLAGS=-O3 -Wall -Wextra -MMD -std=c++11
LDLIBS=-lm -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_highgui -laa
facedetect: facedetect.o ascii.o spi.o
clean:
	$(RM) *.o *.d program
-include $(wildcard *.d)
