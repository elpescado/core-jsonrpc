CXX=clang++

CXXFLAGS=-g -Wall -std=c++11 -I/usr/local/include -I/usr/local/include/jsoncpp -I.
LDFLAGS=-g -std=c++11 -L/usr/local/lib -ljsoncpp

SOURCES=\
	channel.cpp \
	error.cpp \
	handler.cpp \
	request.cpp \
	result.cpp \
	service.cpp \
	test/channel.cpp \
	test/error.cpp \
	test/handler.cpp \
	test/integration.cpp \
	test/request.cpp \
	test/result.cpp \
	test/service.cpp \
	gtest/gtest-all.cc

CC_SOURCES :=  $(filter %.cc, $(SOURCES))
CPP_SOURCES := $(filter %.cpp, $(SOURCES))

OBJECTS := $(patsubst %.cpp,%.o,$(CPP_SOURCES)) $(patsubst %.cc,%.o,$(CC_SOURCES))

all: run_tests etest

run_tests: gtest/gtest_main.o $(OBJECTS)
	$(CXX) $(LDFLAGS) gtest/gtest_main.o $(OBJECTS) -o $@

etest: etest.o $(OBJECTS)
	$(CXX) $(LDFLAGS) etest.o $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS)

test:
	./run_tests && ./etest

.PHONY: clean test
