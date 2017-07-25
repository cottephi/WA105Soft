include ../Makefile.inc

CXXFLAGS += -I./inc/ -I../include

EXE = purity.exe

EXPLLINKLIBS  = -L../lib  -ldatamodel 

OBJS = src/purity.o  

all: $(EXE) 

$(EXE): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@ $(EXPLLINKLIBS) 


clean:
	rm -rf $(OBJS) core  purity.exe  

distclean: clean
	rm -rf .*~ *~ *.o *.bak *.a *.so .depend html purity.exe  

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
