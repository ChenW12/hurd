
dir := rumpfs
makemode := server

target = rumpfs
SRCS = main.c pager.c inode.c dir.c truncate.c ialloc.c hyper.c storeinfo.c
OBJS = $(SRCS:.c=.o)
HURDLIBS = diskfs pager iohelp fshelp store ports ihash shouldbeinlibc
LDLIBS = -lpthread

include ../Makeconf
