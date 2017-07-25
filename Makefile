include Makefile.inc

# libraries to compile
MODULES = utils elecsim lowrecon datamodel crp lro qdrift dlarvmc datautils recon tasks montasks guiutils anautils daq calidaq

EXECS   = $(patsubst src/%.cc, bin/%.exe, $(wildcard src/*.cc))
LIBSO   = -L lib/ $(foreach mod, $(MODULES), -l$(mod))

.PHONY: all, buildinit, clean, distclean, buildmod, qscan, benchmark
.PHONY: modules $(MODULES)

all: $(EXECS)

$(EXECS) : buildmod qscan benchmark

# rule for building executables
bin/%.exe : src/%.cc 
	@echo 'Buildng $@ ...'
	@$(CXX) $(CXXFLAGS) $< $(LDFLAGS) -o $@ $(LIBSO)

# build qscan
qscan : 
	make -C Qscan

benchmark : 
	make -C benchmark 	

# build modules
buildmod: buildinit buildit
	@echo $(LIBSO)
#	@for mod in $(MODULES);\
	do make -C $$mod/; done

buildit: $(MODULES)

$(MODULES):
	make -C $@

# run first to set-up includes and directories
buildinit:
	@ [ -d include ] || mkdir include
	@ [ -d bin ]     || mkdir bin
	@ [ -d lib ]     || mkdir lib
	@for mod in $(MODULES);\
	do \
	for file in $$mod/inc/*.h; \
	do cp -u $$file include/; done \
	done

clean:
	@for mod in $(MODULES);\
	do make clean -C $$mod/; done
	make clean -C Qscan
	make clean -C benchmark
	rm -fv $(EXECS)

# clean dist
distclean: clean
	@find include/ -type f -delete
	@for mod in $(MODULES);\
	do make distclean -C $$mod/; done
	make distclean -C Qscan
	make distclean -C benchmark
	rm -fv src/*.*~
	rm -fv include/*.h

