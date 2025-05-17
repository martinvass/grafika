all:
	gcc -o beadando.exe -Iinclude/ src/* -lSOIL -lopengl32 -lglu32 -lglut32 -std=c99

linux:
	gcc -o beadando.exe -Iinclude/ src/* -lSOIL -lGL -lGLU -lglut -std=c99

macos:
	@echo "Building for macOS…"
	gcc -o beadando \
	    -Iinclude \
	    $(shell find src -type f -name '*.c') \
	    -framework OpenGL \
	    -framework GLUT \
	    -framework Cocoa \
	    -std=c99