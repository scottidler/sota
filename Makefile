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

$(addsuffix -setup, $(GITSUBMODS)): $(addsuffix -git, $$@)
#TODO: check if this actually works for generalizing rule below
$(addsuffix -setup, $(MAKE_CONFIGS)): $(addsuffix -config, $$@)

%-setup:
	cd $* && $(MAKE)

%-setup-git:
	@echo "--Setting up submodule: $*"
	@git submodule update --quiet --init $*

#TODO: not a general rule
llvm-setup-config: llvm/config.log

%/config.log: $$*-setup-git
	cd $* && ./configure

include mk/common-rules.mk
