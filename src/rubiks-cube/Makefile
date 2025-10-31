SRCDIR := .
BUILDDIR_ROOT := ../../build
BUILDDIR_LOCAL := $(BUILDDIR_ROOT)/rubiks-cube

CXXFLAGS += -I$(SRCDIR) -I../ai-lib/include -MMD -MP -MF $(shell pwd)/$(DEPDIR)/$*.d

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR_LOCAL)/%.o,$(SOURCES))

.PHONY: all clean

all: $(BUILDDIR_LOCAL) $(OBJECTS)

$(BUILDDIR_LOCAL):
	mkdir -p $(BUILDDIR_LOCAL)

$(BUILDDIR_LOCAL)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -rf $(BUILDDIR_LOCAL) $(DEPDIR)

-include $(wildcard $(DEPDIR)/*.d)
