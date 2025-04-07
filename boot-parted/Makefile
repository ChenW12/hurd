dir		= boot-parted
makemode	= server

SRCS		= main.c store-parted.c
OBJS 		= main.o store-parted.o # Use some magic to convert the variables in SRCS to object files
target		= boot-parted boot-parted.static
HURDLIBS	= machdev trivfs ports shouldbeinlibc store iohelp ihash fshelp

LDLIBS += -lpthread $(and $(HAVE_LIBBZ2),-lbz2) $(and $(HAVE_LIBZ),-lz)

include ../Makeconf

boot-parted.static: $(boot-store-types:%=../libstore/libstore_%.a)
