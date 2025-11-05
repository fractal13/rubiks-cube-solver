# Common Makefile for ai-lib subdirectories

# Include global variables from the project root
include ../../vars.mk

# Compiler and Archiver
CXX ?= g++
AR ?= ar

# Compiler flags
CXXFLAGS += -Wall -Wextra -g -std=c++17

# Archiver flags
ARFLAGS = rcs

# Rule to compile .cpp files into .o files and generate .d dependency files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include all automatically generated dependency files
-include $(wildcard *.d)
