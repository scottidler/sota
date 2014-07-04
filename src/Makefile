# benjaesq@me.com
#

include ../mk/cpp-defs.mk
include ../mk/common-defs.mk

EXEC=sota

LD_FLAGS += -lboost_regex -lboost_iostreams

all: $(EXEC)

$(EXEC): $(OBJS_CXX)
	$(CXX) $^ $(LD_FLAGS) -o $@

# (re)compile objects
%.o: %.cpp %.hpp %.h
	$(CXX) $(CXXFLAGS) -c $<

install: $(DESTDIR)$(bindir)
	$(INSTALL_x) $(EXEC) $(DESTDIR)$(bindir)

clean:
	-rm *.o
	-rm $(EXEC)

include ../mk/common-rules.mk
