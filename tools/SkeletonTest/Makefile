CPPFLAGS=-g -I../../../OpenNI/Include
LDFLAGS=-g
LDLIBS=-L../../../OpenNI/Platform/Linux/Bin/x64-Release/ -lOpenNI

SkeletonTest: SkeletonTest.o
	g++ -o bin/SkeletonTest SkeletonTest.o $(LDFLAGS) $(LDLIBS)

SkeletonTest.o: SkeletonTest.cpp
	g++ $(CPPFLAGS) -c SkeletonTest.cpp
