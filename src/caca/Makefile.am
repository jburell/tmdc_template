# $Id: Makefile.am 242 2004-01-13 15:01:22Z sam $

include_HEADERS = caca.h

lib_LIBRARIES = libcaca.a $(libcaca_pic_a)
libcaca_a_SOURCES = \
	caca.c \
	caca.h \
	caca_internals.h \
	graphics.c \
	event.c \
	math.c \
	line.c \
	box.c \
	conic.c \
	triangle.c \
	sprite.c \
	bitmap.c \
	time.c \
	$(NULL)

if NEED_PIC
libcaca_pic_a = libcaca_pic.a
endif
libcaca_pic_a_SOURCES = $(libcaca_a_SOURCES)
libcaca_pic_a_CPPFLAGS = -fPIC -DPIC

