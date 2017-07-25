include ../Makefile.inc

MOD    = datamodel

OBJS   = $(patsubst src/%.cc, src/%.o, $(wildcard src/*.cc))
LIBMOD = lib$(MOD)
ARXIV  = $(LIBDIR)/$(LIBMOD).a
LIBSO  = $(LIBDIR)/$(LIBMOD).so

# ROOT dictionary generation
DICT    = src/DataModelDict.cc
LINKDEF = inc/DataModelLinkDef.hh
INCS    = RunHeader.h EventHeader.h BeamInfo.h GeomConfig.h
INCS   += DataFragment.h DataChan.h DataCRM.h DataCRP.h
INCS   += VoxelInfo.h VoxelInfoCRM.h VoxelInfoCRP.h
INCS   += RecoObject.h QHit.h Cluster2d.h Track2d.h
INCS   += RecoConfig.h HitReco.h
INCS   += DataLRO.h
INCS   += ChanTrueQ.h AnodeTrueQ.h

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
	@echo "Generating ROOT dictonary for DataModelDict ..."
	rootcint -f $@ -c -I$(INCDIR) $(INCS) $(LINKDEF)

clean:
	@echo "Cleaning up module $(MOD) ..."
	@rm -fv src/*.o
	@rm -fv $(basename $(DICT)).*
	@rm -fv $(ARXIV)
	@rm -fv $(LIBSO)*


distclean: clean
	@rm -fv src/*.*~
	@rm -fv inc/*.*~

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

