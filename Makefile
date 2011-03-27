tgit: tgit.c
		gcc -lgit2 -lz -o tgit tgit.c
clean:
		rm -f tgit
