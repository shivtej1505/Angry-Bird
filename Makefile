all: angry_bird_2D

angry_bird_2D: angry_bird_2D.cpp glad.c
	g++ -o angry_bird angry_bird_2D.cpp glad.c -lGL -lglfw -ldl

clean:
	rm angry_bird_2D
