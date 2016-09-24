CFLAGS=-Wall -g

build: main.o Utils/utils.a
	gcc main.o Utils/utils.a -o stegnography

main.o: main.c
	gcc -c -g main.c

Utils/utils.a:
	cd Utils; make;

en: build
	./stegnography -e Extras/data.txt Extras/audio.wav Extras/out_audio.wav

de: build
	./stegnography -d Extras/out_audio.wav Extras/output_file.txt

clean:
	rm -f stegnography main.o Utils/functions.o Utils/utils.a Extras/out_audio.wav Extras/output_file.txt Extras/tbd*
