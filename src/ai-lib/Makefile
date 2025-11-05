# Root Makefile for ai-lib

PROJECT_ROOT := $(CURDIR)/../..
export PROJECT_ROOT
include $(PROJECT_ROOT)/src/mk/header.mk

LIB_NAME = ai
LIB_DIR = ./lib

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

all: $(SUBDIRS) $(LIB_DIR)/lib$(LIB_NAME).a

$(LIB_DIR)/lib$(LIB_NAME).a: $(ALL_OBJS)
	mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $(ALL_OBJS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) clean;)
	rm -f $(LIB_DIR)/lib$(LIB_NAME).a
