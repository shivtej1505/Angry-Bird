all: angry_bird_2D

angry_bird_2D: angry_bird_2D.cpp glad.c
	g++ -o angry_bird_2D angry_bird_2D.cpp glad.c -lglfw -lftgl -lSOIL -I/usr/local/include -I/usr/local/include/freetype2 -L/usr/local/lib -ldl -lGL
#	g++ -o angry_bird_2D angry_bird_2D.cpp glad.c -lGL -lglfw -ldl

clean:
	rm angry_bird_2D
