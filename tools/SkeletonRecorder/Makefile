CPPFLAGS=-g -I../../../OpenNI/Include
LDFLAGS=-g
LDLIBS=-L../../../OpenNI/Platform/Linux/Bin/x64-Release/ -lOpenNI

SkeletonRecorder: SkeletonRecorder.o
	g++ -o bin/SkeletonRecorder SkeletonRecorder.o $(LDFLAGS) $(LDLIBS)

SkeletonRecorder.o: SkeletonRecorder.cpp
	g++ $(CPPFLAGS) -c SkeletonRecorder.cpp
