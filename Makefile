#!/usr/bin/make -f

.PHONY: clean all install

all:

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp */*.cpp */*/*.cpp))
DEPS = $(OBJS:%.o=%.d)
CLEAN = $(NAME) $(OBJS) $(DEPS)

LDLIBS=-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
NAME=src/wizapp
CXXFLAGS=-Wall -pedantic -std=c++14 -DSDL -DLINUX

PREFIX = /usr/local

%.d: %.cpp
	$(CXX) -MM -MF $@ -MT $@ -MT $*.o $<

all: $(NAME)
	mv $(NAME) wizapp

$(NAME):
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(NAME): $(OBJS)

clean:
	rm -rf $(CLEAN) core

install: all
	install $(NAME) $(DESTDIR)$(PREFIX)/bin

ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
-include $(DEPS)
endif
