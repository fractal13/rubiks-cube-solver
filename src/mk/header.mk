CXX := g++
AR := ar
CXXFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c++20
ARFLAGS := Ucrus
DEPDIR := .d
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))

AI_INCLUDE := $(PROJECT_ROOT)/src/ai-lib/include
AI_LIB_DIR := $(PROJECT_ROOT)/src/ai-lib/lib
AI_LIB     := $(AI_LIB_DIR)/libai.a
CXXFLAGS += -I$(AI_INCLUDE)

LDFLAGS := -L $(AI_LIB_DIR) -lai
BIN_DIR := $(PROJECT_ROOT)/bin

.PRECIOUS: $(DEPDIR)/%.d
