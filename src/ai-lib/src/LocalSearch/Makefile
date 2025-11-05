# Makefile for LocalSearch subdirectory

include ../../common.mk

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: all clean depend

all: $(OBJS)

clean:
	rm -f $(OBJS) *.d

depend:
	$(CXX) $(CXXFLAGS) -MMD -MP $(SRCS)
