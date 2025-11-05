include ../../vars.mk

SRCDIR := .
BUILDDIR_ROOT := ../../build
BUILDDIR_LOCAL := $(BUILDDIR_ROOT)/rubiks-cube
DEPDIR := $(BUILDDIR_LOCAL)/.deps

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
CXXFLAGS += -I$(SRCDIR) -I../ai-lib/include

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR_LOCAL)/%.o,$(SOURCES))

.PHONY: all clean
.PRECIOUS: $(DEPDIR)/%.d

all: $(BUILDDIR_LOCAL) $(DEPDIR) $(OBJECTS)

$(DEPDIR):
	mkdir -p $@

$(BUILDDIR_LOCAL):
	mkdir -p $@

$(DEPDIR)/%.d: $(DEPDIR);
$(BUILDDIR_LOCAL)/%.o: $(SRCDIR)/%.cpp $(DEPDIR)/%.d
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -rf $(BUILDDIR_LOCAL)

-include $(wildcard $(DEPDIR)/*.d)
