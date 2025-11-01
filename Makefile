include vars.mk

BUILDDIR := build
TARGET := rubiks-cube-solver

SUBDIRS := src/ai-lib src/rubiks-cube

ALL_OBJECTS := $(shell find $(BUILDDIR) -name "*.o" -type f)

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
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done
	rm -rf $(BUILDDIR)
