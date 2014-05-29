ROOTDIR:=$(realpath $(PWD))
export ROOTDIR

include mk/common-defs.mk

MAKE_DIRS=src

.SECONDEXPANSION:

.PHONY: $(MAKE_DIRS)

default: all

all install clean: $(MAKE_DIRS)-$$@

%-all %-install %-clean:
	cd $* && make $(subst $*-,,$@) 

include mk/common-rules.mk
