# benjaesq@me.com
#

include ../mk/cpp-defs.mk
include ../mk/common-defs.mk

EXEC=sota

all: $(EXEC)

$(EXEC): $(OBJS_CXX)
	$(CXX) $^ $(LD_FLAGS) -o $@ -lboost_regex -lboost_iostreams

# (re)compile objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

install: $(DESTDIR)$(bindir)
	$(INSTALL_x) $(EXEC) $(DESTDIR)$(bindir)

clean:
	-rm *.o
	-rm $(EXEC)

include ../mk/common-rules.mk
