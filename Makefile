ROOTDIR:=$(realpath $(PWD))
export ROOTDIR

include mk/common-defs.mk

MAKE_DIRS=src

.SECONDEXPANSION:

.PHONY: $(MAKE_DIRS)

default: all

all install clean: $(addsuffix -$$@, $(MAKE_DIRS))

%-all %-install %-clean:
	cd $* && $(MAKE) $(subst $*-,,$@)

include mk/common-rules.mk
