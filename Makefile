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

setup: $(GITSUBMODS)

$(GITSUBMODS):
	@echo "--Setting up submodule: $@"
	@git submodule update --quiet --init $@
	cd $@ && $(MAKE)

$(MAKE_CONFIGS): $(addprefix $$@, /config.log)

%/config.log:
	cd $* && ./configure

include mk/common-rules.mk
