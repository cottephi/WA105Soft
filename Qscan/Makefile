include ../Makefile.inc

CXXFLAGS += -I./inc/ -I../include

EXE = Qscan.exe

EXPLLINKLIBS  = -L../lib -lqdrift -ldatamodel -lcrp -llro -lelecsim -ldlarvmc -lutils -lcalidaq -llowrecon

OBJS = src/Qscan.o src/QscanApp.o  

all: $(EXE) 

$(EXE): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@ $(EXPLLINKLIBS) 


clean:
	rm -rf $(OBJS) core  Qscan.exe 

distclean: clean
	rm -rf .*~ *~ *.o *.bak *.a *.so .depend html Qscan.exe

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
