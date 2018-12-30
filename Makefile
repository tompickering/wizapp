#!/usr/bin/make -f

.PHONY: clean all install

all:

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp **/*.cpp **/**/*.cpp))
DEPS = $(OBJS:%.o=%.d)
CLEAN = $(NAME) $(OBJS) $(DEPS)

LDLIBS=-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
NAME=wizapp
CXXFLAGS=-Wall -pedantic -DSDL -DLINUX

PREFIX = /usr/local

%.d: %.c
	$(CXX) -MM -MF $@ -MT $@ -MT $*.o $<

all: $(NAME)
	$(CXX) $(OBJS) $(LDLIBS) -o $(NAME)

$(NAME):

$(NAME): $(OBJS)

clean:
	rm -rf $(CLEAN) core

install: all
	install $(NAME) $(DESTDIR)$(PREFIX)/bin

ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
-include $(DEPS)
endif
