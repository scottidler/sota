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
$(addsuffix -setup, $(MAKE_CONFIGS)): $(addsuffix -config, $$@)

%-setup:
	cd $* && $(MAKE)

%-setup-git:
	@echo "--Setting up submodule: $*"
	@git submodule update --quiet --init $*

%-setup-config: $(addprefix $$*, /config.log)

%/config.log:
	cd $* && ./configure

llvm-setup-config: llvm-setup-git

include mk/common-rules.mk
