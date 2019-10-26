all:
	@g++ -std=c++11 duck_opengl.cpp -o duck_opengl -framework OpenGL -lglfw && ./duck_opengl
run:
	@./duck_opengl
