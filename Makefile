ROOTDIR:=$(realpath $(PWD))
export ROOTDIR

include mk/common-defs.mk
-include mk/tweaks.mk

# project own sources
MAKE_DIRS=src
# if it is a git submodule that will be built
GITSUBMODS:=llvm z2h
# if it's a configurable module
MAKE_CONFIGS=llvm

ifdef JOBS
MAKEFLAGS += -j $(JOBS) -l $(JOBS)
endif

.SECONDEXPANSION:
.PHONY: $(MAKE_DIRS) $(GITSUBMODS)

default: all

all install clean: $(addsuffix -$$@, $(MAKE_DIRS))

ifeq ($(LLVM_SUBMOD), 1)
src-all: llvm-setup
endif

%-all:
	cd $* && $(MAKE) $(subst $*-,,$@)
%-install:
	cd $* && $(MAKE) $(subst $*-,,$@)
%-clean:
	cd $* && $(MAKE) $(subst $*-,,$@)

setup: $(addsuffix -$$@, $(GITSUBMODS))

# git submodule config gets processed before setup
$(addsuffix -setup, $(GITSUBMODS)): $(addsuffix -git, $$@)
# ./configure gets processed before setup
$(addsuffix -setup, $(MAKE_CONFIGS)): $(addsuffix -config, $$@)

%-setup:
	cd $* && $(MAKE)

%-setup-git:
	@echo "--Setting up submodule: $*"
	@git submodule update --quiet --init $*

%-setup-config: $$*/config.log

%/config.log: $$*-setup-git
	cd $* && ./configure $(CONFIGVARS)

# llvm specifics
llvm-setup-config: llvm/config.log
llvm-setup-config: CONFIGVARS+=--enable-jit

include mk/common-rules.mk
