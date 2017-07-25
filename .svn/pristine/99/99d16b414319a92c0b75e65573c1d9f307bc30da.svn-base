include ../Makefile.inc

MOD    = montasks

OBJS   = $(patsubst src/%.cc, src/%.o, $(wildcard src/*.cc))
LIBMOD = lib$(MOD)
ARXIV  = $(LIBDIR)/$(LIBMOD).a
LIBSO  = $(LIBDIR)/$(LIBMOD).so

# ROOT dictionary generation
DICT    = src/MonTaskDict.cc
LINKDEF = inc/MonTaskLinkDef.hh
INCS    = MonTask.h MonBenchDist.h

OBJS   += $(patsubst src/%.cc, src/%.o, $(DICT))

all: $(ARXIV) $(LIBSO)

$(ARXIV): $(OBJS)
	@echo $(ARXIV)
	@$(AR) $(ARFLAGS) $@ $^

$(LIBSO): $(OBJS)
	$(CXX) $(SOFLAGS)  -Wl,-soname,$(LIBMOD).so.$(VERSION).$(SUBVER) -o $(LIBSO).$(VERSION).$(SUBVER) $^
	@ln -sf $(LIBMOD).so.$(VERSION).$(SUBVER) $(LIBSO)

$(OBJS): $(DICT)

$(DICT): $(LINKDEF) $(patsubst %, inc/%, $(INCS))
	@echo "Generating ROOT dictonary for MonTasks ..."
	rootcint -f $@ -c -I$(INCDIR) $(INCS) $(LINKDEF)

clean:
	@echo "Cleaning up module $(MOD) ..."
	@rm -fv src/*.o
	@rm -fv $(ARXIV)
	@rm -fv $(LIBSO)*

distclean: clean
	@rm -fv src/*.*~
	@rm -fv inc/*.*~

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

