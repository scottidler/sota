ROOTDIR:=$(realpath $(PWD))
export ROOTDIR

include mk/common-defs.mk

# project own sources
MAKE_DIRS=src
# if it is a git submodule that will be built
GITSUBMODS:=llvm
# if it's a configurable module
MAKE_CONFIGS=llvm

.SECONDEXPANSION:
.PHONY: $(MAKE_DIRS) $(GITSUBMODS)

default: all

all install clean: $(addsuffix -$$@, $(MAKE_DIRS))

%-all %-install %-clean:
	cd $* && $(MAKE) $(subst $*-,,$@)

setup: $(addsuffix -$$@, $(GITSUBMODS))

# git submodule config gets processed before setup
$(addsuffix -setup, $(GITSUBMODS)): $(addsuffix -git, $$@)
# ./configure gets processed before setup
$(addsuffix -setup, $(MAKE_CONFIGS)): $(addsuffix -config, $$@)
# git submodule gets processed before ./configure
$(addsuffix -setup-config, $(GITSUBMODS)): $(addsuffix -setup-git,\
						$(GITSUBMODS))

%-setup:
	cd $* && $(MAKE)

%-setup-git:
	@echo "--Setting up submodule: $*"
	@git submodule update --quiet --init $*

%-setup-config: $$*/config.log

%/config.log:
	cd $* && ./configure $(CONFIGVARS)

# llvm specifics
llvm-setup-config: llvm/config.log
llvm-setup-config: CONFIGVARS+=--enable-jit

include mk/common-rules.mk
