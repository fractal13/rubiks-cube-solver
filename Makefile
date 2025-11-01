include vars.mk

BUILDDIR := build
TARGET := rubiks-cube-solver

SUBDIRS := src/ai-lib src/rubiks-cube

# Library path
LIBRARY := $(BUILDDIR)/libai.a

.PHONY: all $(SUBDIRS) clean

all: $(BUILDDIR) $(LIBRARY) $(SUBDIRS) $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(LIBRARY): src/ai-lib
	$(MAKE) -C $< all

$(SUBDIRS):
	$(MAKE) -C $@ all

$(TARGET): $(LIBRARY)
	$(CXX) $(BUILDDIR)/rubiks-cube/*.o $(LIBRARY) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done
	rm -rf $(BUILDDIR)
