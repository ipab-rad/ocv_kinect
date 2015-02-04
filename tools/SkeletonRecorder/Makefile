CPPFLAGS=-g -I../../external/OpenNI/Include
LDFLAGS=-g
LDLIBS=-L../../external/OpenNI/Lib/ -lOpenNI

SkeletonRecorder: SkeletonRecorder.o
	g++ -o bin/SkeletonRecorder SkeletonRecorder.o $(LDFLAGS) $(LDLIBS)

SkeletonRecorder.o: SkeletonRecorder.cpp
	g++ $(CPPFLAGS) -c SkeletonRecorder.cpp
