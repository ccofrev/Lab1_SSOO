salida_out = salida
salida_headers = 	funciones.h jpeg.h
salida_source  = $(salida_headers:.h=.c) main.c
salida_objects = $(salida_source:.c=.o)

CC     = gcc
CFLAGS = -Wall
LIBS =  -std=c11 -ljpeg

depends = .depends

build : $(salida_out) 

$(salida_out) : $(salida_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm $(LIBS)

$(objects) :
	$(CC) $(CFLAGS) -c -o $@ $*.c

$(depends) : $(salida_source) $(salida_headers)
	@$(CC) -MM $(salida_source) > $@


clean :
	$(RM) $(salida_out) $(salida_objects) $(depends)

