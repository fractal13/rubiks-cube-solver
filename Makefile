CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c++20 -Isrc/ai-lib/include
LDFLAGS :=

BUILDDIR := build
TARGET := rubiks-cube-solver

SUBDIRS := src/rubiks-cube src/ai-lib

ALL_OBJECTS := $(foreach dir,$(SUBDIRS),$(wildcard $(BUILDDIR)/$(notdir $(dir))/*.o))

.PHONY: all $(SUBDIRS) clean

all: $(BUILDDIR) $(SUBDIRS) $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(SUBDIRS):
	$(MAKE) -C $@ all

$(TARGET): $(ALL_OBJECTS)
	$(CXX) $(ALL_OBJECTS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) clean)
	rm -rf $(BUILDDIR)
