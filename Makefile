CC = gcc

#ALLEGRO = allegro64

ifdef ALLEGRO
CFLAGS = -I${ALLEGRO}/include 
LDFLAGS = -L${ALLEGRO}/lib 
endif

LDLIBS = -lallegro -lallegro_font -lallegro_ttf -lallegro_image -lallegro_primitives -lallegro_acodec -lallegro_audio

TARGETS = allegro01 allegro02 allegro03 allegro04 allegro05 allegro06 allegro07 allegro08 allegro09 allegro10 allegro11

all: ${TARGETS}

clean:
	rm ${TARGETS}
