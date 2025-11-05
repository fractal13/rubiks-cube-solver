# Root Makefile for ai-lib

include common.mk

CXXFLAGS += -I$(CURDIR)/include

LIB_NAME = ai
LIB_DIR = ../../lib

SUBDIRS = \
	src/Agent \
	src/CSP \
	src/FOL \
	src/LocalSearch \
	src/PL \
	src/Search \
	src/Support

# Find all object files in subdirectories
ALL_OBJS = $(foreach dir,$(SUBDIRS),$(patsubst $(dir)/%.cpp,$(dir)/%.o,$(wildcard $(dir)/*.cpp)))

.PHONY: all clean depend $(SUBDIRS)

all: $(LIB_DIR)/lib$(LIB_NAME).a

$(LIB_DIR)/lib$(LIB_NAME).a: $(ALL_OBJS)
	mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $(ALL_OBJS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) clean;)
	rm -f $(LIB_DIR)/lib$(LIB_NAME).a
	rm -f $(ALL_OBJS)

# Re-generate dependencies for all subdirectories
depend:
	$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) depend;)
