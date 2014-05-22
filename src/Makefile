# benjaesq@me.com
#

include ../mk/cpp-defs.mk

all: $(OBJS_CXX)
	# link the objects here, when known

# (re)compile objects when sources change
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

#install:

include ../mk/common-rules.mk
