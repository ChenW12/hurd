dir		= boot-parted
makemode	= server

SRCS		= main.c store-parted.c
OBJS 		= main.o store-parted.o # Use some magic to convert the variables in SRCS to object files
target		= boot-parted
HURDLIBS	= machdev trivfs ports shouldbeinlibc store

LDLIBS += -lpthread

include ../Makeconf
