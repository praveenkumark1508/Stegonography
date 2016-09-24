#MACROS
SRC_FILES=functions.c
OBJ_FILES=functions.o
OUT_FILES=utils.a

#makefile to create archieve

${OUT_FILES}: ${OBJ_FILES}
	ar -cvq ${OUT_FILES} ${OBJ_FILES}

${OBJ_FILES}:
	gcc -c -g ${SRC_FILES}

.PHONY:clean

clean:
	rm utils.a functions.o -f
