-include ../Makefile.local
-include ../Makefile.rules

CPPFLAGS += -I ../api -I ../depends/install/include

DIR_PATH = $(GSL_DIR)

CPPFLAGS += $(foreach dir, $(DIR_PATH), -I$(dir)/include)
LDFLAGS += $(foreach dir, $(DIR_PATH), -L$(dir)/lib) 

FILES = $(shell git ls-files)
FILES += $(shell git ls-files ../api)

LIBSOURCES = libfastpm.c \
    solver.c \
    factors.c \
    events.c \
    gravity.c \
    transfer.c \
    memory.c \
    utils.c \
    initialcondition.c \
    pngaussian.c \
    constrainedgaussian.c \
    lightcone.c \
    timemachine.c \
    cosmology.c \
    powerspectrum.c \
    logging.c \
    prof.c \
    vpm.c  \
    pmpfft.c  \
    pmghosts.c  \
    painter.c \
    painter-cic.c \
    store.c \
    pm2lpt.c \
    pmapi.c \
    string.c \
    version.c

PFFTLIBS0 = libpfft_omp.a libpfftf_omp.a
PFFTLIBS0 += libfftw3_omp.a libfftw3_mpi.a libfftw3.a
PFFTLIBS0 += libfftw3f_omp.a libfftw3f_mpi.a libfftw3f.a

MPSORTLIBS = ../mpsort/libradixsort.a ../mpsort/libmpsort-mpi.a
# prefix the path
PFFTLIBS = $(PFFTLIBS0:%.a=../depends/install/lib/%.a)

all: libfastpm.a 

libfastpm.a : $(LIBSOURCES:%.c=.objs/%.o)
	$(AR) rc $@ $^

version.c : $(FILES)
	GIT=`git describe --always --dirty --abbrev=10`; \
	echo 'const char * LIBFASTPM_VERSION = "1.0.'$${GIT}'";' > $@

#libfastpm.so : libfastpm.a $(PFFTLIBS)
#	$(CC) -shared -fvisibility=hidden -o $@ $^ 

-include $(LIBSOURCES:%.c=.deps/%.d)

clean:
	rm -rf .objs
	rm -rf .deps
	rm -rf libfastpm.a

