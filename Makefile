ROOTDIR:=$(realpath $(PWD))
export ROOTDIR

include mk/common-defs.mk

MAKE_DIRS=src
GITSUBMODS:=llvm

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
	cd $@ && ./configure
	cd $@ && $(MAKE)

include mk/common-rules.mk
