CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c++20
LDFLAGS :=

BUILDDIR := build
TARGET := rubiks-cube-solver

SUBDIRS := src/rubiks-cube

.PHONY: all $(SUBDIRS) clean

all: $(BUILDDIR) $(SUBDIRS) $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(SUBDIRS):
	$(MAKE) -C $@ all

$(TARGET): $(patsubst %/%.o,%,$(foreach dir,$(SUBDIRS),$(wildcard $(BUILDDIR)/$(notdir $(dir))/*.o)))
	$(CXX) $(foreach dir,$(SUBDIRS),$(wildcard $(BUILDDIR)/$(notdir $(dir))/*.o)) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) clean)
	rm -rf $(BUILDDIR)
