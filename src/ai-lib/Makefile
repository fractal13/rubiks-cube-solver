include ../../vars.mk

SRCDIR := .
BUILDDIR_ROOT := ../../build
BUILDDIR_LOCAL := $(BUILDDIR_ROOT)/ai-lib
DEPDIR := $(BUILDDIR_LOCAL)/.deps

VPATH := src/Agent src/CSP src/FOL src/FOL/more_parsing src/LocalSearch src/PL src/PL/cc src/Search src/Support

# Explicitly specify which source files to compile, excluding the conflicting implementation
SOURCES := $(filter-out src/PL/KnowledgeBase.cpp,$(foreach dir,$(VPATH),$(wildcard $(dir)/*.cpp)))
OBJECTS := $(patsubst src/%.cpp,$(BUILDDIR_LOCAL)/%.o,$(SOURCES))

CXXFLAGS += -I$(SRCDIR)/include -MMD -MP -MF $(DEPDIR)/$*.d

.PHONY: all clean

all: $(BUILDDIR_LOCAL) $(DEPDIR) $(OBJECTS)

OBJDIRS := $(sort $(patsubst src/%,$(BUILDDIR_LOCAL)/%,$(dir $(SOURCES))))
$(BUILDDIR_LOCAL): $(OBJDIRS)

$(OBJDIRS):
	mkdir -p $@

DEPDIRS := $(sort $(patsubst src/%,$(DEPDIR)/%,$(dir $(SOURCES))))
$(DEPDIR): $(DEPDIRS)

$(DEPDIRS):
	mkdir -p $@

$(BUILDDIR_LOCAL)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -rf $(BUILDDIR_LOCAL) $(DEPDIR)

-include $(wildcard $(DEPDIR)/*.d)
