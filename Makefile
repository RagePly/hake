BUILD := build
GOALDIR := ${BUILD}/goals
SRC := src

CC := gcc
CFLAGS := -c -g -std=c2x -Wall -Wextra -fPIC
INCLUDE := -Iinclude
LIBS := -luuid

SRC_ALL := $(wildcard ./src/*)
HEADER_ALL := $(wildcard ./include/*)

SRC_HAKE := main.c goal.c dloader.c id.c paths.c
SRC_GOALS := filesystem.c
FILES_HAKE := $(SRC_HAKE:%.c=${SRC}/%.c)
OBJ_HAKE := $(SRC_HAKE:%.c=${BUILD}/%.o)
SO_GOALS := $(SRC_GOALS:%.c=${GOALDIR}/%.so)

.PHONY: clean all goals

all: hake tags goals

hake: $(OBJ_HAKE)
	$(CC) $^ $(LIBS) -o $@

goals: ${SO_GOALS}

${GOALDIR}/%.so: ${GOALDIR}/%.o
	$(CC) -shared $^ $(LIBS) -o $@

${BUILD}:
	mkdir -p $@

${GOALDIR}:
	mkdir -p $@

${BUILD}/%.o: src/%.c | ${BUILD} ${GOALDIR}
	${CC} ${CFLAGS} $^ ${INCLUDE} -o $@

tags: ${SRC_ALL} ${HEADER_ALL}
	ctags --recurse

clean:
	rm -rf hake tags ${BUILD} || exit 0
