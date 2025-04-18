#   Copyright (C) 2025 Zhaoming Luo <zhmingluo@163.com>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

dir := rumpfs
makemode := server

target = rumpfs
SRCS = main.c pager.c inode.c dir.c truncate.c ialloc.c hyper.c storeinfo.c
OBJS = $(SRCS:.c=.o)
HURDLIBS = diskfs pager iohelp fshelp store ports ihash shouldbeinlibc
LDLIBS = -lpthread

include ../Makeconf
