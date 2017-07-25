include ../Makefile.inc

CXXFLAGS += -I./inc/ -I../include

EXE = bench.exe

EXPLLINKLIBS  = -L../lib -lqdrift -ldatamodel -lcrp -llro -lelecsim -ldlarvmc -lutils -lcalidaq -llowrecon -ldaq 

OBJS = src/bench.o 

all: $(EXE) 

$(EXE): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@ $(EXPLLINKLIBS) 


clean:
	rm -rf $(OBJS) core  bench.exe 

distclean: clean
	rm -rf .*~ *~ *.o *.bak *.a *.so .depend html bench.exe

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
